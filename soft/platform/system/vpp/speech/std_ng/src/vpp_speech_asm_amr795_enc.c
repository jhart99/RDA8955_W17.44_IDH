﻿/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
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



#include "vppp_speech_common.h"


void CII_COD_AMR_795(void)
{
    VoC_push16(RA,DEFAULT);
    VoC_push16(REG7,DEFAULT); // COD_AMR_ANAP_ADDR
    // begin of CII_lpc_M_515); */
    VoC_lw16i(REG6,COD_AMR_A_T_ADDRESS+33);
    VoC_lw16i_set_inc(REG0,STRUCT_COD_AMRSTATE_P_WINDOW_ADDR,1);//st->p_window address
    VoC_lw16i_set_inc(REG1,STATIC_CONST_WIND_200_40_compressd_ADDR,1);//window_200_40 address
    VoC_sw16_d(REG6,PARA4_ADDR_ADDR);/*for calling Levinson*/
    VoC_jal(Coolsand_decompressed_windows);
    VoC_jal(CII_Autocorr);
    /* Lag windowing    */
    VoC_jal(CII_Lag_window);
    /* Levinson Durbin  */
    VoC_jal(CII_Levinson);
    // end of CII_lpc_M_475); */
//  VoC_jal(CII_lsp_795);
    VoC_lw16i(REG7,LSP_LSP_NEW_Q_ADDR);
    VoC_lw16i(REG6,STRUCT_LSPSTATE_LSP_OLD_Q_ADDR);
    VoC_lw16i_set_inc(REG1,COD_AMR_LSP_NEW_ADDRESS,-1);
    VoC_lw16i(REG4,COD_AMR_AQ_T_ADDRESS);
    VoC_lw16i(REG5,COD_AMR_A_T_ADDRESS);
    VoC_push16(REG1,DEFAULT);   // push16(n+2)   &lsp_new[]
    VoC_push32(REG67,IN_PARALLEL);  // push32(n+3)   &lsp_new_q[]/&st->lsp_old_q[]
    VoC_lw16i_set_inc(REG0,STRUCT_LSPSTATE_LSP_OLD_ADDR,1);
    VoC_lw16i(REG7,STRUCT_Q_PLSFSTATE_PAST_RQ_ADDR);
    VoC_lw16_sd(REG6,1,DEFAULT);
    VoC_push16(REG0,DEFAULT);   // push16(n+1)   &st->lsp_old[]
    VoC_push32(REG45,IN_PARALLEL);  // push32(n+2)   &az[]/&azQ[]
    VoC_push32(REG67,DEFAULT);  // push32(n+1)   &st->qSt->past_rq[]/&anap[]
//  VoC_lw16i_short(REG3,7,IN_PARALLEL);
    /***************************************
      Function CII_lsp_M_74
      input:
        &lsp_new_q[]/&st->lsp_old_q[]   : push32(n+3)
        &az[]/&azQ[]            : push32(n+2)
        &st->qSt->past_rq[]/&anap[] : push32(n+1)
        &lsp_new[]          : push16(n+2)
        &st->lsp_old[]          : push16(n+1)
      Optimized by Kenneth   09/20/2004
    ***************************************/
//  VoC_jal(CII_lsp_M_795);
    VoC_lw16i_short(REG6,22,DEFAULT);

    VoC_jal(CII_Az_lsp);        // Az_lsp(&az[MP1 * 3], lsp_new, st->lsp_old);

    VoC_lw16_sd(REG2,0,DEFAULT);
    VoC_lw16i_short(REG6,0,DEFAULT);
    VoC_lw16_sd(REG3,1,DEFAULT);
    VoC_lw32_sd(REG01,1,IN_PARALLEL);
    VoC_movreg16(REG0,REG1,DEFAULT);
    VoC_jal(CII_Int_lpc_1to3);      //Int_lpc_1to3_2(st->lsp_old, lsp_new, az);

    VoC_lw16i_short(REG7,8,DEFAULT);
    VoC_be16_rd(lsp_M475_DTX_L1,REG7,GLOBAL_ENC_USED_MODE_ADDR);
    VoC_lw32_sd(REG45,0,DEFAULT);       // load  &st->qSt->past_rq[]/&anap[]
    VoC_lw16_sd(REG0,1,IN_PARALLEL);    // load  &lsp_new[]
    VoC_lw32_sd(REG67,2,DEFAULT);
    VoC_lw16i_short(INC0,-1,IN_PARALLEL);
    VoC_lw16_d(REG6,GLOBAL_ENC_MODE_ADDR);
    VoC_jal(CII_Q_plsf_3);

    VoC_lw32_sd(REG45,0,DEFAULT);
    VoC_lw16i_short(REG7,3,DEFAULT);
    VoC_lw32_sd(REG01,1,DEFAULT);
    VoC_add16_rr(REG4,REG4,REG7,IN_PARALLEL);
    VoC_lw32_sd(REG23,2,DEFAULT);
    VoC_lw16i_short(REG6,1,IN_PARALLEL);
    VoC_sw16_sd(REG4,2,DEFAULT);//COD_AMR_ANAP_ADDR);
    VoC_jal(CII_Int_lpc_1to3);
lsp_M475_DTX_L1:

    VoC_pop16(REG1,DEFAULT);
    VoC_pop32(RL6,DEFAULT);
    VoC_pop16(REG0,DEFAULT);
    VoC_pop32(RL6,IN_PARALLEL);
    VoC_jal(CII_Copy_M);              // Copy (lsp_new, st->lsp_old, M);

    VoC_pop32(REG23,DEFAULT);
    VoC_movreg16(REG0,REG3,DEFAULT);
    VoC_movreg16(REG1,REG2,DEFAULT);
    VoC_jal(CII_Copy_M); //Copy (lsp_new_q, st->lsp_old_q, M);

    /* From A(z) to lsp. LSP quantization and interpolation */
//   lsp(st->lspSt, mode, *usedMode, A_t, Aq_t, lsp_new, &ana);
    /* Buffer lsp's and energy */
    /*
    dtx_buffer(st->dtx_encSt,
          lsp_new,
          st->new_speech);
    */

//////////////////////////
    VoC_jal(CII_dtx_buffer);

    VoC_lw16_d(REG0,GLOBAL_ENC_USED_MODE_ADDR);
    VoC_be16_rd(cod_amr102_795_go,REG0,CONST_8_ADDR)
    VoC_jump(cod_amr102_795);
cod_amr102_795_go:

    VoC_lw16_d(REG7,COD_AMR_COMPUTE_SID_FLAG_ADDRESS);
    VoC_jal(CII_dtx_enc);
    /*
           CII_dtx_enc2(st->dtx_encSt,
                  compute_sid_flag,
                  st->lspSt->qSt,
                  st->gainQuantSt->gc_predSt,
                  &ana);
    */
    // Set_zero(st->old_exc,    PIT_MAX + L_INTERPOL);
    VoC_lw16i_set_inc(REG0,STRUCT_COD_AMRSTATE_OLD_EXC_ADDR,2);
    VoC_lw32z(REG67,DEFAULT);
    VoC_loop_i(0,77)
    VoC_sw32inc_p(REG67,REG0,DEFAULT);
    VoC_endloop0
//      Set_zero(st->mem_w0,     M);
//      Set_zero(st->mem_err,    M);
//  for ( i = 0; i < M; i++)      state->past_rq[i] = 0;
    VoC_lw16i_set_inc(REG0,STRUCT_COD_AMRSTATE_MEM_W0_ADDR,2);
    VoC_lw16i_set_inc(REG1,STRUCT_COD_AMRSTATE_MEM_ERROR_ADDR,2);
    VoC_lw16i_set_inc(REG2,STRUCT_Q_PLSFSTATE_PAST_RQ_ADDR,2);
    VoC_loop_i_short(5,DEFAULT)
    VoC_startloop0
    VoC_sw32inc_p(REG67,REG0,DEFAULT);
    VoC_sw32inc_p(REG67,REG1,DEFAULT);
    VoC_sw32inc_p(REG67,REG2,DEFAULT);
    VoC_endloop0
//      Set_zero(st->zero,       L_SUBFR);
//      Set_zero(st->hvec,       L_SUBFR);    /* set to zero "h1[-L_SUBFR..-1]" */
    VoC_lw16i_set_inc(REG0,STRUCT_COD_AMRSTATE_ZERO_ADDR,2);
    VoC_lw16i_set_inc(REG1,STRUCT_COD_AMRSTATE_HVEC_ADDR,2);
    VoC_loop_i_short(20,DEFAULT)
    VoC_startloop0
    VoC_sw32inc_p(REG67,REG0,DEFAULT);
    VoC_sw32inc_p(REG67,REG1,DEFAULT);
    VoC_endloop0
    /* Reset lsp states
    lsp_reset(st->lspSt);*/
//  Copy(lsp_init_data, &st->lsp_old[0], M);
//  Copy(st->lsp_old, st->lsp_old_q, M);

//      Copy(lsp_new, st->lspSt->lsp_old, M);
//      Copy(lsp_new, st->lspSt->lsp_old_q, M);
    VoC_lw16i_set_inc(REG0,COD_AMR_LSP_NEW_ADDRESS,2);
    VoC_lw16i_set_inc(REG1,STRUCT_LSPSTATE_LSP_OLD_ADDR,2);
    VoC_lw16i_set_inc(REG2,STRUCT_LSPSTATE_LSP_OLD_Q_ADDR,2);
    VoC_loop_i_short(5,DEFAULT)
    VoC_startloop0
    VoC_lw32inc_p(REG45,REG0,DEFAULT);
    VoC_sw16_d(REG6,STRUCT_COD_AMRSTATE_SHARP_ADDR);
    VoC_sw32inc_p(REG45,REG1,DEFAULT);
    VoC_sw32inc_p(REG45,REG2,DEFAULT);
    VoC_endloop0

    /* Reset clLtp states */
//      cl_ltp_reset(st->clLtpSt);
//      st->sharp = SHARPMIN;       move16 ();
    VoC_jump(cod_amr103);
cod_amr102_795:
    /* check resonance in the filter */
//      lsp_flag = check_lsp(st->tonStabSt, st->lspSt->lsp_old);  move16 ();
    VoC_jal(CII_check_lsp);
cod_amr103:
    VoC_lw32z(REG67,DEFAULT);   // REG7 for i_subfr     // REG6 for subfrnr

    VoC_lw16i_set_inc(REG0,COD_AMR_A_T_ADDRESS,1);
    VoC_lw16i_set_inc(REG3,STRUCT_COD_AMRSTATE_SPEECH_ADDR,1);
    VoC_lw16i_set_inc(REG2,STRUCT_COD_AMRSTATE_WSP_ADDR,1);
cod_amr105:


    VoC_sw32_d(REG67,COD_AMR_SUBFRNR_ADDRESS);
    VoC_blt16_rd(cod_amr1041,REG6,CONST_4_ADDR)
    VoC_jump(cod_amr104);
cod_amr1041:/*
      pre_big(mode, gamma1, gamma1_12k2, gamma2, A_t, i_subfr, st->speech,
              st->mem_w, st->wsp);
*/
    VoC_push16(REG0,DEFAULT);
    VoC_push32(REG23,IN_PARALLEL);
    VoC_lw16i_set_inc(REG1,STATIC_CONST_gamma1_ADDR,1);

    VoC_lw16i_set_inc(REG2,PRE_BIG_AP1_ADDR,1);
    VoC_jal(CII_Weight_Ai);
    VoC_lw16_sd(REG0,0,DEFAULT);
    VoC_lw16i_set_inc(REG1,STATIC_CONST_F_GAMMA2_ADDR,1);
    VoC_lw16i_set_inc(REG2,PRE_BIG_AP2_ADDR,1);
    VoC_jal(CII_Weight_Ai);

    VoC_lw32_sd(REG23,0,DEFAULT);

    VoC_lw16i_short(INC3,1,DEFAULT);
    VoC_lw16i_set_inc(REG0,PRE_BIG_AP1_ADDR,1);
    VoC_jal(CII_Residu_new);
    VoC_lw32_sd(REG23,0,DEFAULT);

    VoC_lw16i_short(REG0,1,DEFAULT);
    VoC_lw16i_short(REG6,40,IN_PARALLEL);

    VoC_lw16i(REG5,PRE_BIG_AP2_ADDR);

    VoC_lw16i_short(INC3,1,DEFAULT);
    VoC_lw16i_set_inc(REG1,STRUCT_COD_AMRSTATE_MEM_W_ADDR,1);

    VoC_movreg16(REG3,REG2,DEFAULT);
    VoC_movreg16(REG7,REG2,IN_PARALLEL);
    /*__________________________________________________________________
      Function:  CII_Syn_filt
     prameters:
       &a[0]  ->reg5
       &x[0]  -> reg3(inc 1 )
       &y     ->REG7
       lg     ->REG6
       &mem[0]->REG1 (incr=1) push32:
       update ->reg0

     Version 1.0  Create
     Version 1.1 optimized by Kenneth 09/01/2004
    ____________________________________________________________________*/
    VoC_jal(CII_Syn_filt);

    VoC_lw32z(ACC0,DEFAULT);
//////////////////////////////////////////
    VoC_lw16_d(REG6,COD_AMR_SUBFRNR_ADDRESS);
    VoC_bne16_rd(cod_amr1061,REG6,CONST_0_ADDR)
    VoC_bne16_rd(cod_amr1061,REG6,CONST_2_ADDR)
    VoC_jump(cod_amr106);
cod_amr1061:
    /* Find open loop pitch lag for two subframes */
//  VoC_jal(CII_Pitch_ol_M795_795);
    VoC_lw16_d(REG4,STRUCT_VADSTATE1_TONE_ADDR);        //st->tone
    VoC_sw32_d(ACC0,STRUCT_COD_AMRSTATE_OL_GAIN_FLG_ADDR);//0

    VoC_lw16i_set_inc(REG0,STRUCT_COD_AMRSTATE_OLD_WSP_ADDR-1,2);
    VoC_add16_rd(REG0,REG0,COD_AMR_I_SUBFR_ADDRESS);
    VoC_add16_rd(REG5,REG0,CONST_1_ADDR);                       //&(old_wsp)


    VoC_bez16_d(PITCH_OL_NOT_DTX,GLOBAL_DTX_ADDR)
    VoC_shr16_ri(REG4,1,DEFAULT);

PITCH_OL_NOT_DTX:

    VoC_lw32inc_p(REG67,REG0,DEFAULT);
    VoC_multf32_rr(ACC0,REG7,REG7,DEFAULT);
    VoC_sw16_d(REG4,STRUCT_VADSTATE1_TONE_ADDR);    //store st->tone

    VoC_loop_i(0,(PITCH_OL_M795_LOOP_NUM-1)/2)
    VoC_bimac32inc_pp(REG0,REG0);
    VoC_endloop0

    /*--------------------------------------------------------*
     * Scaling of input signal.                               *
     *                                                        *
     *   if Overflow        -> scal_sig[i] = signal[i]>>3     *
     *   else if t0 < 1^20  -> scal_sig[i] = signal[i]<<3     *
     *   else               -> scal_sig[i] = signal[i]        *
     *--------------------------------------------------------*/

    /*--------------------------------------------------------*
     *  Verification for risk of overflow.                    *
     *--------------------------------------------------------*/
    VoC_movreg16(REG0,REG5,DEFAULT);
    VoC_lw16i_short(REG3,PITCH_OL_M795_LOOP_NUM/4+1,IN_PARALLEL);
    VoC_lw16i_set_inc(REG1,PITCH_OL_M475_SCALED_SIGNAL_ADDR,2);


    VoC_aligninc_pp(REG0,REG0,DEFAULT);

    VoC_lw16i_short(REG2,3,DEFAULT);
    VoC_be32_rd(PITCH_OL_795_SCALE_END,ACC0,CONST_0x7FFFFFFF_ADDR)
    VoC_lw16i_short(REG2,0,DEFAULT);
    VoC_bgt32_rd(PITCH_OL_795_SCALE_END,ACC0, CONST_0x100000_ADDR )
    VoC_lw16i_short(REG2,-3,DEFAULT);

PITCH_OL_795_SCALE_END:



    VoC_jal(CII_scale);
    /* calculate all coreelations of scal_sig, from pit_min to pit_max */
    /*copy scaled_signal to RAM_X*/
    VoC_lw16i_set_inc(REG0,PITCH_OL_M475_SCALED_SIGNAL_ADDR,2);
    VoC_lw16i_set_inc(REG1,PITCH_OL_M122_SCALED_SIGNAL2_ADDR,2);
    VoC_lw16i(REG6,56);
    VoC_jal(CII_copy_xy);

    VoC_lw16i_set_inc(REG0,PITCH_OL_M475_SCALED_SIGNAL_ADDR+143,2);
    VoC_lw16i_set_inc(REG1,PITCH_OL_M122_SCALED_SIGNAL2_ADDR,2);
    VoC_lw16i_set_inc(REG2,PITCH_OL_M475_CORR_ADDR-2,2);

// begin of CII_comp_corr
    VoC_movreg16(REG6,REG0,DEFAULT);
    VoC_lw16i_short(REG3,1,IN_PARALLEL);
    VoC_add16_rr(REG7,REG1,REG3,DEFAULT);
    VoC_lw32z(ACC0,IN_PARALLEL);
    VoC_aligninc_pp(REG0,REG1,DEFAULT);
//  exit_on_odd_address = OFF;

    VoC_loop_i(1,124)
    VoC_loop_i_short(40,DEFAULT)
    VoC_startloop0
    VoC_bimac32inc_pp(REG0,REG1);
    VoC_endloop0

    VoC_movreg32(REG01,REG67,DEFAULT);
    VoC_sw32inc_p(RL6,REG2,IN_PARALLEL);
    VoC_movreg32(RL6,ACC0,DEFAULT);
    VoC_lw32z(ACC0,IN_PARALLEL);
    VoC_aligninc_pp(REG0,REG1,DEFAULT);
    VoC_add16_rr(REG7,REG7,REG3,DEFAULT);
    VoC_endloop1
    VoC_sw32_p(RL6,REG2,DEFAULT);
//  exit_on_odd_address = ON;
    //    VoC_lw16i_short(REG5,40,IN_PARALLEL);
// end of CII_comp_corr

    /*--------------------------------------------------------------------*
     *  The pitch lag search is divided in three sections.                *
     *  Each section cannot have a pitch multiple.                        *
     *  We find a maximum for each section.                               *
     *  We compare the maximum of each section by favoring small lags.    *
     *                                                                    *
     *  First section:  lag delay = pit_max     downto 4*pit_min          *
     *  Second section: lag delay = 4*pit_min-1 downto 2*pit_min          *
     *  Third section:  lag delay = 2*pit_min-1 downto pit_min            *
     *--------------------------------------------------------------------*/
    /*
        j = shl (pit_min, 2);
        p_max1 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                          pit_max, j, &max1, dtx);
    */
    VoC_lw16i_set_inc(REG0,PITCH_OL_M475_CORR_ADDR,2);
    VoC_lw16i_set_inc(REG1,143,-1);
//  VoC_lw16i(REG4,64);


    VoC_lw32_d(REG45,CONST_40_64_ADDR);

    VoC_jal(CII_Lag_max_M475_795);
    /*
        p_max2 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                          i, j, &max2, dtx);

        */
    VoC_push32(REG67,DEFAULT);
    VoC_lw16i_short(REG5,40,IN_PARALLEL);
    VoC_lw16i_set_inc(REG0,PITCH_OL_M475_CORR_ADDR+128,2);

    VoC_lw16i_set_inc(REG1,79,-1);

    VoC_movreg16(REG4,REG5,DEFAULT);

    VoC_jal(CII_Lag_max_M475_795);
    /*
        p_max3 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                          i, pit_min, &max3, dtx);

        */
    VoC_lw16i_short(REG4,20,DEFAULT);
    VoC_lw16i_short(REG5,40,IN_PARALLEL);

    VoC_lw16i_set_inc(REG0,PITCH_OL_M475_CORR_ADDR+208,2);
    VoC_sw32_d(REG67,PITCH_OL_M475_P_MAX2_ADDR);
    VoC_lw16i_set_inc(REG1,39,-1);


    VoC_jal(CII_Lag_max_M475_795);
    VoC_sw32_d(REG67,PITCH_OL_M475_P_MAX3_ADDR);

    VoC_bez16_d(PITCH_OL_M795_04a,GLOBAL_DTX_ADDR)
    VoC_bnez16_d(PITCH_OL_M795_04b,COD_AMR_SUBFRNR_ADDRESS)
    /* calculate max high-passed filtered correlation of all lags */
PITCH_OL_M795_04a:
    VoC_jump(PITCH_OL_M795_04);
PITCH_OL_M795_04b:


    /* calculate max high-passed filtered correlation of all lags */
    VoC_lw16i_set_inc   (REG0,PITCH_OL_M475_CORR_ADDR+0,2);
    VoC_lw16i_set_inc   (REG1,PITCH_OL_M475_CORR_ADDR+2,2);
    VoC_lw16i_set_inc   (REG2,PITCH_OL_M475_CORR_ADDR+4,2);
// begin of CII_hp_max
    VoC_lw32_d(ACC0,CONST_0x80000000_ADDR); //max
    VoC_lw32inc_p(RL6,REG1,DEFAULT);
    VoC_loop_i(1,122)
    VoC_shr32_ri(RL6,-1,DEFAULT);
    VoC_sub32inc_rp(RL6,RL6,REG0,DEFAULT);
    VoC_sub32inc_rp(RL6,RL6,REG2,DEFAULT);
    VoC_bnltz32_r(HP_MAX_01,RL6)
    VoC_sub32_dr(RL6,CONST_0_ADDR,RL6);
HP_MAX_01:
    VoC_bgt32_rr(HP_MAX_02,ACC0,RL6)
    VoC_movreg32(ACC0,RL6,DEFAULT);
HP_MAX_02:
    VoC_lw32inc_p(RL6,REG1,DEFAULT);
    VoC_endloop1

    VoC_jal(CII_NORM_L_ACC0);
    VoC_sub16_rd(REG3,REG1,CONST_1_ADDR);   //shift1
    VoC_lw16i_set_inc(REG0,PITCH_OL_M475_SCALED_SIGNAL_ADDR+143,2);
    VoC_lw16i_set_inc(REG1,PITCH_OL_M122_SCALED_SIGNAL2_ADDR+143,2);
    VoC_shr32_ri(ACC0,1,DEFAULT);
    VoC_movreg16(REG4,ACC0_HI,DEFAULT);         //max16
    /* compute energy */

//  exit_on_odd_address = OFF;
    VoC_aligninc_pp(REG0,REG1,DEFAULT);
    VoC_lw32z(ACC0,IN_PARALLEL);
    VoC_loop_i_short(40,DEFAULT)
    VoC_startloop0
    VoC_bimac32inc_pp(REG0,REG1);
    VoC_endloop0

    VoC_lw16i(REG0,PITCH_OL_M475_SCALED_SIGNAL_ADDR+143);
    VoC_lw16i(REG1,PITCH_OL_M122_SCALED_SIGNAL2_ADDR+143-1);
    VoC_movreg32(RL6,ACC0,DEFAULT); //t0

    VoC_aligninc_pp(REG0,REG1,DEFAULT);
    VoC_lw32z(ACC0,IN_PARALLEL);
    VoC_loop_i_short(40,DEFAULT)
    VoC_startloop0
    VoC_bimac32inc_pp(REG0,REG1);
    VoC_endloop0
//  exit_on_odd_address = ON;

    /* high-pass filtering */
    VoC_shr32_ri(RL6,-1,DEFAULT);
    VoC_shr32_ri(ACC0,-1,DEFAULT);
    VoC_sub32_rr(ACC0,RL6,ACC0,DEFAULT);
    VoC_bnltz32_r(HP_MAX_03,ACC0)
    VoC_sub32_dr(ACC0,CONST_0_ADDR,ACC0);
HP_MAX_03:
    /* max/t0 */
    VoC_jal(CII_NORM_L_ACC0);
    VoC_sub16_rr(REG7,REG3,REG1,DEFAULT);   //shift
    VoC_movreg16(REG1,ACC0_HI,IN_PARALLEL); //t016
    VoC_movreg16(REG0,REG4,DEFAULT);
    VoC_lw16i_short(REG2,0,IN_PARALLEL);        //cor_max
    VoC_bez16_r(HP_MAX_04,REG1)
    VoC_jal(CII_DIV_S);
HP_MAX_04:
    VoC_shr16_rr(REG2,REG7,DEFAULT);
    VoC_NOP();
// end of CII_hp_max
    /* update complex background detector */
    VoC_sw16_d(REG2,STRUCT_VADSTATE1_BEST_CORR_HP_ADDR);

PITCH_OL_M795_04:
    /*--------------------------------------------------------------------*
     * Compare the 3 sections maximum, and favor small lag.               *
     *--------------------------------------------------------------------*/
    VoC_lw16i(REG6,27853);
    VoC_pop32(REG45,DEFAULT);
    VoC_multf16_rr(REG7,REG6,REG5,DEFAULT);
    VoC_pop16(REG0,DEFAULT);
    VoC_bnlt16_rd(PITCH_OL_M795_05,REG7,PITCH_OL_M475_MAX2_ADDR)
    VoC_lw32_d(REG45,PITCH_OL_M475_P_MAX2_ADDR);
PITCH_OL_M795_05:
    VoC_multf16_rr( REG7,REG6,REG5,DEFAULT);
    VoC_lw16i(REG6,COD_AMR_T_OP_ADDRESS);
    VoC_bnlt16_rd(cod_amr106,REG7,PITCH_OL_M475_MAX3_ADDR)
    VoC_lw16_d  (REG4,PITCH_OL_M475_P_MAX3_ADDR);
cod_amr106:


    VoC_lw16_d(REG7,COD_AMR_SUBFRNR_ADDRESS);
    VoC_shr16_ri(REG7,1,DEFAULT);
    VoC_add16_rr(REG6,REG6,REG7,DEFAULT);

    VoC_pop32(REG23,DEFAULT);
    VoC_sw16_p(REG4,REG6,DEFAULT);
    VoC_lw16i_short(REG4,11,DEFAULT);
    VoC_lw16i_short(REG5,40,IN_PARALLEL);
    VoC_add16_rr(REG2,REG2,REG5,DEFAULT);
    VoC_add16_rr(REG3,REG3,REG5,IN_PARALLEL);
    VoC_lw32_d(REG67,COD_AMR_SUBFRNR_ADDRESS);
    VoC_add16_rd(REG6,REG6,CONST_1_ADDR);

    VoC_bne16_rd(cod_amr601,REG6,CONST_3_ADDR)
    VoC_add16_rd(REG7,REG7,CONST_80_ADDR);
cod_amr601:
    VoC_add16_rr(REG0,REG0,REG4,DEFAULT);
    VoC_lw16i_short(INC0,1,IN_PARALLEL);
    VoC_sw32_d(REG67,COD_AMR_SUBFRNR_ADDRESS);

    VoC_jump(cod_amr105);
cod_amr104:
    /* run VAD pitch detection */
    VoC_bez16_d(cod_amr110,GLOBAL_DTX_ADDR)
    VoC_jal(CII_vad_pitch_detection);
cod_amr110:
    VoC_lw16_d(REG0,GLOBAL_ENC_USED_MODE_ADDR);
    VoC_bne16_rd(the_end1,REG0,CONST_8_ADDR)        // if (sub((Word16)*usedMode, (Word16)MRDTX) == 0)
    VoC_jump(the_end);
the_end1:
    /*------------------------------------------------------------------------*
    *          Loop for every subframe in the analysis frame                 *
    *------------------------------------------------------------------------*
    *  To find the pitch and innovation parameters. The subframe size is     *
    *  L_SUBFR and the loop is repeated L_FRAME/L_SUBFR times.               *
    *     - find the weighted LPC coefficients                               *
    *     - find the LPC residual signal res[]                               *
    *     - compute the target signal for pitch search                       *
    *     - compute impulse response of weighted synthesis filter (h1[])     *
    *     - find the closed-loop pitch parameters                            *
    *     - encode the pitch dealy                                           *
    *     - update the impulse response h1[] by including fixed-gain pitch   *
    *     - find target vector for codebook search                           *
    *     - codebook search                                                  *
    *     - encode codebook address                                          *
    *     - VQ of pitch and codebook gains                                   *
    *     - find synthesis speech                                            *
    *     - update states of weighting filter                                *
    *------------------------------------------------------------------------*/

    VoC_lw16i_short(REG7,0,DEFAULT);    // REG7 for loop i_subfr
    VoC_lw16i_short(REG6,1,DEFAULT);    // REG6 for loop evenSubfr
    VoC_lw16i_short(REG5,0,IN_PARALLEL);    // REG5 for loop subfrNr
    VoC_lw16i(REG0,COD_AMR_A_T_ADDRESS);    // REG0 for A
    VoC_lw16i(REG1,COD_AMR_AQ_T_ADDRESS);   // REG1 for Aq
    VoC_push16(REG5,DEFAULT);
    VoC_push16(REG0,DEFAULT);
    VoC_push16(REG1,DEFAULT);
    VoC_push16(REG6,DEFAULT);
    VoC_push16(REG7,DEFAULT);
cod_amr112:
    VoC_blt16_rd(cod_amr1111,REG7,CONST_160_ADDR)
    /* Save states for the MR475 mode */
    VoC_jump(cod_amr111);
cod_amr1111:
    /*-----------------------------------------------------------------*
       * - Preprocessing of subframe                                     *
       *-----------------------------------------------------------------*/
    /*         subframePreProc(*usedMode, gamma1, gamma1_12k2,
                             gamma2, A, Aq, &st->speech[i_subfr],
                             st->mem_err, st->mem_w0, st->zero,
                             st->ai_zero, &st->exc[i_subfr],
                             st->h1, xn, res, st->error);
    */
    VoC_lw16i(RL7_HI,STRUCT_COD_AMRSTATE_MEM_W0_ADDR);
    VoC_jal(CII_subframePreProc);
    /* copy the LP residual (res2 is modified in the CL LTP search)    */
    // Copy (res, res2, L_SUBFR);
    VoC_lw16i_set_inc(REG0,COD_AMR_RES_ADDRESS,2);
    VoC_lw16i_set_inc(REG1,COD_AMR_RES3_ADDRESS,2);
    VoC_lw16i_short(REG6,10,DEFAULT);
    VoC_lw16_sd(REG7,0,DEFAULT);
    VoC_lw16i(REG5,STRUCT_COD_AMRSTATE_EXC_ADDR);
    VoC_add16_rr(REG7,REG7,REG5,DEFAULT);
    VoC_jal(CII_copy_xy);

    /*-----------------------------------------------------------------*
     * - Closed-loop LTP search                                        *
     *-----------------------------------------------------------------*/
    /*      cl_ltp(st->clLtpSt, st->tonStabSt, *usedMode, i_subfr, T_op, st->h1,
                 &st->exc[i_subfr], res2, xn, lsp_flag, xn2, y1,
                 &T0, &T0_frac, &gain_pit, gCoeff, &ana,
                 &gp_limit);*/
    VoC_lw16i_short(REG1,20,DEFAULT);   // REG1 for pit_min

    VoC_sw16_d(REG7,COD_AMR_EXC_ADDR_ADDRESS);

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//  begin of  CII_cl_ltp
    //*T0 = Pitch_fr(clSt->pitchSt,mode, T_op, exc, xn, h1, L_SUBFR, frameOffset,T0_frac, &resu3, &index);
    /*++++++++++++++++++++++++++++++++++++++++++++++++++*/
    //  begin of  CII_Pitch_fr

    VoC_lw16_sd(REG7,0,DEFAULT);        // REG7 for i_subfr

    VoC_lw16_d(REG0,COD_AMR_T_OP_ADDRESS);

    VoC_lw16i_short(REG2,3,DEFAULT);
    VoC_lw16i_short(REG3,6,IN_PARALLEL);

    VoC_lw16i_short(REG5,0,DEFAULT);    // REG5 for delta_search=1;


    VoC_bez16_r(AMR_PITCH_FR201,REG7)

    VoC_lw16_d(REG0,COD_AMR_T_OP_ADDRESS+1);
    VoC_be16_rd(AMR_PITCH_FR201,REG7,CONST_80_ADDR)
    VoC_lw16i_short(REG5,1,DEFAULT);    // REG5 for delta_search=0;
    VoC_lw16_d(REG0,STRUCT_PITCH_FRSTATE_T0_PREV_SUBFRAME_ADDR);

    VoC_lw16i_short(REG2,0xa,DEFAULT);
    VoC_lw16i_short(REG3,0x13,IN_PARALLEL);


AMR_PITCH_FR201:
    VoC_sub16_rr(REG4,REG0,REG2,DEFAULT);   //      *t0_min = sub(T0, delta_low);
    VoC_sw16_d(REG5,Pitch_delta_search_addr);// save delta_search

    /* REG0 for T0;REG23 for delta_frc_low,delta_frc_range; REG1 for pit_min; REG6 for PIT_MAX; REG45 for t0_min,t0_max */
    /*++++++++++++++++++++++++++++++++++++++++++++++++*/
    // begin of CII_getRange

    VoC_bgt16_rr(GET_RANGE1,REG4,REG1)      //      if (sub(*t0_min, pitmin) < 0) {
    VoC_movreg16(REG4,REG1,DEFAULT);    //      *t0_min = pitmin;                                  move16();
GET_RANGE1:
    VoC_add16_rr(REG5,REG4,REG3,DEFAULT);   //      *t0_max = add(*t0_min, delta_range);
    VoC_bngt16_rd(GET_RANGE2,REG5,CONST_143_ADDR)       //      if (sub(*t0_max, pitmax) > 0) {
    VoC_lw16i(REG5,143);                //      *t0_max = pitmax;                                  move16();
    VoC_sub16_rr(REG4,REG5,REG3,DEFAULT);   //      *t0_min = sub(*t0_max, delta_range);
GET_RANGE2:
    // end of CII_getRange
    /*++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*-----------------------------------------------------------------------*
     *           Find interval to compute normalized correlation             *
     *-----------------------------------------------------------------------*/
    VoC_lw16i_short(REG2,4,DEFAULT);
    VoC_lw16i_set_inc(REG0,Pitch_fr6_corr_v_addr,2);
    VoC_sw32_d(REG45,Pitch_fr6_t0_min_addr);

    VoC_sub16_rr(REG4,REG4,REG2,DEFAULT);           //t_min = sub (t0_min, L_INTER_SRCH);
    VoC_add16_rr(REG5,REG5,REG2,IN_PARALLEL);       //t_max = add (t0_max, L_INTER_SRCH);

    VoC_lw16i_set_inc(REG1,STRUCT_COD_AMRSTATE_H1_ADDR,-2); /*REG1=h[n]地址*/
    VoC_sub16_rr(REG2,REG0,REG4,DEFAULT);           //REG2 for address &corr_v[-t_min];
    VoC_push32(REG45,IN_PARALLEL);      // stack32 0 for t_min t_max
//    Norm_Corr (exc, xn, h, L_subfr, t_min, t_max, corr);
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    // begin of CII_Norm_Corr

    VoC_sub16_dr(REG0,COD_AMR_EXC_ADDR_ADDRESS,REG4);   //k = -t_min;
    VoC_lw16i_set_inc(REG3,PITCH_F6_EXCF_ADDR,1);
    VoC_push16(REG2,DEFAULT);   // stack16 1 for corr_v[-t_min] address
    VoC_push16(REG0,DEFAULT);   // stack16 0 for exc[k] address
//    Convolve (&exc[k], h, excf, L_subfr);
    VoC_jal(CII_Convolve);
    VoC_lw32z(ACC0,DEFAULT);  //  s = 0;
    VoC_lw16i_set_inc(REG0,PITCH_F6_EXCF_ADDR,2);
    VoC_loop_i_short(20,DEFAULT)  //  for (j = 0; j < L_subfr; j++)
    VoC_startloop0
    VoC_bimac32inc_pp(REG0,REG0);   // s = L_mac (s, excf[j], excf[j]);
    VoC_endloop0


    VoC_lw16i_short(REG4,-3,DEFAULT);                               //-h_fac
    VoC_lw16i_short(REG5,0,IN_PARALLEL);        // scaling
    VoC_bngt32_rd(PITCH_F6_1001,ACC0,CONST_0x04000000_ADDR);

    /* scale "excf[]" to avoid overflow */
    VoC_lw16i_set_inc(REG0,PITCH_F6_EXCF_ADDR,2);           /* REG0 for excf[i] address */
    VoC_lw16i_set_inc(REG1,PITCH_F6_EXCF_ADDR,2);           /* REG1 for s_excf[i] address */
    VoC_lw16i_short(REG2,2,DEFAULT);
    VoC_lw16i_short(REG3,10,IN_PARALLEL);
    VoC_jal(CII_scale);

    VoC_lw16i_short(REG4,-1,DEFAULT);                           //-h_fac
    VoC_lw16i_short(REG5,2,DEFAULT);
PITCH_F6_1001:

//    for (j = 0; j < L_subfr; j++)
//        scaled_excf[j] = shr (excf[j], 2);
    VoC_lw16_sd(REG2,1,DEFAULT);        //REG2 for address &corr_v[-t_min];
    VoC_lw32_sd(REG67,0,DEFAULT);   // REG6 for t_min, REG7 for t_max
    VoC_add16_rr(REG3,REG2,REG7,DEFAULT);

    VoC_lw16i_short(INC2,1,DEFAULT);
    VoC_sw16_d(REG3,Pitch_fr6_max_loop_addr);       // for decide the last loop
    VoC_add16_rr(REG2,REG2,REG6,DEFAULT);       // REG2 for corr_norm[i] address
    VoC_lw16_sd(REG3,0,IN_PARALLEL);            // REG3 for exc[k] address


PITCH_F6_1002:  // begin loop
    VoC_lw32z(ACC0,DEFAULT);
    VoC_lw16i_set_inc(REG0,PITCH_F6_EXCF_ADDR,2);


    VoC_loop_i_short(20,DEFAULT)
    VoC_push32(REG45,IN_PARALLEL);  // -h_fac scaling
    VoC_startloop0
    VoC_bimac32inc_pp(REG0,REG0);
    VoC_endloop0
    VoC_push32(REG23,DEFAULT);      // save exc corr_v address
    VoC_movreg32(REG01,ACC0,DEFAULT);
    VoC_jal(CII_Inv_sqrt);
    VoC_movreg32(REG67,REG01,DEFAULT);

    /* Compute correlation between xn[] and excf[] */
    VoC_lw16i_set_inc(REG0,PITCH_F6_EXCF_ADDR,2);
    VoC_lw16i_set_inc(REG1,COD_AMR_XN_ADDRESS,2);
    VoC_loop_i_short(20,DEFAULT);
    VoC_lw32z(ACC0,IN_PARALLEL);
    VoC_startloop0
    VoC_bimac32inc_pp(REG0,REG1);
    VoC_endloop0

    VoC_shru16_ri(REG6,1,DEFAULT);   //nor_l

    VoC_movreg32(REG45,ACC0,DEFAULT);       //corr_h
    VoC_shru16_ri(REG4,1,DEFAULT);      //corr_l

//        s = Mpy_32 (corr_h, corr_l, norm_h, norm_l);
    VoC_multf32_rr(ACC0,REG7,REG5,DEFAULT);
    VoC_multf16_rr(REG0,REG7,REG4,IN_PARALLEL);

    VoC_multf16_rr(REG1,REG6,REG5,DEFAULT);
    VoC_lw16i_short(REG3,1,IN_PARALLEL);
    VoC_mac32_rr(REG0,REG3,DEFAULT);
    VoC_mac32_rr(REG1,REG3,DEFAULT);
    VoC_pop32(REG23,DEFAULT);       // REG3 for exc[k]
//  VoC_shr32_ri(ACC0,-16,DEFAULT);
    VoC_lw16i_short(INC3,-1,DEFAULT);
//        corr_norm[i] = extract_h (L_shl (s, 16));
    VoC_sw16inc_p(ACC0_LO,REG2,DEFAULT);
    VoC_pop32(REG45,DEFAULT);   // -h_fac scaling

    VoC_bgt16_rd(PITCH_F6_1003,REG2,Pitch_fr6_max_loop_addr)

    VoC_inc_p(REG3,DEFAULT);      //      k--;
    VoC_lw16i_set_inc(REG0,STRUCT_COD_AMRSTATE_H1_ADDR+39,-1);
    VoC_lw16i_set_inc(REG1,PITCH_F6_EXCF_ADDR+38,1);
    VoC_lw16_p(REG6,REG3,DEFAULT);  // REG6 for exc[k]
    VoC_push32(REG23,DEFAULT);
    VoC_multf32inc_rp(REG23,REG6,REG0,DEFAULT);   //   s = L_mult (exc[k], h[j]);

    VoC_loop_i_short(39,DEFAULT)  //   for (j = L_subfr - 1; j > 0; j--)
    VoC_startloop0
    VoC_shr32_rr(REG23,REG4,DEFAULT);       //  s = L_shl (s, h_fac);
    VoC_add16inc_rp(REG3,REG3,REG1,DEFAULT);
    VoC_multf32inc_rp(REG23,REG6,REG0,DEFAULT);   //   s = L_mult (exc[k], h[j]);
    exit_on_warnings = OFF;
    VoC_sw16_p(REG3,REG1,DEFAULT);    //  s_excf[j] = add (extract_h (s), s_excf[j - 1]);
    exit_on_warnings = ON;
    VoC_sub16_rd(REG1,REG1,CONST_2_ADDR);
    VoC_endloop0

    VoC_shr16_rr(REG6,REG5,DEFAULT);
    VoC_pop32(REG23,DEFAULT);
    VoC_sw16_d(REG6,PITCH_F6_EXCF_ADDR);     //  s_excf[0] = shr (exc[k], scaling);
    VoC_jump(PITCH_F6_1002);
PITCH_F6_1003:


    // end of CII_Norm_Corr
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    VoC_pop32(REG45,DEFAULT);
    VoC_pop16(REG2,DEFAULT);
    /*-----------------------------------------------------------------------*
     *                           Find integer pitch                          *
     *-----------------------------------------------------------------------*/

    VoC_lw16_sd(REG0,0,DEFAULT);    //  corr[]
    VoC_lw16i_short(INC0,1,IN_PARALLEL);
    VoC_lw32_d(REG45,Pitch_fr6_t0_min_addr);
    VoC_add16_rr(REG0,REG0,REG4,DEFAULT);
    VoC_movreg16(REG1,REG4,DEFAULT);            //REG1 for lag   lag = t0_min;
    VoC_lw16inc_p(REG7,REG0,DEFAULT);           //REG2 for max
    VoC_sub16_rr(REG6,REG5,REG4,IN_PARALLEL);       //REG6 for loop number
    VoC_movreg16(REG2,REG7,DEFAULT);            //  max = corr[t0_min];
    VoC_lw16i_short(REG5,1,IN_PARALLEL);
    VoC_add16_rr(REG4,REG4,REG5,DEFAULT);


    VoC_bngtz16_r(AMR_PITCH_FR103,REG6)

    VoC_loop_r(0,REG6);     //  for (i = t0_min + 1; i <= t0_max; i++)
    VoC_lw16inc_p(REG7,REG0,DEFAULT);
    VoC_sub16_rr(REG3,REG7,REG2,DEFAULT);
    VoC_bltz16_r(AMR_PITCH_FR104,REG3)          //   if (sub (corr[i], max) >= 0) {

    VoC_movreg16(REG2,REG7,DEFAULT);        // max = corr[i];
    VoC_movreg16(REG1,REG4,IN_PARALLEL);    //     lag = i;
AMR_PITCH_FR104:
    VoC_add16_rr(REG4,REG4,REG5,DEFAULT);
    VoC_endloop0;
AMR_PITCH_FR103:




    /*-----------------------------------------------------------------------*
     *                        Find fractional pitch                          *
     *-----------------------------------------------------------------------*/
    VoC_lw16i_short(REG5,-2,DEFAULT);

    VoC_lw16i_short(INC2,-1,DEFAULT);
    VoC_lw16i_short(REG4,0x54,IN_PARALLEL);


    VoC_bnez16_d(AMR_PITCH_FR105,Pitch_delta_search_addr)
    VoC_bgt16_rr(AMR_PITCH_FR106,REG1,REG4)     // 0x54 if ((delta_search == 0) && (sub (lag, max_frac_lag) > 0))
AMR_PITCH_FR105:


    VoC_lw16_sd(REG2,0,DEFAULT);

    VoC_lw16i_short(REG4,1,DEFAULT);

//AMR_PITCH_FR110:
//             searchFrac (&lag, &frac, last_frac, corr, flag3);
//          searchFrac (&lag, &frac, last_frac, corr, flag3);
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    //   begin of CII_searchFrac


    VoC_add16_rr(REG2,REG1,REG2,DEFAULT);   //REG2 for corr[*lag] address    x1 = &x[0];

    VoC_lw16i_set_inc(REG3,STATIC_CONST_INTER_6_25_ADDR,1);     //&inter_6[0]
    VoC_push16(REG1,DEFAULT);       //  REG1 ->lag

    VoC_lw32_d(RL6,CONST_0x80000000_ADDR); // frac = 0;         // RL6_LO for frac RL6_HI for last_frac     // RL6_HI for max
    VoC_lw16i_short(INC0,6,DEFAULT);
    VoC_lw16i_short(INC1,6,IN_PARALLEL);




    VoC_loop_i(1,5)                  //for (i = add (*frac); i <= last_frac; i++)

    VoC_movreg16(REG6,REG5,DEFAULT);
    VoC_push32(REG23,DEFAULT);
// corr_int = Interpol_3or6 (&corr[*lag], i, flag3);
//    begin of Interpol_3or6
    VoC_shr16_ri(REG6,-1,DEFAULT);  //   frac = shl (frac, 1);   /* inter_3[k] = inter_6[2*k] -> k' = 2*k */
    VoC_bnltz16_r(INTERPOL_3OR6_101,REG6)  //  if (frac < 0)
    VoC_add16_rd(REG6,REG6,CONST_6_ADDR);   //  frac = add (frac, UP_SAMP_MAX);
    VoC_sub16_rr(REG2,REG2,REG4,DEFAULT);   // x--;
INTERPOL_3OR6_101:
    VoC_add16_rr(REG0,REG3,REG6,DEFAULT);       // c1 = &inter_6[frac];
    VoC_sub16_rr(REG1,REG3,REG6,DEFAULT);
    VoC_add16_rd(REG1,REG1,CONST_6_ADDR);       // c2 = &inter_6[sub (UP_SAMP_MAX, frac)];
    VoC_add16_rr(REG3,REG2,REG4,DEFAULT);   // x2 = &x[1];
    VoC_loop_i_short(4,DEFAULT)
    VoC_lw32z(ACC0,IN_PARALLEL);
    VoC_startloop0
    VoC_mac32inc_pp(REG0,REG2,DEFAULT); //  s = L_mac (s, x1[-i], c1[k]);
    VoC_mac32inc_pp(REG1,REG3,DEFAULT); //  s = L_mac (s, x2[i], c2[k]);
    VoC_endloop0
//    end of Interpol_3or6
    VoC_movreg16(REG0,RL6_HI,DEFAULT);
    VoC_add32_rd(REG23,ACC0,CONST_0x00008000_ADDR);
    VoC_bngt16_rr(SEARCHFRAC101,REG3,REG0);        // if (sub (corr_int, max) > 0)
    VoC_movreg16(RL6_LO,REG5,DEFAULT);          // *frac = i;
    VoC_movreg16(RL6_HI,REG3,IN_PARALLEL);          // max = corr_int;
SEARCHFRAC101:
    VoC_add16_rr(REG5,REG5,REG4,DEFAULT);       //i++
    VoC_pop32(REG23,DEFAULT);

    VoC_endloop1





    VoC_pop16(REG1,DEFAULT);
    VoC_movreg16(REG0,RL6_LO,DEFAULT);  // laod frac


    VoC_bne16_rd(SEARCHFRAC105,REG0,CONST_neg2_ADDR)
    VoC_lw16i_short(REG0,1,DEFAULT);    // *frac = 1;
    VoC_sub16_rr(REG1,REG1,REG4,IN_PARALLEL);// *lag = sub (*lag, 1);
SEARCHFRAC105:

    VoC_bne16_rd(SEARCHFRAC104,REG0,CONST_2_ADDR)
    VoC_lw16i_short(REG0,0xFFFF,DEFAULT);   // *frac = -1;
    VoC_add16_rr(REG1,REG1,REG4,IN_PARALLEL);// *lag = add (*lag, 1);
SEARCHFRAC104:
    VoC_movreg16(RL6_LO,REG0,DEFAULT);
    //   end of CII_searchFrac
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
AMR_PITCH_FR106:
    /* REG1 for lag */
    /*-----------------------------------------------------------------------*
     *                           encode pitch                                *
     *-----------------------------------------------------------------------*/
    VoC_pop16(REG2,DEFAULT);
    /* encode with 1/3 subsample resolution */
//  VoC_jal(CII_Enc_lag3_795);
    VoC_movreg16(REG7,RL6_LO,DEFAULT);      // REG7 for t0_frac


    VoC_sub16_rd(REG2,REG1,Pitch_fr6_t0_min_addr);      // i = sub (T0, T0_min);
    VoC_add16_rr(REG3,REG2,REG2,DEFAULT);   // REG2 for i
    VoC_add16_rr(REG2,REG2,REG3,DEFAULT);   //      i = add (add (T0, T0), T0);
    VoC_add16_rd(REG2,REG2,CONST_2_ADDR);
    VoC_add16_rr(REG2,REG2,REG7,DEFAULT);   // index = add (add (i, 2), T0_frac);


    VoC_bnez16_d(ENC_LAG3_100,Pitch_delta_search_addr)

    VoC_add16_rd(REG2,REG1,(50+TABLE_MR67_ADDR));   //index = add (T0, 112);

    VoC_bgt16_rd(ENC_LAG3_100,REG1,(64+TABLE_MR475_ADDR) )      // CONST_0x55_ADDR  if (sub (T0, 85) <= 0)

    VoC_add16_rr(REG2,REG1,REG1,DEFAULT);   // REG2 for i
    VoC_add16_rr(REG2,REG2,REG1,DEFAULT);   //      i = add (add (T0, T0), T0);
    VoC_lw16i_short(REG6,58,IN_PARALLEL);
    VoC_sub16_rr(REG2,REG2,REG6,DEFAULT);
    VoC_add16_rr(REG2,REG2,REG7,DEFAULT);       //  index = add (sub (i, 58), T0_frac);
//  VoC_jump(ENC_LAG3_102);

ENC_LAG3_100:


//ENC_LAG3_102:
//     *ana_index = REGS[2].reg;
    /*-----------------------------------------------------------------------*
     *                          update state variables                       *
     *-----------------------------------------------------------------------*/

    VoC_lw16_sd(REG7,5,DEFAULT);//COD_AMR_ANAP_ADDR
    VoC_sw16_d(REG1,STRUCT_PITCH_FRSTATE_T0_PREV_SUBFRAME_ADDR);    //st->T0_prev_subframe = lag;
    VoC_sw16_p(REG2,REG7,DEFAULT);
    VoC_lw16i_short(REG2,1,IN_PARALLEL);    //REG2 for flag3
    VoC_add16_rr(REG7,REG7,REG2,DEFAULT); // REG7++
    VoC_sw16_d(REG1,COD_AMR_T0_ADDRESS);
    VoC_sw16_sd(REG7,5,DEFAULT);// COD_AMR_ANAP_ADDR
    VoC_sw16_d(RL6_LO,COD_AMR_T0_FRAC_ADDRESS);
//   *(*anap)++ = index;

    //  end of CII_Pitch_fr
    /*++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*---------------------------------------------------------------------------
    Function:  void CII_Pred_lt_3or6(void)
    Word16 exc[],        in/out: REG5
    Word16 T0,           input : REG6
    Word16 frac,         input : REG7
    Word16 L_subfr,      input : subframe size
    Word16 flag3         input : REG2
    -----------------------------------------------------------------------------*/
    VoC_lw16_d(REG5,COD_AMR_EXC_ADDR_ADDRESS);
    VoC_lw32_d(REG67,COD_AMR_T0_ADDRESS);//get T0 addr REG6  get frac in REG7
    VoC_jal(CII_Pred_lt_3or6);
    /* Convolve(exc, h1, y1, L_SUBFR); */
    VoC_lw16d_set_inc(REG0,COD_AMR_EXC_ADDR_ADDRESS,2);
    VoC_lw16i_set_inc(REG3,COD_AMR_Y1_ADDRESS,1);
    VoC_lw16i_set_inc(REG1,STRUCT_COD_AMRSTATE_H1_ADDR,-2); /*REG7=h[n]地址*/
    VoC_jal(CII_Convolve);
    /* gain_pit is Q14 for all modes
    *gain_pit = G_pitch(mode, xn, y1, g_coeff, L_SUBFR); */
    VoC_jal(CII_G_pitch);
    //  REG2 for gain_pit
    /* check if the pitch gain should be limit due to resonance in LPC filter */
    VoC_lw16_d(REG0,COD_AMR_LSP_FLAG_ADDRESS);
//  VoC_lw16i_short(REG7,0,DEFAULT);        // REG7 for  gpc_flag = 0;
    VoC_movreg16(RL7_LO,REG2,DEFAULT);  // RL7_LO->gain_pit
    VoC_lw16i(RL7_HI,0x7FFF);       // RL7_HI->gp_limit                 // REG6 for *gp_limit = MAX_16;

    VoC_bez16_d(cl_ltp202,COD_AMR_LSP_FLAG_ADDRESS)         //  if ((lsp_flag != 0)
    VoC_bngt16_rd(cl_ltp202,REG2,CONST_15565_ADDR);     // (sub(*gain_pit, GP_CLIP) > 0))

    /*       gpc_flag = check_gp_clipping(tonSt, *gain_pit);  move16 (); */
    VoC_shr16_ri(REG2,3,DEFAULT);       // sum = shr(g_pitch, 3);
    VoC_lw16i_set_inc(REG3,STRUCT_TONSTABSTATE_GP_ADDR,1);
    VoC_loop_i_short(7,DEFAULT)     // for (i = 0; i < N_FRAME; i++)
    VoC_startloop0
    VoC_add16inc_rp(REG2,REG2,REG3,DEFAULT);    //     sum = add(sum, st->gp[i]);
    VoC_endloop0

    VoC_bngt16_rd(cl_ltp202,REG2,CONST_15565_ADDR)              // if (sub(sum, GP_CLIP) > 0)
//  VoC_lw16i_short(REG7,1,DEFAULT);// return 1;
//cl_ltp101:
    /* REG7 for gpc_flag */
    /* special for the MR475, MR515 mode; limit the gain to 0.85 to */
    /* cope with bit errors in the decoder in a better way.         */


//  VoC_bez16_r(cl_ltp202,REG7);                    // if (gpc_flag != 0)
    VoC_lw16i(RL7_LO,15565);   // *gain_pit = GP_CLIP;
    VoC_movreg16(RL7_HI,RL7_LO,DEFAULT);       // *gp_limit = GP_CLIP;
cl_ltp202:
    VoC_movreg16(REG3,RL7_LO,DEFAULT);
    VoC_sw16_d(RL7_LO,COD_AMR_GAIN_PIT_ADDRESS);
    VoC_sw16_d(RL7_HI,COD_AMR_GP_LIMIT_ADDRESS);

    /* update target vector und evaluate LTP residual */

    VoC_lw16i_set_inc(REG0,COD_AMR_Y1_ADDRESS,1 );                  /* REG0 for y1[] first address */
    VoC_lw16i_set_inc(REG1,COD_AMR_XN_ADDRESS,1 );                  /* REG1 for xn[] first address */
    VoC_lw16i_set_inc(REG2,COD_AMR_XN2_ADDRESS,1 );             /* REG2 for xn2[] first address */

    VoC_loop_i(1,2)
    VoC_multf32inc_rp(REG45,REG3,REG0,DEFAULT);       // L_temp = L_mult(y1[i], *gain_pit);
    VoC_loop_i_short(40,DEFAULT)                        /*for (i = 0; i < L_SUBFR; i++) */
    VoC_startloop0
    VoC_shr32_ri(REG45,-1,DEFAULT);                 // L_temp = L_shl(L_temp, 1);
    VoC_sub16inc_pr(REG4,REG1,REG5,DEFAULT);        // xn2[i] = sub(xn[i], extract_h(L_temp));
    VoC_multf32inc_rp(REG45,REG3,REG0,DEFAULT);       // L_temp = L_mult(y1[i], *gain_pit);
    exit_on_warnings =OFF;
    VoC_sw16inc_p(REG4,REG2,DEFAULT);
    exit_on_warnings =ON;
    VoC_endloop0

    VoC_lw16_d(REG0,COD_AMR_EXC_ADDR_ADDRESS);      /* REG0 for &exc[i_subfr] address */
    VoC_lw16i(REG1,COD_AMR_RES_ADDRESS);
    VoC_movreg16(REG2,REG1,DEFAULT);    /* REG1 and REG2 for res2[] first address */
    VoC_endloop1
//  end of CII_cl_ltp
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    /* update LTP lag history
    if ((subfrNr == 0) && (st->ol_gain_flg[0] > 0))
    {
       st->old_lags[1] = T0;     move16 ();
    }
    */
    VoC_lw16_d(REG7,COD_AMR_T0_ADDRESS);
    VoC_lw16_sd(REG6,4,DEFAULT);
    VoC_bnez16_r(cod_amr116,REG6)
    VoC_bngtz16_d(cod_amr116,STRUCT_COD_AMRSTATE_OL_GAIN_FLG_ADDR)
    VoC_sw16_d(REG7,STRUCT_COD_AMRSTATE_OLD_LAGS_ADDR+1);
cod_amr116:

    /*  if ((sub(subfrNr, 3) == 0) && (st->ol_gain_flg[1] > 0))
          {
             st->old_lags[0] = T0;     move16 ();
          }
    */
    VoC_bne16_rd(cod_amr117,REG6,CONST_3_ADDR)
    VoC_bngtz16_d(cod_amr117,STRUCT_COD_AMRSTATE_OL_GAIN_FLG_ADDR+1)
    VoC_sw16_d(REG7,STRUCT_COD_AMRSTATE_OLD_LAGS_ADDR);
cod_amr117:
    /*-----------------------------------------------------------------*
     * - Inovative codebook search (find index and gain)               *
     *-----------------------------------------------------------------
    cbsearch(xn2, st->h1, T0, st->sharp, gain_pit, res2,
             code, y2, &ana, *usedMode, subfrNr);*/
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// begin of function    CII_cbsearch);

    VoC_lw16i_set_inc(REG0, COD_AMR_XN2_ADDRESS,1);
    VoC_sw16_d(REG6,COD_AMR_SUBFRNR_ADDRESS);
    VoC_lw16i_set_inc(REG1, STRUCT_COD_AMRSTATE_H1_ADDR,1);
    VoC_lw16d_set_inc(REG2, STRUCT_COD_AMRSTATE_SHARP_ADDR,1);
    VoC_lw16d_set_inc(REG3, COD_AMR_GAIN_PIT_ADDRESS,1);
    VoC_lw16i(REG4, COD_AMR_RES_ADDRESS);
    VoC_lw16i(REG5, COD_AMR_CODE_ADDRESS);

    VoC_push16(REG0,DEFAULT);
    VoC_push16(REG1,DEFAULT);
    VoC_push16(REG2,DEFAULT);
    VoC_push16(REG3,DEFAULT);
    VoC_push16(REG4,DEFAULT);
    VoC_push16(REG5,DEFAULT);

    VoC_lw16i(REG0, COD_AMR_Y2_ADDRESS);
    VoC_lw16_d(REG1, COD_AMR_SUBFRNR_ADDRESS);

    VoC_lw16_sd(REG2,11,DEFAULT); //COD_AMR_ANAP_ADDR
    VoC_push16(REG0,DEFAULT);
    VoC_push16(REG1,DEFAULT);
    VoC_push16(REG2,DEFAULT);

//stack16:   mode,  xn2 ,st->h1, st->sharp,   gain_pit, res2,  code, y2, subfrNr, anap
    VoC_jal(CII_code_4i40_17bits_795);
    VoC_jal(CII_gainQuant_795);
//stack16:   mode,  xn2 ,st->h1, st->sharp,   gain_pit, res2,  code, y2, subfrNr, anap
    // if want to save anap, do here  (COD_AMR_ANAP_ADDR)
    VoC_loop_i_short(8,DEFAULT);
    VoC_startloop0
    VoC_pop16(REG0, DEFAULT);
    VoC_endloop0
    VoC_sw16_sd(REG5,5,DEFAULT);

// end of function  CII_cbsearch);
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


    /* update gain history */
//      update_gp_clipping(st->tonStabSt, gain_pit);
//   Copy(&st->gp[1], &st->gp[0], N_FRAME-1);
//   st->gp[N_FRAME-1] = shr(g_pitch, 3);
    VoC_lw16_d(REG6,COD_AMR_GAIN_PIT_ADDRESS);
    VoC_shr16_ri(REG6,3,DEFAULT);
    VoC_lw16i_set_inc(REG0,STRUCT_TONSTABSTATE_GP_ADDR,1);
    VoC_lw16i_set_inc(REG1,STRUCT_TONSTABSTATE_GP_ADDR+1,1);
    VoC_loop_i_short(6,DEFAULT)
    VoC_startloop0
    VoC_lw16inc_p(REG7,REG1,DEFAULT);
    VoC_lw16i(RL7_LO,STRUCT_COD_AMRSTATE_MEM_W0_ADDR);
    VoC_sw16inc_p(REG7,REG0,DEFAULT);
    VoC_endloop0
    VoC_sw16_p(REG6,REG0,DEFAULT);
    VoC_lw16i_short(REG0,0,IN_PARALLEL);    // 0 for not sf
    /* Subframe Post Porcessing
    subframePostProc(st->speech, *usedMode, i_subfr, gain_pit,
                     gain_code, Aq, synth, xn, code, y1, y2, st->mem_syn,
                     st->mem_err, st->mem_w0, st->exc, &st->sharp);*/
    VoC_lw16i(RL7_HI,STRUCT_COD_AMRSTATE_MEM_SYN_ADDR);

    VoC_jal(CII_subframePostProc);
    VoC_lw16i_short(REG1,1,DEFAULT);

    VoC_lw16_sd(REG7,0,DEFAULT);    // i_subfr
    VoC_lw16i_short(REG0,40,IN_PARALLEL);

    VoC_add16_rr(REG7,REG7,REG0,DEFAULT);
    VoC_lw16_sd(REG5,2,IN_PARALLEL);    // A_Q

    VoC_lw16_sd(REG6,1,DEFAULT);    // evensb
    VoC_sub16_rr(REG6,REG1,REG6,DEFAULT);// evenSubfr = sub(1, evenSubfr);
    VoC_sw16_sd(REG7,0,DEFAULT);    // i_subfr
    VoC_lw16i_short(REG2,11,IN_PARALLEL);
    VoC_sw16_sd(REG6,1,DEFAULT);    // evensb
    VoC_add16_rr(REG5,REG5,REG2,IN_PARALLEL);
    VoC_lw16_sd(REG4,3,DEFAULT);    // A
    VoC_sw16_sd(REG5,2,DEFAULT);
    VoC_add16_rr(REG4,REG4,REG2,IN_PARALLEL);
    VoC_lw16_sd(REG3,4,DEFAULT);    // subNr     // subfrNr = add(subfrNr, 1);
    VoC_add16_rr(REG3,REG3,REG1,DEFAULT);
    VoC_sw16_sd(REG4,3,DEFAULT);
    VoC_sw16_sd(REG3,4,DEFAULT);

    VoC_jump(cod_amr112);
cod_amr111:
    VoC_loop_i_short(5,DEFAULT)
    VoC_startloop0
    VoC_pop16(REG7,DEFAULT);
    VoC_endloop0

//   Copy(&st->old_exc[L_FRAME], &st->old_exc[0], PIT_MAX + L_INTERPOL);
    VoC_lw16i_set_inc(REG0,STRUCT_COD_AMRSTATE_OLD_EXC_ADDR+160,2);
    VoC_lw16i_set_inc(REG1,STRUCT_COD_AMRSTATE_OLD_EXC_ADDR,2);
    exit_on_warnings = OFF;
    VoC_lw32inc_p(RL7,REG0,DEFAULT);
    VoC_loop_i(1,77)
    VoC_lw32inc_p(RL7,REG0,DEFAULT);
    VoC_sw32inc_p(RL7,REG1,DEFAULT);
    VoC_endloop1
    exit_on_warnings = ON;

the_end:
    VoC_jal(CII_PRM2_BITS_POST_PROCESS);
    VoC_pop16(RA,DEFAULT);
    VoC_NOP();
    VoC_return;
}





/*need reg2,reg3,rl7_hi,reg6,reg7,RL6(stack32)*/
/*after popping alp16 a more pop16 is needed*/

void CII_COMPUTE_ALP1_M74_795(void)
{



    VoC_lw16_sd(REG1,4,DEFAULT);            // SEARCH_10I40_PS0_ADDR
    VoC_movreg16(REG0,REG6,DEFAULT);

    VoC_lw16i_set_inc(REG0, C1035PF_RR_DIAG_ADDR+35, -5);
    exit_on_warnings=OFF;
    VoC_sw32_sd(REG01,2,DEFAULT);
    exit_on_warnings=ON;
    VoC_add16_rr(REG0,REG6,REG0,IN_PARALLEL);

    VoC_mult16_rd(REG6,REG6,CONST_4_ADDR);
    VoC_lw16i_set_inc(REG1, SEARCH_CODE_CP_RR_ADDR+35*4, 2);

    VoC_add16_rr(REG1,REG6,REG1,DEFAULT);
    VoC_pop32(RL6,IN_PARALLEL);                //rl6:alp0+0x8000



    VoC_loop_i(1,8)

    VoC_loop_r_short(REG2,DEFAULT);
    VoC_multf32inc_rp(ACC0,REG7,REG0,IN_PARALLEL);
    VoC_startloop0
    VoC_bimac32inc_rp(REG45,REG1);
    VoC_endloop0

    VoC_push16(ACC1_HI,DEFAULT);
    VoC_sub32_rr(REG01,REG01,RL7,IN_PARALLEL);

    VoC_add32_rr(ACC1,ACC0,RL6,DEFAULT);

    VoC_endloop1

    VoC_lw16i_set_inc(REG0,C1035PF_DN_ADDR,5);
    VoC_push16(ACC1_HI,DEFAULT);
    VoC_lw32_sd(REG23,1,IN_PARALLEL);
//output:
//r4:ps;r2:ix;r7:alp
//input:
//r3:ps0
//r2:i1

// Begin of function SEARCH_LOOP_M74
    VoC_movreg16(REG1,REG0,DEFAULT);
    VoC_add16_rr(REG0,REG0,REG2,IN_PARALLEL);

    VoC_lw16i_short(RL6_LO,5,DEFAULT);
    VoC_lw16i_short(RL6_HI,0,IN_PARALLEL);

    VoC_add32_rr(REG23,REG23,RL6,DEFAULT);

    VoC_lw16i_short(REG6,-1,DEFAULT);
    VoC_lw16i_short(REG7,1,IN_PARALLEL);        //reg67:    sq  and alp

    VoC_pop16(REG5,DEFAULT);
    VoC_add16inc_rp(REG4,REG3,REG0,IN_PARALLEL);

    VoC_loop_i_short(8,DEFAULT);
    VoC_startloop0
    VoC_multf16_rr(REG4,REG4,REG4,DEFAULT);

    VoC_multf32_rr(ACC0,REG6,REG5,DEFAULT);
    VoC_multf32_rr(ACC1,REG7,REG4,DEFAULT);
    VoC_NOP();
    VoC_bngt32_rr(SEARCH_CODE_I3_END,ACC1,ACC0)
    VoC_movreg32(REG67,REG45,DEFAULT);
    VoC_sub16_rr(REG2,REG0,REG1,IN_PARALLEL);

SEARCH_CODE_I3_END:

    VoC_pop16(REG5,DEFAULT);
    VoC_add16inc_rp(REG4,REG3,REG0,DEFAULT);
    VoC_endloop0

    VoC_sub32_rr(REG23,REG23,RL6,DEFAULT);  //ix

    VoC_lw16i_short(REG4,0,DEFAULT);
    VoC_add16_rr(REG1,REG1,REG2,IN_PARALLEL);

    VoC_be16_rd(SEARCH_CODE_END,REG6,CONST_0x0000FFFF_ADDR)

    VoC_add16_rp(REG4,REG3,REG1,DEFAULT);       //ps
SEARCH_CODE_END:
    //r4:ps;r2:ix;r7:alp
    VoC_lw32_d(ACC1,CONST_0x00008000_ADDR);
    VoC_sw16_sd (REG4,4,DEFAULT);   //SEARCH_10I40_PS0_ADDR);
    VoC_return;
}



/***********************************************

  Optimized by Kenneth  10/10/2004
************************************************/
void CII_search_4i40_795(void)
{

    /*my pointers*/
    VoC_push32(ACC0,DEFAULT);


    VoC_lw16i(REG0, SEARCH_CODE_CP_RR_ADDR +7);
    VoC_lw16i_set_inc(REG1,SEARCH_CODE_CP_RR_ADDR,1);

    VoC_lw16i(REG4,C1035PF_RR_SIDE_ADDR );
    VoC_lw16i(REG5,C1035PF_RR_DIAG_ADDR  );


    VoC_sw16_d(REG0,SEARCH_CODE_CP_RR_7_ADDR_ADDR);             //&rr[0] and &dn[0]
    VoC_sw32_d(REG45,C1035PF_RR_SIDE_ADDR_ADDR);
    VoC_movreg16(REG0, REG5, DEFAULT);
    VoC_push16(RA,IN_PARALLEL);

    VoC_lw16i_set_inc(REG2,C1035PF_DN_ADDR,1);
    VoC_lw16i_set_inc(REG3,SEARCH_10I40_RRV_ADDR,1);

    VoC_sw32_d(REG01,C1035PF_RR_DIAG_ADDR_2_ADDR);
    VoC_sw32_d(REG23,SEARCH_CODE_DN_ADDR_ADDR);

    /*end of my pointers*/
    VoC_lw16i_short(REG4,-1,DEFAULT);
    VoC_lw16i_short(REG5,1,IN_PARALLEL);

    VoC_push16(REG0,DEFAULT);           //push16 PS0
    VoC_lw16i_short(REG0,0,IN_PARALLEL);
    VoC_lw16i_set_inc(REG1,C1035PF_CODVEC_ADDR,1); //REG1 addr of codvec[0]


    VoC_loop_i_short(4,DEFAULT);
    VoC_push32(REG45,IN_PARALLEL); // psk =-1;alpk = 1;

    VoC_startloop0
    VoC_lw16i_short(REG3,3,DEFAULT);
    VoC_sw16inc_p(REG0,REG1,DEFAULT);
    VoC_push16(REG0,DEFAULT);           // push16 ipos[0~3]
    VoC_inc_p(REG0,IN_PARALLEL);
    VoC_endloop0

SEARCH_M74_LOOP_TRACK:
    /* fix starting position */
    VoC_sw16_sd(REG3,3,DEFAULT);
    VoC_lw16i_short(REG0,0,DEFAULT);
    VoC_sw16_d(REG3,SEARCH_M74_TRACK_ADDR);         //i=1
    VoC_sw16_sd(REG0,0,DEFAULT);
    VoC_lw16i_short(REG1,1,IN_PARALLEL);
    VoC_lw16i_short(REG2,2,DEFAULT);
    VoC_sw16_sd(REG1,1,DEFAULT);
    VoC_sw16_sd(REG2,2,DEFAULT);

    /*------------------------------------------------------------------*
     * main loop: try 4 tracks.                                         *
     *------------------------------------------------------------------*/
SEARCH_M74_LOOP_I:
    VoC_lw16_sd     (REG4,0,DEFAULT);
    VoC_sw16_d      (REG0,SEARCH_10I40_I_ADDR);
    /*----------------------------------------------------------------*
     * i0 loop: try 4 positions (use position with max of corr.).     *
     *----------------------------------------------------------------*/
SEARCH_M74_LOOP_I0:
    VoC_lw16i       (REG0,C417PF_DN2_ADDR);
    VoC_add16_rr(       REG0,REG0,REG4,DEFAULT);
    VoC_sw16_d      (REG4,SEARCH_10I40_I0_ADDR);
    VoC_lw16_p(     REG5,REG0,DEFAULT);
    VoC_bltz16_r(SEARCH_M74_LOOP_I0_END,REG5)
    //r4:i0
    VoC_add16_rd        (REG1,REG4,C1035PF_RR_DIAG_ADDR_ADDR);  //&rr[i0][i0]
    VoC_add16_rd        (REG0,REG4,SEARCH_CODE_DN_ADDR_ADDR);

    VoC_multf32_pd  (ACC0,REG1,CONST_D_1_4_ADDR);
    VoC_lw16_p(     REG6,REG0,DEFAULT);
    VoC_add32_rd        (ACC0,ACC0,CONST_0x00008000_ADDR);


    /*copy rr[i0] to cp_rr*/
    VoC_sw16_sd(REG6,4,DEFAULT);    //SEARCH_10I40_PS0_ADDR
    VoC_movreg16(REG2,REG4,IN_PARALLEL);
    VoC_push32(ACC0,DEFAULT);                       //alp0+0x8000

    /*set all units of cp_rr to zero*/
    VoC_lw32z(ACC1,IN_PARALLEL);

    VoC_lw16i_set_inc(REG0,SEARCH_CODE_CP_RR_ADDR,2);
    VoC_loop_i(0,80)
    VoC_sw32inc_p(ACC1,REG0,DEFAULT);
    VoC_endloop0

    VoC_lw16i_set_inc   (REG3,SEARCH_CODE_CP_RR_ADDR,4);
    VoC_jal         (CII_SEARCH_COPY);



    /*----------------------------------------------------------------*
     * i1 loop: 8 positions.                                          *
     *----------------------------------------------------------------*/
    /*compute alp1*/
    VoC_lw16_sd(REG6,1,DEFAULT);            //reg6:i1
    VoC_lw16i_short(RL7_HI,22,IN_PARALLEL); //rl7_hi:22


    VoC_lw16i_short(RL7_LO,0,DEFAULT);
    VoC_lw16i_short(REG2,1,IN_PARALLEL);        //reg2:loop num

    VoC_lw32_d(REG45,CONST_0x00004000L_ADDR);   //reg3:&double_1_2  COSNT_1_2_ADDR
    VoC_lw16i(REG7,_1_4);                   //reg7:_1_4

    VoC_jal     (CII_COMPUTE_ALP1_M74_795);


    VoC_multf32_rd  (ACC0,REG7,CONST_D_1_4_ADDR);
    VoC_sw16_d  (REG2,SEARCH_10I40_I1_ADDR);
    VoC_add32_rr        (ACC0,ACC0,ACC1,DEFAULT);

    /*copy rr[i1] to cp_rr*/
    VoC_lw16i_set_inc   (REG3,SEARCH_CODE_CP_RR_ADDR+1,4);
    VoC_push32(     ACC0,DEFAULT);                      //alp0+0x8000
    VoC_jal         (CII_SEARCH_COPY);

    /*compute alp2*/
    VoC_lw16_sd(REG6,2,DEFAULT);            //reg6:i1

    VoC_lw16i_short(RL7_HI,22,IN_PARALLEL); //rl7_hi:22

    VoC_lw16i_short(RL7_LO,0,DEFAULT);
    VoC_lw16i_short(REG2,1,IN_PARALLEL);        //reg2:loop num

    VoC_lw32_d(REG45,CONST_D_1_8_ADDR); //reg3:&double_1_8


    VoC_lw16i(REG7,_1_16);                  //reg7:_1_16

    VoC_jal     (CII_COMPUTE_ALP1_M74_795);

    VoC_sw16_d  (REG2,SEARCH_10I40_I2_ADDR);
    VoC_movreg16(       ACC0_HI,REG7,DEFAULT);
    VoC_lw16i_short(    ACC0_LO,0,IN_PARALLEL);

    VoC_add32_rr    (ACC0,ACC0,ACC1,DEFAULT);
    /*copy rr[i2] to cp_rr*/
    VoC_lw16i_short(INC1,-1,IN_PARALLEL);
    /*copy rr[i2] to cp_rr*/
// copy 8 number to  cp_rr

/////////////////////////////////////////////////////
// IN:
//     REG3:dest addr
//     REG2:i2
//     REG4:ipos[3]
//     INCR1 =-1
//     INCR3 =?
// OUT:
//     NONE
// REG USED:
//     REG0,REG1,REG2,REG3,REG4,REG5
// /////////////////////////////////////////////////

    VoC_lw16i_set_inc(REG3,SEARCH_CODE_CP_RR_ADDR+2,20);
    VoC_lw16_sd(REG4,3,DEFAULT);    //reg4:i3
    VoC_lw16i_short(REG0,4,IN_PARALLEL);
    VoC_mult16_rr(REG0,REG4,REG0,DEFAULT);
    VoC_push32(ACC0,DEFAULT);                       //alp0+0x8000
    VoC_add16_rr(REG3, REG3, REG0,DEFAULT);

    // REG0 for i2*i2
    VoC_mult16_rr(REG0, REG2, REG2,DEFAULT);
    // INCR0=5

    VoC_lw16i_short(INC0,5,DEFAULT);
    // REG0 for i2*(i2-1)
    VoC_sub16_rr(REG0, REG0, REG2,DEFAULT);
    // REG1 for i2
    VoC_movreg16(REG1, REG2,IN_PARALLEL);

    // REG0 for i2*(i2-1)/2
    VoC_shr16_ri(REG0,1,DEFAULT);
    VoC_lw16i_short(REG5,5,IN_PARALLEL);

    // REG0 for &rr_side[i2-1][0](rr[i2][0])
    // REG1 for &rr_diag[i2]
    VoC_add32_rd(REG01, REG01, C1035PF_RR_SIDE_ADDR_ADDR);

    // REG4 for &rr_side[i2-1][0]+i2
    VoC_add16_rr(REG4,REG0,REG2,DEFAULT);
    // REG0 for &rr_side[i2-1][ipos(3)]                 (rr[i2][ipos(3)]
    VoC_add16_rr(REG0,REG0,REG4,IN_PARALLEL);

    // REG4 for &rr_side[i2-1][0]+i2+5
    // prepare for comparison because the REG0 will be pre-incremented 5
    VoC_add16_rr(REG4,REG4,REG5,DEFAULT);
    // REG5 for  &rr_diag[i2]
    VoC_movreg16(REG5,REG1,IN_PARALLEL);

    // REG2 loop count
    VoC_lw16i_short(REG1,8,DEFAULT);
    // REG4 for rr_side[i2-1][ipos(3)]
    // REG0 for &rr_side[i2-1][ipos(3)]+5
    VoC_lw16inc_p(REG4, REG0,IN_PARALLEL);

    exit_on_warnings=OFF;
    // save &rr_side[i2-1][0]+i2 to RR_SIDE_DIAG_TEMP_ADDR
    VoC_sw16_d(REG4,RR_SIDE_DIAG_TEMP_ADDR);
    exit_on_warnings=ON;

    // jump if i2<=i3 (&rr_side[i2-1][0]+i2<=&rr_side[i2-1][ipos(3)])


    // loop 8 time, maybe break if i3>=i2

    VoC_loop_i_short(8,DEFAULT);
    VoC_startloop0;
    // jump if i2<=i3 (&rr_side[i2-1][0]+i2+5<=&rr_side[i2-1][ipos(3)]+5)


    VoC_bnlt16_rd(SEARCH_COPY_label1, REG0, RR_SIDE_DIAG_TEMP_ADDR)

    VoC_sw16inc_p(REG4, REG3,DEFAULT);
    // loop count - 1
    VoC_inc_p(REG1,DEFAULT);
    // REG0 is incremented 5(STEP) each time
    VoC_lw16inc_p(REG4, REG0,IN_PARALLEL);
    VoC_endloop0
SEARCH_COPY_label1:

    // REG4 fo rr_diag[i2]
    VoC_lw16_p(REG4, REG5,DEFAULT);
    // jump if i2!=i3 (&rr_side[i2-1][0]+i2+5<=&rr_side[i2-1][ipos(3)]+5)
    VoC_bne16_rd( SEARCH_COPY_label2,REG0,RR_SIDE_DIAG_TEMP_ADDR);
    // loop count -1
    VoC_inc_p(REG1,DEFAULT);
    VoC_sw16inc_p(REG4, REG3,DEFAULT);
SEARCH_COPY_label2:

    VoC_bez16_r(SEARCH_COPY_label_end, REG1)

    // REG4 for i3-i2
    VoC_sub16_rd(REG4,REG0,RR_SIDE_DIAG_TEMP_ADDR);
    // REG4 for i3
    VoC_add16_rr(REG4,REG4,REG2,DEFAULT);
    // REG2 is incremented by 25
    VoC_lw16i_short(INC2,25,IN_PARALLEL);
    // REG0 for i3*i3
    VoC_mult16_rr(REG0,REG4,REG4,DEFAULT);
    VoC_lw16i_short(REG5,5,DEFAULT);
    // REG0 for i3*(i3-1)

    VoC_sub16_rr(REG0,REG0,REG4,DEFAULT);

    // REG0 for (i3*i3-1)/2
    VoC_shr16_ri(REG0,1,DEFAULT);
    // REG0 for & rr_side+(i3*i3-1)/2
    VoC_add16_rd(REG0, REG0, C1035PF_RR_SIDE_ADDR_ADDR);

    // REG0 for & rr_side+i3*(i3-1)/2+i2
    VoC_add16_rr(REG0,REG0,REG2,DEFAULT);
    // REG2 for i3*5
    VoC_mult16_rr(REG2,REG4,REG5,DEFAULT);

    // REG4 for rr_side[i2+5][i3] (rr[i2+5][i3])
    VoC_lw16_p(REG4, REG0,DEFAULT);


    // REG2 for i3*5+10
    VoC_add16_rd(REG2,REG2,CONST_10_ADDR);


    // REG0 for &rr_side[i2+10][i3]
    VoC_add16_rr(REG0, REG0, REG2,DEFAULT);
    // continue loop to 8 times


    VoC_loop_r_short(REG1,DEFAULT)
    // REG2 for 5*i3+10
    VoC_inc_p(REG2,IN_PARALLEL);
    VoC_startloop0
    VoC_lw16_p(REG4, REG0,DEFAULT);
    VoC_add16_rr(REG0, REG0, REG2,IN_PARALLEL);
    exit_on_warnings=OFF;
    VoC_sw16inc_p(REG4, REG3,DEFAULT);
    VoC_inc_p(REG2,IN_PARALLEL);
    exit_on_warnings=ON;
    VoC_endloop0;

SEARCH_COPY_label_end:

    /*compute alp3*/
    VoC_lw16_sd(REG6,3,DEFAULT);            //reg6:i1

    VoC_lw16i_short(RL7_HI,24,IN_PARALLEL);  //rl7_hi:24
    VoC_lw16i_short(RL7_LO,0,DEFAULT);

    VoC_lw16i_short(REG2,2,IN_PARALLEL);        //reg2:loop num
    VoC_lw32_d(REG45,CONST_D_1_8_ADDR); //reg3:&double_1_8


    VoC_lw16i(REG7,_1_16);                  //reg7:_1_16

    VoC_jal     (CII_COMPUTE_ALP1_M74_795);

    /*----------------------------------------------------------------*
     * memorise codevector if this one is better than the last one.   *
     *----------------------------------------------------------------*/
    VoC_lw32_sd(REG45,0,DEFAULT); //psk =-1;    alpk = 1;
    VoC_multf32_rr(ACC0,REG6,REG5,DEFAULT);
    VoC_msu32_rr(REG7,REG4,DEFAULT);
    VoC_sw16_d  (REG2,SEARCH_10I40_I3_ADDR);

    VoC_bngtz32_r(SEARCH_M74_LOOP_I0_END,ACC0)
    VoC_sw32_sd(REG67,0,DEFAULT);
    VoC_lw32_d(ACC0,SEARCH_10I40_I0_ADDR);
    VoC_lw32_d(ACC1,SEARCH_10I40_I0_ADDR+2);
    VoC_sw32_d(ACC0,C1035PF_CODVEC_ADDR);
    VoC_sw32_d(ACC1,C1035PF_CODVEC_ADDR+2);

SEARCH_M74_LOOP_I0_END:
    VoC_lw16i_short(        REG5,5,DEFAULT);
    VoC_lw16i_short(        REG1,40,IN_PARALLEL);
    VoC_add16_rd        (REG4,REG5,SEARCH_10I40_I0_ADDR);
    VoC_bgt16_rr(SEARCH_M74_LOOP_I0,REG1,REG4)
    /*----------------------------------------------------------------*
     * Cyclic permutation of i0,i1,i2 and i3.                         *
     *----------------------------------------------------------------*/
    VoC_lw16_sd (REG0,0,DEFAULT );
    VoC_lw16i_short(REG6,1,IN_PARALLEL);
    VoC_lw16_sd (REG1,1,DEFAULT );
    VoC_lw16_sd (REG2,2,DEFAULT );
    VoC_lw16_sd (REG3,3,DEFAULT );
    VoC_sw16_sd (REG2,3,DEFAULT );
    VoC_sw16_sd (REG1,2,DEFAULT );
    VoC_sw16_sd (REG0,1,DEFAULT );
    VoC_sw16_sd (REG3,0,DEFAULT );

    VoC_add16_rd        (REG0,REG6,SEARCH_10I40_I_ADDR);

    VoC_bnlt16_rd(SEARCH_M74_LOOP_Ia,REG0,CONST_4_ADDR);
    VoC_jump(SEARCH_M74_LOOP_I);
SEARCH_M74_LOOP_Ia:

    VoC_add16_rd        (REG3,REG6,SEARCH_M74_TRACK_ADDR);
    VoC_bnlt16_rd(SEARCH_M74_LOOP_TRACKa,REG3,CONST_5_ADDR);
    VoC_jump(SEARCH_M74_LOOP_TRACK);
SEARCH_M74_LOOP_TRACKa:

    VoC_loop_i_short(5,DEFAULT);
    VoC_startloop0
    VoC_pop16(REG0,DEFAULT);
    VoC_endloop0
    VoC_pop16(RA,DEFAULT);
    VoC_pop32(ACC0,IN_PARALLEL);
    VoC_pop32(ACC0,DEFAULT);
    VoC_return;
}



void CII_code_4i40_17bits_795(void)

{
    VoC_push16(RA, DEFAULT);
//stack16:   RA,  xn2 ,st->h1, st->sharp,   gain_pit, res2,  code, y2, subfrNr, anap , RA

    VoC_lw16_sd(REG0,8,DEFAULT);
    VoC_lw16_sd(REG3,7,DEFAULT);
    VoC_jal(CII_cbseach_subr1);
    VoC_lw16i_short(ACC1_LO,1, DEFAULT);   //ACC1_LO  : sf
    VoC_lw16i_short(REG4,5, DEFAULT);   // REG4  :nb_track & step
    VoC_lw16i_short(REG5,8, DEFAULT);   //REG5  : 40/step
    VoC_jal(CII_cor_h_x);                          // incr0 and  incr1 =1

//  VoC_lw16i_short(REG4,4, DEFAULT);   //8-n

    VoC_lw16i_short(REG3,0,DEFAULT);

    VoC_lw32_d(REG67,CONST_0x80017fff_ADDR);
    //use _sd mode!!
    VoC_lw16i_set_inc(REG0,C1035PF_SIGN_ADDR,1);                    /* REG0 for sign[-1]*/
    VoC_lw16i_set_inc(REG2,C1035PF_DN_ADDR,1);              /* REG2 for dn[] first address */
    VoC_lw16i_set_inc(REG1,C417PF_DN2_ADDR,1);              /* REG1 for dn2[] first address */

    //for 0 and -32767

    VoC_loop_i_short(40,DEFAULT);                       /*    for (i = 0; i < L_CODE; i++)  */

    VoC_startloop0
    //    C1035PF_SIGN_ADDR and     C1035PF_DN_ADDR  should in two RAM!!!
    VoC_lw16_p(REG5, REG2, DEFAULT);
    VoC_sw16_p(REG6,REG0,IN_PARALLEL);

    VoC_bnltz16_r(SET_SIGN101,REG5)
    VoC_sub16_rr(REG5,REG3,REG5,DEFAULT);           /*val = negate (val);*/
    /* modify dn[] according to the fixed sign */
    VoC_sw16_p(REG7,REG0,DEFAULT);          /*sign[i] = -32767; */
SET_SIGN101:
    //REG1    dn2[i]
    VoC_sw16inc_p(REG5,REG1,DEFAULT);           /* dn2[i] = ;*/
    //REG2    dn[i]
    VoC_sw16inc_p(REG5,REG2,DEFAULT);
    VoC_inc_p(REG0,IN_PARALLEL);
    VoC_endloop0

    VoC_lw16i_short(RL6_LO, -1,DEFAULT);
    // _sd  mode
    VoC_lw16i(REG1,C417PF_DN2_ADDR);

    VoC_lw16i_short(INC3,5,DEFAULT);
    VoC_movreg16(REG2, REG1, IN_PARALLEL);
    VoC_lw16i_set_inc(REG0,0,1);                        /* REG0 for i */

SET_SIGN105:

    VoC_loop_i(1,4);
    VoC_movreg16(REG7, REG6, DEFAULT);       // 0x7fff
    VoC_add16_rr(REG3, REG1, REG0, IN_PARALLEL);

    VoC_loop_i_short(8, DEFAULT);
    VoC_startloop0
    VoC_lw16_p(REG5, REG3, DEFAULT);
    VoC_bltz16_r(SET_SIGN103, REG5)
    VoC_bngt16_rr(SET_SIGN103, REG7, REG5)
    VoC_movreg16(REG2, REG3, DEFAULT);
    VoC_movreg16(REG7, REG5, IN_PARALLEL);
SET_SIGN103:
    VoC_inc_p(REG3, DEFAULT);
    VoC_endloop0
    VoC_sw16_p(RL6_LO, REG2, DEFAULT);
    VoC_endloop1

    VoC_inc_p(REG0, DEFAULT);
    VoC_blt16_rd(SET_SIGN105, REG0,CONST_5_ADDR)

    VoC_jal(CII_cor_h);

    VoC_jal(CII_search_4i40_795);

    //  index = build_code(codvec, dn_sign, code, h, y, sign);

    VoC_lw16_sd(REG0, 4, DEFAULT);

    VoC_lw32z(ACC0,DEFAULT);
    VoC_lw16i_short(INC0,2,IN_PARALLEL);
    VoC_loop_i_short(20,DEFAULT);
    VoC_startloop0
    VoC_sw32inc_p(ACC0,REG0,DEFAULT);
    VoC_endloop0


    VoC_lw32_d(RL6, CONST_0x7fff1fff_ADDR);
    VoC_lw32_d(RL7, CONST_0x8000e000_ADDR);

    VoC_lw16i(ACC1_HI,C1035PF_SIGN_ADDR);
    VoC_lw16i_set_inc(REG0,C1035PF_CODVEC_ADDR,1);
    VoC_lw16i_set_inc(REG1,build_code_sign_ADDR,1);
    VoC_lw32z(REG67,DEFAULT);     //    indx = 0;    rsign = 0;
    VoC_lw32z(ACC0,IN_PARALLEL);
    VoC_loop_i(1,4);

    VoC_lw16inc_p(REG4, REG0,DEFAULT);
    //REG4    i
    VoC_multf16_rd(REG6, REG4, CONST_0x199a_ADDR);  // 6554
//REG6   index
    VoC_lw16i_short(REG5, -3,DEFAULT);
    VoC_multf32_rd(REG23,REG6,CONST_5_ADDR);
    VoC_NOP();
    VoC_shr32_ri(REG23, 1, DEFAULT);
    VoC_sub16_rr(REG3,REG4, REG2, DEFAULT);
    VoC_lw16i_set_inc(REG2,STATIC_CONST_GRAY_ADDR,1);
    VoC_add16_rr(REG2, REG6, REG2, DEFAULT);
    VoC_lw16i_set_inc(REG2,C1035PF_SIGN_ADDR,1);
    exit_on_warnings=OFF;
    VoC_lw16_p(REG6, REG2, DEFAULT); //REG6   index
    exit_on_warnings=ON;
    VoC_add16_rr(REG2, REG4, REG2, DEFAULT);


    VoC_be16_rd(build_code_MR74_END, REG3, CONST_1_ADDR)
    VoC_lw16i_short(REG5, -6,DEFAULT);
    VoC_be16_rd(build_code_MR74_END, REG3, CONST_2_ADDR)
    VoC_bne16_rd(build_code_MR74_END_2, REG3, CONST_3_ADDR)
    VoC_lw16i_short(REG5, -10,DEFAULT);
build_code_MR74_END:
    VoC_shr16_rr(REG6, REG5,DEFAULT);
build_code_MR74_END_2:

    VoC_bne16_rd(build_code_MR74_END_1, REG3, CONST_4_ADDR)

    VoC_shr16_ri(REG6, -10,DEFAULT);
    VoC_add16_rd(REG6, REG6, CONST_D_1_64_ADDR);//512
    VoC_lw16i_short(REG3, 3, DEFAULT);
build_code_MR74_END_1:       //REG3   track     REG6   index
//      VoC_jal(CII_cbseach_cala_cod_795);
//stack16:   RA,  xn2 ,st->h1, st->sharp,   gain_pit, res2,  code, y2, subfrNr, anap , RA,RA
    VoC_lw16_p(ACC1_LO, REG2,DEFAULT);
    //ACC0_LO  :  j
    VoC_lw16_sd(REG2, 4,DEFAULT);   // code  incr=1

    VoC_add16_rr(REG2, REG4, REG2, DEFAULT);
    VoC_movreg16(REG4,ACC1_LO,  IN_PARALLEL);
    //REG4   :  j
    VoC_bngtz16_r(build_code_M475_200, REG4)
    VoC_lw16i_short(REG4, 1,DEFAULT);
    VoC_sub16_dr(REG3,CONST_0_ADDR, REG3);
    VoC_shr16_rr(REG4, REG3, DEFAULT);
    VoC_sw16inc_p(RL6_LO, REG2, IN_PARALLEL);
    VoC_add16_rr(REG7, REG4, REG7, DEFAULT);
    //REG7  :  rsign
    VoC_sw16inc_p(RL6_HI, REG1, IN_PARALLEL);
    VoC_jump(build_code_M475_200_end)
build_code_M475_200:
    VoC_sw16inc_p(RL7_HI, REG1, DEFAULT);
    VoC_sw16inc_p(RL7_LO, REG2, DEFAULT);
build_code_M475_200_end:
    VoC_add32_rr(ACC0, ACC0, REG67, DEFAULT);
    VoC_endloop1

    VoC_movreg16(ACC1_HI,REG7,DEFAULT);
    VoC_movreg16(ACC1_LO,ACC0_LO,IN_PARALLEL);
    VoC_lw16_sd(REG7, 8, DEFAULT);           //st->h1
    VoC_lw16_sd(REG0, 3, DEFAULT);         // Y2
    VoC_lw16i_short(REG6,4,IN_PARALLEL);
    /**********************************************
      Function CII_build_code_com2
      input: REG6->NB_PULSE
         REG7->h[]
         REG0(incr=1)->y[]
      Note: This function can used in all mode
    **********************************************/
    VoC_jal(CII_build_code_com2);
    VoC_movreg32(REG67,ACC1,DEFAULT);
// end of build code
    VoC_lw16_sd(REG0,4,DEFAULT);    // CODE
    VoC_lw16_sd(REG3,7,DEFAULT);
    VoC_jal(CII_cbseach_subr1);
    VoC_lw16_sd(REG0, 1, DEFAULT);     //  ANAP
    VoC_pop16(RA, DEFAULT);
    VoC_sw16inc_p(REG6,REG0, DEFAULT);    //sign
    VoC_sw16inc_p(REG7,REG0, DEFAULT);    //index
    VoC_sw16_sd(REG0,0,DEFAULT);    // restore ANAP
    VoC_return;
}







/*``````````````````````````````````````````````````````
PARAMETERS:

REG0:&corr_ptr[-lag_max]    INC 2
REG1:lag_max                INC -1
REG4:number of loops ;
REG5:L_frame/2

OUTPUT: REG7:cor_max
RETURN: REG6:p_max

````````````````````````````````````````````````````````*/

void CII_Lag_max_M475_795(void)
{

    VoC_push16(RA,DEFAULT);
    VoC_lw32_d(RL6,CONST_0x80000000_ADDR);      //RL6 for max
    VoC_lw32inc_p(RL7,REG0,DEFAULT);
    VoC_movreg16(REG6,REG1,IN_PARALLEL);

    VoC_loop_r(1,REG4)
    VoC_bgt32_rr(LAG_MAX02,RL6,RL7)
    VoC_movreg32(RL6,RL7,DEFAULT);
    VoC_movreg16(REG6,REG1,IN_PARALLEL);//REG6:p_max;
LAG_MAX02:;
    VoC_lw32inc_p(RL7,REG0,DEFAULT);
    VoC_inc_p(REG1,IN_PARALLEL);                        //i--
    VoC_endloop1


    /*return p_max*/
    VoC_push16(REG6,DEFAULT);

    /* compute energy */
    VoC_lw16i_set_inc(REG0,PITCH_OL_M475_SCALED_SIGNAL_ADDR+143,2);
    VoC_sub16_rr(REG0,REG0,REG6,DEFAULT);           //&scal_sig[-p_max]
    VoC_lw32z(ACC0,DEFAULT);

    VoC_aligninc_pp(REG0,REG0,DEFAULT);
    VoC_loop_r(1,REG5)
    VoC_bimac32inc_pp(REG0,REG0);
    VoC_endloop1

    /* 1/sqrt(energy) */
    VoC_bez16_d(LAG_MAX_03,GLOBAL_DTX_ADDR)
    /* check tone */
    //RL6:max ACC0:t0
    VoC_add32_rd(REG67,ACC0,CONST_0x00008000_ADDR);
    VoC_bngtz16_r(LAG_MAX_03,REG7)
    //  VoC_lw16i(REG6,21298);
    VoC_multf32_rd(REG67,REG7,CONST_21298_ADDR);

    VoC_lw16i(REG0,0x4000);
    VoC_or16_rd(REG0,STRUCT_VADSTATE1_TONE_ADDR);
    VoC_bngt32_rr(LAG_MAX_03,RL6,REG67)

    //  VoC_NOP();
    VoC_sw16_d(REG0,STRUCT_VADSTATE1_TONE_ADDR);

LAG_MAX_03:

    VoC_movreg32(REG01,ACC0,DEFAULT);               //L_x

    VoC_jal(CII_Inv_sqrt);

    /* max = max/sqrt(energy)  */
    VoC_movreg32(REG45,RL6,DEFAULT);
    VoC_shru16_ri(REG4,1,DEFAULT);
    VoC_shru16_ri(REG0,1,IN_PARALLEL);



    VoC_multf32_rr(ACC0,REG5,REG1,DEFAULT);
    VoC_multf16_rr(REG6,REG5,REG0,IN_PARALLEL);

    VoC_multf16_rr(REG4,REG4,REG1,DEFAULT);
    VoC_lw16i_short(REG7,1,IN_PARALLEL);


    VoC_mac32_rr(REG6,REG7,DEFAULT);
    VoC_mac32_rr(REG4,REG7,DEFAULT);

    VoC_pop16(REG6,DEFAULT);
    VoC_pop16(RA,DEFAULT);
    VoC_movreg16(REG7,ACC0_LO,DEFAULT);
    VoC_return;

}





// only used in mode 515/59/67/74/795/102
void  CII_gainQuant_795(void)
{
//stack16 : mode,frac_coeff,exp_coeff,coeff_lo,coeff,exp_max,code,GC_PREDST,GC_PREDUNQST,ANAP,RA
//          10       9         8        7         6   5       4       3         2         1   0
    VoC_push16(RA,DEFAULT);

    VoC_lw16i_set_inc(REG0, STRUCT_GC_PREDST_ADDR,2);
    VoC_lw16i_set_inc(REG1, STURCT_GC_PREDUNQST_ADDR,2);
    VoC_lw16i_set_inc(REG2, gainQuant_frac_coeff_ADDRESS,1);
    VoC_lw16i_set_inc(REG3, gainQuant_exp_coeff_ADDRESS,1);
    VoC_lw16i(REG4, MR475_gain_quant_exp_max_ADDRESS);
    VoC_lw16i(REG5, MR475_gain_quant_coeff_ADDRESS);
    VoC_lw16i(REG6, MR475_gain_quant_coeff_lo_ADDRESS);

    VoC_sw16_sd(REG2,9,DEFAULT);
    VoC_sw16_sd(REG3,8,DEFAULT);
    VoC_sw16_sd(REG6,7,DEFAULT);
    VoC_sw16_sd(REG5,6,DEFAULT);
    VoC_sw16_sd(REG4,5,DEFAULT);
    VoC_sw16_sd(REG0,3,DEFAULT);
    VoC_sw16_sd(REG1,2,DEFAULT);

    VoC_lw16i_short(REG7,5,DEFAULT);        //reg7  :  mode
    VoC_lw16_sd(REG1, 3, DEFAULT);          // STRUCT_GC_PREDST_ADDR
    VoC_jal(CII_gc_pred);                   //ACC1_HI :frac_en;REG1: exp_en ; REG7: exp_gcode0; REG6:frac_gcode0
    VoC_movreg16(REG0,ACC1_HI, DEFAULT);
    VoC_push32(REG67,DEFAULT);

    VoC_push32(REG01, DEFAULT);
    //stack32:      LO:frac_gcode0    HI: exp_gcode0        LO:frac_en    HI: exp_en

    VoC_lw16_sd(REG0, 9,DEFAULT);       //  gainQuant_frac_coeff_ADDRESS
    VoC_lw16_sd(REG1, 8,DEFAULT);       // gainQuant_exp_coeff_ADDRESS

//  VoC_jal(CII_calc_filt_energies_795);     // REG2:     cod_gain_frac       REG3:    cod_gain_exp
    /***************************************************
     Function: CII_calc_filt_energies_795
     INPUT:  reg0:    frac_coeff  incr=1
             reg1:    exp_coeff  incr=1
     used in all mode except 122
    ****************************************************/
//  VoC_lw32z(RL7,DEFAULT);
    VoC_lw16i_set_inc(REG3, COD_AMR_Y2_ADDRESS, 2);
    VoC_lw16i_set_inc(REG2, calc_filt_energies_Y2_ADDRESS, 2);

    VoC_loop_i_short(20, DEFAULT);
    VoC_lw32inc_p(REG45, REG3,IN_PARALLEL);
    VoC_startloop0
    VoC_shr16_ri(REG4, 3,DEFAULT);
    VoC_shr16_ri(REG5, 3,IN_PARALLEL);
    VoC_lw32inc_p(REG45, REG3,DEFAULT);
    exit_on_warnings=OFF;
    VoC_sw32inc_p(REG45, REG2,DEFAULT);
    exit_on_warnings=ON;
    VoC_endloop0

    VoC_lw16i_set_inc(REG3, COD_AMR_GCOEFF_ADDRESS, 2);
    VoC_lw16i_short(REG6,0,DEFAULT);
    VoC_lw32inc_p(RL6, REG3,DEFAULT);
    VoC_lw32inc_p(REG45, REG3,DEFAULT);
    // reg0:    frac_coeff  incr=1
    // reg1:    exp_coeff  incr=1
    VoC_sw16inc_p(RL6_LO, REG0,DEFAULT);
    VoC_sw16inc_p(RL6_HI, REG1,DEFAULT);
    VoC_sub16_rr(REG4, REG6, REG4,IN_PARALLEL);
    VoC_add16_rd(REG5,  REG5,CONST_1_ADDR);
    VoC_sw16inc_p(REG4, REG0,DEFAULT);
    VoC_sw16inc_p(REG5, REG1,DEFAULT);

    VoC_lw16i_set_inc(REG3, calc_filt_energies_Y2_ADDRESS, 2);
    VoC_jal(CII_calc_filt_energies_subr_795);
    VoC_lw16i_short(REG3,10,DEFAULT);
    VoC_sw16inc_p(ACC0_HI, REG0,DEFAULT);
    VoC_sub16_rr(REG5, REG5,REG3,IN_PARALLEL);
    VoC_lw16i_set_inc(REG3, COD_AMR_XN_ADDRESS, 2);
    VoC_sw16inc_p(REG5, REG1,DEFAULT);

    VoC_jal(CII_calc_filt_energies_subr_795);
    VoC_movreg16(REG6, ACC0_HI,DEFAULT);
    VoC_sw16inc_p(REG5, REG1,IN_PARALLEL);
    VoC_sub16_dr(REG6, CONST_0_ADDR, REG6);
    VoC_lw16i_set_inc(REG3, COD_AMR_Y1_ADDRESS, 2);
    VoC_sw16inc_p(REG6, REG0,DEFAULT);
    VoC_jal(CII_calc_filt_energies_subr_795);
    VoC_sw16inc_p(ACC0_HI, REG0,DEFAULT);
    VoC_sw16inc_p(REG5, REG1,DEFAULT);

//  VoC_jal(calc_475_795_com_795);
    /*********************************************
            Function:calc_475_795_com_795
            only used in mode 475 and 795
    ***********************************************/

    VoC_lw16i_set_inc(REG3, COD_AMR_XN2_ADDRESS, 2);
    VoC_jal(CII_calc_filt_energies_subr_795);
    VoC_movreg16(REG4, ACC0_HI, DEFAULT);
    VoC_lw32z(REG23, DEFAULT);

    VoC_bngtz16_r(label_calc_475_795_com_end, REG4)
    VoC_lw16i_short(REG6, 3,DEFAULT);
    VoC_sub16_rr(REG0, REG0, REG6,DEFAULT);            //frac_coeff[2]
    VoC_sub16_rr(REG1, REG1, REG6,IN_PARALLEL);     //exp_coeff[2]
    VoC_sub16_rd(REG5, REG5, CONST_15_ADDR);
    VoC_shr16_ri(REG4, 1, DEFAULT);
    VoC_sub16_rp(REG3, REG5, REG1, IN_PARALLEL);
    VoC_lw16_p(REG1, REG0, DEFAULT);
    VoC_movreg16(REG0, REG4, IN_PARALLEL);
    VoC_jal(CII_DIV_S);     // input in REG0, REG1   // output in  REG2   // REG0/REG1   // used register RL6, RL7
label_calc_475_795_com_end:

    VoC_lw16i_short(REG5,-1,DEFAULT);

    VoC_jal(CII_MR795_gain_quant);

    VoC_pop32(ACC0,DEFAULT);
    VoC_pop32(ACC0,DEFAULT);
    VoC_lw16_sd(REG5, 3, IN_PARALLEL);     //  STRUCT_GC_PREDST_ADDR

    VoC_jal(CII_gc_pred_update);

    VoC_pop16(RA,DEFAULT);
    VoC_pop16(REG5, DEFAULT);
    VoC_return;
}

void  CII_MR795_gain_code_quant3_subr(void)
{
    VoC_shru16_ri(REG4, 1,DEFAULT);
//  VoC_and16_ri(REG4,0x7fff);           //   L_Extract (L_tmp, &g2_code_h, &g2_code_l);
    // REG4   g2_code_l     REG5    g2_code_h
    VoC_mac32_rp(REG5, REG3, DEFAULT);
    VoC_multf16inc_rp(REG5, REG5, REG2,DEFAULT);
    VoC_multf16inc_rp(REG4, REG4, REG3,DEFAULT);
    VoC_mac32_rr(REG5, REG7,DEFAULT);                       // L_tmp = Mac_32  (L_tmp0, coeff[2], coeff_lo[2],

    VoC_mac32_rr(REG4, REG7,DEFAULT);                    //   g2_code_h, g2_code_l);
    VoC_return;
}




void CII_MR795_gain_code_quant3_subr2(void)
{
    VoC_movreg32( REG45,RL6, DEFAULT);
    VoC_mult16_rd(REG1, REG5, CONST_3_ADDR);
    VoC_lw16i_set_inc(REG0, STATIC_CONST_qua_gain_code_ADDR, 1);
    VoC_add16_rr(REG0, REG1, REG0, DEFAULT);
    VoC_sub16_dr(REG3, CONST_9_ADDR, REG3);
    VoC_multf32inc_rp(ACC0, REG2, REG0, DEFAULT);

    VoC_lw16inc_p(REG7, REG0, DEFAULT);
    VoC_shr32_rr(ACC0, REG3, DEFAULT);

    VoC_lw16inc_p(REG6, REG0, DEFAULT);
//REG6  qua_ener    REG7  qua_ener_MR122
    VoC_sw16_d(ACC0_HI, COD_AMR_GAIN_CODE_ADDRESS);
    VoC_return;
}

/*****************************************************************
  function: CII_MR795_gain_quant
  Input: stack32:      LO:frac_gcode0    HI: exp_gcode0        LO:frac_en    HI: exp_en
  REG2:     cod_gain_frac       REG3:    cod_gain_exp
 OUTPUT:   REG6  qua_ener    REG7  qua_ener_MR122
*******************************************************************/
void  CII_MR795_gain_quant(void)

{
//stack16 : mode,frac_coeff,exp_coeff,coeff_lo,coeff,exp_max,code,GC_PREDST,GC_PREDUNQST,ANAP,RA,RA
//          11       10       9         8        7        6   5       4       3           2   1   0
    VoC_push16(RA, DEFAULT);
    VoC_push32(REG23,DEFAULT);
//  VoC_jal(CII_q_gain_pitch_795);
// incr1 =1
//return  : REG5  is  *gain    REG4 is  index
    VoC_lw32z(REG45,DEFAULT);
    VoC_lw16i_set_inc(REG0, STATIC_CONST_QUA_GAIN_PITCH_ADDR,1);
    VoC_lw16i_set_inc(REG2, 0, 1);
    VoC_lw16i(REG3, 0x7fff);
    VoC_loop_i_short(16, DEFAULT);   // for (i = 1; i < NB_QUA_PITCH; i++)   #define NB_QUA_PITCH  16
    VoC_startloop0
    VoC_lw16inc_p(REG1, REG0, DEFAULT);
    VoC_bgt16_rd(q_gain_pitch_110, REG1, COD_AMR_GP_LIMIT_ADDRESS)
    VoC_sub16_dr(REG1, COD_AMR_GAIN_PIT_ADDRESS, REG1);
    VoC_bgtz16_r(q_gain_pitch_111, REG1)
    VoC_sub16_rr(REG1, REG5, REG1,DEFAULT);
q_gain_pitch_111:
    //REG1:  err
    VoC_bngt16_rr(q_gain_pitch_110, REG3, REG1)
    VoC_movreg16(REG3, REG1, DEFAULT);
    VoC_movreg16(REG4, REG2, IN_PARALLEL);
q_gain_pitch_110:
    VoC_inc_p(REG2, DEFAULT);
    VoC_endloop0
    //REG4  index
    VoC_lw16i_set_inc(REG0, STATIC_CONST_QUA_GAIN_PITCH_ADDR,1);
    VoC_add16_rr(REG1, REG0, REG4, DEFAULT);
    VoC_movreg16(REG3,REG4,  DEFAULT);
    //REG3   ii
    VoC_lw16inc_p(REG5, REG1, DEFAULT);




    VoC_bez16_r(q_gain_pitch_125, REG4)
    VoC_lw16_p(REG6, REG1, DEFAULT);
    VoC_sub16_rd(REG3, REG4, CONST_2_ADDR);

    VoC_be16_rd(q_gain_pitch_125, REG4, CONST_15_ADDR)   // sub (index, NB_QUA_PITCH-1) == 0
    VoC_bgt16_rd(q_gain_pitch_125, REG6, COD_AMR_GP_LIMIT_ADDRESS)
    VoC_sub16_rd(REG3, REG4, CONST_1_ADDR);
q_gain_pitch_125:
    VoC_add16_rr(REG0, REG0, REG3, DEFAULT);
    VoC_lw16i_set_inc(REG1, MR795_gain_quant_g_pitch_cand_ADDRESS,1);
    VoC_lw16i_set_inc(REG2, MR795_gain_quant_g_pitch_cind_ADDRESS,1);

    VoC_loop_i_short(3, DEFAULT);               // for (i = 0; i < 3; i++)
    VoC_lw16i_short(INC3,1,IN_PARALLEL);
    VoC_startloop0                                  //     gain_cind[i] = ii;
    VoC_lw16inc_p(REG6, REG0, DEFAULT);     //     gain_cand[i] = qua_gain_pitch[ii];
    VoC_sw16inc_p(REG3, REG2, DEFAULT);     //     ii = add (ii, 1);
    VoC_inc_p(REG3, IN_PARALLEL);           // }
    VoC_sw16inc_p(REG6, REG1, DEFAULT);
    VoC_endloop0

    VoC_sw16_d(REG5, COD_AMR_GAIN_PIT_ADDRESS);
    //return  : REG4   :  gain_pit_index
    VoC_lw32_sd(REG01, 2,DEFAULT);
    VoC_lw16i_short(REG1, 14, DEFAULT);

    VoC_jal(CII_Pow2);              // gcode0 = extract_l(Pow2(14, frac_gcode0));
    VoC_push16(REG2, DEFAULT);

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*************************************************
    Function :CII_MR795_gain_code_quant3
    INPUT :
           REG2    gcode0
        REG4   :  gain_pit_index
        RL6_LO:    frac_gcode0    RL6_HI:     exp_gcode0
    stack32:    LO:frac_gcode0    HI: exp_gcode0      LO:frac_en    HI: exp_en       LO:cod_gain_frac    HI: cod_gain_exp
    OUTPUT:
    REG6  qua_ener    REG7  qua_ener_MR122
    REG4        gain_pit_ind
    REG5               gain_cod_ind
    *************************************************/
// begin of CII_MR795_gain_code_quant3
//stack16 : mode,frac_coeff,exp_coeff,coeff_lo,coeff,exp_max,code,GC_PREDST,GC_PREDUNQST,ANAP,RA,RA ,gcode0
//          12      11       10       9         8        7        6   5       4           3   2   1   0
    VoC_lw16_sd(REG0, 7, DEFAULT);         //  MR475_gain_quant_exp_max_ADDRESS
    VoC_lw32_sd(REG23, 2,IN_PARALLEL);

    VoC_lw16_sd(REG1, 10, DEFAULT);            //gainQuant_exp_coeff_ADDRESS
    VoC_inc_p(REG3,DEFAULT);

//  VoC_jal(CII_MR475_gain_quant_subr_795);
    VoC_lw16i_short(REG4,11,DEFAULT);
    VoC_sub16_rr(REG3, REG3, REG4,DEFAULT);
    VoC_sub16inc_pd(REG4, REG1, CONST_13_ADDR);
//  VoC_sub16inc_pd(REG5, REG1, CONST_14_ADDR);
    VoC_lw16i_short(REG5,14,DEFAULT);
    VoC_sub16inc_pr(REG5, REG1, REG5,DEFAULT);
    VoC_sw16inc_p(REG4, REG0, DEFAULT);
    VoC_sw16inc_p(REG5, REG0, DEFAULT);
    VoC_movreg16(REG5, REG3,IN_PARALLEL);
    VoC_shr16_ri(REG5, -1,DEFAULT);
    VoC_add16_rd(REG5, REG5, CONST_15_ADDR);
    VoC_add16inc_rp(REG4, REG5, REG1, DEFAULT);

    VoC_add16inc_rp(REG5, REG3, REG1, DEFAULT);
    VoC_sw16inc_p(REG4, REG0, DEFAULT);

    VoC_add16_rd(REG3, REG3, CONST_1_ADDR);
    VoC_add16inc_rp(REG4, REG3, REG1, DEFAULT);
    VoC_sw16inc_p(REG5, REG0, DEFAULT);
    VoC_sw16inc_p(REG4, REG0, DEFAULT);

    VoC_lw16_sd(REG0, 7, DEFAULT);         //  MR475_gain_quant_exp_max_ADDRESS
    VoC_lw16_sd(REG1, 8, DEFAULT);          // MR475_gain_quant_coeff_ADDRESS

    VoC_loop_i_short(4, DEFAULT);
    VoC_lw16inc_p(REG3, REG0, IN_PARALLEL);
    VoC_startloop0
    VoC_lw16inc_p(REG5, REG0, DEFAULT);
    VoC_bngt16_rr(MR475_gain_quant_120, REG5, REG3)
    VoC_movreg16(REG3, REG5, DEFAULT);
MR475_gain_quant_120:
    VoC_lw16_sd(REG2, 9, DEFAULT);               //coeff_lo
    VoC_endloop0
    VoC_lw16_sd(REG0, 7, DEFAULT);         //  MR475_gain_quant_exp_max_ADDRESS
    VoC_add16_rd(REG6, REG3,CONST_1_ADDR); //REG6   e_max

    VoC_lw16_sd(REG3, 11, DEFAULT);        //  gainQuant_frac_coeff_ADDRESS
    VoC_loop_i_short(5, DEFAULT);

    VoC_startloop0
    VoC_sub16inc_rp(REG7, REG6, REG0, DEFAULT);
    VoC_lw32z(REG45,DEFAULT);

    VoC_lw16inc_p(REG5, REG3, DEFAULT);
    VoC_shr32_rr(REG45,REG7, DEFAULT);
    VoC_shru16_ri(REG4,1, DEFAULT);


    VoC_sw16inc_p(REG5, REG1, DEFAULT);
    VoC_sw16inc_p(REG4, REG2, DEFAULT);
    VoC_endloop0

    VoC_lw32_d(RL7, CONST_0x7FFFFFFF_ADDR);         //RL7  :  dist_min

    VoC_lw32z(RL6, DEFAULT);                 // RL6_hi , j     RL6_LO  ,i
    VoC_lw32z(ACC1, IN_PARALLEL);

    VoC_lw16i_set_inc(REG1, MR795_gain_quant_g_pitch_cand_ADDRESS,1);

    VoC_lw16i_short(REG7,1,DEFAULT);

    VoC_loop_i(1, 3)


    VoC_lw16_sd(REG3, 8, DEFAULT);          // MR475_gain_quant_coeff_ADDRESS
    VoC_lw16_p(REG4, REG1, DEFAULT);
    VoC_multf16_rr(REG5, REG4, REG4,DEFAULT); //REG4  g_pitch  REG5  g2_pitch
    VoC_lw16_sd(REG2, 9, DEFAULT);    //coeff_lo
    VoC_multf32inc_rp(ACC0,REG5, REG3, DEFAULT);
    VoC_multf16inc_rp(REG5, REG5, REG2,DEFAULT);
    VoC_lw16i_set_inc(REG0, STATIC_CONST_qua_gain_code_ADDR, 3);
    VoC_mac32_rr(REG5, REG7,DEFAULT);
    VoC_multf16inc_rp(REG5, REG4, REG2,IN_PARALLEL);
    VoC_mac32inc_rp(REG4, REG3, DEFAULT);
    VoC_mac32_rr(REG5, REG7,DEFAULT);

    VoC_loop_i_short(32, DEFAULT);
    VoC_startloop0
    VoC_lw16_sd(REG5,0,DEFAULT);     //REG5   gcode0
    VoC_multf16inc_rp(REG6, REG5, REG0,DEFAULT);   //REG6   g_code
    VoC_push32(REG23,DEFAULT);

    VoC_multf32_rr(REG45, REG6, REG6,DEFAULT);
    VoC_push32(ACC0,IN_PARALLEL);

    VoC_jal(CII_MR795_gain_code_quant3_subr);

    VoC_multf16inc_rp(REG4, REG6, REG2,DEFAULT);
    VoC_mac32inc_rp(REG6, REG3, DEFAULT);             //  L_tmp = Mac_32_16(L_tmp, coeff[3], coeff_lo[3],     g_code);

    VoC_mac32_rr(REG4, REG7,DEFAULT);
    VoC_multf32_rp(REG45, REG6, REG1,IN_PARALLEL);         //  L_tmp = L_mult(g_code, g_pitch);

    VoC_jal(CII_MR795_gain_code_quant3_subr);

    VoC_bngt32_rr(MR795_gain_code_quant3_100, RL7,ACC0 )
    VoC_movreg32(RL7, ACC0, DEFAULT);
    VoC_movreg32(RL6,ACC1,  IN_PARALLEL);        // RL6_hi ,i     RL6_LO  ,j
MR795_gain_code_quant3_100:
    VoC_lw16i_short(ACC0_HI,1,DEFAULT);
    VoC_lw16i_short(ACC0_LO,0,IN_PARALLEL);

    //  VoC_add32_rd(ACC1, ACC1, CONST_0x10000_ADDR);

    VoC_pop32(ACC0,DEFAULT);
    VoC_add32_rr(ACC1, ACC1,ACC0,IN_PARALLEL);

    VoC_pop32(REG23,DEFAULT);
    VoC_endloop0

    VoC_inc_p(REG1, DEFAULT);
    VoC_lw16i_short(ACC1_HI,0,IN_PARALLEL);
    VoC_add32_rd(ACC1, ACC1, CONST_1_ADDR);
    VoC_endloop1

    VoC_lw32_sd(REG23,2, DEFAULT);      //REG3   exp_gcode0
    VoC_lw16_sd(REG2,0,DEFAULT);

    VoC_jal(CII_MR795_gain_code_quant3_subr2);

    VoC_lw16i_set_inc(REG0, MR795_gain_quant_g_pitch_cand_ADDRESS, 1);
    VoC_push32(REG67, DEFAULT);
    VoC_add16_rr(REG0, REG4, REG0, IN_PARALLEL);
    VoC_lw16i_set_inc(REG1, MR795_gain_quant_g_pitch_cind_ADDRESS, 1);
    VoC_add16_rr(REG1, REG4, REG1, DEFAULT);


    VoC_lw16_p(REG7, REG0, DEFAULT); // REG0   gain_pit
    VoC_lw32z(ACC0, IN_PARALLEL);
    VoC_lw16_p(REG4, REG1, DEFAULT);      //REG4   gain_pit_ind
    VoC_lw32z(ACC1, IN_PARALLEL);
    VoC_sw16_d(REG7, COD_AMR_GAIN_PIT_ADDRESS);

// end of CII_MR795_gain_code_quant3
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    VoC_push32(REG45, DEFAULT);
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// begin of CII_calc_unfilt_energies
//stack16 : mode,frac_coeff,exp_coeff,coeff_lo,coeff,exp_max,code,GC_PREDST,GC_PREDUNQST,ANAP,RA, RA ,gcode0
//          12      11       10       9         8        7      6    5        4           3   2   1   0

    VoC_lw32z(RL6, IN_PARALLEL);
    VoC_lw16_sd(REG0,6,DEFAULT);
    VoC_lw32z(RL7, IN_PARALLEL);
    VoC_lw16d_set_inc(REG1, COD_AMR_EXC_ADDR_ADDRESS, 1);
    VoC_lw16i_set_inc(REG2, COD_AMR_RES3_ADDRESS, 1);

    VoC_loop_i_short(40,DEFAULT);
    VoC_lw16i_short(REG6,15,IN_PARALLEL);
    VoC_startloop0
    VoC_mac32inc_pp(REG2,REG2,DEFAULT);
    VoC_add32_rr(RL6,RL6,RL7,DEFAULT);
    VoC_lw16_p(REG4,REG1,DEFAULT);
    VoC_mac32inc_pp(REG1,REG1,IN_PARALLEL);
    VoC_multf32inc_rp(RL7,REG4,REG0,DEFAULT);
    VoC_endloop0

    VoC_lw16i_set_inc(REG0, MR795_gain_quant_frac_en_ADDRESS, 1);
    VoC_lw16i_set_inc(REG3, MR795_gain_quant_exp_en_ADDRESS, 1);

    VoC_add32_rr(RL6,RL6,RL7,DEFAULT);
    VoC_lw32z(REG45,IN_PARALLEL);
    VoC_lw16i(REG4, 400);



    VoC_bngt32_rr(calc_unfilt_energies_100, REG45, ACC0);

    VoC_lw32z(ACC0, DEFAULT);
    VoC_lw16i_short(REG1, -15,IN_PARALLEL);

    VoC_jump(calc_unfilt_energies_1001)

calc_unfilt_energies_100:
    VoC_jal(CII_NORM_L_ACC0);
    VoC_sub16_rr(REG1, REG6,REG1,DEFAULT);

calc_unfilt_energies_1001:

    VoC_sw16inc_p(ACC0_HI, REG0, DEFAULT);
    VoC_sw16inc_p(REG1, REG3, DEFAULT);
    VoC_movreg32(ACC0,ACC1,IN_PARALLEL);

    VoC_loop_i(1,2);
    VoC_jal(CII_NORM_L_ACC0);
    VoC_sub16_rr(REG1,REG6,REG1,DEFAULT);
    VoC_sw16inc_p(ACC0_HI, REG0, DEFAULT);
    VoC_sw16inc_p(REG1, REG3, DEFAULT);

    VoC_movreg32(ACC0,RL6,IN_PARALLEL);
    VoC_lw16i_short(REG6,2,DEFAULT);
    VoC_endloop1

    VoC_lw16d_set_inc(REG2, COD_AMR_EXC_ADDR_ADDRESS, 1);
    VoC_lw16i_set_inc(REG1, COD_AMR_RES3_ADDRESS, 1);

    VoC_multf32inc_rp(REG45, REG7, REG2, DEFAULT);//  REG7->GAIN_PIT

    VoC_loop_i_short(40, DEFAULT);
    VoC_lw32z(ACC0, IN_PARALLEL);
    VoC_startloop0
    VoC_shr32_ri(REG45, -1, DEFAULT);
    VoC_add32_rd(REG45, REG45, CONST_0x00008000_ADDR);
    VoC_sub16inc_pr(REG5, REG1, REG5, DEFAULT);
    VoC_multf32inc_rp(REG45, REG7, REG2, DEFAULT);//  REG7->GAIN_PIT
    exit_on_warnings =OFF;
    VoC_mac32_rr(REG5, REG5, DEFAULT);
    exit_on_warnings =ON;
    VoC_endloop0

    VoC_jal(CII_NORM_L_ACC0);
    VoC_sub16_dr(REG1, CONST_15_ADDR, REG1);
    VoC_sw16inc_p(ACC0_HI, REG0, DEFAULT);
    VoC_sw16inc_p(REG1, REG3, DEFAULT);

    VoC_movreg16(REG4, REG1, IN_PARALLEL);//REG4   exp
    VoC_movreg16(REG1, ACC0_HI, DEFAULT);  //   REG1     ltp_res_en
    VoC_lw16_d(REG0, MR795_gain_quant_frac_en_ADDRESS);  //REG0   frac_en[0]
    VoC_lw16i_short(REG7, 0, DEFAULT); //   REG7     ltpg

    VoC_bngtz16_r(calc_unfilt_energies_200, REG1)
    VoC_bez16_r(calc_unfilt_energies_200, REG0)

    VoC_shr16_ri(REG0, 1,DEFAULT);
    VoC_jal(CII_DIV_S); //REG2   pred_gain
    VoC_sub16_rd(REG4, REG4, MR795_gain_quant_exp_en_ADDRESS); //   REG4    exp
    VoC_add16_rd(REG4, REG4, CONST_3_ADDR);
    VoC_movreg16(ACC0_HI, REG2, DEFAULT);
    VoC_lw16i_short(ACC0_LO, 0, IN_PARALLEL);

    VoC_shr32_rr(ACC0, REG4, DEFAULT);
    VoC_jal(CII_Log2);           //REG4:ltpg_exp;   REG5:ltpg_frac
    VoC_lw16i_short(REG6,27,DEFAULT);
    VoC_multf32_rd(ACC0, REG5,CONST_1_ADDR);

    VoC_sub16_rr(REG7, REG4, REG6,DEFAULT);
    VoC_movreg16(ACC0_HI, REG7, DEFAULT);
    VoC_shr32_ri(ACC0, -13, DEFAULT);
    VoC_add32_rd(REG67, ACC0, CONST_0x00008000_ADDR);// REG7     ltpg
calc_unfilt_energies_200:
// end of CII_calc_unfilt_energies
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//   begin of CII_gain_adapt        //  REG7     alpha
    VoC_lw16i_short(REG6, 0, DEFAULT);                                             //#define LTP_GAIN_THR1 2721 /* 2721 Q13 = 0.3322 ~= 1.0 / (10*log10(2)) */
    VoC_lw16i_short(REG1,1,DEFAULT);

    VoC_bngt16_rd(gain_adapt_100, REG7, (1076+STATIC_mr795_1_lsf_compressed_ADDR))         //2721  #define LTP_GAIN_THR2 5443 /* 5443 Q13 = 0.6644 ~= 2.0 / (10*log10(2)) */
    VoC_lw16i_short(REG6, 1, DEFAULT);

    VoC_bngt16_rd(gain_adapt_100, REG7, CONST_5443_ADDR)
    VoC_lw16i_short(REG6, 2, DEFAULT);    //REG6    adapt
gain_adapt_100:

    VoC_lw16i_short(REG4, 0, DEFAULT);
    VoC_lw16_d(REG5, COD_AMR_GAIN_CODE_ADDRESS);
    VoC_shr32_ri(REG45, 1, DEFAULT);
    VoC_movreg16(REG0,REG5,IN_PARALLEL);
    VoC_add32_rd(REG45,REG45,CONST_0x00008000_ADDR);

    VoC_lw16_d(REG4, STRUCT_GAINADAPTSTATE_ONSET_ADDR);  //REG4     st->onset
    VoC_bngt16_rd(gain_adapt_200, REG5, STRUCT_GAINADAPTSTATE_PREV_GC_ADDR)


    VoC_bngt16_rd(gain_adapt_200, REG0,(24+19+TABLE_MR102_ADDR)) //200
    VoC_lw16i_short(REG4, 8, DEFAULT);
    VoC_jump(gain_adapt_200_end)
gain_adapt_200:

    VoC_bez16_r(gain_adapt_200_end, REG4)
    VoC_sub16_rr(REG4, REG4, REG1,DEFAULT);

gain_adapt_200_end:
    VoC_lw16i_short(REG5, 5, DEFAULT);
    VoC_sw16_d(REG0, STRUCT_GAINADAPTSTATE_PREV_GC_ADDR);

    VoC_lw16i_set_inc(REG0, STRUCT_GAINADAPTSTATE_LTPG_MEM_ADDR, 1);
    VoC_bez16_r(gain_adapt_300, REG4)
    VoC_bnlt16_rd(gain_adapt_300, REG6, CONST_2_ADDR)
    VoC_add16_rr(REG6 , REG6 , REG1,DEFAULT);
gain_adapt_300:

    VoC_sw16_p(REG7, REG0,DEFAULT);
    VoC_sw16_d(REG4, STRUCT_GAINADAPTSTATE_ONSET_ADDR);



    VoC_push16(REG6, DEFAULT);

    VoC_jal(CII_gmed_n);    //  REG1    filt
    VoC_pop16(REG6, DEFAULT);
    VoC_lw16i_short(REG7, 0,DEFAULT); //REG7   result

    VoC_bnez16_r(gain_adapt_400, REG6)
    VoC_bgt16_rd(gain_adapt_400, REG1, CONST_5443_ADDR)
    VoC_lw16i(REG7, 16384);
    VoC_shr16_ri(REG1, -2, DEFAULT);
    VoC_multf16_rd(REG1, REG1, CONST_24660_ADDR);
    VoC_bltz16_r(gain_adapt_400, REG1)
//  VoC_NOP();
    VoC_sub16_rr(REG7, REG7, REG1,DEFAULT);//REG7   result
gain_adapt_400:

    VoC_lw16i_set_inc(REG0, STRUCT_GAINADAPTSTATE_LTPG_MEM_ADDR+4, -1);
    VoC_lw16i_set_inc(REG1, STRUCT_GAINADAPTSTATE_LTPG_MEM_ADDR+3, -1);

    VoC_bnez16_d(gain_adapt_500, STRUCT_GAINADAPTSTATE_PREV_ALPHA_ADDR)
    VoC_shr16_ri(REG7,1, DEFAULT);
gain_adapt_500:
    VoC_lw16inc_p(REG3, REG1,DEFAULT);
    VoC_sw16_d(REG7, STRUCT_GAINADAPTSTATE_PREV_ALPHA_ADDR);//REG7    alpha

    VoC_loop_i_short(4, DEFAULT);

    VoC_startloop0
    VoC_lw16inc_p(REG3, REG1,DEFAULT);
    exit_on_warnings=OFF;
    VoC_sw16inc_p(REG3, REG0,DEFAULT);
    exit_on_warnings=ON;
    VoC_endloop0
// end of CII_gain_adapt
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    VoC_movreg16(REG1, REG7, DEFAULT);      //REG1     alpha
    VoC_pop32(REG23, IN_PARALLEL);      //REG2   gain_pit_ind ;   REG3    gain_cod_ind
    VoC_pop32(REG67, DEFAULT);

    VoC_pop32(REG45, DEFAULT);      // REG4:     cod_gain_frac       REG5:    cod_gain_exp
    VoC_pop16(REG0, IN_PARALLEL);           //  REG0:          gcode0

    VoC_bez16_d(MR795_gain_quant_L9, MR795_gain_quant_frac_en_ADDRESS)
    VoC_bgtz16_r(MR795_gain_quant_L10, REG1)
MR795_gain_quant_L9:
//  VoC_NOP();
    VoC_jump(MR795_gain_quant_100);
MR795_gain_quant_L10:

    VoC_push16(REG2, DEFAULT);          //REG2        gain_pit_ind

    VoC_lw32_sd(RL7,0,DEFAULT);
    VoC_lw32_sd(REG23,1,DEFAULT);  // REG3   exp_gcode0

    VoC_sw16_d(RL7_LO, MR795_gain_quant_frac_en_ADDRESS+3);
    VoC_sw16_d(RL7_HI, MR795_gain_quant_exp_en_ADDRESS+3);


    VoC_sub16_rr(REG5, REG3, REG5, DEFAULT);
    VoC_lw32_sd(RL6,1,IN_PARALLEL);

    VoC_sub16_rd(REG5, REG5, CONST_10_ADDR);
    VoC_shr16_rr(REG4, REG5, DEFAULT); //REG4    gain_cod_unq     REG1   alpha      REG0:          gcode0
//stack32:    LO:frac_gcode0    HI: exp_gcode0      LO:frac_en    HI: exp_en
//stack16 : mode,frac_coeff,exp_coeff,coeff_lo,coeff,exp_max,code,GC_PREDST,GC_PREDUNQST,ANAP,RA, RA ,gain_pit_ind
//          12      11       10       9         8        7      6    5        4           3   2   1   0

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//  begin of CII_MR795_gain_code_quant_mod
    VoC_movreg16(REG7, RL6_HI,IN_PARALLEL);  //REG7   exp_gcode0

    VoC_lw16_d(REG5, COD_AMR_GAIN_CODE_ADDRESS);
    VoC_sub16_rd(REG7, REG7,CONST_10_ADDR );
    VoC_shr16_rr(REG5, REG7, DEFAULT);  //REG5     gain_code
    VoC_movreg32(RL7, REG01, DEFAULT);  // RL7_lo   gcode0   RL7_hi   alpha
    VoC_push32(REG45, DEFAULT);     //    REG4    gain_cod_unq          REG5:       gain_code
    VoC_lw16_d(REG5, COD_AMR_GAIN_PIT_ADDRESS);
    VoC_multf16_rr(REG5, REG5, REG5, DEFAULT);//      REG5   g2_pitch

    VoC_lw16i_set_inc(REG0, MR795_gain_quant_frac_en_ADDRESS+1, 1);
    VoC_lw16i_set_inc(REG1, MR795_gain_quant_exp_en_ADDRESS+1, 1);

    VoC_lw16_sd(REG2,8,DEFAULT);    //coeff
    VoC_lw16i_short(REG6,2,IN_PARALLEL);
    VoC_lw16_sd(REG3,9,DEFAULT);    //coeff_lo
    VoC_lw16i_short(REG7,2,IN_PARALLEL);
    VoC_add32_rr(REG23,REG23,REG67,DEFAULT);
    VoC_movreg16(REG4,RL7_HI, IN_PARALLEL);     //  REG4    alpha
    VoC_multf32inc_rp(REG67, REG4, REG0, DEFAULT);
    VoC_sw16_sd(REG3,9,DEFAULT);    //coeff_lo+2
    VoC_shr32_ri(REG67, -1,DEFAULT);    //REG7 : tmp
    VoC_sw16_sd(REG2,8,DEFAULT);    //coeff+2
    VoC_multf32_rr(ACC1, REG7, REG5, IN_PARALLEL);//    ACC1 :  L_t1

    VoC_lw16i_set_inc(REG3, MR475_gain_quant_exp_max_ADDRESS+1, 1);
    VoC_sub16inc_pd(REG5, REG1, CONST_15_ADDR);

    VoC_multf32inc_rp(REG67, REG4, REG0, DEFAULT);
    VoC_sw16inc_p(REG5, REG3, DEFAULT);               //   exp_coeff[1] = sub (exp_en[1], 15);
    VoC_shr32_ri(REG67, -1,DEFAULT);

    VoC_multf16_rd(REG7, REG7, COD_AMR_GAIN_PIT_ADDRESS);
    VoC_movreg16(REG5, RL6_HI,DEFAULT); // REG5         exp_gcode0
    VoC_sub16_rd(REG6, REG5, CONST_10_ADDR);
    VoC_sw16inc_p(REG7, REG2, DEFAULT);
    VoC_add16inc_rp(REG6, REG6, REG1, DEFAULT);    //

    VoC_shr16_ri(REG5, -1,DEFAULT);
    VoC_sw16inc_p(REG6, REG3, DEFAULT);

    VoC_multf32_rp(REG67, REG4, REG0, DEFAULT);
    VoC_sub16_rd(REG5, REG5, CONST_7_ADDR);
    VoC_shr32_ri(REG67, -1,DEFAULT);

    VoC_add16inc_rp(REG5, REG5, REG1, DEFAULT);
    VoC_sw16inc_p(REG7, REG2, DEFAULT);
    VoC_sw16_p(REG5, REG3, DEFAULT);
    VoC_lw16i_short(REG6,1,IN_PARALLEL);

    VoC_sub16_dr(REG7, CONST_0x00007FFF_ADDR, REG4);
    VoC_add16_rr(REG7, REG7, REG6,DEFAULT); //  REG7   one_alpha
    VoC_multf16inc_rp(REG7, REG7, REG0, DEFAULT);
    VoC_add16inc_rp(REG6, REG6,REG3, DEFAULT);
    VoC_multf32_rd(ACC0, REG4, MR795_gain_quant_frac_en_ADDRESS); //    ACC0  :   L_tmp
    VoC_sw16inc_p(REG7, REG2, DEFAULT);
    VoC_sw16inc_p(REG6, REG3, DEFAULT);

    //INPUT:  acc0  L_x
    VoC_jal(CII_sqrt_l_exp);
    //OUTPUT: REG1    exp ;RETURN:   ACC0


    VoC_add16_rd(REG1, REG1, (28+TABLE_MR475_ADDR)); //47
    VoC_sub16_dr(REG1, MR795_gain_quant_exp_en_ADDRESS, REG1);
    VoC_lw16i_set_inc(REG2, MR475_gain_quant_exp_max_ADDRESS, 1);

    VoC_add16_rd(REG5, REG1, (91+TABLE_MR475_ADDR));    //VoC_lw16i(REG5,31);   REG5  e_max


    VoC_sw16inc_p(REG1, REG2,DEFAULT);

    VoC_loop_i_short(4, DEFAULT);
    VoC_lw16_p(REG3, REG2, IN_PARALLEL);        //  REG3     exp_coeff[1]
    VoC_startloop0
    VoC_lw16inc_p(REG1, REG2, DEFAULT);
    VoC_bngt16_rr(MR795_gain_code_quant_mod_100, REG1, REG5)
    VoC_movreg16(REG5, REG1,DEFAULT);
MR795_gain_code_quant_mod_100:
    VoC_movreg16(RL6_LO, RL7_LO, DEFAULT);  // RL7_lo   gcode0   //RL6_LO :  gcode0    RL6_HI:     exp_gcode0
    VoC_endloop0

    VoC_sub16_rr(REG6, REG5, REG3, DEFAULT); //REG6     tmp
    VoC_lw32_d(RL7, CONST_0x7FFFFFFF_ADDR);

    VoC_lw16_sd(REG2,8,DEFAULT);    //coeff+2
    VoC_lw16_sd(REG3,9,DEFAULT);    //coeff_lo+2

    VoC_lw16i_set_inc(REG1, MR475_gain_quant_exp_max_ADDRESS+2, 1);

    VoC_loop_i_short(3, DEFAULT);
    VoC_shr32_rr(ACC1, REG6, IN_PARALLEL);
    VoC_startloop0
    VoC_xor16_rr(REG6, REG6, DEFAULT);
    VoC_sub16inc_rp(REG4,  REG5, REG1,IN_PARALLEL); //REG1     -tmp
    VoC_lw16_p(REG7, REG2, DEFAULT);
    VoC_shr32_rr(REG67, REG4, DEFAULT);
    VoC_shru16_ri(REG6, 1, DEFAULT);

    VoC_sw16inc_p(REG7, REG2, DEFAULT);
    VoC_sw16inc_p(REG6, REG3, DEFAULT);
    VoC_endloop0
//  VoC_lw16i(REG6,31);
    VoC_lw16i_short(REG7,1,DEFAULT);
    VoC_sub16_rd(REG5, REG5, (91+TABLE_MR475_ADDR));//31

    VoC_sub16_rd(REG6, REG5, MR475_gain_quant_exp_max_ADDRESS);

    VoC_and16_rr(REG7,REG6,DEFAULT);
    VoC_shr16_ri(REG6, 1,DEFAULT);
    VoC_shr32_rr(ACC0, REG6,DEFAULT);
    VoC_lw16i_short(REG5,1,IN_PARALLEL);

    VoC_lw16i_set_inc(REG2, STATIC_CONST_qua_gain_code_ADDR, 3);
    VoC_push32(RL6, DEFAULT);
    VoC_lw16i_short(REG3, 0,IN_PARALLEL );

    VoC_bez16_r(MR795_gain_code_quant_mod_200, REG7)
    VoC_movreg32(REG67, ACC0, DEFAULT);
    VoC_shru16_ri(REG6, 1,DEFAULT);
//  VoC_and16_rr(REG6,REG0,DEFAULT);    //reg7  coeff[0]         REG6  coeff_lo[0]
    VoC_multf16_rd(REG6, REG6, CONST_0x5a82_ADDR);
    VoC_multf32_rd(ACC0, REG7, CONST_0x5a82_ADDR);        ///23170
    VoC_mac32_rr(REG6, REG5,DEFAULT);
MR795_gain_code_quant_mod_200:
    VoC_push32(ACC1,DEFAULT);   //ACC0   L_t1      ACC1    L_t0
    VoC_movreg16(RL6_HI, REG3, DEFAULT);
    VoC_sub32_rd(ACC1, ACC0,CONST_0x00008000_ADDR);

    VoC_loop_i(1, 32)
    VoC_lw16_sd(REG0,8,DEFAULT);    //coeff+2
    VoC_lw32_sd(REG67, 2,IN_PARALLEL);  // reg7->gain_code
    VoC_lw16_sd(REG1,9,DEFAULT);    //coeff_lo+2
    VoC_movreg16(REG6, RL6_LO, IN_PARALLEL);

    VoC_multf16inc_rp(REG4, REG6, REG2, DEFAULT);      //   REG4    g_code
    VoC_lw32_sd(ACC0,0,DEFAULT);
    VoC_NOP();
    VoC_bngt16_rr(MR795_gain_code_quant_mod_300, REG7, REG4)
    VoC_multf16inc_rp(REG6, REG4, REG1,DEFAULT);
    VoC_mac32inc_rp(REG4, REG0,DEFAULT);

    VoC_multf32_rr(REG67,REG4, REG4, IN_PARALLEL);                    //   L_tmp = L_mult (g_code, g_code);
    exit_on_warnings =OFF;
    VoC_mac32_rr(REG6, REG5,DEFAULT);        //  L_tmp = Mac_32_16 (L_t1, coeff[2], coeff_lo[2], g_code);
    exit_on_warnings =ON;
    VoC_mac32_rp(REG7, REG0,DEFAULT);
    VoC_shru16_ri(REG6, 1, IN_PARALLEL);  //REG6  g2_code_l   REG7   g2_code_h

    VoC_multf16inc_rp(REG6, REG6, REG0,DEFAULT);
    VoC_multf16inc_rp(REG7, REG7, REG1,DEFAULT);
    VoC_mac32_rr(REG6, REG5,DEFAULT);

    VoC_mac32_rr(REG7, REG5,DEFAULT);
    VoC_push32(REG01, IN_PARALLEL);

    VoC_jal(CII_sqrt_l_exp);//REG1: exp   acc0: L_tmp
    VoC_shr16_ri(REG1, 1, DEFAULT);
    VoC_shr32_rr(ACC0, REG1,DEFAULT);                           //     L_tmp = L_shr (L_tmp, shr (exp, 1));
    VoC_pop32(REG01, IN_PARALLEL);

    VoC_sub32_rr(REG67, ACC0, ACC1, DEFAULT);          //     tmp = round (L_sub (L_tmp, L_t0));
    VoC_multf32_rr(ACC0, REG7, REG7,DEFAULT)    ;            //  L_tmp = L_mult (tmp, tmp);
    VoC_lw32_sd(REG67, 2,IN_PARALLEL);  // REG6->gain_cod_unq
    VoC_sub16_rr(REG4, REG4, REG6, DEFAULT);            //tmp = sub (g_code, gain_cod_unq);
    VoC_multf32_rr(REG67, REG4, REG4,DEFAULT)   ; //   L_tmp = L_mult (tmp, tmp);
    VoC_NOP();
    VoC_shru16_ri(REG6, 1, DEFAULT);
    VoC_mac32_rp(REG7, REG0,DEFAULT);

    //REG6  d2_code_l   REG7   d2_code_h           //L_Extract (L_tmp, &d2_code_h, &d2_code_l);

    VoC_multf16inc_rp(REG6, REG6, REG0,DEFAULT);
    VoC_multf16inc_rp(REG7, REG7, REG1,DEFAULT);
    VoC_mac32_rr(REG6, REG5,DEFAULT);
    VoC_mac32_rr(REG7, REG5,DEFAULT);  //    L_tmp = Mac_32(L_tmp, coeff[4], coeff_lo[4], d2_code_h, d2_code_l);

    VoC_NOP();
    VoC_bngt32_rr(MR795_gain_code_quant_mod_400, RL7, ACC0)
    VoC_movreg32(RL7, ACC0, DEFAULT);
    VoC_movreg16(RL6_HI, REG3, IN_PARALLEL);        //RL6_HI  :  index
MR795_gain_code_quant_mod_400:
    VoC_inc_p(REG3,DEFAULT);
    VoC_endloop1

MR795_gain_code_quant_mod_300:
    VoC_pop32(ACC0,DEFAULT);
    VoC_pop32(REG23, DEFAULT);  //REG2   gcode0         REG3    exp_gcode0
    VoC_pop32(REG45,DEFAULT);

    VoC_jal(CII_MR795_gain_code_quant3_subr2);
    VoC_pop16(REG2, DEFAULT);
    VoC_movreg16(REG3,REG5,IN_PARALLEL);
//  end of CII_MR795_gain_code_quant_mod
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

MR795_gain_quant_100:
    VoC_lw16_sd(REG0,2,DEFAULT);           // COD_AMR_ANAP_ADDR
    VoC_lw16i_short(INC0,1,IN_PARALLEL);
    VoC_pop16(RA, DEFAULT);
    VoC_sw16inc_p(REG2, REG0, DEFAULT);//REG2        gain_pit_ind
    VoC_sw16inc_p(REG3, REG0, DEFAULT); // REG3               gain_cod_ind
    VoC_sw16_sd(REG0,1,DEFAULT);  // restore ANAP
    VoC_return;

}



void CII_calc_filt_energies_subr_795(void)
{
    //called by CII_calc_filt_energies_795
    VoC_push16(RA, DEFAULT);
    VoC_lw16i_set_inc(REG2,calc_filt_energies_Y2_ADDRESS,2);

    VoC_loop_i_short(20, DEFAULT);
//  VoC_movreg32(ACC0, RL7, IN_PARALLEL);
    VoC_lw32z(ACC0,IN_PARALLEL);
    VoC_startloop0
    VoC_bimac32inc_pp(REG2, REG3);
    VoC_endloop0

    VoC_movreg16(REG2,REG1, DEFAULT);
    VoC_jal(CII_NORM_L_ACC0);
    VoC_sub16_dr(REG5,  CONST_7_ADDR,REG1);

    VoC_pop16(RA, DEFAULT);
    VoC_movreg16(REG1,REG2, DEFAULT);
    VoC_return;
}

