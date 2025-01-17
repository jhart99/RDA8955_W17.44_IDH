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







BYTE Map182_Regs[1];
BYTE Map182_IRQ_Enable;
BYTE Map182_IRQ_Cnt;




void Map182_Init()
{

    MapperInit = Map182_Init;


    MapperWrite = Map182_Write;


    MapperSram = Map0_Sram;


    MapperApu = Map0_Apu;


    MapperReadApu = Map0_ReadApu;


    MapperVSync = Map0_VSync;


    MapperHSync = Map182_HSync;


    MapperPPU = Map0_PPU;


    MapperRenderScreen = Map0_RenderScreen;


    SRAMBANK = SRAM;


    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );


    if ( NesHeader.byVRomSize > 0 )
    {
        for ( int nPage = 0; nPage < 8; ++nPage )
        {
            PPUBANK[ nPage ] = VROMPAGE( nPage );
        }
        InfoNES_SetupChr();
    }


    Map182_Regs[0] = 0;
    Map182_IRQ_Enable = 0;
    Map182_IRQ_Cnt = 0;


    K6502_Set_Int_Wiring( 1, 1 );
}




void Map182_Write( WORD wAddr, BYTE byData )
{
    switch( wAddr & 0xF003 )
    {

        case 0x8001:
            if ( byData & 0x01 )
            {
                InfoNES_Mirroring( 0 );
            }
            else
            {
                InfoNES_Mirroring( 1 );
            }
            break;

        case 0xA000:
            Map182_Regs[0] = byData & 0x07;
            break;

        case 0xC000:
            switch( Map182_Regs[0] )
            {

                case 0x00:
                    PPUBANK[ 0 ] = VROMPAGE( ( ( byData & 0xFE ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
                    PPUBANK[ 1 ] = VROMPAGE( ( ( byData & 0xFE ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
                    InfoNES_SetupChr();
                    break;

                case 0x01:
                    PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
                    InfoNES_SetupChr();
                    break;

                case 0x02:
                    PPUBANK[ 2 ] = VROMPAGE( ( ( byData & 0xFE ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
                    PPUBANK[ 3 ] = VROMPAGE( ( ( byData & 0xFE ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
                    InfoNES_SetupChr();
                    break;

                case 0x03:
                    PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
                    InfoNES_SetupChr();
                    break;


                case 0x04:
                    ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
                    break;

                case 0x05:
                    ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
                    break;


                case 0x06:
                    PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
                    InfoNES_SetupChr();
                    break;

                case 0x07:
                    PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
                    InfoNES_SetupChr();
                    break;
            }
            break;

        case 0xE003:
            Map182_IRQ_Cnt = byData;
            Map182_IRQ_Enable = byData;
            break;
    }
}




void Map182_HSync()
{

    if ( Map182_IRQ_Enable )
    {
        if (  PPU_Scanline <= 240 )
        {
            if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
            {
                if ( !( --Map182_IRQ_Cnt ) )
                {
                    Map182_IRQ_Cnt = 0;
                    Map182_IRQ_Enable = 0;
                    IRQ_REQ;
                }
            }
        }
    }
}
