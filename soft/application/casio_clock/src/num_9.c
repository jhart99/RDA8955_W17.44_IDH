#include "lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_NUM_9
#define LV_ATTRIBUTE_IMG_NUM_9
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_NUM_9 uint8_t num_9_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
  /*Pixel format: Red: 3 bit, Green: 3 bit, Blue: 2 bit*/
  0xff, 0xff, 0xdb, 0xb6, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb6, 0xdb, 0xdb, 0xff, 0xff, 
  0xff, 0xb7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb7, 0x00, 0x6e, 0xff, 
  0xff, 0xb7, 0xdb, 0x92, 0x6e, 0x6e, 0x6e, 0x6e, 0x6e, 0x92, 0xdb, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0xb6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x6d, 0x00, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0x49, 0x24, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x25, 0x49, 0xff, 
  0xff, 0xff, 0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdb, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x49, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x6d, 0xdb, 0x92, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb6, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x00, 0x49, 0xff, 
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0x49, 0xdb, 0xff, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
  /*Pixel format: Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
  0xff, 0xff, 0xff, 0xff, 0x18, 0xc6, 0xf4, 0xa4, 0x14, 0xa5, 0x14, 0xa5, 0x14, 0xa5, 0x14, 0xa5, 0x14, 0xa5, 0xf4, 0xa4, 0x18, 0xc6, 0x7a, 0xd6, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x55, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0xa5, 0x00, 0x00, 0x4d, 0x6b, 0xff, 0xff, 
  0xff, 0xff, 0x55, 0xad, 0xd7, 0xbd, 0x51, 0x8c, 0x6e, 0x73, 0x6e, 0x73, 0x6e, 0x73, 0x6e, 0x73, 0x6e, 0x73, 0xae, 0x73, 0xb6, 0xb5, 0x00, 0x00, 0x49, 0x4a, 0xff, 0xff, 
  0xff, 0xff, 0x8a, 0x52, 0x00, 0x00, 0xd3, 0x9c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x51, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x71, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x71, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xcb, 0x5a, 0x00, 0x00, 0x31, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x14, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0x29, 0x4a, 0x82, 0x10, 0xbf, 0xff, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xdf, 0xff, 0x25, 0x29, 0xc7, 0x39, 0xff, 0xff, 
  0xff, 0xff, 0x1c, 0xe7, 0x79, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7a, 0xd6, 0x5d, 0xef, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x49, 0x4a, 0x04, 0x21, 0x45, 0x29, 0x45, 0x29, 0x45, 0x29, 0x45, 0x29, 0x24, 0x21, 0x8a, 0x52, 0x38, 0xc6, 0xcf, 0x7b, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3d, 0xef, 0x00, 0x00, 0x29, 0x4a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x14, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x00, 0x00, 0x6a, 0x52, 0xff, 0xff, 
  0xff, 0xff, 0x9a, 0xd6, 0xcb, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xc6, 0x00, 0x00, 0x28, 0x42, 0xff, 0xff, 
  0xff, 0xff, 0xdb, 0xde, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xef, 0x7b, 0xa7, 0x39, 0x39, 0xce, 0xff, 0xff, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
  /*Pixel format: Red: 5 bit, Green: 6 bit, Blue: 5 bit BUT the 2 bytes are swapped*/
  0xff, 0xff, 0xff, 0xff, 0xc6, 0x18, 0xa4, 0xf4, 0xa5, 0x14, 0xa5, 0x14, 0xa5, 0x14, 0xa5, 0x14, 0xa5, 0x14, 0xa4, 0xf4, 0xc6, 0x18, 0xd6, 0x7a, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xad, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa5, 0x34, 0x00, 0x00, 0x6b, 0x4d, 0xff, 0xff, 
  0xff, 0xff, 0xad, 0x55, 0xbd, 0xd7, 0x8c, 0x51, 0x73, 0x6e, 0x73, 0x6e, 0x73, 0x6e, 0x73, 0x6e, 0x73, 0x6e, 0x73, 0xae, 0xb5, 0xb6, 0x00, 0x00, 0x4a, 0x49, 0xff, 0xff, 
  0xff, 0xff, 0x52, 0x8a, 0x00, 0x00, 0x9c, 0xd3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x51, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x5a, 0xcb, 0x00, 0x00, 0x8c, 0x31, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x14, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0x4a, 0x29, 0x10, 0x82, 0xff, 0xbf, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xde, 0xfb, 0xff, 0xdf, 0x29, 0x25, 0x39, 0xc7, 0xff, 0xff, 
  0xff, 0xff, 0xe7, 0x1c, 0xce, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd6, 0x7a, 0xef, 0x5d, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4a, 0x49, 0x21, 0x04, 0x29, 0x45, 0x29, 0x45, 0x29, 0x45, 0x29, 0x45, 0x21, 0x24, 0x52, 0x8a, 0xc6, 0x38, 0x7b, 0xcf, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x3d, 0x00, 0x00, 0x4a, 0x29, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x14, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x34, 0x00, 0x00, 0x52, 0x6a, 0xff, 0xff, 
  0xff, 0xff, 0xd6, 0x9a, 0x5a, 0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x38, 0x00, 0x00, 0x42, 0x28, 0xff, 0xff, 
  0xff, 0xff, 0xde, 0xdb, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0xef, 0x39, 0xa7, 0xce, 0x39, 0xff, 0xff, 
#endif
#if LV_COLOR_DEPTH == 32
  /*Pixel format: Fix 0xFF: 8 bit, Red: 8 bit, Green: 8 bit, Blue: 8 bit*/
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc2, 0xc2, 0xc2, 0xff, 0x9e, 0x9e, 0x9e, 0xff, 0xa2, 0xa2, 0xa2, 0xff, 0xa2, 0xa2, 0xa2, 0xff, 0xa2, 0xa2, 0xa2, 0xff, 0xa2, 0xa2, 0xa2, 0xff, 0xa2, 0xa2, 0xa2, 0xff, 0x9d, 0x9d, 0x9d, 0xff, 0xbf, 0xbf, 0xbf, 0xff, 0xce, 0xce, 0xce, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xa9, 0xa9, 0xa9, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x6a, 0x6a, 0x6a, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xaa, 0xaa, 0xaa, 0xff, 0xb9, 0xb9, 0xb9, 0xff, 0x8a, 0x8a, 0x8a, 0xff, 0x6d, 0x6d, 0x6d, 0xff, 0x6e, 0x6e, 0x6e, 0xff, 0x6e, 0x6e, 0x6e, 0xff, 0x6e, 0x6e, 0x6e, 0xff, 0x6d, 0x6d, 0x6d, 0xff, 0x74, 0x74, 0x74, 0xff, 0xb3, 0xb3, 0xb3, 0xff, 0x00, 0x00, 0x00, 0xff, 0x49, 0x49, 0x49, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x51, 0x51, 0x51, 0xff, 0x00, 0x00, 0x00, 0xff, 0x99, 0x99, 0x99, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x8a, 0x8a, 0x8a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x8b, 0x8b, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x8b, 0x8b, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x86, 0x86, 0x86, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa1, 0xa1, 0xa1, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x45, 0x45, 0x45, 0xff, 0x12, 0x12, 0x12, 0xff, 0xf6, 0xf6, 0xf6, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0xf9, 0xf9, 0xf9, 0xff, 0x26, 0x26, 0x26, 0xff, 0x37, 0x37, 0x37, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xdf, 0xdf, 0xdf, 0xff, 0xcc, 0xcc, 0xcc, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xce, 0xce, 0xce, 0xff, 0xea, 0xea, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4a, 0x4a, 0x4a, 0xff, 0x22, 0x22, 0x22, 0xff, 0x27, 0x27, 0x27, 0xff, 0x27, 0x27, 0x27, 0xff, 0x27, 0x27, 0x27, 0xff, 0x27, 0x27, 0x27, 0xff, 0x23, 0x23, 0x23, 0xff, 0x51, 0x51, 0x51, 0xff, 0xc3, 0xc3, 0xc3, 0xff, 0x79, 0x79, 0x79, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe5, 0xe5, 0xe5, 0xff, 0x00, 0x00, 0x00, 0xff, 0x46, 0x46, 0x46, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x9f, 0x9f, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4e, 0x4e, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa4, 0xa4, 0xa4, 0xff, 0x00, 0x00, 0x00, 0xff, 0x4d, 0x4d, 0x4d, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xd1, 0xd1, 0xd1, 0xff, 0x5a, 0x5a, 0x5a, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xc3, 0xc3, 0xc3, 0xff, 0x00, 0x00, 0x00, 0xff, 0x44, 0x44, 0x44, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xd7, 0xd7, 0xd7, 0xff, 0x04, 0x04, 0x04, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x7b, 0x7b, 0x7b, 0xff, 0x36, 0x36, 0x36, 0xff, 0xc5, 0xc5, 0xc5, 0xff, 0xff, 0xff, 0xff, 0xff, 
#endif
};

const lv_img_dsc_t num_9 = {
  .header.always_zero = 0,
  .header.w = 14,
  .header.h = 19,
  .data_size = 266 * LV_COLOR_SIZE / 8,
  .header.cf = LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED,
  .data = num_9_map,
};