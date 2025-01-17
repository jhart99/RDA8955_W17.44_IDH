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



#include "hal_i2c.h"
#include "hal_camera.h"
#include "camera_m.h"

typedef struct
{
    UINT8 Addr;            // Register Address
    UINT8 Data;             // Register Data
    UINT8 Wait;            // Time to wait after write
} CAM_REG_T;

typedef struct
{
    UINT16 Len;             // Size of array
    CAM_REG_T *RegVal;      // Array of registers
} CAM_REG_GROUP_T;

typedef enum
{
    CAM_MODE_IDLE,
    CAM_MODE_VIEWFINDER,
    CAM_MODE_PREP_IMG_CAPTURE,
    CAM_MODE_IMG_CAPTURE,
    CAM_MODE_PREP_VID_CAPTURE,
    CAM_MODE_VID_CAPTURE
} CAM_MODE_T;

// On FPGA platform, Pixel Clock must not be too fast or xfer screws up
#define CAM_CLK_CFG 0x81

// use this for 320x240 (VGA) capture
PRIVATE const CAM_REG_T RG_InitPowerOnQvga[] =
{
    {0xfe,0x80,1},

    {0xfe,0x00,1},     // set page0

    {0x1a,0x16,1},
    {0xd2,0x10,1},   // close AEC
    {0x22,0x55,1},   //close AWB

    {0x5a,0x56,1},
    {0x5b,0x40,1},
    {0x5c,0x4a,1},

    {0x22,0x57,1},

#if 0//fix 14.3
    {0x01,0xfa,1},
    {0x02,0xd4,1},
    {0x0f,0x01,1},

    {0x03,0x01,1},
    {0x04,0x90,1},

    {0xe2,0x00,1},
    {0xe3,0x64,1},

    {0xe4,0x02,1},
    {0xe5,0xbc,1},
    {0xe6,0x02,1},
    {0xe7,0xbc,1},
    {0xe8,0x02,1},
    {0xe9,0xbc,1},
    {0xea,0x09,1},
    {0xeb,0xc4,1},
#elif 0 //mclk =13MHZ fps =8.3
    {0x01,0x2c,1},
    {0x02,0x88,1},
    {0x0f,0x02,1},

    {0x03,0x00,1},
    {0x04,0x9c,1},

    {0xe2,0x00,1},
    {0xe3,0x34,1},

    {0xe4,0x02,1},
    {0xe5,0x70,1},
    {0xe6,0x02,1},
    {0xe7,0x70,1},
    {0xe8,0x02,1},
    {0xe9,0x70,1},
    {0xea,0x04,1},
    {0xeb,0x10,1},
#else //mclk =26MHZ fps =16.6--7.2
    {0x01,0x2c,1},
    {0x02,0x88,1},
    {0x0f,0x02,1},

    {0x03,0x01,1},
    {0x04,0x38,1},

    {0xe2,0x00,1},
    {0xe3,0x68,1},

    {0xe4,0x02,1},
    {0xe5,0x70,1},
    {0xe6,0x03,1},
    {0xe7,0x40,1},
    {0xe8,0x05,1},
    {0xe9,0xb0,1},
    {0xea,0x08,1},
    {0xeb,0x20,1},

#endif
    {0x05,0x00,1},
    {0x06,0x00,1},
    {0x07,0x00,1},
    {0x08,0x00,1},
    {0x09,0x01,1},
    {0x0a,0xe8,1},
    {0x0b,0x02,1},
    {0x0c,0x88,1},
    {0x0d,0x02,1},
    {0x0e,0x02,1},
    {0x10,0x22,1},
    {0x11,0x0d,1},
    {0x12,0x2a,1},
    {0x13,0x00,1},
    {0x15,0x0a,1},
    {0x16,0x05,1},
    {0x17,0x01,1},

    {0x1b,0x03,1},
    {0x1c,0xc1,1},
    {0x1d,0x08,1},
    {0x1e,0x20,1},
    {0x1f,0x16,1},

    {0x20,0xff,1},
    {0x21,0xf8,1},
    {0x24,0xa2,1},
    {0x25,0x0f,1},
    //output sync_mode
    {0x26,0x03,1},
    {0x2f,0x01,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// grab_t ////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x30,0xf7,1},
    {0x31,0x40,1},
    {0x32,0x00,1},
    {0x39,0x04,1},
    {0x3a,0x20,1},
    {0x3b,0x20,1},
    {0x3c,0x02,1},
    {0x3d,0x02,1},
    {0x3e,0x02,1},
    {0x3f,0x02,1},

    //gain
    {0x50,0x24,1},

    {0x53,0x80,1},
    {0x54,0x80,1},
    {0x55,0x80,1},
    {0x56,0x80,1},

    /////////////////////////////////////////////////////////////////////
    /////////////////////////// LSC_t  ////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x8b,0x20,1},
    {0x8c,0x20,1},
    {0x8d,0x20,1},
    {0x8e,0x10,1},
    {0x8f,0x10,1},
    {0x90,0x10,1},
    {0x91,0x3c,1},
    {0x92,0x50,1},
    {0x5d,0x12,1},
    {0x5e,0x1a,1},
    {0x5f,0x24,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// DNDD_t  ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x60,0x07,1},
    {0x61,0x0e,1},
    {0x62,0x0f,1}, //0x0c
    {0x64,0x03,1},
    {0x66,0xe8,1},
    {0x67,0x86,1},
    {0x68,0xa2,1},

    /////////////////////////////////////////////////////////////////////
    /////////////////////////// asde_t ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x69,0x20,1},
    {0x6a,0x0f,1},
    {0x6b,0x00,1},
    {0x6c,0x53,1},
    {0x6d,0x83,1},
    {0x6e,0xac,1},
    {0x6f,0xac,1},
    {0x70,0x15,1},
    {0x71,0x33,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// eeintp_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x72,0xdc,1},
    {0x73,0x80,1},
    //for high resolution in light scene
    {0x74,0x02,1},
    {0x75,0x3f,1},
    {0x76,0x02,1},
    {0x77,0x54,1},
    {0x78,0x88,1},
    {0x79,0x81,1},
    {0x7a,0x81,1},
    {0x7b,0x22,1},
    {0x7c,0xff,1},


    /////////////////////////////////////////////////////////////////////
    ///////////////////////////CC_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x93,0x48,1},
    {0x94,0x00,1},
    {0x95,0x05,1},
    {0x96,0xe8,1},
    {0x97,0x40,1},
    {0x98,0xf8,1},
    {0x9c,0x00,1},
    {0x9d,0x00,1},
    {0x9e,0x00,1},


    /////////////////////////////////////////////////////////////////////
    ///////////////////////////YCP_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xb1,0x42,1},
    {0xb2,0x42,1},
    {0xb8,0x20,1},
    {0xbe,0x36,1},
    {0xbf,0x00,1},
    /////////////////////////////////////////////////////////////////////
    ///////////////////////////AEC_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xd0,0xcb,1},
    {0xd1,0x10,1},

    {0xd3,0x50,1},
    {0xd5,0xf2,1},
    {0xd6,0x16,1},
    {0xdb,0x92,1},
    {0xdc,0xa5,1},
    {0xdf,0x23,1},
    {0xd9,0x00,1},
    {0xda,0x00,1},
    {0xe0,0x09,1},

    {0xec,0x20,1},
    {0xed,0x04,1},
    {0xee,0xa0,1},
    {0xef,0x40,1},
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////GAMMA//////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    // for FAE to choose the gamma curve according to different LCD
    {0x9F,0x0e,1},         //gamma curve lvl2
    {0xA0,0x1c,1},
    {0xA1,0x34,1},
    {0xA2,0x48,1},
    {0xA3,0x5a,1},
    {0xA4,0x6b,1},
    {0xA5,0x7b,1},
    {0xA6,0x95,1},
    {0xA7,0xab,1},
    {0xA8,0xbf,1},
    {0xA9,0xce,1},
    {0xAA,0xd9,1},
    {0xAB,0xe4,1},
    {0xAC,0xec,1},
    {0xAD,0xf7,1},
    {0xAE,0xfd,1},
    {0xAF,0xff,1},

#if 0
    {0x9F,0x10,1},         // gamma curve lvl3
    {0xA0,0x20,1},
    {0xA1,0x38,1},
    {0xA2,0x4e,1},
    {0xA3,0x63,1},
    {0xA4,0x76,1},
    {0xA5,0x87,1},
    {0xA6,0xa2,1},
    {0xA7,0xb8,1},
    {0xA8,0xca,1},
    {0xA9,0xd8,1},
    {0xAA,0xe3,1},
    {0xAB,0xeb,1},
    {0xAC,0xf0,1},
    {0xAD,0xf8,1},
    {0xAE,0xfd,1},
    {0xAF,0xff,1},

    {0x9F,0x14,1},         // gamma curve lvl4
    {0xA0,0x28,1},
    {0xA1,0x44,1},
    {0xA2,0x5d,1},
    {0xA3,0x72,1},
    {0xA4,0x86,1},
    {0xA5,0x95,1},
    {0xA6,0xb1,1},
    {0xA7,0xc6,1},
    {0xA8,0xd5,1},
    {0xA9,0xe1,1},
    {0xAA,0xea,1},
    {0xAB,0xf1,1},
    {0xAC,0xf5,1},
    {0xAD,0xfb,1},
    {0xAE,0xfe,1},
    {0xAF,0xff,1},

#endif

    //Y_gamma
    {0xc0,0x00,1},
    {0xc1,0x0B,1},
    {0xc2,0x15,1},
    {0xc3,0x27,1},
    {0xc4,0x39,1},
    {0xc5,0x49,1},
    {0xc6,0x5A,1},
    {0xc7,0x6A,1},
    {0xc8,0x89,1},
    {0xc9,0xA8,1},
    {0xca,0xC6,1},
    {0xcb,0xE3,1},
    {0xcc,0xFF,1},

    /////////////////////////////////////////////////////////////////
    /////////////////////////// ABS_t ///////////////////////////////
    /////////////////////////////////////////////////////////////////
    {0xf0,0x02,1},
    {0xf1,0x01,1},
    {0xf2,0x00,1},
    {0xf3,0x30,1},

    /////////////////////////////////////////////////////////////////
    /////////////////////////// Measure Window ///////////////////////
    /////////////////////////////////////////////////////////////////
    {0xf7,0x04,1},
    {0xf8,0x02,1},
    {0xf9,0x9f,1},
    {0xfa,0x78,1},

    //---------------------------------------------------------------
    {0xfe,0x01,1},

    /////////////////////////////////////////////////////////////////
    ///////////////////////////AWB_p/////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {0x00,0xf5,1},
//  {0x01,0x0a,1},
    {0x02,0x1a,1},
    {0x0a,0xa0,1},
    {0x0b,0x60,1},
    {0x0c,0x08,1},
    {0x0e,0x4c,1},
    {0x0f,0x39,1},
    {0x11,0x3f,1},
    {0x12,0x72,1},
    {0x13,0x39,1},
    {0x14,0x42,1},
    {0x15,0x43,1},
    {0x16,0xc2,1},
    {0x17,0xa8,1},
    {0x18,0x18,1},
    {0x19,0x40,1},
    {0x1a,0xd0,1},
    {0x1b,0xf5,1},

    {0x70,0x40,1},
    {0x71,0x58,1},
    {0x72,0x30,1},
    {0x73,0x48,1},
    {0x74,0x20,1},
    {0x75,0x60,1},

    {0xfe,0x00,1},

    {0xd2,0x90,1},  // Open AEC
    {0x23,0x00,1},
    {0x2d,0x0a,1},
    {0x20,0xff,1},
    {0x73,0x00,1},
    {0x77,0x33,1},
    {0xb3,0x40,1},
    {0xb4,0x80,1},
    {0xb5,0x00,1},
    {0xba,0x00,1},
    {0xbb,0x00,1},

    {0x8b,0x22,1},
    {0x71,0x43,1},
    {0x31,0x60,1},
    {0x1c,0x49,1},
    {0x1d,0x98,1},
    {0x10,0x26,1},
    {0x1a,0x26,1},

    {0x14,0x10,20},  // Mirror UpsideDown


    //*****subsampleratio=4 start****//
    {0xfe,0x01,1},
    {0x54,0x22,1},
    {0x55,0x03,1},
    {0x56,0x00,1},
    {0x57,0x00,1},
    {0x58,0x00,1},
    {0x59,0x00,1},

    {0xfe,0x00,1},
    //*****subsampleratio=4 end****//

    {0xff,0xff,0},
};

// use this for 640x480 (VGA) capture
PRIVATE const CAM_REG_T RG_InitPowerOnVga[] =
{
    {0xfe,0x80,1},

    {0xfe,0x00,1},     // set page0

    {0x1a,0x16,1},
    {0xd2,0x10,1},   // close AEC
    {0x22,0x55,1},   //close AWB

    {0x5a,0x56,1},
    {0x5b,0x40,1},
    {0x5c,0x4a,1},

    {0x22,0x57,1},

#if 0//fix 14.3
    {0x01,0xfa,1},
    {0x02,0xd4,1},
    {0x0f,0x01,1},

    {0x03,0x01,1},
    {0x04,0x90,1},

    {0xe2,0x00,1},
    {0xe3,0x64,1},

    {0xe4,0x02,1},
    {0xe5,0xbc,1},
    {0xe6,0x02,1},
    {0xe7,0xbc,1},
    {0xe8,0x02,1},
    {0xe9,0xbc,1},
    {0xea,0x09,1},
    {0xeb,0xc4,1},
#elif 0 //mclk =13MHZ fps =8.3
    {0x01,0x2c,1},
    {0x02,0x88,1},
    {0x0f,0x02,1},

    {0x03,0x00,1},
    {0x04,0x9c,1},

    {0xe2,0x00,1},
    {0xe3,0x34,1},

    {0xe4,0x02,1},
    {0xe5,0x70,1},
    {0xe6,0x02,1},
    {0xe7,0x70,1},
    {0xe8,0x02,1},
    {0xe9,0x70,1},
    {0xea,0x04,1},
    {0xeb,0x10,1},
#else //mclk =26MHZ fps =16.6--7.2
    {0x01,0x2c,1},
    {0x02,0x88,1},
    {0x0f,0x02,1},

    {0x03,0x01,1},
    {0x04,0x38,1},

    {0xe2,0x00,1},
    {0xe3,0x68,1},

    {0xe4,0x02,1},
    {0xe5,0x70,1},
    {0xe6,0x03,1},
    {0xe7,0x40,1},
    {0xe8,0x05,1},
    {0xe9,0xb0,1},
    {0xea,0x08,1},
    {0xeb,0x20,1},

#endif
    {0x05,0x00,1},
    {0x06,0x00,1},
    {0x07,0x00,1},
    {0x08,0x00,1},
    {0x09,0x01,1},
    {0x0a,0xe8,1},
    {0x0b,0x02,1},
    {0x0c,0x88,1},
    {0x0d,0x02,1},
    {0x0e,0x02,1},
    {0x10,0x22,1},
    {0x11,0x0d,1},
    {0x12,0x2a,1},
    {0x13,0x00,1},
    {0x15,0x0a,1},
    {0x16,0x05,1},
    {0x17,0x01,1},

    {0x1b,0x03,1},
    {0x1c,0xc1,1},
    {0x1d,0x08,1},
    {0x1e,0x20,1},
    {0x1f,0x16,1},

    {0x20,0xff,1},
    {0x21,0xf8,1},
    {0x24,0xa2,1},
    {0x25,0x0f,1},
    //output sync_mode
    {0x26,0x03,1},
    {0x2f,0x01,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// grab_t ////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x30,0xf7,1},
    {0x31,0x40,1},
    {0x32,0x00,1},
    {0x39,0x04,1},
    {0x3a,0x20,1},
    {0x3b,0x20,1},
    {0x3c,0x02,1},
    {0x3d,0x02,1},
    {0x3e,0x02,1},
    {0x3f,0x02,1},

    //gain
    {0x50,0x24,1},

    {0x53,0x80,1},
    {0x54,0x80,1},
    {0x55,0x80,1},
    {0x56,0x80,1},

    /////////////////////////////////////////////////////////////////////
    /////////////////////////// LSC_t  ////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x8b,0x20,1},
    {0x8c,0x20,1},
    {0x8d,0x20,1},
    {0x8e,0x10,1},
    {0x8f,0x10,1},
    {0x90,0x10,1},
    {0x91,0x3c,1},
    {0x92,0x50,1},
    {0x5d,0x12,1},
    {0x5e,0x1a,1},
    {0x5f,0x24,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// DNDD_t  ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x60,0x07,1},
    {0x61,0x0e,1},
    {0x62,0x0f,1}, //0x0c
    {0x64,0x03,1},
    {0x66,0xe8,1},
    {0x67,0x86,1},
    {0x68,0xa2,1},

    /////////////////////////////////////////////////////////////////////
    /////////////////////////// asde_t ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x69,0x20,1},
    {0x6a,0x0f,1},
    {0x6b,0x00,1},
    {0x6c,0x53,1},
    {0x6d,0x83,1},
    {0x6e,0xac,1},
    {0x6f,0xac,1},
    {0x70,0x15,1},
    {0x71,0x33,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// eeintp_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x72,0xdc,1},
    {0x73,0x80,1},
    //for high resolution in light scene
    {0x74,0x02,1},
    {0x75,0x3f,1},
    {0x76,0x02,1},
    {0x77,0x54,1},
    {0x78,0x88,1},
    {0x79,0x81,1},
    {0x7a,0x81,1},
    {0x7b,0x22,1},
    {0x7c,0xff,1},


    /////////////////////////////////////////////////////////////////////
    ///////////////////////////CC_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x93,0x48,1},
    {0x94,0x00,1},
    {0x95,0x05,1},
    {0x96,0xe8,1},
    {0x97,0x40,1},
    {0x98,0xf8,1},
    {0x9c,0x00,1},
    {0x9d,0x00,1},
    {0x9e,0x00,1},


    /////////////////////////////////////////////////////////////////////
    ///////////////////////////YCP_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xb1,0x42,1},
    {0xb2,0x42,1},
    {0xb8,0x20,1},
    {0xbe,0x36,1},
    {0xbf,0x00,1},
    /////////////////////////////////////////////////////////////////////
    ///////////////////////////AEC_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xd0,0xcb,1},
    {0xd1,0x10,1},

    {0xd3,0x50,1},
    {0xd5,0xf2,1},
    {0xd6,0x16,1},
    {0xdb,0x92,1},
    {0xdc,0xa5,1},
    {0xdf,0x23,1},
    {0xd9,0x00,1},
    {0xda,0x00,1},
    {0xe0,0x09,1},

    {0xec,0x20,1},
    {0xed,0x04,1},
    {0xee,0xa0,1},
    {0xef,0x40,1},
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////GAMMA//////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    // for FAE to choose the gamma curve according to different LCD
    {0x9F,0x0e,1},         //gamma curve lvl2
    {0xA0,0x1c,1},
    {0xA1,0x34,1},
    {0xA2,0x48,1},
    {0xA3,0x5a,1},
    {0xA4,0x6b,1},
    {0xA5,0x7b,1},
    {0xA6,0x95,1},
    {0xA7,0xab,1},
    {0xA8,0xbf,1},
    {0xA9,0xce,1},
    {0xAA,0xd9,1},
    {0xAB,0xe4,1},
    {0xAC,0xec,1},
    {0xAD,0xf7,1},
    {0xAE,0xfd,1},
    {0xAF,0xff,1},

#if 0
    {0x9F,0x10,1},         // gamma curve lvl3
    {0xA0,0x20,1},
    {0xA1,0x38,1},
    {0xA2,0x4e,1},
    {0xA3,0x63,1},
    {0xA4,0x76,1},
    {0xA5,0x87,1},
    {0xA6,0xa2,1},
    {0xA7,0xb8,1},
    {0xA8,0xca,1},
    {0xA9,0xd8,1},
    {0xAA,0xe3,1},
    {0xAB,0xeb,1},
    {0xAC,0xf0,1},
    {0xAD,0xf8,1},
    {0xAE,0xfd,1},
    {0xAF,0xff,1},

    {0x9F,0x14,1},         // gamma curve lvl4
    {0xA0,0x28,1},
    {0xA1,0x44,1},
    {0xA2,0x5d,1},
    {0xA3,0x72,1},
    {0xA4,0x86,1},
    {0xA5,0x95,1},
    {0xA6,0xb1,1},
    {0xA7,0xc6,1},
    {0xA8,0xd5,1},
    {0xA9,0xe1,1},
    {0xAA,0xea,1},
    {0xAB,0xf1,1},
    {0xAC,0xf5,1},
    {0xAD,0xfb,1},
    {0xAE,0xfe,1},
    {0xAF,0xff,1},

#endif

    //Y_gamma
    {0xc0,0x00,1},
    {0xc1,0x0B,1},
    {0xc2,0x15,1},
    {0xc3,0x27,1},
    {0xc4,0x39,1},
    {0xc5,0x49,1},
    {0xc6,0x5A,1},
    {0xc7,0x6A,1},
    {0xc8,0x89,1},
    {0xc9,0xA8,1},
    {0xca,0xC6,1},
    {0xcb,0xE3,1},
    {0xcc,0xFF,1},

    /////////////////////////////////////////////////////////////////
    /////////////////////////// ABS_t ///////////////////////////////
    /////////////////////////////////////////////////////////////////
    {0xf0,0x02,1},
    {0xf1,0x01,1},
    {0xf2,0x00,1},
    {0xf3,0x30,1},

    /////////////////////////////////////////////////////////////////
    /////////////////////////// Measure Window ///////////////////////
    /////////////////////////////////////////////////////////////////
    {0xf7,0x04,1},
    {0xf8,0x02,1},
    {0xf9,0x9f,1},
    {0xfa,0x78,1},

    //---------------------------------------------------------------
    {0xfe,0x01,1},

    /////////////////////////////////////////////////////////////////
    ///////////////////////////AWB_p/////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {0x00,0xf5,1},
//  {0x01,0x0a,1},
    {0x02,0x1a,1},
    {0x0a,0xa0,1},
    {0x0b,0x60,1},
    {0x0c,0x08,1},
    {0x0e,0x4c,1},
    {0x0f,0x39,1},
    {0x11,0x3f,1},
    {0x12,0x72,1},
    {0x13,0x39,1},
    {0x14,0x42,1},
    {0x15,0x43,1},
    {0x16,0xc2,1},
    {0x17,0xa8,1},
    {0x18,0x18,1},
    {0x19,0x40,1},
    {0x1a,0xd0,1},
    {0x1b,0xf5,1},

    {0x70,0x40,1},
    {0x71,0x58,1},
    {0x72,0x30,1},
    {0x73,0x48,1},
    {0x74,0x20,1},
    {0x75,0x60,1},

    {0xfe,0x00,1},

    {0xd2,0x90,1},  // Open AEC
    {0x23,0x00,1},
    {0x2d,0x0a,1},
    {0x20,0xff,1},
    {0x73,0x00,1},
    {0x77,0x33,1},
    {0xb3,0x40,1},
    {0xb4,0x80,1},
    {0xb5,0x00,1},
    {0xba,0x00,1},
    {0xbb,0x00,1},

    {0x8b,0x22,1},
    {0x71,0x43,1},
    {0x31,0x60,1},
    {0x1c,0x49,1},
    {0x1d,0x98,1},
    {0x10,0x26,1},
    {0x1a,0x26,1},

    {0x14,0x10,20},  // Mirror UpsideDown

    {0xff,0xff,0},
};

// use this for 160*120 (QQVGA) capture
PRIVATE const CAM_REG_T RG_InitPowerOnQqvga[] =
{
    {0xfe,0x80,1},

    {0xfe,0x00,1},     // set page0

    {0x1a,0x16,1},
    {0xd2,0x10,1},   // close AEC
    {0x22,0x55,1},   //close AWB

    {0x5a,0x56,1},
    {0x5b,0x40,1},
    {0x5c,0x4a,1},

    {0x22,0x57,1},

#if 0//fix 14.3
    {0x01,0xfa,1},
    {0x02,0xd4,1},
    {0x0f,0x01,1},

    {0x03,0x01,1},
    {0x04,0x90,1},

    {0xe2,0x00,1},
    {0xe3,0x64,1},

    {0xe4,0x02,1},
    {0xe5,0xbc,1},
    {0xe6,0x02,1},
    {0xe7,0xbc,1},
    {0xe8,0x02,1},
    {0xe9,0xbc,1},
    {0xea,0x09,1},
    {0xeb,0xc4,1},
#elif 0 //mclk =13MHZ fps =8.3
    {0x01,0x2c,1},
    {0x02,0x88,1},
    {0x0f,0x02,1},

    {0x03,0x00,1},
    {0x04,0x9c,1},

    {0xe2,0x00,1},
    {0xe3,0x34,1},

    {0xe4,0x02,1},
    {0xe5,0x70,1},
    {0xe6,0x02,1},
    {0xe7,0x70,1},
    {0xe8,0x02,1},
    {0xe9,0x70,1},
    {0xea,0x04,1},
    {0xeb,0x10,1},
#else //mclk =26MHZ fps =16.6--7.2
    {0x01,0x2c,1},
    {0x02,0x88,1},
    {0x0f,0x02,1},

    {0x03,0x01,1},
    {0x04,0x38,1},

    {0xe2,0x00,1},
    {0xe3,0x68,1},

    {0xe4,0x02,1},
    {0xe5,0x70,1},
    {0xe6,0x03,1},
    {0xe7,0x40,1},
    {0xe8,0x05,1},
    {0xe9,0xb0,1},
    {0xea,0x08,1},
    {0xeb,0x20,1},

#endif
    {0x05,0x00,1},
    {0x06,0x00,1},
    {0x07,0x00,1},
    {0x08,0x00,1},
    {0x09,0x01,1},
    {0x0a,0xe8,1},
    {0x0b,0x02,1},
    {0x0c,0x88,1},
    {0x0d,0x02,1},
    {0x0e,0x02,1},
    {0x10,0x22,1},
    {0x11,0x0d,1},
    {0x12,0x2a,1},
    {0x13,0x00,1},
    {0x15,0x0a,1},
    {0x16,0x05,1},
    {0x17,0x01,1},

    {0x1b,0x03,1},
    {0x1c,0xc1,1},
    {0x1d,0x08,1},
    {0x1e,0x20,1},
    {0x1f,0x16,1},

    {0x20,0xff,1},
    {0x21,0xf8,1},
    {0x24,0xa2,1},
    {0x25,0x0f,1},
    //output sync_mode
    {0x26,0x03,1},
    {0x2f,0x01,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// grab_t ////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x30,0xf7,1},
    {0x31,0x40,1},
    {0x32,0x00,1},
    {0x39,0x04,1},
    {0x3a,0x20,1},
    {0x3b,0x20,1},
    {0x3c,0x02,1},
    {0x3d,0x02,1},
    {0x3e,0x02,1},
    {0x3f,0x02,1},

    //gain
    {0x50,0x24,1},

    {0x53,0x80,1},
    {0x54,0x80,1},
    {0x55,0x80,1},
    {0x56,0x80,1},

    /////////////////////////////////////////////////////////////////////
    /////////////////////////// LSC_t  ////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x8b,0x20,1},
    {0x8c,0x20,1},
    {0x8d,0x20,1},
    {0x8e,0x10,1},
    {0x8f,0x10,1},
    {0x90,0x10,1},
    {0x91,0x3c,1},
    {0x92,0x50,1},
    {0x5d,0x12,1},
    {0x5e,0x1a,1},
    {0x5f,0x24,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// DNDD_t  ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x60,0x07,1},
    {0x61,0x0e,1},
    {0x62,0x0f,1}, //0x0c
    {0x64,0x03,1},
    {0x66,0xe8,1},
    {0x67,0x86,1},
    {0x68,0xa2,1},

    /////////////////////////////////////////////////////////////////////
    /////////////////////////// asde_t ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x69,0x20,1},
    {0x6a,0x0f,1},
    {0x6b,0x00,1},
    {0x6c,0x53,1},
    {0x6d,0x83,1},
    {0x6e,0xac,1},
    {0x6f,0xac,1},
    {0x70,0x15,1},
    {0x71,0x33,1},
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// eeintp_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x72,0xdc,1},
    {0x73,0x80,1},
    //for high resolution in light scene
    {0x74,0x02,1},
    {0x75,0x3f,1},
    {0x76,0x02,1},
    {0x77,0x54,1},
    {0x78,0x88,1},
    {0x79,0x81,1},
    {0x7a,0x81,1},
    {0x7b,0x22,1},
    {0x7c,0xff,1},


    /////////////////////////////////////////////////////////////////////
    ///////////////////////////CC_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x93,0x48,1},
    {0x94,0x00,1},
    {0x95,0x05,1},
    {0x96,0xe8,1},
    {0x97,0x40,1},
    {0x98,0xf8,1},
    {0x9c,0x00,1},
    {0x9d,0x00,1},
    {0x9e,0x00,1},


    /////////////////////////////////////////////////////////////////////
    ///////////////////////////YCP_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xb1,0x42,1},
    {0xb2,0x42,1},
    {0xb8,0x20,1},
    {0xbe,0x36,1},
    {0xbf,0x00,1},
    /////////////////////////////////////////////////////////////////////
    ///////////////////////////AEC_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xd0,0xcb,1},
    {0xd1,0x10,1},

    {0xd3,0x50,1},
    {0xd5,0xf2,1},
    {0xd6,0x16,1},
    {0xdb,0x92,1},
    {0xdc,0xa5,1},
    {0xdf,0x23,1},
    {0xd9,0x00,1},
    {0xda,0x00,1},
    {0xe0,0x09,1},

    {0xec,0x20,1},
    {0xed,0x04,1},
    {0xee,0xa0,1},
    {0xef,0x40,1},
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////GAMMA//////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    // for FAE to choose the gamma curve according to different LCD
    {0x9F,0x0e,1},         //gamma curve lvl2
    {0xA0,0x1c,1},
    {0xA1,0x34,1},
    {0xA2,0x48,1},
    {0xA3,0x5a,1},
    {0xA4,0x6b,1},
    {0xA5,0x7b,1},
    {0xA6,0x95,1},
    {0xA7,0xab,1},
    {0xA8,0xbf,1},
    {0xA9,0xce,1},
    {0xAA,0xd9,1},
    {0xAB,0xe4,1},
    {0xAC,0xec,1},
    {0xAD,0xf7,1},
    {0xAE,0xfd,1},
    {0xAF,0xff,1},

#if 0
    {0x9F,0x10,1},         // gamma curve lvl3
    {0xA0,0x20,1},
    {0xA1,0x38,1},
    {0xA2,0x4e,1},
    {0xA3,0x63,1},
    {0xA4,0x76,1},
    {0xA5,0x87,1},
    {0xA6,0xa2,1},
    {0xA7,0xb8,1},
    {0xA8,0xca,1},
    {0xA9,0xd8,1},
    {0xAA,0xe3,1},
    {0xAB,0xeb,1},
    {0xAC,0xf0,1},
    {0xAD,0xf8,1},
    {0xAE,0xfd,1},
    {0xAF,0xff,1},

    {0x9F,0x14,1},         // gamma curve lvl4
    {0xA0,0x28,1},
    {0xA1,0x44,1},
    {0xA2,0x5d,1},
    {0xA3,0x72,1},
    {0xA4,0x86,1},
    {0xA5,0x95,1},
    {0xA6,0xb1,1},
    {0xA7,0xc6,1},
    {0xA8,0xd5,1},
    {0xA9,0xe1,1},
    {0xAA,0xea,1},
    {0xAB,0xf1,1},
    {0xAC,0xf5,1},
    {0xAD,0xfb,1},
    {0xAE,0xfe,1},
    {0xAF,0xff,1},

#endif

    //Y_gamma
    {0xc0,0x00,1},
    {0xc1,0x0B,1},
    {0xc2,0x15,1},
    {0xc3,0x27,1},
    {0xc4,0x39,1},
    {0xc5,0x49,1},
    {0xc6,0x5A,1},
    {0xc7,0x6A,1},
    {0xc8,0x89,1},
    {0xc9,0xA8,1},
    {0xca,0xC6,1},
    {0xcb,0xE3,1},
    {0xcc,0xFF,1},

    /////////////////////////////////////////////////////////////////
    /////////////////////////// ABS_t ///////////////////////////////
    /////////////////////////////////////////////////////////////////
    {0xf0,0x02,1},
    {0xf1,0x01,1},
    {0xf2,0x00,1},
    {0xf3,0x30,1},

    /////////////////////////////////////////////////////////////////
    /////////////////////////// Measure Window ///////////////////////
    /////////////////////////////////////////////////////////////////
    {0xf7,0x04,1},
    {0xf8,0x02,1},
    {0xf9,0x9f,1},
    {0xfa,0x78,1},

    //---------------------------------------------------------------
    {0xfe,0x01,1},

    /////////////////////////////////////////////////////////////////
    ///////////////////////////AWB_p/////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {0x00,0xf5,1},
//  {0x01,0x0a,1},
    {0x02,0x1a,1},
    {0x0a,0xa0,1},
    {0x0b,0x60,1},
    {0x0c,0x08,1},
    {0x0e,0x4c,1},
    {0x0f,0x39,1},
    {0x11,0x3f,1},
    {0x12,0x72,1},
    {0x13,0x39,1},
    {0x14,0x42,1},
    {0x15,0x43,1},
    {0x16,0xc2,1},
    {0x17,0xa8,1},
    {0x18,0x18,1},
    {0x19,0x40,1},
    {0x1a,0xd0,1},
    {0x1b,0xf5,1},

    {0x70,0x40,1},
    {0x71,0x58,1},
    {0x72,0x30,1},
    {0x73,0x48,1},
    {0x74,0x20,1},
    {0x75,0x60,1},

    {0xfe,0x00,1},

    {0xd2,0x90,1},  // Open AEC
    {0x23,0x00,1},
    {0x2d,0x0a,1},
    {0x20,0xff,1},
    {0x73,0x00,1},
    {0x77,0x33,1},
    {0xb3,0x40,1},
    {0xb4,0x80,1},
    {0xb5,0x00,1},
    {0xba,0x00,1},
    {0xbb,0x00,1},

    {0x8b,0x22,1},
    {0x71,0x43,1},
    {0x31,0x60,1},
    {0x1c,0x49,1},
    {0x1d,0x98,1},
    {0x10,0x26,1},
    {0x1a,0x26,1},

    {0x14,0x10,20},  // Mirror UpsideDown

    //*****subsampleratio=4 start****//
    {0xfe,0x01,1},
    {0x54,0x44,1},
    {0x55,0x03,1},
    {0x56,0x00,1},
    {0x57,0x00,1},
    {0x58,0x00,1},
    {0x59,0x00,1},
    {0xfe,0x00,1},
    //*****subsampleratio=4 end****//

    {0xff,0xff,0},
};

PRIVATE const CAM_REG_T RG_VidFormatVga[] =
{
};

PRIVATE const CAM_REG_T RG_VidFormatQvga[] =
{
};

PRIVATE const CAM_REG_T RG_VidFormatQqvga[] =
{
};

typedef struct
{
    // Camera information structure
    char                desc[40];

    UINT8               snrtype;            //sensor type(RGB or YUV)
    UINT8               pclk;               //use PCLK of sensor
    UINT8               clkcfg;             //[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
    HAL_I2C_BPS_T       i2crate;            //I2C rate : KHz
    UINT8               i2caddress;         //I2C address
    UINT8               i2cispaddress;      //ISP I2C address
    UINT8               pwrcfg;             //sensor power initial configure(SIF REG801 BIT[0]~BIT[2])
    BOOL                rstactiveh;         //sensor reset active hi/low polarity (TRUE = active hi)
    BOOL                snrrst;             //Reset sensor enable

    CAM_MODE_T          cammode;


    BOOL                capturevideo;
    BOOL                captureframe;
    UINT8               numactivebuffers;
    UINT8               currentbuf;

    UINT8               brightness;         //brightness
    UINT8               contrast;           //contrast

    CAM_FORMAT_T        format;
    UINT32              npixels;
    CAM_IMAGE_T         vidbuffer;

    CAM_REG_GROUP_T     standby;            //sensor standby register
    CAM_REG_GROUP_T     Initpoweron;        //SENSOR ISP initial configure

    CAM_REG_GROUP_T     sifpwronseq;        //Sif config sequence(Reg.800 bit0~2) when sensor power on
    CAM_REG_GROUP_T     sifstdbyseq;        //Sif config sequence(Reg.800 bit0~2) when sensor standby
    CAM_REG_GROUP_T     dsif;               //SIF initial configure
    //    TSnrSizeCfgGroup    snrSizeCfg;         //sensor size configure information

    // This 'callback' mechanism is not necessary because the encapsulation is taken
    // care of by the environment
    //    PSensorSetReg       snrSetRegCall;      //set reg callback
    //    PSensorGetReg       snrGetRegCall;
    //    PSnrIdChk           snrIdChkCall;
    //    PSnrMirrorFlip      snrMirrorFlipCall;
    //    PSnrContrast        snrContrastCall;
    //    PSnrBrightness      snrBrightnessCall;
    //    PSnrMode            snrModeCall;
    //    PSnrGetEt           snrGetEt;
    //    PSnrSetEt           snrSetEt;
    //    TFlashParm          flashparm;
    CAM_IRQ_HANDLER_T   camirqhandler;
} CAM_INFO_T;

PRIVATE volatile CAM_INFO_T gSensorInfo =
{
    "BG0313",      // char desc[40];

    0,                      // UINT8 snrtype; (rgb)
    1,                      // UINT8 pclk; use or don't use PCLK of sensor???
    0,                      // UINT8 clkcfg; ???
    HAL_I2C_BPS_100K,       // UINT16 i2crate;
    0x21,                   // UINT8 i2caddress; this is the _REAL_ slave address.
    //  The value written in phase 1 of a write cycle will be 0x42
    //  while the value just before the read will be 0x43
    0x21,                   // UINT8 i2cispaddress;
    0x02,                   // UINT8 pwrcfg;???
    TRUE,                   // BOOL rstactiveh;  // active high
    FALSE,                  // BOOL snrrst;

    CAM_MODE_IDLE,          // cammode

    FALSE,                  // Capture video mode
    FALSE,                  // Capturing single frame
    0,                      // Number of active buffers reserved by the upper layer
    0,                      // current frame buffer

    3,                      // UINT8 brightness;
    4,                      // UINT8 contrast;
    CAM_FORMAT_RGB565,      // Format
    CAM_NPIX_VGA,           // npixels(VGA, QVGA, QQVGA...)
    NULL,                   // Video Buffer.  First frame buffer for video or image buffer

    {0, NULL},              // CAM_REG_GROUP_T standby;
    // Should start with QQVGA, but for testing, start with VGA
    //    {sizeof(RG_InitPowerOnQqvga) / sizeof(CAM_REG_T), (CAM_REG_T*)&RG_InitPowerOnQqvga}, // CAM_REG_GROUP_T Initpoweron;
    {sizeof(RG_InitPowerOnVga) / sizeof(CAM_REG_T), (CAM_REG_T*)&RG_InitPowerOnVga}, // CAM_REG_GROUP_T Initpoweron;

    {0, NULL},              // CAM_REG_GROUP_T sifpwronseq;
    {0, NULL},              // CAM_REG_GROUP_T sifstdbyseq;
    {0, NULL},              // CAM_REG_GROUP_T dsif; initial value
    NULL                    // IRQ Callback
};

// ############
// Private
// ############

// These are the commands specific to the SCCB
#define PHASE3_WR_PH1   0x11000     // Start & Write    Slave ID
#define PHASE3_WR_PH2   0x1000      // Write            Slave Register Address
#define PHASE3_WR_PH3   0x1100      // Stop & Write     Slave Register Data

#define PHASE2_WR_PH1   0x11000     // Start & Write    Slave ID
#define PHASE2_WR_PH2   0x1100      // Stop & Write     Slave Register Address (to read)

#define PHASE2_RD_PH1   0x11000     // Start & Write    Slave ID
#define PHASE2_RD_PH2   0x111       // Stop & Read & NACK   Slave Register Read Data

PRIVATE VOID camerap_WriteOneReg( UINT8 Addr, UINT8 Data);
PRIVATE VOID camerap_Delay(UINT16 Wait_mS);
PRIVATE VOID camerap_WriteReg(CAM_REG_T RegWrite);
PRIVATE UINT8 camerap_ReadReg(UINT8 Addr);
PRIVATE VOID camerap_SendRgSeq(const CAM_REG_T *RegList, UINT16 Len);
PRIVATE VOID camerap_InteruptHandler(HAL_CAMERA_IRQ_CAUSE_T cause);
PRIVATE UINT8 camerap_GetId(VOID);

