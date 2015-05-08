#ifndef __LCD_GRAPHICS_H__
#define __LCD_GRAPHICS_H__

const static uint8_t Image_LCD_GameSelect[] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255, 63,223,223,
  223,255,127,127,255,255,127,127,127,255,127,127,255,255,255,127,
  255,255,127,127,127,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,  0,  0,  0,  0,192, 32,192,  0,  0,128,128,128,
    0,  0,160,  0,128,128,128,  0,128,128,  0,  0,128,128,  0,  0,
  128,192,128,  0,160,  0,  0,128,128,  0,  0,128,128,128,  0,  0,
  128,128,128,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,
  255,255,255,248,247,246,240,255,241,245,240,255,240,255,255,240,
  255,255,240,255,248,245,244,255,244,245,241,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,128,128,128,142,131,130,
  131,142,128,143,128,128,143,128,143,128,143,128,128,143,128,128,
  143,128,142,138,143,128,128,143,136,128,143,128,135,136,136,135,
  128,143,128,128,143,128,139,138,142,128,128,128
};

const static uint8_t Image_LCD_AnimationSelect[] =
{
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 32, 32,
   32,  0,128,128,  0,  0,128,128,128,  0,128,128,  0,  0,  0,128,
    0,  0,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,255,255,255,255, 63,223, 63,255,255,127,127,127,
  255,255, 95,255,127,127,127,255,127,127,255,255,127,127,255,255,
  127, 63,127,255, 95,255,255,127,127,255,255,127,127,127,255,255,
  127,127,127,255,255,255,128,128,128,128,128,128,128,128,128,128,
  128,128,128,135,136,137,143,128,142,138,143,128,143,128,128,143,
  128,128,143,128,135,138,139,128,139,138,142,128,128,128,128,128,
  128,128,128,128,128,128,128,128,128,128,255,255,255,241,252,253,
  252,241,255,240,255,255,240,255,240,255,240,255,255,240,255,255,
  240,255,241,245,240,255,255,240,247,255,240,255,248,247,247,248,
  255,240,255,255,240,255,244,245,241,255,255,255
};

// Character bitmaps for HP Simplified 8pt
const static uint8_t hPSimplified_8ptBitmaps[] = 
{
 // 'a' (9 pixels wide)
 // 
 // 
 // 
 // ### 
 // # 
 // ### 
 // # # 
 // # # 
 // ### 
 // 
 // 
 0x00, 0x00, 0x00, 0xC8, 0x28, 0x28, 0xF0, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 


 // 'b' (9 pixels wide)
 // 
 // # 
 // # 
 // #### 
 // # # 
 // # # 
 // # # 
 // # # 
 // #### 
 // 
 // 
 0x00, 0x00, 0xFE, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 
 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 


 // 'c' (9 pixels wide)
 // 
 // 
 // 
 // ### 
 // # 
 // # 
 // # 
 // # 
 // ### 
 // 
 // 
 0x00, 0x00, 0x00, 0xF0, 0x08, 0x08, 0x08, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 


 // 'd' (9 pixels wide)
 // 
 // # 
 // # 
 // #### 
 // # # 
 // # # 
 // # # 
 // # # 
 // #### 
 // 
 // 
 0x00, 0x00, 0xF0, 0x08, 0x08, 0x08, 0xFE, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 


 // 'e' (9 pixels wide)
 // 
 // 
 // 
 // ## 
 // # # 
 // #### 
 // # 
 // # 
 // ### 
 // 
 // 
 0x00, 0x00, 0x00, 0xF0, 0x28, 0x28, 0x30, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 


 // 'f' (9 pixels wide)
 // 
 // ## 
 // # 
 // ### 
 // # 
 // # 
 // # 
 // # 
 // # 
 // 
 // 
 0x00, 0x00, 0x00, 0x08, 0xFE, 0x0A, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 


 // 'g' (9 pixels wide)
 // 
 // 
 // 
 // #### 
 // # # 
 // # # 
 // # # 
 // # # 
 // #### 
 // # 
 // #### 
 0x00, 0x00, 0xF0, 0x08, 0x08, 0x08, 0xF8, 0x00, 0x00, 
 0x00, 0x00, 0x04, 0x05, 0x05, 0x05, 0x03, 0x00, 0x00, 


 // 'h' (9 pixels wide)
 // 
 // # 
 // # 
 // #### 
 // # # 
 // # # 
 // # # 
 // # # 
 // # # 
 // 
 // 
 0x00, 0x00, 0xFE, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 
 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 


 // 'i' (9 pixels wide)
 // 
 // # 
 // 
 // # 
 // # 
 // # 
 // # 
 // # 
 // # 
 // 
 // 
 0x00, 0x00, 0x00, 0x00, 0xFA, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 


 // 'j' (9 pixels wide)
 // 
 // # 
 // 
 // # 
 // # 
 // # 
 // # 
 // # 
 // # 
 // # 
 // ## 
 0x00, 0x00, 0x00, 0x00, 0xFA, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x00, 


 // 'k' (9 pixels wide)
 // 
 // # 
 // # 
 // # # 
 // # # 
 // ## 
 // ## 
 // # # 
 // # # 
 // 
 // 
 0x00, 0x00, 0x00, 0xFE, 0x60, 0x90, 0x08, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 


 // 'l' (9 pixels wide)
 // 
 // # 
 // # 
 // # 
 // # 
 // # 
 // # 
 // # 
 // ## 
 // 
 // 
 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 


 // 'm' (9 pixels wide)
 // 
 // 
 // 
 // ### ## 
 // # # # 
 // # # # 
 // # # # 
 // # # # 
 // # # # 
 // 
 // 
 0x00, 0xF8, 0x08, 0x08, 0xF0, 0x08, 0x08, 0xF0, 0x00, 
 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 


 // 'n' (9 pixels wide)
 // 
 // 
 // 
 // #### 
 // # # 
 // # # 
 // # # 
 // # # 
 // # # 
 // 
 // 
 0x00, 0x00, 0xF8, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 
 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 


 // 'o' (9 pixels wide)
 // 
 // 
 // 
 // ### 
 // # # 
 // # # 
 // # # 
 // # # 
 // ### 
 // 
 // 
 0x00, 0x00, 0xF0, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 


 // 'p' (9 pixels wide)
 // 
 // 
 // 
 // #### 
 // # # 
 // # # 
 // # # 
 // # # 
 // #### 
 // # 
 // # 
 0x00, 0x00, 0xF8, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 
 0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 


 // 'q' (9 pixels wide)
 // 
 // 
 // 
 // #### 
 // # # 
 // # # 
 // # # 
 // # # 
 // #### 
 // # 
 // # 
 0x00, 0x00, 0xF0, 0x08, 0x08, 0x08, 0xF8, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x07, 0x00, 0x00, 


 // 'r' (9 pixels wide)
 // 
 // 
 // 
 // ### 
 // # 
 // # 
 // # 
 // # 
 // # 
 // 
 // 
 0x00, 0x00, 0x00, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 


 // 's' (9 pixels wide)
 // 
 // 
 // 
 // ### 
 // # 
 // ## 
 // ## 
 // # 
 // ### 
 // 
 // 
 0x00, 0x00, 0x30, 0x28, 0x48, 0xC8, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 


 // 't' (9 pixels wide)
 // 
 // 
 // # 
 // ### 
 // # 
 // # 
 // # 
 // # 
 // ## 
 // 
 // 
 0x00, 0x00, 0x00, 0x08, 0xFC, 0x08, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 


 // 'u' (9 pixels wide)
 // 
 // 
 // 
 // # # 
 // # # 
 // # # 
 // # # 
 // # # 
 // #### 
 // 
 // 
 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 


 // 'v' (9 pixels wide)
 // 
 // 
 // 
 // # # 
 // # # 
 // # # 
 // # # 
 // # # 
 // # 
 // 
 // 
 0x00, 0x00, 0x18, 0xE0, 0x00, 0xE0, 0x18, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 


 // 'w' (9 pixels wide)
 // 
 // 
 // 
 // # # # 
 // # # # 
 // # # # # 
 // # # # # 
 // # # # # 
 // # # 
 // 
 // 
 0x00, 0xF8, 0x00, 0xE0, 0x18, 0xE0, 0x00, 0xF8, 0x00, 
 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 


 // 'x' (9 pixels wide)
 // 
 // 
 // 
 // # # 
 // # # 
 // ## 
 // ## 
 // # # 
 // # # 
 // 
 // 
 0x00, 0x00, 0x98, 0x60, 0x60, 0x98, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 


 // 'y' (9 pixels wide)
 // 
 // 
 // 
 // # # 
 // # # 
 // # # 
 // # # 
 // # # 
 // # 
 // # 
 // ## 
 0x00, 0x00, 0x18, 0xE0, 0x00, 0xE0, 0x18, 0x00, 0x00, 
 0x00, 0x00, 0x04, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 


 // 'z' (9 pixels wide)
 // 
 // 
 // 
 // #### 
 // # 
 // # 
 // # 
 // # 
 // #### 
 // 
 // 
 0x00, 0x00, 0x88, 0x48, 0x28, 0x18, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00
};


const static uint8_t Image_Arrow_8ptBitmaps[] = {
	248,112, 32,  0,  0,  0
};

#endif