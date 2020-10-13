/**
 * @file    font_6x8.c
 * @author  meng_yu
 * @brief   font_6x8 function realize
 * @version 0.0.1
 * @date    2020-10-09
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "fonts.h"

//normal char table
//char offset 32
//ASCII char: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
//PC2LCD2002��YinMa+ZhuHangShi+NiXiang+C51fmt

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

static const uint8_t GuLimChe_12x24[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"!",1*/
0x00,0x00,0x00,0x00,0x10,0x01,0x10,0x01,0x10,0x01,0x10,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*""",2*/
0x00,0x00,0x00,0x00,0x60,0x06,0x60,0x06,0x60,0x06,0x60,0x06,0x60,0x06,0xFC,0x0F,0x30,0x03,0x30,0x03,0x30,0x03,0x30,0x03,
0x30,0x03,0x30,0x03,0xFE,0x0F,0x98,0x01,0x98,0x01,0x98,0x01,0x98,0x01,0x98,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"#",3*/
0x60,0x00,0x60,0x00,0xF8,0x01,0x6C,0x03,0x66,0x06,0x66,0x06,0x66,0x00,0x66,0x00,0x66,0x00,0x6C,0x00,0x78,0x00,0xE0,0x01,
0x60,0x03,0x60,0x06,0x60,0x06,0x66,0x06,0x66,0x06,0x66,0x06,0x6C,0x03,0xF8,0x01,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,/*"$",4*/
0x00,0x00,0x00,0x00,0x3C,0x06,0x66,0x06,0x66,0x03,0x66,0x03,0xE6,0x01,0xE6,0x01,0xBC,0x01,0xC0,0x00,0xC0,0x00,0x60,0x00,
0x60,0x00,0xB0,0x07,0xF0,0x0C,0xF0,0x0C,0xD8,0x0C,0xD8,0x0C,0xCC,0x0C,0x8C,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"%",5*/
0x00,0x00,0x00,0x00,0x70,0x00,0xD8,0x00,0x8C,0x01,0x8C,0x01,0x8C,0x01,0x8C,0x01,0xD8,0x00,0x70,0x00,0x30,0x00,0x78,0x00,
0xCC,0x0C,0xCC,0x0C,0x86,0x0D,0x86,0x07,0x06,0x07,0x06,0x03,0x8C,0x07,0xF8,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"&",6*/
0x00,0x00,0x00,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"'",7*/
0x00,0x00,0x00,0x03,0x80,0x01,0xC0,0x00,0xC0,0x00,0x60,0x00,0x60,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,
0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x60,0x00,0x60,0x00,0xC0,0x00,0xC0,0x00,0x80,0x01,0x00,0x03,0x00,0x00,/*"(",8*/
0x00,0x00,0x0C,0x00,0x18,0x00,0x30,0x00,0x30,0x00,0x60,0x00,0x60,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x60,0x00,0x60,0x00,0x30,0x00,0x30,0x00,0x18,0x00,0x0C,0x00,0x00,0x00,/*")",9*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x66,0x06,0xFC,0x03,0xF0,0x00,
0xF0,0x00,0x98,0x01,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"*",10*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xFE,0x07,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"+",11*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0xE0,0x00,0x80,0x00,0x60,0x00,0x00,0x00,0x00,0x00,/*",",12*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x07,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"-",13*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*".",14*/
0x00,0x04,0x00,0x04,0x00,0x06,0x00,0x06,0x00,0x02,0x00,0x03,0x00,0x03,0x00,0x01,0x80,0x01,0x80,0x01,0x80,0x00,0xC0,0x00,
0xC0,0x00,0x40,0x00,0x60,0x00,0x60,0x00,0x20,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x18,0x00,0x18,0x00,0x08,0x00,0x00,0x00,/*"/",15*/
0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x0C,0x06,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,
0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x0C,0x06,0x0C,0x06,0x18,0x03,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"0",16*/
0x00,0x00,0x00,0x00,0xC0,0x00,0xE0,0x00,0xF8,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"1",17*/
0x00,0x00,0x00,0x00,0xF8,0x01,0x0C,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x00,0x06,0x00,0x03,0x00,0x03,0x80,0x01,
0xC0,0x00,0x70,0x00,0x18,0x00,0x0C,0x00,0x0C,0x00,0x06,0x00,0x06,0x00,0xFE,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"2",18*/
0x00,0x00,0x00,0x00,0xF8,0x01,0x0C,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x00,0x06,0x00,0x06,0x00,0x03,0xF0,0x01,0x00,0x03,
0x00,0x06,0x00,0x06,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"3",19*/
0x00,0x00,0x00,0x00,0x80,0x03,0x80,0x03,0xC0,0x03,0x40,0x03,0x60,0x03,0x20,0x03,0x30,0x03,0x10,0x03,0x18,0x03,0x08,0x03,
0x0C,0x03,0x04,0x03,0x06,0x03,0xFE,0x0F,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"4",20*/
0x00,0x00,0x00,0x00,0xFC,0x03,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0xFC,0x01,0x0C,0x03,0x00,0x06,
0x00,0x06,0x00,0x06,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"5",21*/
0x00,0x00,0x00,0x00,0xF8,0x01,0x0C,0x03,0x06,0x06,0x06,0x06,0x06,0x00,0x06,0x00,0x06,0x00,0xF6,0x01,0x0E,0x03,0x06,0x06,
0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"6",22*/
0x00,0x00,0x00,0x00,0xFE,0x07,0x00,0x06,0x00,0x06,0x00,0x03,0x00,0x03,0x00,0x03,0x80,0x01,0x80,0x01,0x80,0x01,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"7",23*/
0x00,0x00,0x00,0x00,0xF8,0x01,0x0C,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0xF8,0x01,0x0C,0x03,
0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"8",24*/
0x00,0x00,0x00,0x00,0xF8,0x01,0x0C,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x07,
0xF8,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"9",25*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*":",26*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0xE0,0x00,0x80,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*";",27*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x07,0x80,0x03,0xE0,0x00,0x70,0x00,0x1C,0x00,0x0E,0x00,0x06,0x00,
0x0E,0x00,0x1C,0x00,0x70,0x00,0xE0,0x00,0x80,0x03,0x00,0x07,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"<",28*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x07,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFE,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"=",29*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x0E,0x00,0x1C,0x00,0x70,0x00,0xE0,0x00,0x80,0x03,0x00,0x07,0x00,0x06,
0x00,0x07,0x80,0x03,0xE0,0x00,0x70,0x00,0x1C,0x00,0x0E,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*">",30*/
0x00,0x00,0x00,0x00,0xF8,0x01,0x0C,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x00,0x03,0x00,0x03,0x80,0x01,0xC0,0x00,
0xC0,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"?",31*/
0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x0C,0x0C,0x46,0x0D,0xE6,0x0D,0xB6,0x0D,0xB6,0x0D,0xB6,0x0D,
0xB6,0x0D,0xB6,0x0D,0xB6,0x0D,0xE6,0x06,0x06,0x00,0x0C,0x0C,0x18,0x06,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"@",32*/
0x00,0x00,0x00,0x00,0x40,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xB0,0x01,0xB0,0x01,0xB0,0x01,0x18,0x03,0x18,0x03,0x18,0x03,
0x0C,0x06,0x0C,0x06,0xFC,0x07,0x0C,0x06,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"A",33*/
0x00,0x00,0x00,0x00,0xFC,0x01,0x06,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x03,0xFE,0x01,0x06,0x03,
0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x03,0xFC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"B",34*/
0x00,0x00,0x00,0x00,0xF0,0x03,0x18,0x06,0x0C,0x0C,0x0C,0x0C,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x0C,0x0C,0x0C,0x0C,0x18,0x06,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"C",35*/
0x00,0x00,0x00,0x00,0xF8,0x01,0x0C,0x03,0x0C,0x06,0x0C,0x06,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,
0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x06,0x0C,0x06,0x0C,0x03,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"D",36*/
0x00,0x00,0x00,0x00,0xFC,0x07,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0xFE,0x03,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0xFC,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"E",37*/
0x00,0x00,0x00,0x00,0xFC,0x07,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0xFE,0x03,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"F",38*/
0x00,0x00,0x00,0x00,0xF0,0x03,0x18,0x06,0x0C,0x0C,0x0C,0x0C,0x06,0x0C,0x06,0x00,0x06,0x00,0x06,0x00,0xC6,0x0F,0x06,0x0C,
0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x0C,0x0C,0x0C,0x0E,0x18,0x0F,0xF0,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"G",39*/
0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0xFE,0x07,0x06,0x06,
0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"H",40*/
0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"I",41*/
0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,
0x00,0x03,0x00,0x03,0x00,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x8C,0x01,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"J",42*/
0x00,0x00,0x00,0x00,0x06,0x04,0x06,0x06,0x06,0x03,0x86,0x01,0xC6,0x00,0x66,0x00,0x36,0x00,0x3E,0x00,0x6E,0x00,0x66,0x00,
0xC6,0x00,0xC6,0x00,0x86,0x01,0x86,0x01,0x06,0x03,0x06,0x03,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"K",43*/
0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0xFC,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"L",44*/
0x00,0x00,0x00,0x00,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x1E,0x0F,0x1E,0x0F,0x16,0x0D,0xB6,0x0D,
0xB6,0x0D,0xA6,0x0C,0xE6,0x0C,0xE6,0x0C,0x46,0x0C,0x46,0x0C,0x46,0x0C,0x06,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"M",45*/
0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x0E,0x06,0x0E,0x06,0x1E,0x06,0x16,0x06,0x36,0x06,0x26,0x06,0x66,0x06,0x46,0x06,
0xC6,0x06,0x86,0x06,0x86,0x07,0x06,0x07,0x06,0x07,0x06,0x06,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"N",46*/
0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x0C,0x06,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,
0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x0C,0x06,0x0C,0x06,0x18,0x03,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"O",47*/
0x00,0x00,0x00,0x00,0xFC,0x01,0x06,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x03,0xFE,0x01,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"P",48*/
0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x0C,0x06,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,
0x06,0x0C,0x06,0x0C,0x06,0x0C,0x66,0x0C,0xCC,0x06,0x8C,0x07,0x18,0x03,0xF0,0x07,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,/*"Q",49*/
0x00,0x00,0x00,0x00,0xFC,0x01,0x06,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x03,0xFE,0x01,0x06,0x03,
0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"R",50*/
0x00,0x00,0x00,0x00,0xF8,0x01,0x0C,0x03,0x06,0x06,0x06,0x06,0x06,0x00,0x06,0x00,0x0C,0x00,0x38,0x00,0xE0,0x00,0x80,0x01,
0x00,0x03,0x00,0x06,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"S",51*/
0x00,0x00,0x00,0x00,0xFE,0x07,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"T",52*/
0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"U",53*/
0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x98,0x01,0x98,0x01,
0x98,0x01,0x98,0x01,0xF0,0x00,0xF0,0x00,0xF0,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"V",54*/
0x00,0x00,0x00,0x00,0x46,0x0C,0x46,0x0C,0x46,0x0C,0x46,0x0C,0xE6,0x0C,0xE6,0x0C,0xE6,0x0C,0xAC,0x06,0xAC,0x06,0xAC,0x06,
0xAC,0x06,0xBC,0x07,0xBC,0x07,0x18,0x03,0x18,0x03,0x18,0x03,0x18,0x03,0x18,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"W",55*/
0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x0C,0x03,0x0C,0x03,0x98,0x01,0x98,0x01,0xF0,0x00,0xF0,0x00,0x60,0x00,0x60,0x00,
0xF0,0x00,0xF0,0x00,0x98,0x01,0x98,0x01,0x0C,0x03,0x0C,0x03,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"X",56*/
0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x98,0x01,0x98,0x01,0xF0,0x00,0xF0,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"Y",57*/
0x00,0x00,0x00,0x00,0xFE,0x07,0x00,0x06,0x00,0x03,0x00,0x03,0x80,0x01,0x80,0x01,0xC0,0x00,0xC0,0x00,0x60,0x00,0x60,0x00,
0x30,0x00,0x30,0x00,0x18,0x00,0x18,0x00,0x0C,0x00,0x0C,0x00,0x06,0x00,0xFE,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"Z",58*/
0x00,0x00,0xE0,0x03,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xE0,0x03,0x00,0x00,/*"[",59*/
0x00,0x00,0x00,0x00,0x4C,0x06,0x4C,0x06,0x4C,0x06,0x4C,0x06,0x4C,0x06,0xEC,0x06,0xEC,0x06,0xFE,0x0F,0xAC,0x06,0xAC,0x06,
0xAC,0x06,0xBC,0x07,0xB8,0x03,0x18,0x03,0x18,0x03,0x18,0x03,0x18,0x03,0x18,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"\",60*/
0x00,0x00,0xF8,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xF8,0x00,0x00,0x00,/*"]",61*/
0x00,0x00,0x60,0x00,0xF0,0x00,0x98,0x01,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"^",62*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x0F,0x00,0x00,/*"_",63*/
0x00,0x00,0x00,0x00,0x30,0x00,0x60,0x00,0xC0,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"`",64*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x00,0x06,
0x00,0x06,0xF0,0x07,0x18,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x18,0x07,0xF0,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"a",65*/
0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0xEC,0x01,0x1C,0x03,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x1C,0x03,0xEC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"b",66*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x0C,0x00,
0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x06,0x18,0x03,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"c",67*/
0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0xF0,0x06,0x18,0x07,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x18,0x07,0xF0,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"d",68*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0xFC,0x07,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x06,0x18,0x03,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"e",69*/
0x00,0x00,0x00,0x00,0xC0,0x03,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xFC,0x03,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"f",70*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x06,0x18,0x07,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x18,0x07,0xF0,0x06,0x00,0x06,0x0C,0x06,0x18,0x03,0xF0,0x01,0x00,0x00,/*"g",71*/
0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0xEC,0x01,0x1C,0x03,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"h",72*/
0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"i",73*/
0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x60,0x00,0x3C,0x00,0x00,0x00,/*"j",74*/
0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x06,0x0C,0x03,0x8C,0x01,0xCC,0x00,
0x6C,0x00,0x3C,0x00,0x7C,0x00,0xCC,0x00,0x8C,0x01,0x0C,0x03,0x0C,0x06,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"k",75*/
0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"l",76*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x74,0x07,0xCC,0x0C,0xCC,0x0C,0xCC,0x0C,
0xCC,0x0C,0xCC,0x0C,0xCC,0x0C,0xCC,0x0C,0xCC,0x0C,0xCC,0x0C,0xCC,0x0C,0xCC,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"m",77*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEC,0x00,0x9C,0x01,0x0C,0x03,0x0C,0x03,
0x0C,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"n",78*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x18,0x03,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"o",79*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEC,0x01,0x1C,0x03,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x1C,0x03,0xEC,0x01,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,/*"p",80*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x06,0x18,0x07,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x18,0x07,0xF0,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,/*"q",81*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xB0,0x03,0x70,0x00,0x30,0x00,0x30,0x00,
0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"r",82*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x01,0x18,0x03,0x0C,0x06,0x0C,0x06,
0x18,0x00,0xF0,0x00,0x80,0x03,0x00,0x06,0x0C,0x06,0x0C,0x06,0x18,0x03,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"s",83*/
0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xFC,0x03,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"t",84*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,
0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x18,0x07,0xF0,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"u",85*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x06,0x0C,0x06,0x0C,0x06,0x18,0x03,
0x18,0x03,0x18,0x03,0xB0,0x01,0xB0,0x01,0xB0,0x01,0xE0,0x00,0xE0,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"v",86*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x0C,0x46,0x0C,0x46,0x0C,0x46,0x0C,
0xEC,0x06,0xEC,0x06,0xAC,0x06,0xAC,0x06,0xB8,0x03,0x18,0x03,0x18,0x03,0x18,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"w",87*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x06,0x0C,0x06,0x18,0x03,0x18,0x03,
0xB0,0x01,0xE0,0x00,0xE0,0x00,0xB0,0x01,0x18,0x03,0x18,0x03,0x0C,0x06,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"x",88*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x18,0x06,
0x18,0x06,0x18,0x06,0x30,0x03,0x30,0x03,0xE0,0x01,0xE0,0x01,0xC0,0x00,0xC0,0x00,0x60,0x00,0x30,0x00,0x1C,0x00,0x00,0x00,/*"y",89*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x07,0x00,0x06,0x00,0x03,0x80,0x01,
0x80,0x01,0xC0,0x00,0x60,0x00,0x60,0x00,0x30,0x00,0x18,0x00,0x0C,0x00,0xFC,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"z",90*/
0x00,0x00,0xC0,0x01,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x38,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xC0,0x01,0x00,0x00,/*"{",91*/
0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,/*"|",92*/
0x00,0x00,0x70,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x80,0x03,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x70,0x00,0x00,0x00,/*"}",93*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0xEC,0x06,
0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"~",94*/
};

GLCD_FONT GLCD_Font_12x24 = {
  12,                                  ///< Character width
  24,                                  ///< Character height
  32,                                  ///< Character offset
  96,                                  ///< Character count
  GuLimChe_12x24                       ///< Characters bitmaps
};
