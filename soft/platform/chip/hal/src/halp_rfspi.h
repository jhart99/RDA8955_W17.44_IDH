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



#include "hal_rfspi.h"
#include "hal_sys.h"


#ifndef _HALP_RFSPI_H_
#define _HALP_RFSPI_H_


// =============================================================================
// hal_RfspiIrqHandler
// -----------------------------------------------------------------------------
/// RF-SPI module IRQ handler.
/// This function is used by the private IRQ module to clear the IRQ and call
/// the user handler with the proper status.
// =============================================================================
PROTECTED VOID hal_RfspiIrqHandler(UINT8 interruptId);

#endif // _HALP_RFSPI_H_




