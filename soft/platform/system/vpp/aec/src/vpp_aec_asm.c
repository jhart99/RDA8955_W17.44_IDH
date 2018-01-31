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


















#include "cs_types.h"
#include "vppp_aec_asm_map.h"


#define RAM_X_BEGIN_ADDR               0x0000
#define RAM_Y_BEGIN_ADDR               0x4000

#define TO32(lo,hi)                    ((lo & 0xffff) | ((hi &0xffff) << 16))

#ifndef MICRON_ALIGN

#define MICRON_ALIGN(a)

#endif


// ******************************
//            COMMON
// ******************************

CONST INT32 g_vppHelloCommonCode[] MICRON_ALIGN(32)=
{
#include "vpp_aec_asm_common.tab"
};

CONST INT32 g_vppHelloCommonCodeSize MICRON_ALIGN(32)= sizeof(g_vppHelloCommonCode);


// ******************************
//         OTHER CODE
// ******************************

CONST INT32 g_vppHelloHiCode[] MICRON_ALIGN(32)=
{
#include "vpp_aec_asm_hi.tab"
};

CONST INT32 g_vppHelloByeCode[] MICRON_ALIGN(32)=
{
#include "vpp_aec_asm_bye.tab"
};

// ******************************
//            CONST
// ******************************

CONST INT32 g_vppHelloConst[] MICRON_ALIGN(32)=
{
#include "vpp_aec_const.tab"
};