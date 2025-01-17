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


#include "hal_debug.h"
#include "halp_debug.h"
#include "hal_debugpaltrc.h"

// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================






// =============================================================================
// hal_SoftAbort
// -----------------------------------------------------------------------------
/// FIXME
// =============================================================================
PUBLIC VOID hal_SoftAbort(VOID)
{
    HAL_PROFILE_PULSE(XCPU_ERROR);
    hal_DbgFatalTrigger(0);
#ifndef PAL_NO_DBGTRC
    hal_DbgPalFlushTrace ();
#endif
}




