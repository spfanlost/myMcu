/**
 * @file    fonts.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-10-09
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _FONTS_H_
#define _FONTS_H_

#include <stdint.h>

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/
/* Font definitions */
typedef struct _GLCD_FONT {
	uint16_t width;         ///< Character width
	uint16_t height;        ///< Character height
	uint32_t offset;        ///< Character offset
	uint32_t count;         ///< Character count
	const uint8_t *bitmap;        ///< Characters bitmaps
} const GLCD_FONT;

extern GLCD_FONT GLCD_Font_6x12;
extern GLCD_FONT GLCD_Font_8x16;
extern GLCD_FONT GLCD_Font_12x24;

#endif /* _FONTS_H_ */
