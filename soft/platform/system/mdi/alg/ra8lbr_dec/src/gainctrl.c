/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/





#include "coder.h"
#include "assembly.h"

#define MAX_NPSAMPS     (1024 / NPARTS)
#define MAX_LOGNPSAMPS  7                   /* log2(MAX_NPSAMPS) */
#define FRAC_MASK       (MAX_NPSAMPS - 1)

static const int npsampsTab[NUM_MLT_SIZES] = {256 / NPARTS, 512 / NPARTS, 1024 / NPARTS};
static const int nplog2Tab[NUM_MLT_SIZES] =  {5, 6, 7};

/* 2^(x/128) - format = Q30 */
static const int POW2NTAB[128] =
{
    0x40000000, 0x4058f6a8, 0x40b268fa, 0x410c57a2, 0x4166c34c, 0x41c1aca7, 0x421d1462, 0x4278fb2b,
    0x42d561b4, 0x433248ae, 0x438fb0cb, 0x43ed9ac0, 0x444c0740, 0x44aaf702, 0x450a6abb, 0x456a6323,
    0x45cae0f2, 0x462be4e2, 0x468d6fae, 0x46ef8210, 0x47521cc6, 0x47b5408c, 0x4818ee22, 0x487d2646,
    0x48e1e9ba, 0x4947393f, 0x49ad1598, 0x4a137f88, 0x4a7a77d4, 0x4ae1ff43, 0x4b4a169c, 0x4bb2bea5,
    0x4c1bf829, 0x4c85c3f1, 0x4cf022ca, 0x4d5b157e, 0x4dc69cdd, 0x4e32b9b4, 0x4e9f6cd4, 0x4f0cb70c,
    0x4f7a9930, 0x4fe91413, 0x50582888, 0x50c7d765, 0x51382182, 0x51a907b4, 0x521a8ad7, 0x528cabc3,
    0x52ff6b55, 0x5372ca68, 0x53e6c9da, 0x545b6a8b, 0x54d0ad5a, 0x55469329, 0x55bd1cdb, 0x56344b52,
    0x56ac1f75, 0x57249a29, 0x579dbc57, 0x581786e6, 0x5891fac1, 0x590d18d3, 0x5988e209, 0x5a055751,
    0x5a82799a, 0x5b0049d4, 0x5b7ec8f2, 0x5bfdf7e5, 0x5c7dd7a4, 0x5cfe6923, 0x5d7fad59, 0x5e01a53f,
    0x5e8451d0, 0x5f07b405, 0x5f8bccdb, 0x60109d51, 0x60962665, 0x611c6919, 0x61a3666d, 0x622b1f66,
    0x62b39509, 0x633cc85b, 0x63c6ba64, 0x64516c2e, 0x64dcdec3, 0x6569132f, 0x65f60a7f, 0x6683c5c3,
    0x6712460b, 0x67a18c68, 0x683199ed, 0x68c26fb1, 0x69540ec9, 0x69e6784d, 0x6a79ad56, 0x6b0daeff,
    0x6ba27e65, 0x6c381ca6, 0x6cce8ae1, 0x6d65ca38, 0x6dfddbcc, 0x6e96c0c3, 0x6f307a41, 0x6fcb096f,
    0x70666f76, 0x7102ad80, 0x719fc4b9, 0x723db650, 0x72dc8374, 0x737c2d55, 0x741cb528, 0x74be1c20,
    0x75606374, 0x76038c5b, 0x76a7980f, 0x774c87cc, 0x77f25cce, 0x78991854, 0x7940bb9e, 0x79e947ef,
    0x7a92be8b, 0x7b3d20b6, 0x7be86fba, 0x7c94acde, 0x7d41d96e, 0x7deff6b6, 0x7e9f0606, 0x7f4f08ae,
};

/**************************************************************************************
 * Function:    DecodeGainInfo
 *
 * Description: decode the gain window parameters for each frame
 *
 * Inputs:      pointer to initialized Gecko2Info struct
 *              pointer to uninitialized GAINC struct
 *              number of bits remaining in bitstream for this frame
 *
 * Outputs:     GAINC struct filled in with number and location of attacks, and gains
 *
 * Return:      number of bits remaining in bitstream, -1 if out-of-bits
 **************************************************************************************/
int DecodeGainInfo(Gecko2Info *gi, GAINC *gainc, int availbits)
{
    int i, nbits, code,tst_bit;
    BitStreamInfo *bsi = &(gi->bsi);

    /* unpack nattacks */

    code = RA_TRYBITS(bsi, 16);

    nbits = 0;
    do
    {
        tst_bit =   code>>15;
        code    =  code<<1;
        code    =  code & 0xffff;
        nbits++;
    }
    while (tst_bit);

    RA_SKIPBITS(bsi, nbits);

    gainc->nats = nbits - 1;            /* nats = number of ones */
    availbits -= nbits;

    ASSERT (gainc->nats <= MAXNATS);

    if (gainc->nats > 0)
    {
        for (i = 0; i < gainc->nats; i++)
        {
            tst_bit = RA_TRYBITS(bsi,LOCBITS+1+GAINBITS);
            gainc->loc[i] = tst_bit>>5;
            code = tst_bit & 16;

            if (!code)
            {
                gainc->gain[i] = -1;
                RA_SKIPBITS(bsi,LOCBITS+1);
                availbits = availbits - LOCBITS -1;
            }
            else
            {
                code = tst_bit & 0xf;
                gainc->gain[i] = CODE2GAIN(code);

                RA_SKIPBITS(bsi,LOCBITS+1+GAINBITS);
                availbits = availbits - LOCBITS -1 - GAINBITS;
            }
        }
    }
    gainc->maxExGain = 0;

    if (availbits < 0)
        return -1;

    return availbits;
}

/**************************************************************************************
 * Function:    CopyGainInfo
 *
 * Description: copy contents of one GAINC struct into another one
 *
 * Inputs:      pointer to initialized GAINC struct (source)
 *              pointer to uninitialized GAINC struct (destination)
 *
 * Outputs:     gaincDest which is identical to gaincSource
 *
 * Return:      none
 *
 * Notes:       this prevents compiler from generating a memcpy for gainc0 = gainc1
 *                (we want to avoid a CRT lib call, for portability of asm)
 **************************************************************************************/
void CopyGainInfo(GAINC *gaincDest, GAINC *gaincSource)
{
    int nBytes = sizeof(GAINC);
    unsigned char *d = (unsigned char *)gaincDest;
    unsigned char *s = (unsigned char *)gaincSource;

    for ( ; nBytes != 0; nBytes--)
        *d++ = *s++;

}

/**************************************************************************************
 * Function:    CalcGainChanges
 *
 * Description: reconstruct segmented gain window
 *
 * Inputs:      pointer to uninitialized exgain array
 *              pointer to old (overlap) and current gain info structs
 *
 * Outputs:     exgain[0, ... 2*NPARTS] with expanded gains at each switch point
 *
 * Return:      none
 *
 * Notes:       each frame is divided into NPARTS segments, with
 *                npsamps = nSamples / NPARTS samples in each one
 *              the gain window can only change at these boundaries
 *              if exgain[i] != exgain[i-1] the gain is interpolated logarithmically
 *                between the two points (buf[(i-1)*npsamps to buf[i*npsamps])
 **************************************************************************************/
static void CalcGainChanges(int *exgain, GAINC *gainc0, GAINC *gainc1)
{
    int i, nats, maxGain, offset;

    /* second half - expand gains, working backwards */
    exgain[NPARTS+NPARTS] = 0;                          /* always finish at 1.0 */
    nats = gainc1->nats;                                /* gain changes left */

    for (i = NPARTS-1; i >= 0; i--)
    {
        if (nats && (i == gainc1->loc[nats-1]))         /* at gain change */
            exgain[i+NPARTS] = gainc1->gain[--nats];    /* use it */
        else
            exgain[i+NPARTS] = exgain[i+NPARTS+1];      /* repeat last gain */
    }

    /* pull any discontinuity through first half by offsetting all
     *   gains with starting gain of second half
     */
    offset = exgain[NPARTS];

    /* first half - expand gains, working backwards */
    nats = gainc0->nats;                                /* gain changes left */
    for (i = NPARTS-1; i >= 0; i--)
    {
        if (nats && (i == gainc0->loc[nats-1]))         /* at gain change */
            exgain[i] = gainc0->gain[--nats] + offset;  /* use it */
        else
            exgain[i] = exgain[i+1];                    /* repeat last gain */
    }

    /* find max gain for each half (input and overlap) */
    maxGain = 0;
    for (i = 0; i <= NPARTS; i++)
    {
        if (exgain[i] > maxGain)
            maxGain = exgain[i];
    }
    gainc0->maxExGain = maxGain;

    maxGain = 0;
    for (i = NPARTS; i <= 2*NPARTS; i++)
    {
        if (exgain[i] > maxGain)
            maxGain = exgain[i];
    }
    gainc1->maxExGain = maxGain;

    return;
}

/**************************************************************************************
 * Function:    InterpolatePCM
 *
 * Description: apply gain window to first half of current frame, and overlap-add
 *                with second half of previous frame, to produce PCM output
 *
 * Inputs:      table index (for transform size)
 *              pointer to initialized exgain array (NPARTS+1 gain values)
 *              first half of IMLT output for current frame (buf),
 *                synthesis window has not yet been applied
 *              overlap from previous frame (buf + MAXNMLT), synthesis and gain
 *                windows were already applied
 *              max exgain for first half of current frame
 *              buffer for output PCM
 *              number of channels
 *              number of fraction bits present in current and overlap data
 *
 * Outputs:     nSamples samples of 16-bit PCM output
 *
 * Return:      none
 *
 * Notes:       this processes one channel at a time, but skips every other sample in
 *                the output buffer (pcm) for stereo interleaving
 **************************************************************************************/
static void InterpolatePCM(int tabidx, int *exgain, int *buf, int maxGain, short *pcm, int nChans, int fbitsPCM, int fbitsOver)
{
    int in, npsamps, part,i,res;
    int shiftLo, shiftHi, currGainLo, currGainHi, gainDiffLo, gainDiffHi;
    int gainLo0, gainLo1, gainHi0, gainHi1, w0, w1, f0, f1, oc;
    int *over0, *over1, shift[2], rndMask;
    short *pcm0, *pcm1;
    const int *wnd;

    shift[0] = 0;
    shift[1] = 0;
    rndMask = 0;
    if (fbitsPCM > fbitsOver)
    {
        shift[0] = MIN(fbitsPCM - fbitsOver, 31);
        fbitsPCM = fbitsOver;
    }
    else if (fbitsPCM < fbitsOver)
    {
        shift[1] = MIN(fbitsOver - fbitsPCM, 31);
    }

    if (fbitsPCM > 0)
        rndMask = (1 << (fbitsPCM - 1));
    ASSERT(fbitsPCM >= 0);

    npsamps = npsampsTab[tabidx];
    over0 = buf + MAXNMLT;
    over1 = over0 + NPARTS * npsamps - 1;
    buf += (nmltTab[tabidx] >> 1) - 1;

    wnd = window + windowOffset[tabidx];
    pcm0 = pcm;
    pcm1 = pcm + (NPARTS * npsamps) * nChans - 1;

    gainLo1 = exgain[0];
    gainHi0 = exgain[NPARTS];

    for (part = 0; part < NPARTS/2; part++)
    {
        npsamps = npsampsTab[tabidx];

        gainLo0 = gainLo1;
        gainLo1 = exgain[part + 1];
        gainHi1 = gainHi0;
        gainHi0 = exgain[NPARTS - part - 1];

        currGainHi =  gainHi1 << MAX_LOGNPSAMPS;
        gainDiffHi = (gainHi0 - gainHi1) << (MAX_LOGNPSAMPS - nplog2Tab[tabidx]);
        currGainHi += gainDiffHi;

        currGainLo =  gainLo0 << MAX_LOGNPSAMPS;
        gainDiffLo = (gainLo1 - gainLo0) << (MAX_LOGNPSAMPS - nplog2Tab[tabidx]);

        /* interpolate the gain window: in = in * 2^(gain0) * 2^((gain1 - gain0)*i/npsamps) */
        if (gainDiffLo || gainDiffHi)
        {
            /* slow path - interpolated section of gain window */
            for ( ; npsamps != 0; npsamps--)
            {
                in = *buf--;
                w0 = *wnd++;
                w1 = *wnd++;

                shiftLo = maxGain - (currGainLo >> MAX_LOGNPSAMPS) + shift[0];
                oc = *over0++;
                MULSHIFT32hx(w0, in, f0);
                f0 = MULSHIFT32(POW2NTAB[currGainLo & FRAC_MASK], f0) << 2;
                res = CLIPTOSHORT( ((f0 >> shiftLo) + (oc >> shift[1]) + rndMask) >> fbitsPCM );

                for(i=nChans; i>0; i--)
                    *pcm0++ = res;

                currGainLo += gainDiffLo;

                shiftHi = maxGain - (currGainHi >> MAX_LOGNPSAMPS) + shift[0];
                oc = *over1--;
                MULSHIFT32hx(w1, in, f1);
                f1 = MULSHIFT32(POW2NTAB[currGainHi & FRAC_MASK], f1) << 2;
                res = CLIPTOSHORT( ((f1 >> shiftHi) + (oc >> shift[1]) + rndMask) >> fbitsPCM );

                for(i=nChans; i>0; i--)
                    *pcm1-- = res;

                currGainHi += gainDiffHi;
            }
        }
        else
        {
            /* fast path - constant section of gain window */
            shiftLo = maxGain - gainLo0 + shift[0];
            shiftHi = maxGain - gainHi1 + shift[0];
            for ( ; npsamps != 0; npsamps--)
            {
                in = *buf--;
                w0 = *wnd++;
                w1 = *wnd++;

                oc = *over0++;
                MULSHIFT32hx(w0, in, f0);
                res = CLIPTOSHORT( ((f0 >> shiftLo) + (oc >> shift[1]) + rndMask) >> fbitsPCM );

                for(i=nChans; i>0; i--)
                    *pcm0++ = res;

                oc = *over1--;
                MULSHIFT32hx(w1, in, f1);
                res = CLIPTOSHORT( ((f1 >> shiftHi) + (oc >> shift[1]) + rndMask) >> fbitsPCM );

                for(i=nChans; i>0; i--)
                    *pcm1-- = res;
            }
        }
    }
}

/**************************************************************************************
 * Function:    InterpolateOverlap
 *
 * Description: apply gain window to second half of current frame, and save for
 *                overlap-add next frame
 *
 * Inputs:      table index (for transform size)
 *              pointer to initialized exgain array (NPARTS+1 gain values)
 *              second half of IMLT output for current frame (buf + nmlt/2),
 *                synthesis window has not yet been applied
 *              max exgain for second half of current frame
 *
 * Outputs:     nSamples samples of gain windowed data for overlap
 *                (stored at buf + MAXNMLT)
 *
 * Return:      none
 **************************************************************************************/
static void InterpolateOverlap(int tabidx, int *exgain, int *buf, int maxGain)
{
    int in, npsamps, part;
    int shiftLo, shiftHi, currGainLo, currGainHi, gainDiffLo, gainDiffHi;
    int gainLo0, gainLo1, gainHi0, gainHi1, w0, w1, wtmp;
    int *over0, *over1;
    const int *wnd;

    npsamps = npsampsTab[tabidx];
    over0 = buf + MAXNMLT;
    over1 = over0 + NPARTS * npsamps - 1;
    buf += (nmltTab[tabidx] >> 1);
    wnd = window + windowOffset[tabidx];

    gainLo1 = exgain[0];
    gainHi0 = exgain[NPARTS];

    for (part = 0; part < NPARTS/2; part++)
    {
        npsamps = npsampsTab[tabidx];

        gainLo0 = gainLo1;
        gainLo1 = exgain[part + 1];
        gainHi1 = gainHi0;
        gainHi0 = exgain[NPARTS - part - 1];

        currGainHi =  gainHi1 << MAX_LOGNPSAMPS;
        gainDiffHi = (gainHi0 - gainHi1) << (MAX_LOGNPSAMPS - nplog2Tab[tabidx]);
        currGainHi += gainDiffHi;

        currGainLo =  gainLo0 << MAX_LOGNPSAMPS;
        gainDiffLo = (gainLo1 - gainLo0) << (MAX_LOGNPSAMPS - nplog2Tab[tabidx]);

        /* interpolate the gain window: in = in * 2^(gain0) * 2^((gain1 - gain0)*i/npsamps) */
        if (gainDiffLo || gainDiffHi)
        {
            /* slow path - interpolated section of gain window */
            for ( ; npsamps != 0; npsamps--)
            {
                in = *buf++;
                w0 = *wnd++;
                w1 = *wnd++;

                shiftLo = maxGain - (currGainLo >> MAX_LOGNPSAMPS);
                MULSHIFT32hx(w1, in, wtmp);
                w1 =      wtmp  >> shiftLo;
                *over0++ = MULSHIFT32(POW2NTAB[currGainLo & FRAC_MASK], w1) << 2;
                currGainLo += gainDiffLo;

                shiftHi = maxGain - (currGainHi >> MAX_LOGNPSAMPS);
                MULSHIFT32hx(w0, in,wtmp);
                w0 =   -wtmp >> shiftHi;
                *over1-- = MULSHIFT32(POW2NTAB[currGainHi & FRAC_MASK], w0) << 2;
                currGainHi += gainDiffHi;
            }
        }
        else
        {
            /* fast path - constant section of gain window */
            shiftLo = maxGain - gainLo0;
            shiftHi = maxGain - gainHi1;
            for ( ; npsamps != 0; npsamps--)
            {
                in = *buf++;
                w0 = *wnd++;
                w1 = *wnd++;

                MULSHIFT32hx(w1, in, wtmp);
                *over0++ =   wtmp >> shiftLo;
                MULSHIFT32hx(w0, in, wtmp);
                *over1-- = -wtmp >> shiftHi;
            }
        }
    }
}

/* default fraction bits, not counting any extras from dequantizer */
#define FBITS_OUT_IMLT (FBITS_OUT_DQ - FBITS_LOST_IMLT)

/**************************************************************************************
 * Function:    DecWindowWithAttacks
 *
 * Description: apply synthesis window, perform gain windowing of current frame,
 *                do overlap-add, and produce one frame of decoded PCM
 *              (general case - either gain window has attacks or the default Q format
 *                is not being used)
 *
 * Inputs:      table index (for transform size)
 *              input buffer (output of IMLT, before synthesis window)
 *              buffer for output PCM
 *              number of channels
 *              gain control structs for overlap and current frames
 *              number of extra integer bits present in current and overlap data
 *                (relative to the default format of FBITS_OUT_IMLT)
 *
 * Outputs:     nSamples samples of 16-bit PCM output
 *
 * Return:      none
 *
 * Notes:       this processes one channel at a time, but skips every other sample in
 *                the output buffer (pcm) for stereo interleaving
 **************************************************************************************/
void DecWindowWithAttacks(int tabidx, int *buf, short *pcm, int nChans, GAINC *gainc0, GAINC *gainc1, int xbits[2])
{
    int i, s, fbitsPCM, fbitsOver;
    int *pt0;
    int *pt1;
    int exgain[2*NPARTS+1];

    fbitsOver = FBITS_OUT_IMLT - xbits[0] - gainc0->maxExGain;
    CalcGainChanges(exgain, gainc0, gainc1);
    fbitsPCM  = FBITS_OUT_IMLT - xbits[1] - gainc0->maxExGain;

    /* this is EXTREMELY unlikely (gain window so high that we would have negative fraction bits)
     *   so just do << and clip whole frame to 0 fraction bits
     * can think of this as artifically adding fraction bits, or just doing gain window
     *   in 2 stages (first pass = window by constant power of 2, second pass = window
     *   by original gain window / constant power of 2)
     * whole purpose is so Interpolate functions can be hard-coded to >> only (fast)
     */
    s = 0;
    if (fbitsOver < 0 || fbitsPCM < 0)
    {
        s = MAX(-fbitsOver, -fbitsPCM);
        pt0 = buf;
        pt1 = buf + MAXNSAMP;

        for (i = npsampsTab[tabidx]*NPARTS; i >0 ; i--)
        {
            CLIP_2N_SHIFT(*pt0, s);
            CLIP_2N_SHIFT(*pt1, s);
            pt0++;
            pt1++;
        }

        fbitsOver+= s;
        fbitsPCM += s;
    }

    InterpolatePCM(tabidx, exgain, buf, gainc0->maxExGain, pcm, nChans, fbitsPCM, fbitsOver);
    InterpolateOverlap(tabidx, exgain + NPARTS, buf, gainc1->maxExGain);

    /* undo extreme gain window scaling */
    if (s)
    {
        pt0 = buf;
        pt1 = buf + MAXNSAMP;

        for (i = npsampsTab[tabidx]*NPARTS; i >0 ; i--)
        {
            *pt0 = (*pt0)>>s;
            *pt1 = (*pt1)>>s;
            pt0++;
            pt1++;
        }
    }

    return;
}

/**************************************************************************************
 * Function:    DecWindowNoAttacks
 *
 * Description: apply synthesis window, perform gain windowing of current frame,
 *                do overlap-add, and produce one frame of decoded PCM
 *              (fast case - no gain window attacks and the data is in the default
 *                Q format with FBITS_OUT_IMLT fraction bits in input)
 *
 * Inputs:      table index (for transform size)
 *              input buffer (output of IMLT, before synthesis window)
 *              pcm buffer
 *              number of channels
 *
 * Outputs:     nSamples samples of 16-bit PCM output
 *
 * Return:      none
 *
 * Notes:       this processes one channel at a time, but skips every other sample in
 *                the output buffer (pcm) for stereo interleaving
 *              this should fit in registers on ARM - make sure compiler does this
 *                correctly!
 **************************************************************************************/

void DecWindowNoAttacks(int tabidx, int *buf0, short *pcm0, int nChans)
{
    int nmlt, nmltHalf;
    int in, oc, w0, w1, f0, f1, f2, f3;
    int *buf1, *over0, *over1;
    short *pcm1;
    const int *wnd;
    int i,res0,res1;

    nmlt = nmltTab[tabidx];
    nmltHalf = nmlt >> 1;

    over0 = buf0 + MAXNMLT;
    over1 = over0 + nmlt - 1;

    buf0 += nmltHalf;
    buf1  = buf0 - 1;
    wnd = window + windowOffset[tabidx];
    pcm1 = pcm0 + nmlt * nChans -1;

    for ( ; nmltHalf != 0; nmltHalf--)
    {
        /* load window coefficients */
        w0 = *wnd++;
        w1 = *wnd++;

        /* apply window to generate first N samples */
        in = *buf1--;
        MULSHIFT32hx(w0, in, f0);
        MULSHIFT32hx(w1, in, f1);

        /* overlap-add with second N samples from last frame */
        oc = *over0; res0= CLIPTOSHORT( (f0 + oc + 16) >> 5 );
        oc = *over1; res1= CLIPTOSHORT( (f1 + oc + 16) >> 5 );

        for(i=nChans; i>0; i--)
        {
            *pcm0++ = res0;
            *pcm1-- = res1;
        }

        /* apply window to generate second nmlt samples, save for overlap with next frame */
        in = *buf0++;
        MULSHIFT32hx(w1, in, f2);
        *over0++ =  f2;
        MULSHIFT32hx(w0, in, f3);
        *over1-- = -f3;
    }

    return;
}

