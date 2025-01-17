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


#ifdef CHIP_HAS_AP

#ifdef FPGA_8810_RF_V1
#include "drv_pasw_calib_fpga.h"
#else
#include "drv_pasw_calib_modem.h"
#endif

#else // !CHIP_HAS_AP

#include "drv_pasw_calib_default.h"

#endif // !CHIP_HAS_AP

