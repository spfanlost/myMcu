#include "mcu.h"
#include "stdlib.h"
#include "ltdc.h"

uint16_t g_Lcd_Width = 480;  /* 显示屏分辨率-宽度 */
uint16_t g_Lcd_Height = 272; /* 显示屏分辨率-高度 */
uint16_t Width = 480;     //480 * 272
uint16_t Height = 272;

uint8_t g_LcdDirection = 0; /* 显示方向.0，1，2，3 */

uint32_t s_CurrentFrameBuffer;
uint8_t s_CurrentLayer;

static const uint32_t layer_address[GUI_NUM_LAYERS] = {LCD429_FRAME_BUFFER, LCD429_FRAME_BUFFER + BUFFER_OFFSET};
static int32_t        layer_pending_buffer[GUI_NUM_LAYERS];
static uint32_t       layer_buffer_index[GUI_NUM_LAYERS];
static uint32_t       layer_xsize[GUI_NUM_LAYERS];
static uint32_t       layer_ysize[GUI_NUM_LAYERS];
static uint32_t       layer_bpp[GUI_NUM_LAYERS];

#define TFT_BL_Pin LL_GPIO_PIN_5
#define TFT_BL_GPIO_Port GPIOC

void LCD_DisplayOn(void)
{
    /* Enable LCD Backlight */ 
    LL_GPIO_SetOutputPin(TFT_BL_GPIO_Port, TFT_BL_Pin);
    /* Display On */
    LTDC_Cmd(ENABLE); /* display ON */
}

void LCD_DisplayOff(void)
{
    /* Enable LCD Backlight */ 
    // GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    LL_GPIO_ResetOutputPin(TFT_BL_GPIO_Port, TFT_BL_Pin);
    /* Display Off */
    LTDC_Cmd(DISABLE); 
}

static void MX_LTDC_Init(void)
{
#if (LCD_PIXFORMAT == LCD_PIXFORMAT_RGB565)
    /*
    ****************************************************************************
    *R3 -->  PH9 | G2 --> PH13 | B3 --> PG11 | VSYNC --> PI10
    *R4 --> PH10 | G3 --> PH14 | B4 --> PG12 | HSYNC -->  PI9
    *R5 --> PH11 | G4 --> PH15 | B5 -->  PI5 | DE    --> PF10
    *R6 -->  PB1 | G5 -->  PI0 | B6 -->  PI6 | CLK   -->  PG7
    *R7 -->  PG6 | G6 -->  PI1 | B7 -->  PI7 | BL_EN --> PC5 
    *            | G7 -->  PI2 |
    ****************************************************************************
    */
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* 使能GPIO时钟 */
    // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB  | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG 
    //                        | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);

      
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_14;

    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_LTDC);   
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);  
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    // GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
    LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    // GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_LTDC);
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_11 | GPIO_Pin_12;
    // GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_6|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
    LL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    
    // GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_LTDC);   
    // GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_LTDC);
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13
    //                               | GPIO_Pin_14 | GPIO_Pin_15;
    // GPIO_Init(GPIOH, &GPIO_InitStructure);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_13|
                            LL_GPIO_PIN_14|LL_GPIO_PIN_15;
    LL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    // GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_LTDC);   
    // GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_LTDC);
    // GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_LTDC);
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5
    //                               | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
    // GPIO_Init(GPIOI, &GPIO_InitStructure);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_5|
                            LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_10;
    LL_GPIO_Init(GPIOI, &GPIO_InitStruct);

#else
    
    /*
    * RGB888 GPIO_Init
    ****************************************************************************
    *R0 -->  PH2 | G0 -->  PE5  | B0 -->  PE4  | VSYNC -->  PI9
    *R1 -->  PH3 | G1 -->  PE6  | B1 -->  PG12 | HSYNC -->  PI10
    *R2 -->  PH8 | G2 -->  PH13 | B2 -->  PG10 | DE    -->  PF10
    *R3 -->  PH9 | G3 -->  PH14 | B3 -->  PG11 | CLK   -->  PG7
    *R4 -->  PH10| G4 -->  PH15 | B4 -->  PI4  | BL_EN -->  PC5
    *R5 -->  PH11| G5 -->  PI0  | B5 -->  PI5  |
    *R6 -->  PH12| G6 -->  PI1  | B6 -->  PI6  |
    *R7 -->  PG6 | G7 -->  PI2  | B7 -->  PI7  |
    ****************************************************************************
    */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
    LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_0|LL_GPIO_PIN_1
                          |LL_GPIO_PIN_2|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6
                          |LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
    LL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
    LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_8|LL_GPIO_PIN_9
                          |LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_13
                          |LL_GPIO_PIN_14|LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
    LL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_10|LL_GPIO_PIN_11
                          |LL_GPIO_PIN_12;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
    LL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    
    #endif

    // /* BL_EN pin configuration */
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    // GPIO_Init(GPIOC, &GPIO_InitStructure);
    // LL_GPIO_ResetOutputPin(TFT_BL_GPIO_Port, TFT_BL_Pin);
    GPIO_InitStruct.Pin = TFT_BL_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(TFT_BL_GPIO_Port, &GPIO_InitStruct);
    
    // LL_GPIO_SetOutputPin(TFT_BL_GPIO_Port, TFT_BL_Pin);

}
void LTDC_LayerInit(LTDC_Layer_TypeDef *LTDC_Layerx)
{
    // LTDC_Layer_Init.LTDC_HorizontalStart = HBP + 1;
    // LTDC_Layer_Init.LTDC_HorizontalStop = (xSize + HBP);
    // LTDC_Layer_Init.LTDC_VerticalStart = VBP + 1;
    // LTDC_Layer_Init.LTDC_VerticalStop = (ySize + VBP);
    // LTDC_Layer_Init.LTDC_PixelFormat = LTDC_Pixelformat_ARGB8888;
    /* Configures the horizontal start and stop position */
    LTDC_Layerx->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
    LTDC_Layerx->WHPCR = ((HBP + 1) | (Width + HBP) << 16);

    /* Configures the vertical start and stop position */
    LTDC_Layerx->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
    LTDC_Layerx->WVPCR = ((VBP + 1) | (Height + VBP) << 16);

    /* Specifies the pixel format */
    LTDC_Layerx->PFCR &= ~(LTDC_LxPFCR_PF);
    LTDC_Layerx->PFCR = LCD_PIXFORMAT;

    // LTDC_Layer_Init.LTDC_ConstantAlpha = 255; 
    // LTDC_Layer_Init.LTDC_DefaultColorBlue = 0;        
    // LTDC_Layer_Init.LTDC_DefaultColorGreen = 0;       
    // LTDC_Layer_Init.LTDC_DefaultColorRed = 0;         
    // LTDC_Layer_Init.LTDC_DefaultColorAlpha = 0;
    // LTDC_Layer_Init.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
    // LTDC_Layer_Init.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
    /* Configures the default color values */
    LTDC_Layerx->DCCR &= ~(LTDC_LxDCCR_DCBLUE | LTDC_LxDCCR_DCGREEN | LTDC_LxDCCR_DCRED | LTDC_LxDCCR_DCALPHA);
    //                 B            G          R           A
    LTDC_Layerx->DCCR = ((0 << 0) | (0 << 8) | (0 << 16) | (0 << 24));
    
    /* Specifies the constant alpha value */
    LTDC_Layerx->CACR &= ~(LTDC_LxCACR_CONSTA);
    LTDC_Layerx->CACR = 255;

    // /* Configure blending factors */       
    // LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
    // LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
    /* Specifies the blending factors */
    LTDC_Layerx->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);
    LTDC_Layerx->BFCR = (LTDC_BlendingFactor1_CA | LTDC_BlendingFactor2_CA);
    
    // LTDC_Layer_Init.LTDC_CFBLineLength = BytesPerLine + 3;
    // LTDC_Layer_Init.LTDC_CFBPitch = BytesPerLine;
    /* Configures the color frame buffer pitch in byte */
    LTDC_Layerx->CFBLR &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);
    // LTDC_Layerx->CFBLR = (((Width * Get_PixelFormat_byte(LCD_PIXFORMAT)) + 3) | ( Height << 16));
    LTDC_Layerx->CFBLR = (((Width * Get_PixelFormat_byte(LCD_PIXFORMAT)) + 3) | 
        ((Width * Get_PixelFormat_byte(LCD_PIXFORMAT)) << 16));

    // LTDC_Layer_Init.LTDC_CFBLineNumber = ySize; 
    /* Configures the frame buffer line number */
    LTDC_Layerx->CFBLNR &= ~(LTDC_LxCFBLNR_CFBLNBR);
    LTDC_Layerx->CFBLNR = Width;

    // LTDC_Layer_Init.LTDC_CFBStartAdress = layer_address[LayerIndex];
    // /* Configures the color frame buffer start address */
    // LTDC_Layerx->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
    // LTDC_Layerx->CFBAR = (LTDC_CFBStartAdress);
}

/**********************************************************************************************************
*       LCD_TFT 同步时序配置（整理自官方做的一个截图，言简意赅）：
*       ----------------------------------------------------------------------------
*    
*                                                 Total Width
*                             <--------------------------------------------------->
*                       Hsync width HBP             Active Width                HFP
*                             <---><--><--------------------------------------><-->
*                         ____    ____|_______________________________________|____ 
*                             |___|   |                                       |    |
*                                     |                                       |    |
*                         __|         |                                       |    |
*            /|\    /|\  |            |                                       |    |
*             | VSYNC|   |            |                                       |    |
*             |Width\|/  |__          |                                       |    |
*             |     /|\     |         |                                       |    |
*             |  VBP |      |         |                                       |    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Heigh    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Heigh |      |         |/ / / / / / Active Display Area / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |                                                      |
*             |  VFP |      |                                                      |
*            \|/    \|/_____|______________________________________________________|
*            
*     
*     每个LCD设备都有自己的同步时序值：
*     Horizontal Synchronization (Hsync) 
*     Horizontal Back Porch (HBP)       
*     Active Width                      
*     Horizontal Front Porch (HFP)     
*   
*     Vertical Synchronization (Vsync)  
*     Vertical Back Porch (VBP)         
*     Active Heigh                       
*     Vertical Front Porch (VFP)         
*     
*     LCD_TFT 窗口水平和垂直的起始以及结束位置 :
*     ----------------------------------------------------------------
*   
*     HorizontalStart = (Offset_X + Hsync + HBP);
*     HorizontalStop  = (Offset_X + Hsync + HBP + Window_Width - 1); 
*     VarticalStart   = (Offset_Y + Vsync + VBP);
*     VerticalStop    = (Offset_Y + Vsync + VBP + Window_Heigh - 1);
*
*********************************************************************************************************/

void LCD429_ConfigLTDC(void)
{
    uint32_t PLLSAIN = 192;
    uint32_t PLLSAIQ = 7;
    uint32_t PLLSAIR = 3;

    MX_LTDC_Init();
    
    LCD_DisplayOff();
    
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_LTDC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);
    // /* Configure PLLSAI prescalers for LCD */
    // RCC_PLLSAIConfig(192, 7, 3);
    // RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
    /* Configure PLLSAI prescalers for LCD */ //PLLSAIN=192, PLLSAIQ=7, PLLSAIR=3
    RCC->PLLSAICFGR = (PLLSAIN << 6) | (PLLSAIQ << 24) | (PLLSAIR << 28);
    RCC->DCKCFGR &= ~RCC_DCKCFGR_PLLSAIDIVR; //Clear PLLSAIDIVR[2:0] bits
    RCC->DCKCFGR |= (1<<16);                 //00:=/2 01:=/4 10:=/8 11:=/16
    // LL_RCC_PLLSAI_ConfigDomain_LTDC();
    LL_RCC_PLLSAI_Enable();
    while (!LL_RCC_PLLSAI_IsReady());
    
    // /* Polarity configuration */
    // /* Initialize the horizontal synchronization polarity as active low */
    // LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
    // /* Initialize the vertical synchronization polarity as active low */  
    // LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
    // /* Initialize the data enable polarity as active low */
    // LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
    // /* Initialize the pixel clock polarity as input pixel clock */ 
    // LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
    LTDC->GCR &= ~(0xf0000000); // LTDC GCR Mask
    //           HSPOL   VSPOL  DEPOL   PCPOL
    LTDC->GCR |= (0 << 31) | (0 << 30) | (0 << 29) | (0 << 28);

    // LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
    // LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
    // LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;
    /* sets the background color value */
    LTDC->BCCR &= ~(0xffffff); //BCCR Mask
    //             BCRED  BCGREEN BCBLUE
    LTDC->BCCR |= (0 << 16) | (0 << 8) | (0 << 0);

    // LTDC_InitStruct.LTDC_HorizontalSync = HSW;
    // /* Configure vertical synchronization height */
    // LTDC_InitStruct.LTDC_VerticalSync = VSW;
    // /* Configure accumulated horizontal back porch */
    // LTDC_InitStruct.LTDC_AccumulatedHBP = HBP; 
    // /* Configure accumulated vertical back porch */
    // LTDC_InitStruct.LTDC_AccumulatedVBP = VBP;  
    // /* Configure accumulated active width */  
    // LTDC_InitStruct.LTDC_AccumulatedActiveW = xSize + HBP;
    // /* Configure accumulated active height */
    // LTDC_InitStruct.LTDC_AccumulatedActiveH = ySize + VBP;
    // /* Configure total width */
    // LTDC_InitStruct.LTDC_TotalWidth = xSize + HBP + HFP; 
    // /* Configure total height */
    // LTDC_InitStruct.LTDC_TotalHeigh = ySize + VBP + VFP;
    /* Sets Synchronization size */
    LTDC->SSCR &= ~((0xfff << 16) | (0x7ff << 0)); //SSCR HSW && VSH Mask
    //             HSW           VSW
    LTDC->SSCR |= (HSW << 16) | (VSW << 0);

    /* Sets Accumulated Back porch */
    LTDC->BPCR &= ~((0xfff << 16) | (0x7ff << 0)); //SSCR AHBP && AVBP Mask
    //             AHBP         AVBP
    LTDC->BPCR |= (HBP << 16) | (VBP << 0);

    /* Sets Accumulated Active Width */
    LTDC->AWCR &= ~((0xfff << 16) | (0x7ff << 0)); //SSCR AAW && AAH Mask
    //              AAW                     AAH
    LTDC->AWCR |= ((Width + HBP) << 16) | (Height + VBP);

    /* Sets Total Width */
    LTDC->TWCR &= ~((0xfff << 16) | (0x7ff << 0)); //SSCR TOTALW && TOTALH Mask
    //              TOTALW                       TOTALH
    LTDC->TWCR |= ((Width + HBP + HFP) << 16) | (Height + VBP + VFP);

    // LTDC_ITConfig(LTDC_IER_LIE, ENABLE);
    // NVIC_SetPriority(LTDC_IRQn, 0);
    // NVIC_EnableIRQ(LTDC_IRQn); 

    /* Configure Layer1 */
    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
    LTDC_LayerInit(LTDC_Layer1);
    LTDC_Layer1->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
    LTDC_Layer1->CFBAR = layer_address[0];
    /*  Enable layer */
    LTDC_LayerCmd(LTDC_Layer1, ENABLE);
    /* Reload configuration */
    LTDC_ReloadConfig(LTDC_VBReload);

    /* Configure Layer2 */
    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM w/ Offset */
    LTDC_LayerInit(LTDC_Layer2);
    LTDC_Layer2->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
    LTDC_Layer2->CFBAR = layer_address[1];
    /*  Enable layer */
    LTDC_LayerCmd(LTDC_Layer2, ENABLE);
    /* Reload configuration */
    LTDC_ReloadConfig(LTDC_VBReload);
    
    LCD_DisplayOn();
    
    g_Lcd_Width = Width;   /* 显示屏分辨率-宽度 */
    g_Lcd_Height = Height; /* 显示屏分辨率-高度 */
}

/*
*********************************************************************************************************
*	函 数 名: LCD429_FillRect
*	功能说明: 用一个颜色值填充一个矩形。使用STM32F429内部DMA2D硬件绘制。
*	形    参:
*			_usX,_usY: 矩形左上角的坐标
*			_usHeight : 矩形的高度
*			_usWidth  : 矩形的宽度
*			_usColor  : 颜色代码
*********************************************************************************************************
*/
void LCD429_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint32_t _usColor)
{
    uint32_t Xaddress = 0;
    uint16_t OutputOffset = 0;
    uint16_t NumberOfLine = 0;
    uint16_t PixelPerLine = 0;

    /* 根据显示方向设置不同的参数 */
    if (g_LcdDirection == 0) /* 横屏 */
    {
        Xaddress = s_CurrentFrameBuffer + sizeof(uint32_t) * (g_Lcd_Width * _usY + _usX);
        OutputOffset = g_Lcd_Width - _usWidth;
        NumberOfLine = _usHeight;
        PixelPerLine = _usWidth;
    }
    else if (g_LcdDirection == 1) /* 横屏180°*/
    {
        Xaddress = s_CurrentFrameBuffer + sizeof(uint32_t) * (g_Lcd_Width * (g_Lcd_Height - _usHeight - _usY) + g_Lcd_Width - _usX - _usWidth);
        OutputOffset = g_Lcd_Width - _usWidth;
        NumberOfLine = _usHeight;
        PixelPerLine = _usWidth;
    }
    else if (g_LcdDirection == 2) /* 竖屏 */
    {
        Xaddress = s_CurrentFrameBuffer + sizeof(uint32_t) * (g_Lcd_Height * (g_Lcd_Width - _usX - _usWidth) + _usY);
        OutputOffset = g_Lcd_Height - _usHeight;
        NumberOfLine = _usWidth;
        PixelPerLine = _usHeight;
    }
    else if (g_LcdDirection == 3) /* 竖屏180° */
    {
        Xaddress = s_CurrentFrameBuffer + sizeof(uint32_t) * (g_Lcd_Height * _usX + g_Lcd_Height - _usHeight - _usY);
        OutputOffset = g_Lcd_Height - _usHeight;
        NumberOfLine = _usWidth;
        PixelPerLine = _usHeight;
    }

    /* 软件复位 DMA2D */
    RCC->AHB1RSTR |= RCC_AHB1Periph_DMA2D;
    RCC->AHB1RSTR &= ~RCC_AHB1Periph_DMA2D;

    /* 配置DMA2D操作模式为 R2M (寄存器到存储器） */
    DMA2D->CR &= (uint32_t)0xFFFCE0FC;
    DMA2D->CR |= DMA2D_R2M;

    /* 配置输出的颜色模式为 RGB565 */
    DMA2D->OPFCCR &= ~(uint32_t)DMA2D_OPFCCR_CM;
    DMA2D->OPFCCR |= (DMA2D_ARGB8888);

    /* 设置颜色值 */
    DMA2D->OCOLR |= _usColor;

    /* Configures the output memory address */
    DMA2D->OMAR = Xaddress;

    /* Configure  the line Offset */
    DMA2D->OOR &= ~(uint32_t)DMA2D_OOR_LO;
    DMA2D->OOR |= OutputOffset;

    /* Configure the number of line and pixel per line */
    DMA2D->NLR &= ~(DMA2D_NLR_NL | DMA2D_NLR_PL);
    DMA2D->NLR |= (NumberOfLine | (PixelPerLine << 16));

    /* Start Transfer */
    DMA2D->CR |= (uint32_t)DMA2D_CR_START;

    /* Wait for CTC Flag activation */
    while ((DMA2D->ISR & DMA2D_FLAG_TC) == 0)
        ;
}

/*
*********************************************************************************************************
*	函 数 名: LCD429_SetLayer
*	功能说明: 切换层。只是更改程序变量，以便于后面的代码更改相关寄存器。硬件支持2层。
*********************************************************************************************************
*/
void LCD429_SetLayer(uint8_t _ucLayer)
{
    s_CurrentFrameBuffer = layer_address[_ucLayer];
    s_CurrentLayer = LCD_LAYER_1;
}

uint8_t Get_PixelFormat_byte(uint32_t PixelFormat)
{
    uint8_t temp;

    if (PixelFormat == LCD_PIXFORMAT_ARGB8888)
        temp = 4;
    else if (PixelFormat == LCD_PIXFORMAT_RGB888)
        temp = 3;
    else if ((PixelFormat == LCD_PIXFORMAT_ARGB4444) ||
             (PixelFormat == LCD_PIXFORMAT_RGB565) ||
             (PixelFormat == LCD_PIXFORMAT_ARGB1555) ||
             (PixelFormat == LCD_PIXFORMAT_AL88))
        temp = 2;
    else
        temp = 1;
    return temp;
}

void LTDC_LayerPixelFormat(LTDC_Layer_TypeDef *LTDC_Layerx, uint32_t PixelFormat)
{

    uint8_t temp;
    uint32_t tempreg;
    tempreg = LTDC_Layerx->PFCR;
    temp = Get_PixelFormat_byte(tempreg);

    tempreg = (LTDC_Layerx->CFBLR >> 16);
    tempreg = (tempreg / temp);

    temp = Get_PixelFormat_byte(PixelFormat);

    /* Reconfigures the color frame buffer pitch in byte */
    LTDC_Layerx->CFBLR = ((tempreg * temp) << 16) | ((tempreg * temp) + 3);

    /* Reconfigures the color frame buffer start address */
    LTDC_Layerx->PFCR = PixelFormat;
}

void LCD429_SetPixelFormat(uint32_t PixelFormat)
{
    if (s_CurrentLayer == LCD_LAYER_1)
    {
        LTDC_LayerPixelFormat(LTDC_Layer1, PixelFormat);
    }
    else
    {
        LTDC_LayerPixelFormat(LTDC_Layer2, PixelFormat);
    }
    /* Sets the Reload type */
    LTDC->SRCR = (uint32_t)LTDC_IMReload;
}

void LCD429_ClrScr(uint32_t _usColor)
{
#if 1
    LCD429_FillRect(0, 0, g_Lcd_Height, g_Lcd_Width, _usColor);
#else
    uint16_t *index;
    uint32_t i;

    index = (uint16_t *)s_CurrentFrameBuffer;

    for (i = 0; i < g_Lcd_Height * g_Lcd_Width; i++)
    {
        *index++ = _usColor;
    }
#endif
}

void LTDC_LayerAddress(LTDC_Layer_TypeDef* LTDC_Layerx, uint32_t Address)
{
  /* Reconfigures the color frame buffer start address */
  LTDC_Layerx->CFBAR = Address;
}
/**
  * @brief  reload layers registers with new parameters 
  * @param  LTDC_Reload: specifies the type of reload.
  *   This parameter can be one of the following values:
  *     @arg LTDC_IMReload: Vertical blanking reload.
  *     @arg LTDC_VBReload: Immediate reload.  
  * @retval None
  */

void LTDC_ReloadConfig(uint32_t LTDC_Reload)
{
  /* Sets the Reload type */
  LTDC->SRCR = (uint32_t)LTDC_Reload;
}

/**
  * @brief  Enables or disables the specified LTDC's interrupts.
  * @param  LTDC_IT: specifies the LTDC interrupts sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg LTDC_IT_LI: Line Interrupt Enable.
  *     @arg LTDC_IT_FU: FIFO Underrun Interrupt Enable.
  *     @arg LTDC_IT_TERR: Transfer Error Interrupt Enable.
  *     @arg LTDC_IT_RR: Register Reload interrupt enable.  
  #define LTDC_IER_LIE   
  #define LTDC_IER_FUIE  
  #define LTDC_IER_TERRIE
  #define LTDC_IER_RRIE  
  * @param NewState: new state of the specified LTDC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LTDC_ITConfig(uint32_t LTDC_IT, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    LTDC->IER |= LTDC_IT;
  }
  else
  {
    LTDC->IER &= (uint32_t)~LTDC_IT;
  }
}

/**
  * @brief  End-Of-Frame-Interrupt for managing multiple buffering
  * @param  None 
  * @retval None
  */
void LTDC_ISR_Handler(void) 
{
  uint32_t Addr;
  uint32_t i;

  LTDC->ICR = (uint32_t)LTDC_IER_LIE;
  for (i = 0; i < GUI_NUM_LAYERS; i++) 
  {
    if (layer_pending_buffer[i] >= 0) 
    {

      Addr =layer_address[i] + layer_xsize[i] * layer_ysize[i] * layer_pending_buffer[i] * layer_bpp[i];

      LTDC_LayerAddress((i == 0 ? LTDC_Layer1 : LTDC_Layer2), Addr);
      LTDC_ReloadConfig(LTDC_VBReload);

      // GUI_MULTIBUF_ConfirmEx(i, layer_pending_buffer[i]);

      layer_buffer_index[i] = layer_pending_buffer[i];
      layer_pending_buffer[i] = -1;
    }
  }
}

/**
  * @brief  Enables or disables CLUT.
  * @param  NewState: new state of CLUT.
  * @param  LTDC_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: LTDC_Layer1, LTDC_Layer2  
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

void LTDC_CLUTCmd(LTDC_Layer_TypeDef* LTDC_Layerx, FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    /* Enable CLUT by setting CLUTEN bit */
    LTDC_Layerx->CR |= (uint32_t)LTDC_LxCR_CLUTEN;
  }
  else
  {
    /* Disable CLUT by clearing CLUTEN bit */
    LTDC_Layerx->CR &= ~(uint32_t)LTDC_LxCR_CLUTEN;
  }
  
  /* Reload shadow register */
  LTDC->SRCR = LTDC_IMReload;
}

/**
  * @brief  Enables or disables the LTDC_Layer Controller.
  * @param  LTDC_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: LTDC_Layer1, LTDC_Layer2
  * @param  NewState: new state of the LTDC_Layer peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

void LTDC_LayerCmd(LTDC_Layer_TypeDef* LTDC_Layerx, FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    /* Enable LTDC_Layer by setting LEN bit */
    LTDC_Layerx->CR |= (uint32_t)LTDC_LxCR_LEN;
  }
  else
  {
    /* Disable LTDC_Layer by clearing LEN bit */
    LTDC_Layerx->CR &= ~(uint32_t)LTDC_LxCR_LEN;
  }
}

void LTDC_Cmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /* Enable LTDC by setting LTDCEN bit */
        LTDC->GCR |= (uint32_t)LTDC_GCR_LTDCEN;
    }
    else
    {
        /* Disable LTDC by clearing LTDCEN bit */
        LTDC->GCR &= ~(uint32_t)LTDC_GCR_LTDCEN;
    }
}
void LCD_Clear(u8 layer, u32 color)
{
    u32 index=0; 
    
    u32*addr = (u32*)layer_address[layer];
    u32 totalpoint=480;
    totalpoint*=272; 	
    for(index=0;index<totalpoint;index++)
    {
        addr[index]=color;	
    }
} 

void LCD429_InitHard(void)
{
    LCD429_ConfigLTDC();
    LCD_Clear(LCD_LAYER_1, ARGB_GREY);
    LCD429_SetPixelFormat(LCD_PIXFORMAT);

    LCD429_SetLayer(LCD_LAYER_1);
    LCD429_FillRect(50, 50, 100, 100, ARGB(100, 255, 0 , 0));
    LTDC_LayerCmd(LTDC_Layer1, ENABLE);
    LTDC_Cmd(ENABLE);

    LCD429_SetLayer(LCD_LAYER_2); 
    LCD429_FillRect(100, 100, 100, 100, ARGB_YELLOW);
    LTDC_LayerCmd(LTDC_Layer2, ENABLE); 
    
    LTDC_Cmd(ENABLE);
}


