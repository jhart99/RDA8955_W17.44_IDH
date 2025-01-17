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


#include "cs_types.h"
#include "sxs_io.h"

#include "hal_camera.h"
#include "hal_gpio.h"
#include "drv_camera.h"
#include "pmd_m.h"
#include "camera_driver.h"
#include "camera_private.h"
#include "tgt_camd_cfg.h"

#ifdef I2C_BASED_ON_GPIO
#include "gpio_i2c.h"
#else
PRIVATE HAL_I2C_BUS_ID_T g_camdI2cBusId = HAL_I2C_BUS_ID_INVALID;
#endif


#define CAM_GC6113_ID   0xb8

#if defined(VDOREC_MODE_VGA) || defined(VDOREC_MODE_QVGA)
#error "VGA/QVGA recording not supported on GC6113 yet."
#endif

#if defined(_CAM_SPECIAL_GC6113_)
typedef enum
{
    CAM_OUT_NULL = 0,
    CAM_OUT_QQVGA,
    CAM_OUT_QVGA,
    CAM_OUT_MODE_MAX
} CAM_OUT_MODE_E;
PRIVATE CAM_OUT_MODE_E s_camOutMode = CAM_OUT_NULL;
extern UINT8* gTransBufPtr[2];
extern UINT8 gTransBufIdx;
extern UINT32 gTransBufSize;
PRIVATE HAL_CAMERA_SPI_MODE_E s_gc6113_mode = SPI_MODE_NO;
#else
static UINT32 s_prevCount = 0;
PRIVATE HAL_CAMERA_SPI_MODE_E s_gc6113_mode = SPI_MODE_MASTER2_2;
#endif

PRIVATE BOOL s_camOpened = FALSE;
PRIVATE BOOL gIsCameraStartXfer = FALSE;

extern BOOL isVideoRecMode;

PRIVATE UINT8 camerap_ReadReg(UINT8 Addr)
{
#ifdef I2C_BASED_ON_GPIO
    UINT8 data = 0;
    pas_gpio_i2c_read_data_sensor(0x40, &Addr, 1, &data, 1);
    return data;
#else
    if (hal_I2cSendRawByte(g_camdI2cBusId, ((gSensorInfo.i2caddress<<1) & 0xfe), PHASE2_WR_PH1) == HAL_ERR_RESOURCE_BUSY)
    {
        return(0x1f);
    }
    if (hal_I2cSendRawByte(g_camdI2cBusId, Addr, PHASE2_WR_PH2) == HAL_ERR_RESOURCE_BUSY)
    {
        return(0x1e);
    }

    if (hal_I2cSendRawByte(g_camdI2cBusId, ((gSensorInfo.i2caddress<<1) | 0x01), PHASE2_RD_PH1) == HAL_ERR_RESOURCE_BUSY)
    {
        return(0x1d);
    }

    return(hal_I2cReadRawByte(g_camdI2cBusId, PHASE2_RD_PH2));
#endif
}

PRIVATE VOID  camerap_WriteOneReg( UINT8 Addr, UINT8 Data)
{
#ifdef I2C_BASED_ON_GPIO
    UINT8 i = 0;

    while (i++ < 3)
        if( pas_gpio_i2c_write_data_sensor(0x40, &Addr, 1, &Data, 1) == TRUE)
            return;

    hal_DbgAssert("gc6113: i2c no ack when writing reg 0x%x", Addr);
#else
    hal_I2cSendRawByte(g_camdI2cBusId, ((gSensorInfo.i2caddress<<1) & 0xfe), PHASE3_WR_PH1);
    hal_I2cSendRawByte(g_camdI2cBusId, Addr, PHASE3_WR_PH2);
    hal_I2cSendRawByte(g_camdI2cBusId, Data, PHASE3_WR_PH3);
#endif

}

PRIVATE VOID camerap_WriteReg(CAM_REG_T RegWrite)
{
    UINT8 i = 0;
#ifdef I2C_BASED_ON_GPIO
    while (i++ < 3)
        if( pas_gpio_i2c_write_data_sensor(0x40, (UINT8*)(&(RegWrite.Addr)), 1, &(RegWrite.Data), 1) == TRUE)
            return;

    hal_DbgAssert("gc6113: i2c no ack when writing reg 0x%x", RegWrite.Addr);

#else
    hal_I2cSendRawByte(g_camdI2cBusId, ((gSensorInfo.i2caddress<<1) & 0xfe), PHASE3_WR_PH1);
    hal_I2cSendRawByte(g_camdI2cBusId, RegWrite.Addr, PHASE3_WR_PH2);
    hal_I2cSendRawByte(g_camdI2cBusId, RegWrite.Data, PHASE3_WR_PH3);
#endif
    // Do delay here using RegWrite.Wait mS
    camerap_Delay(RegWrite.Wait);

}

PRIVATE VOID camerap_SendRgSeq(const CAM_REG_T *RegList, UINT16 Len)
{
    UINT16 RegCount;
    for (RegCount = 0; RegCount < Len; ++RegCount)
    {
        camerap_WriteReg(RegList[RegCount]);
    }
}


UINT8 testimag[320*240];
PRIVATE void camerap_PowerOn(CAM_SIZE_T CamOutSize, UINT16 IspOutWidth, UINT16 IspOutHeight, CAM_FORMAT_T Format)
{
    HAL_CAMERA_CFG_T CamConfig = {0,};
    HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};
    UINT16 camOutWidth = 0, camOutHeight = 0;

    SXS_TRACE(TSTDOUT,"DRV_CAM: Power On, camOutSize %d ispw %d isph %d", CamOutSize, IspOutWidth, IspOutHeight);
    hal_HstSendEvent(0xaacc001a); hal_HstSendEvent(s_gc6113_mode); hal_HstSendEvent(CamOutSize); hal_HstSendEvent(IspOutWidth); //240
    hal_HstSendEvent(IspOutHeight);//320


    CamConfig.rstActiveH = FALSE;
    CamConfig.pdnActiveH = TRUE;
    CamConfig.dropFrame = FALSE;
    CamConfig.camClkDiv = 12;        // 156/6 = 26M
    CamConfig.endianess = NO_SWAP;
    CamConfig.camId = camera_GetCameraID();
    CamConfig.cropEnable = FALSE;
    CamConfig.colRatio= COL_RATIO_1_1;
    CamConfig.rowRatio= ROW_RATIO_1_1;
    CamConfig.vsync_inv = TRUE;
    CamConfig.spi_pixels_per_line = IspOutWidth/2;
    CamConfig.spi_pixels_per_column = IspOutHeight;
    CamConfig.spi_yuv_out = SPI_OUT_Y1_V0_Y0_U0;
    CamConfig.spi_mode = s_gc6113_mode;
    CamConfig.spi_little_endian_en = FALSE;
    CamConfig.spi_ctrl_clk_div = 1;

    pmd_EnablePower(PMD_POWER_CAMERA,TRUE);

    hal_CameraOpen(&CamConfig);
    hal_CameraIrqSetMask(mask);
    hal_CameraPowerDown(FALSE);

#ifdef I2C_BASED_ON_GPIO
    gc6113_gpio_i2c_init_sensor();
#else
    g_camdI2cBusId = tgt_GetCamdConfig()->i2cBusId;
    hal_I2cOpen(g_camdI2cBusId);
#endif

    if (!gSensorInfo.snrrst)
        hal_CameraReset(FALSE);

    camerap_SendRgSeq(RG_Init_PowerOn,  (sizeof(RG_Init_PowerOn) / sizeof(CAM_REG_T)));

    switch (s_gc6113_mode)
    {
        case SPI_MODE_MASTER2_1:
            SXS_TRACE(TSTDOUT,"spi master 2_1 mode in use");
            camerap_SendRgSeq(RG_Reinit_MASTER2_1,  (sizeof(RG_Reinit_MASTER2_1) / sizeof(CAM_REG_T)));   hal_HstSendEvent(0xaacc000d);
            break;
        case SPI_MODE_MASTER2_2:
            SXS_TRACE(TSTDOUT,"spi master 2_2 mode in use");
            camerap_SendRgSeq(RG_Reinit_MASTER2_2,  (sizeof(RG_Reinit_MASTER2_2) / sizeof(CAM_REG_T)));
            break;
        case SPI_MODE_MASTER2_4:
            SXS_TRACE(TSTDOUT,"spi master 2_4 mode in use");
            camerap_SendRgSeq(RG_Reinit_MASTER2_4,  (sizeof(RG_Reinit_MASTER2_4) / sizeof(CAM_REG_T)));
            break;
        case SPI_MODE_NO:
            SXS_TRACE(TSTDOUT,"no spi master in use");
            break;
        default:
            SXS_TRACE(TSTDOUT,"unsupported spi mode");
            break;
    }

    if (isVideoRecMode == TRUE)
    {
        camerap_SendRgSeq(RG_Reinit_QQVGA_2,  (sizeof(RG_Reinit_QQVGA_2) / sizeof(CAM_REG_T)));
#if !defined(_CAM_SPECIAL_GC6113_)
        camerap_SendRgSeq(RG_VidFormatQqvga, (sizeof(RG_VidFormatQqvga) / sizeof(CAM_REG_T)));
#endif
    }
    else if (CamOutSize == CAM_NPIX_QVGA)
    {
        hal_HstSendEvent(0xaacc000b);
        camerap_SendRgSeq(RG_Reinit_QVGA,  (sizeof(RG_Reinit_QVGA) / sizeof(CAM_REG_T)));

    }

#if defined(_CAM_SPECIAL_GC6113_)
    s_camOutMode = CAM_OUT_QQVGA;
#else
    s_prevCount = 0;
#endif
    s_camOpened = TRUE;

}

PRIVATE void camerap_PowerOff(void)
{
    HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};

    if (s_camOpened)
    {
        hal_CameraIrqSetMask(mask);
        // disable IFC
        if(gIsCameraStartXfer == TRUE)
        {
            hal_CameraStopXfer(TRUE);
            gSensorInfo.cammode = CAM_MODE_IDLE;
            gIsCameraStartXfer = FALSE;
        }
        // Disable Camera Interface Module
        hal_CameraClose();

        // Disable I2C
#ifdef I2C_BASED_ON_GPIO
        gpio_i2c_close();
#else
        hal_I2cClose(g_camdI2cBusId);
#endif
        // Disable the LDO for the camera
        pmd_EnablePower(PMD_POWER_CAMERA, FALSE);
        s_camOpened = FALSE;
    }
#if defined(_CAM_SPECIAL_GC6113_)
    s_camOutMode = CAM_OUT_NULL;
#else
    s_prevCount = 0;
#endif
}

PRIVATE void camerap_Reserve(CAM_IRQ_HANDLER_T FrameReady)
{
    gSensorInfo.camirqhandler = FrameReady;
    hal_CameraIrqSetHandler(camerap_InteruptHandler);
}

PRIVATE void camerap_PrepareImageCaptureL(CAM_FORMAT_T ImageFormat, UINT32 SizeIndex, CAM_IMAGE_T Buffer)
{

    hal_HstSendEvent(0xaaccfff0);
#if !defined(_CAM_SPECIAL_GC6113_)
    if (s_prevCount == 0)
#endif
    {
        HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};

        hal_CameraIrqSetMask(mask);

        if (gIsCameraStartXfer == TRUE)
        {
            hal_CameraStopXfer(FALSE);
            gIsCameraStartXfer = FALSE;
        }

        gSensorInfo.cammode = CAM_MODE_PREVIEW_STAGE_1;
        gSensorInfo.vidbuffer = Buffer;
        gSensorInfo.format = ImageFormat;
        gSensorInfo.npixels = SizeIndex;

#if defined(_CAM_SPECIAL_GC6113_)
        if (s_camOutMode != CAM_OUT_QQVGA && !isVideoRecMode)
        {
            // change the clock divider
            camerap_SendRgSeq(RG_Reinit_QQVGA, (sizeof(RG_Reinit_QQVGA) / sizeof(CAM_REG_T)));
            hal_CameraSetupClockDivider(6);
            s_camOutMode = CAM_OUT_QQVGA;
        }
        mask.fstart = 1;
#else
        s_prevCount = 2;
        mask.fend = 1;
#endif // _CAM_SPECIAL_GC6113_

        hal_CameraIrqSetMask(mask);
        hal_CameraControllerEnable(TRUE);
    }
#if !defined(_CAM_SPECIAL_GC6113_)
    else
    {
        UINT32 status = hal_SysEnterCriticalSection();
        s_prevCount++;
        hal_SysExitCriticalSection(status);
    }
#endif
}

PRIVATE void camerap_CaptureImage()
{
    HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};
    hal_HstSendEvent(0xaaccfff1);
    hal_CameraIrqSetMask(mask);
    if (gIsCameraStartXfer == TRUE)
    {
        hal_CameraStopXfer(FALSE);
        gIsCameraStartXfer = FALSE;
    }
    gSensorInfo.cammode = CAM_MODE_CAPTURE_STAGE_1;
#if defined(_CAM_SPECIAL_GC6113_)
    if (s_camOutMode != CAM_OUT_QVGA)
    {
        uint16 tmp_val_1=0,tmp_val_2=0;
        uint32 dest_val=0;

        camerap_WriteOneReg(0xfe,0x00);
        camerap_WriteOneReg(0xe5,0x00);
        tmp_val_1=camerap_ReadReg(0x03);
        tmp_val_2=camerap_ReadReg(0x04);

        dest_val=((tmp_val_1<<8)|(tmp_val_2))*104/260;
        camerap_WriteOneReg(0x03,(dest_val&0xff00)>>8);
        camerap_WriteOneReg(0x04,dest_val&0x00ff);
        camerap_Delay(120);
        hal_CameraSetupClockDivider(15);
        camerap_SendRgSeq(RG_Reinit_QVGA, (sizeof(RG_Reinit_QVGA) / sizeof(CAM_REG_T)));
        s_camOutMode = CAM_OUT_QVGA;
    }
    mask.fstart = 1;
#else
    s_prevCount = 0;
    mask.fend = 1;
#endif // _CAM_SPECIAL_GC6113_

    hal_CameraIrqSetMask(mask);
    hal_CameraControllerEnable(TRUE);
}

PRIVATE void camerap_Release(void)
{
    // 'De-initialize' camera
    // Release resource
}

PRIVATE VOID camerap_Delay(UINT16 Wait_mS)
{
    UINT32 Ticks_16K;

    // Here is the delay function used by the camera driver.  It can be changed to use a non-blocking
    // wait if necessary, but for now, just use hal_TimDelay
    Ticks_16K = (16348 * Wait_mS)/1000;
    hal_TimDelay(Ticks_16K);
}

PRIVATE UINT8 camerap_GetId(VOID)
{
    camerap_WriteOneReg(0xfc,0x12);
    hal_TimDelay(10);
    return(camerap_ReadReg(0x00));
}

PRIVATE void camerap_SetContrastL(UINT32 Contrast)
{

    switch(Contrast)
    {
        case 1:
            camerap_WriteOneReg(0xb3, 0x48);
            break;
        case 2:
            camerap_WriteOneReg(0xb3, 0x44);
            break;
        case 3:
            camerap_WriteOneReg(0xb3, 0x40);
            break;
        case 4:
            camerap_WriteOneReg(0xb3, 0x3d);
            break;
        case 5:
            camerap_WriteOneReg(0xb3, 0x38);
            break;
        default: break;
    }
}

PRIVATE void camerap_SetBrightnessL(UINT32 Brightness)
{

    switch(Brightness)
    {
        case 1:    //0
            camerap_WriteOneReg(0xb5, 0xc0);
            //camerap_WriteOneReg(0xd3, 0x30);
            break;
        case 2:   //-2
            camerap_WriteOneReg(0xb5, 0xf0);
            //camerap_WriteOneReg(0xd3, 0x48);
            break;
        case 3: //-1
            camerap_WriteOneReg(0xb5, 0x00);
            //camerap_WriteOneReg(0xd3, 0x70);
            break;
        case 4:  //+1
            camerap_WriteOneReg(0xb5, 0x30);
            //camerap_WriteOneReg(0xd3, 0x74);
            break;
        case 5:  //+2
            camerap_WriteOneReg(0xb5, 0x40);
            //camerap_WriteOneReg(0xd3, 0x78);
            break;
        default :
            break;
    }
}

PRIVATE void camerap_SetFlashL(CAM_FLASH_T Flash)
{
    return;
}

PRIVATE void camerap_SetExposureL(CAM_EXPOSURE_T Exposure)
{

    switch(Exposure)
    {
        case CAM_EXPOSURE_AUTO:     //0
            camerap_WriteOneReg(0xb5, 0x00);
            //camerap_WriteOneReg(0xd3, 0x30);
            break;
        case CAM_EXPOSURE_NEGATIVE_2:   //-2
            camerap_WriteOneReg(0xb5, 0xe0);
            //camerap_WriteOneReg(0xd3, 0x48);
            break;
        case CAM_EXPOSURE_NEGATIVE_1: //-1
            camerap_WriteOneReg(0xb5, 0xf0);
            //camerap_WriteOneReg(0xd3, 0x70);
            break;
        case CAM_EXPOSURE_POSITIVE_1:  //+1
            camerap_WriteOneReg(0xb5, 0x20);
            //camerap_WriteOneReg(0xd3, 0x74);
            break;
        case CAM_EXPOSURE_POSITIVE_2:  //+2
            camerap_WriteOneReg(0xb5, 0x30);
            //camerap_WriteOneReg(0xd3, 0x78);
            break;

        default : break;
    }
}

PRIVATE void camerap_SetWhiteBalanceL(CAM_WHITEBALANCE_T WhiteBalance)
{
    UINT8 TempReg;

    //TempReg = camerap_ReadReg(0x22);
    switch(WhiteBalance)
    {
        case CAM_WB_AUTO:  /*Auto */
            camerap_WriteOneReg(0x22, 0x73);
            break;
        case CAM_WB_DAYLIGHT:   /*晴天太阳光*/
            camerap_WriteOneReg(0x22,0x71);
            camerap_WriteOneReg(0x5a,0x50);  //  50 45 40
            camerap_WriteOneReg(0x5b,0x45);
            camerap_WriteOneReg(0x5c,0x40);
            break;
        case CAM_WB_OFFICE:   /*日光灯*/
            camerap_WriteOneReg(0x22,71);
            camerap_WriteOneReg(0x5a,0x40);
            camerap_WriteOneReg(0x5b,0x42);
            camerap_WriteOneReg(0x5c,0x50);
            break;
        case CAM_WB_CLOUDY:  /*阴天*/
            camerap_WriteOneReg(0x22,0x71);   // Disable AWB
            camerap_WriteOneReg(0x5a,0x5a); //WB_manual_gain // 5a 42 40
            camerap_WriteOneReg(0x5b,0x42);
            camerap_WriteOneReg(0x5c,0x40);
            break;
        case CAM_WB_TUNGSTEN:  /*钨丝灯*/
            camerap_WriteOneReg(0x22,0x71);
            camerap_WriteOneReg(0x5a,0x48);
            camerap_WriteOneReg(0x5b,0x40);
            camerap_WriteOneReg(0x5c,0x5c);
            break;

        default : break;
    }
}

PRIVATE VOID camerap_SetSpecialEffect(CAM_SPECIALEFFECT_T SpecialEffect)
{
    // UINT8 TempReg;
    // TempReg = camera_ReadReg(0x47);
    switch(SpecialEffect)
    {
        case CAM_SPE_NORMAL :
            camerap_WriteOneReg(0x23,0x00);
            camerap_WriteOneReg(0xba,0x00);
            camerap_WriteOneReg(0xbb,0x00);
            break;

        case CAM_SPE_ANTIQUE    :
            camerap_WriteOneReg(0x23,0x02);
            camerap_WriteOneReg(0xba,0xd0);
            camerap_WriteOneReg(0xbb,0x28);
            break;

        case CAM_SPE_REDISH :
            camerap_WriteOneReg(0x23,0x02);
            camerap_WriteOneReg(0xba,0x10);
            camerap_WriteOneReg(0xbb,0x50);  //28 terry
            break;

        case CAM_SPE_GREENISH:
            camerap_WriteOneReg(0x23,0x02);
            camerap_WriteOneReg(0xba,0xe0);
            camerap_WriteOneReg(0xbb,0xe0);
            break;

        case CAM_SPE_BLUEISH    :
            camerap_WriteOneReg(0x23,0x02);
            camerap_WriteOneReg(0xba,0x50);
            camerap_WriteOneReg(0xbb,0xe0);
            break;

        case CAM_SPE_BLACKWHITE :
            camerap_WriteOneReg(0x23,0x02);
            camerap_WriteOneReg(0xba,0x00);
            camerap_WriteOneReg(0xbb,0x00);
            break;

        case CAM_SPE_NEGATIVE:
            camerap_WriteOneReg(0x23,0x01);
            camerap_WriteOneReg(0xba,0x00);
            camerap_WriteOneReg(0xbb,0x00);
            break;

        case CAM_SPE_BLACKWHITE_NEGATIVE:
            camerap_WriteOneReg(0x23,0x03);
            camerap_WriteOneReg(0xba,0x00);
            camerap_WriteOneReg(0xbb,0x00);
            break;

        default: break;
    }
}

PRIVATE VOID camerap_InteruptHandler(HAL_CAMERA_IRQ_CAUSE_T cause)
{
    HAL_CAMERA_IRQ_CAUSE_T mask = {0, };
    hal_HstSendEvent(0xaabb00aa); hal_HstSendEvent(gSensorInfo.cammode);
    switch (gSensorInfo.cammode)
    {
        case CAM_MODE_PREVIEW_STAGE_1:
#if defined(_CAM_SPECIAL_GC6113_)
            if (cause.fstart)
#else
            if (cause.fend)
#endif
            {


                hal_HstSendEvent(0xaabb0001);
                hal_CameraIrqSetMask(mask);

                //            hal_CameraStartXfer((gSensorInfo.npixels<<1), (UINT8*)(gSensorInfo.vidbuffer));
                hal_CameraStartXfer((320*240), (UINT8*)testimag);
// hal_CameraStartXfer((gSensorInfo.npixels<<1), (UINT8*)(gSensorInfo.vidbuffer));


                gIsCameraStartXfer = TRUE;

                mask.fend = 1;
                hal_CameraIrqSetMask(mask);

                gSensorInfo.cammode = CAM_MODE_PREVIEW_STAGE_2;
            }
            break;
        case CAM_MODE_PREVIEW_STAGE_2:
#if defined(_CAM_SPECIAL_GC6113_)
            if (cause.fend)
            {
                hal_CameraIrqSetMask(mask);
                gIsCameraStartXfer = FALSE;
                gSensorInfo.cammode = CAM_MODE_IDLE;
                if (hal_CameraStopXfer(FALSE) == XFER_SUCCESS)
                {


                    gSensorInfo.camirqhandler(0, gSensorInfo.currentbuf);
                }
                else
                {
                    gSensorInfo.camirqhandler(1, gSensorInfo.currentbuf);
                }
            }
#else
            if (cause.fend)
            {
                hal_CameraIrqSetMask(mask);
                gIsCameraStartXfer = FALSE;
                camerap_Delay(1);
                if (hal_CameraStopXfer(FALSE) == XFER_SUCCESS)
                {
                    hal_HstSendEvent(0xaabb0002);   hal_HstSendEvent(testimag);   hal_DbgAssert(" frame s ");

                    gSensorInfo.camirqhandler(0, gSensorInfo.currentbuf);
                }
                else
                {

                    hal_HstSendEvent(0xaabb0003);    hal_HstSendEvent(testimag);   hal_DbgAssert(" frame f ");  //--------

                    gSensorInfo.camirqhandler(1, gSensorInfo.currentbuf);
                }

                if (--s_prevCount)
                {
                    hal_HstSendEvent(0xaadd0001);
                    hal_CameraStartXfer((gSensorInfo.npixels<<1), (UINT8*)(gSensorInfo.vidbuffer));


                    gIsCameraStartXfer = TRUE;
                    mask.fend = 1;
                    hal_CameraIrqSetMask(mask);
                }
                else
                {
                    gSensorInfo.cammode = CAM_MODE_IDLE;
                }
            }
#endif
            break;

        case CAM_MODE_CAPTURE_STAGE_1:
#if defined(_CAM_SPECIAL_GC6113_)
            if (cause.fstart)
#else
            if (cause.fend)
#endif
            {
                hal_CameraIrqSetMask(mask);
#if defined(_CAM_SPECIAL_GC6113_)
                hal_CameraStartXfer(gTransBufSize, gTransBufPtr[gTransBufIdx]);
                mask.dma = 1;
#else
                hal_HstSendEvent(0xaaee0001);
                hal_CameraStartXfer((gSensorInfo.npixels<<1), (UINT8*)(gSensorInfo.vidbuffer)); hal_HstSendEvent(0xaadd0002);
#endif
                mask.fend = 1;
                gIsCameraStartXfer = TRUE;
                hal_CameraIrqSetMask(mask);
                gSensorInfo.cammode = CAM_MODE_CAPTURE_STAGE_2;
            }
            break;
        case CAM_MODE_CAPTURE_STAGE_2:
#if defined(_CAM_SPECIAL_GC6113_)
            if (cause.dma)
            {
                UINT8 idx = gTransBufIdx;
                idx = (idx + 1) & 1;
                hal_CameraStartXfer(gTransBufSize, gTransBufPtr[idx]);  hal_HstSendEvent(0xaaee0002);
                gSensorInfo.camirqhandler(2, gTransBufIdx);
                gTransBufIdx = idx;

                mask.dma = 1;
                mask.fend = 1;
                hal_CameraIrqSetMask(mask);
            }
            if (cause.fend)
            {
                hal_CameraIrqSetMask(mask);

                hal_CameraStopXfer(FALSE);
                gIsCameraStartXfer = FALSE;
                gSensorInfo.cammode = CAM_MODE_IDLE;
                gSensorInfo.camirqhandler(3, 0);

                //return to Normal Frame Rate(Preview Mode)
                camerap_WriteOneReg(0xfe,0x00);
                camerap_WriteOneReg(0xe5,0x01);
                //end
            }
#else
            if (cause.fend)
            {
                gIsCameraStartXfer = FALSE;
                if (hal_CameraStopXfer(FALSE) == XFER_SUCCESS)
                {
                    hal_CameraIrqSetMask(mask);
                    gSensorInfo.cammode = CAM_MODE_IDLE;
                    gSensorInfo.camirqhandler(0, gSensorInfo.currentbuf);
                }
                else
                {
                    mask.fend = 1;
                    hal_CameraIrqSetMask(mask);
                    gSensorInfo.cammode = CAM_MODE_CAPTURE_STAGE_1;
                }
            }
#endif
            break;

        default:
            gSensorInfo.camirqhandler(255, 255);
            break;
    }
}
PRIVATE BOOL camerap_CheckProductId()
{
    UINT8 cameraID=0;

    HAL_CAMERA_CFG_T CamConfig = {0,};
    CamConfig.rstActiveH = FALSE;
    CamConfig.pdnActiveH = TRUE;
    CamConfig.dropFrame = FALSE;
    CamConfig.camClkDiv = 15; // 156MHz/15 = 10.4MHz
    CamConfig.endianess = NO_SWAP;//BYTE_SWAP
    CamConfig.colRatio= COL_RATIO_1_1;
    CamConfig.rowRatio= ROW_RATIO_1_1;
    CamConfig.camId = camera_GetCameraID();

    pmd_EnablePower(PMD_POWER_CAMERA,TRUE);

    hal_CameraOpen(&CamConfig);
    hal_CameraReset(TRUE);
    hal_CameraPowerDown(FALSE);

#ifdef I2C_BASED_ON_GPIO
    gc6113_gpio_i2c_init_sensor();
#else
    g_camdI2cBusId = tgt_GetCamdConfig()->i2cBusId;
    hal_I2cOpen(g_camdI2cBusId);
#endif

    if (!gSensorInfo.snrrst)
        hal_CameraReset(FALSE);

    SXS_TRACE(TSTDOUT,"gc6113: start to get the cam id");
    camerap_Delay(20);
    cameraID = camerap_GetId();
    SXS_TRACE(TSTDOUT,"gc6113: got cam id 0x%x", cameraID);

    hal_CameraClose();
    pmd_EnablePower(PMD_POWER_CAMERA, FALSE);

    SXS_TRACE(TSTDOUT,"gc6113(0x%02x): read id is 0x%02x", CAM_GC6113_ID, cameraID);

    if(cameraID == CAM_GC6113_ID)
    {
        hal_HstSendEvent(0xaacc0000|CAM_GC6113_ID);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

PRIVATE VOID camerap_GetImageInfo(UINT32 * pcamout,UINT16 *pwidth,UINT16 *pheight)
{
    *pcamout = CAM_NPIX_QVGA;
    *pwidth = 240;
    *pheight = 320;
}

PUBLIC BOOL camera_gc6113_RegInit(CAM_DRV_T  * pCamDrv)
{
    if(camerap_CheckProductId())
    {
        pCamDrv->camera_PowerOn=camerap_PowerOn;
        pCamDrv->camera_PowerOff=camerap_PowerOff;
        pCamDrv->camera_Reserve=camerap_Reserve;
        pCamDrv->camera_Release=camerap_Release;
        pCamDrv->camera_PrepareImageCaptureL=camerap_PrepareImageCaptureL;
        pCamDrv->camera_CaptureImage=camerap_CaptureImage;
        pCamDrv->camera_SetZoomFactorL=camerap_SetZoomFactorL;
        pCamDrv->camera_ZoomFactor=camerap_ZoomFactor;
        pCamDrv->camera_SetDigitalZoomFactorL=camerap_SetDigitalZoomFactorL;
        pCamDrv->camera_DigitalZoomFactor=camerap_DigitalZoomFactor;
        pCamDrv->camera_SetContrastL=camerap_SetContrastL;
        pCamDrv->camera_Contrast=camerap_Contrast;
        pCamDrv->camera_SetBrightnessL=camerap_SetBrightnessL;
        pCamDrv->camera_Brightness=camerap_Brightness;
        pCamDrv->camera_SetFlashL=camerap_SetFlashL;
        pCamDrv->camera_Flash=camerap_Flash;
        pCamDrv->camera_SetExposureL=camerap_SetExposureL;
        pCamDrv->camera_Exposure=camerap_Exposure;
        pCamDrv->camera_SetWhiteBalanceL=camerap_SetWhiteBalanceL;
        pCamDrv->camera_WhiteBalance=camerap_WhiteBalance;
        pCamDrv->camera_SetSpecialEffect=camerap_SetSpecialEffect;
        pCamDrv->camera_SpecialEffect=camerap_SpecialEffect;
        pCamDrv->camera_SetNightMode=camerap_SetNightMode;
        pCamDrv->camera_NightMode=camerap_NightMode;
        pCamDrv->camera_StartViewFinderDirectL=camerap_StartViewFinderDirectL;
        pCamDrv->camera_StopViewFinder=camerap_StopViewFinder;
        pCamDrv->camera_ViewFinderActive=camerap_ViewFinderActive;
        pCamDrv->camera_SetViewFinderMirror=camerap_SetViewFinderMirror;
        pCamDrv->camera_ViewFinderMirror=camerap_ViewFinderMirror;
        pCamDrv->camera_CancelCaptureImage=camerap_CancelCaptureImage;
        pCamDrv->camera_EnumerateCaptureSizes=camerap_EnumerateCaptureSizes;
        pCamDrv->camera_PrepareVideoCaptureL=camerap_PrepareVideoCaptureL;
        pCamDrv->camera_StartVideoCapture=camerap_StartVideoCapture;
        pCamDrv->camera_StopVideoCapture=camerap_StopVideoCapture;
        pCamDrv->camera_VideoCaptureActive=camerap_VideoCaptureActive;
        pCamDrv->camera_EnumerateVideoFrameSizes=camerap_EnumerateVideoFrameSizes;
        pCamDrv->camera_EnumerateVideoFrameRates=camerap_EnumerateVideoFrameRates;
        pCamDrv->camera_GetFrameSize=camerap_GetFrameSize;
        pCamDrv->camera_SetFrameRate=camerap_SetFrameRate;
        pCamDrv->camera_FrameRate=camerap_FrameRate;
        pCamDrv->camera_BuffersInUse=camerap_BuffersInUse;
        pCamDrv->camera_FramesPerBuffer=camerap_FramesPerBuffer;
        pCamDrv->camera_GetId=camerap_GetId;
        pCamDrv->camera_GetImageInfo = camerap_GetImageInfo;
        return TRUE;
    }

    return FALSE;
}

PRIVATE void camerap_SetZoomFactorL(UINT32 ZoomFactor)
{
}

PRIVATE UINT32 camerap_ZoomFactor(void)
{
    return(0);
}

PRIVATE void camerap_SetDigitalZoomFactorL(UINT32 DigZoomFactor)
{
}

PRIVATE UINT32 camerap_DigitalZoomFactor(void)
{
    return(0);
}

PRIVATE UINT32 camerap_Contrast(void)
{
    return(0);
}

PRIVATE UINT32 camerap_Brightness(void)
{
    return(0);
}

PRIVATE CAM_FLASH_T camerap_Flash(void)
{
    return(CAM_FLASH_NONE);
}

PRIVATE CAM_EXPOSURE_T camerap_Exposure(void)
{
    return(CAM_EXPOSURE_AUTO);
}

PRIVATE CAM_WHITEBALANCE_T camerap_WhiteBalance(void)
{
    return(CAM_WB_AUTO);
}

PRIVATE CAM_SPECIALEFFECT_T camerap_SpecialEffect(VOID)
{
    return(CAM_SPE_NORMAL);
}

PRIVATE VOID camerap_SetNightMode(UINT32 NightMode)
{
    if (NightMode)
    {
        camerap_WriteOneReg(0xfe,0x01);
        camerap_WriteOneReg(0x4c,0x30);
        camerap_WriteOneReg(0xfe,0x00);
    }
    else
    {
        camerap_WriteOneReg(0xfe,0x01);
        camerap_WriteOneReg(0x4c,0x10);
        camerap_WriteOneReg(0xfe,0x00);
    }
}

PRIVATE UINT32 camerap_NightMode(VOID)
{
    return(FALSE);

}

PRIVATE void camerap_StartViewFinderDirectL()
{
    // Not really direct, but sets the camera into viewfinder mode (lower resolution to fit the screen)
    // Needs
}

PRIVATE void camerap_StopViewFinder(void)
{
}

PRIVATE BOOL camerap_ViewFinderActive(void)
{
    return(FALSE);
}

PRIVATE void camerap_SetViewFinderMirror(BOOL Mirror)
{
}

PRIVATE BOOL camerap_ViewFinderMirror(void)
{
    return(FALSE);
}

PRIVATE void camerap_CancelCaptureImage(void)
{
}

PRIVATE void camerap_EnumerateCaptureSizes(CAM_SIZE_T *Size,UINT32 SizeIndex, CAM_FORMAT_T Format)
{
}

PRIVATE void camerap_PrepareVideoCaptureL(CAM_FORMAT_T Format, UINT32 SizeIndex, UINT32 RateIndex, UINT32 BuffersToUse, UINT32 FramesPerBuffer)
{
}

PRIVATE void camerap_StartVideoCapture(void)
{
}

PRIVATE void camerap_StopVideoCapture(void)
{
    if (gSensorInfo.capturevideo)
    {
    }
}

PRIVATE BOOL camerap_VideoCaptureActive(void)
{
    return(gSensorInfo.capturevideo);
}

PRIVATE void camerap_EnumerateVideoFrameSizes(
    CAM_SIZE_T Size,
    UINT32 SizeIndex,
    CAM_FORMAT_T Format)
{
}

PRIVATE void camerap_EnumerateVideoFrameRates()
{
}

PRIVATE void camerap_GetFrameSize(CAM_SIZE_T Size)
{
}

PRIVATE VOID camerap_SetFrameRate(UINT32 FrameRate)
{
}

PRIVATE UINT32 camerap_FrameRate(void)
{
    return(0);
}

PRIVATE UINT32 camerap_BuffersInUse(void)
{
    return(0);
}

PRIVATE UINT32 camerap_FramesPerBuffer(void)
{
    return(0);
}


