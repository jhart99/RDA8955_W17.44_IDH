#include "lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_NUM_4
#define LV_ATTRIBUTE_IMG_NUM_4
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_NUM_4 uint8_t num_4_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
  /*Pixel format: Red: 3 bit, Green: 3 bit, Blue: 2 bit*/
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x92, 0xff, 
  0xff, 0x49, 0x00, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb6, 0x00, 0x49, 0xff, 
  0xff, 0x49, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x24, 0x49, 0xff, 
  0xff, 0xff, 0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdb, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x49, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x6d, 0xb7, 0x6e, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb6, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb6, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x49, 0xff, 0xff, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
  /*Pixel format: Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xdb, 0xde, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xaf, 0x7b, 0xff, 0xff, 
  0xff, 0xff, 0x49, 0x4a, 0x00, 0x00, 0x75, 0xad, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb6, 0xb5, 0x00, 0x00, 0x49, 0x4a, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x51, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x14, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x71, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x71, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x71, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x31, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x14, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0x49, 0x4a, 0x20, 0x00, 0x7e, 0xf7, 0xfc, 0xe6, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0x9e, 0xf7, 0xa3, 0x18, 0xc7, 0x39, 0xff, 0xff, 
  0xff, 0xff, 0xba, 0xd6, 0x39, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xce, 0xdb, 0xde, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x49, 0x4a, 0x04, 0x21, 0x45, 0x29, 0x45, 0x29, 0x45, 0x29, 0x45, 0x29, 0x24, 0x21, 0xaa, 0x52, 0x96, 0xb5, 0x6e, 0x73, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xde, 0x00, 0x00, 0x49, 0x4a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x14, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0xa4, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3c, 0xe7, 0x00, 0x00, 0x29, 0x4a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x39, 0x9a, 0xd6, 0xff, 0xff, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
  /*Pixel format: Red: 5 bit, Green: 6 bit, Blue: 5 bit BUT the 2 bytes are swapped*/
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd6, 0xba, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xde, 0xdb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7b, 0xaf, 0xff, 0xff, 
  0xff, 0xff, 0x4a, 0x49, 0x00, 0x00, 0xad, 0x75, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb5, 0xb6, 0x00, 0x00, 0x4a, 0x49, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x51, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x14, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x31, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x14, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x4a, 0x49, 0x00, 0x20, 0xf7, 0x7e, 0xe6, 0xfc, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xf7, 0x9e, 0x18, 0xa3, 0x39, 0xc7, 0xff, 0xff, 
  0xff, 0xff, 0xd6, 0xba, 0xce, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xce, 0x59, 0xde, 0xdb, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4a, 0x49, 0x21, 0x04, 0x29, 0x45, 0x29, 0x45, 0x29, 0x45, 0x29, 0x45, 0x21, 0x24, 0x52, 0xaa, 0xb5, 0x96, 0x73, 0x6e, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0xfb, 0x00, 0x00, 0x4a, 0x49, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x14, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xf4, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x3c, 0x00, 0x00, 0x4a, 0x29, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x39, 0xc7, 0xd6, 0x9a, 0xff, 0xff, 
#endif
#if LV_COLOR_DEPTH == 32
  /*Pixel format: Fix 0xFF: 8 bit, Red: 8 bit, Green: 8 bit, Blue: 8 bit*/
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd4, 0xd4, 0xd4, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xd8, 0xd8, 0xd8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x75, 0x75, 0x75, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x49, 0x49, 0x49, 0xff, 0x00, 0x00, 0x00, 0xff, 0xac, 0xac, 0xac, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb4, 0xb4, 0xb4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x48, 0x48, 0x48, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x87, 0x87, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa2, 0xa2, 0xa2, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x8b, 0x8b, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x8b, 0x8b, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x8b, 0x8b, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x85, 0x85, 0x85, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa0, 0xa0, 0xa0, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x47, 0x47, 0x47, 0xff, 0x04, 0x04, 0x04, 0xff, 0xed, 0xed, 0xed, 0xff, 0xdd, 0xdd, 0xdd, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xf0, 0xf0, 0xf0, 0xff, 0x15, 0x15, 0x15, 0xff, 0x39, 0x39, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xd4, 0xd4, 0xd4, 0xff, 0xc5, 0xc5, 0xc5, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xc8, 0xc8, 0xc8, 0xff, 0xda, 0xda, 0xda, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4a, 0x4a, 0x4a, 0xff, 0x22, 0x22, 0x22, 0xff, 0x27, 0x27, 0x27, 0xff, 0x27, 0x27, 0x27, 0xff, 0x27, 0x27, 0x27, 0xff, 0x27, 0x27, 0x27, 0xff, 0x23, 0x23, 0x23, 0xff, 0x53, 0x53, 0x53, 0xff, 0xaf, 0xaf, 0xaf, 0xff, 0x6d, 0x6d, 0x6d, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xdb, 0xdb, 0xff, 0x00, 0x00, 0x00, 0xff, 0x47, 0x47, 0x47, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa0, 0xa0, 0xa0, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9d, 0x9d, 0x9d, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4d, 0x4d, 0x4d, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe4, 0xe4, 0xe4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x45, 0x45, 0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x37, 0x37, 0x37, 0xff, 0xd2, 0xd2, 0xd2, 0xff, 0xff, 0xff, 0xff, 0xff, 
#endif
};

const lv_img_dsc_t num_4 = {
  .header.always_zero = 0,
  .header.w = 14,
  .header.h = 19,
  .data_size = 266 * LV_COLOR_SIZE / 8,
  .header.cf = LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED,
  .data = num_4_map,
};