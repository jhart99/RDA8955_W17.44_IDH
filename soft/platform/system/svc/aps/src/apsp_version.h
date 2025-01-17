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

#ifndef _APS_VERSION_H_
#define _APS_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define APS_VERSION_REVISION                     (0x6806430c)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef APS_VERSION_NUMBER
#define APS_VERSION_NUMBER                       (0)
#endif

#ifndef APS_VERSION_DATE
#define APS_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef APS_VERSION_STRING
#define APS_VERSION_STRING                       "APS version string not defined"
#endif

#ifndef APS_VERSION_STRING_WITH_BRANCH
#define APS_VERSION_STRING_WITH_BRANCH           APS_VERSION_STRING " Branch: " "8955.W17.44"
#endif

#define APS_VERSION_STRUCT                       {APS_VERSION_REVISION, \
                                                  APS_VERSION_NUMBER, \
                                                  APS_VERSION_DATE, \
                                                  APS_VERSION_STRING_WITH_BRANCH}

#endif // _APS_VERSION_H_
