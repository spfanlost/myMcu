
#include "common.h"
#include "math.h"
#include "drv_lcd.h"
#include "mcu.h"
#include "mcu_sys.h"
#include "mcu_isr.h"
#include "touch.h"
#include "app_param.h"

struct touch_pen_param pen_dat;
struct touch_dat_param touch_dat;

//X,Y方向与屏幕相同
uint8_t CMD_CHX=0XD0;
uint8_t CMD_CHY=0X90;
//X,Y方向与屏幕相反
// uint8_t CMD_CHX=0X90;
// uint8_t CMD_CHY=0XD0;
const char* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
const char* TP_REMIND_MSG_TBL1="Touch screen calibration failure,Please re calibrate the touch screen.";

signed short Coordinate_xy[4][2];//校准时，坐标缓存值

Coordinate DisplaySample[4] =
{
    { 20,  20 },
    { 220, 20 },
    { 20,  300},
    { 220, 300}
};

//默认为touchtype=0的数据.
uint8_t CMD_RDX=0XD0;
uint8_t CMD_RDY=0X90;

#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 312
#define STR_WIDTH       8       /* 字符宽度 */
#define STR_HEIGHT      16      /* 字符高度 */

static void touch_save_param(void)
{
    memcpy (&g_param.touch_dat, &touch_dat, sizeof(struct touch_dat_param));
    g_param.touch_adj_done = TRUE;
    app_save_param();
}
static dword_t touch_load_param(void)
{
    app_load_param();
    memcpy (&touch_dat, &g_param.touch_dat, sizeof(struct touch_dat_param));
    return g_param.touch_adj_done;
}

void touch_show_string(uint16_t x,uint16_t y,const char *pstr,uint16_t Color)
{
    while(*pstr!='\0')
    {
        if(x>MAX_CHAR_POSX)
        {
            x=0;
            y+=STR_HEIGHT;
        }
        if(y>MAX_CHAR_POSY)
        {
            y=x=0;
            LCD_Clear(WHITE);
        }
        //LCD_ShowChar(x,y,*pstr,16,0,Color);
        LCD_ShowChar(x,y,*pstr,16,0,Color);
        x+=STR_WIDTH;
        pstr++;
    }
}

//SPI写数据
//向触摸屏IC写入1byte数据
//num:要写入的数据
void touch_write_byte(uint8_t dat)
{
    uint8_t count=0;
    for(count=0;count<8;count++)
    {
        if(dat&0x80)
            TDIN_High;
        else
            TDIN_Low;
        dat<<=1;
        TCLK_Low;
        delay_us(1);
        TCLK_High;
    }
}
//SPI读数据
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据
uint16_t touch_read_ad(uint8_t cmd)
{
    uint8_t count=0;
    uint16_t Num=0;
    TCLK_Low;       //先拉低时钟
    TDIN_Low;       //拉低数据线
    TCS_Low;        //选中触摸屏IC
    touch_write_byte(cmd);//发送命令字
    delay_us(6);    //ADS7846的转换时间最长为6us
    TCLK_Low;
    delay_us(1);
    TCLK_High;      //给1个时钟，清除BUSY
    delay_us(1);
    TCLK_Low;
    for(count=0;count<16;count++)//读出16位数据,只有高12位有效
    {
        Num<<=1;
        TCLK_Low;   //下降沿有效
        delay_us(1);
        TCLK_High;
        if(DOUT)
            Num++;
    }
    Num>>=4;        //只有高12位有效.
    TCS_High;       //释放片选
    return(Num);
}
//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 5    //读取次数
#define LOST_VAL 1      //丢弃值
uint16_t touch_dead_xory(uint8_t xy)
{
    uint16_t i, j;
    uint16_t buf[READ_TIMES];
    uint16_t sum=0;
    uint16_t temp;
    for(i=0;i<READ_TIMES;i++)
        buf[i]=touch_read_ad(xy);
    for(i=0;i<READ_TIMES-1; i++)//排序
    {
        for(j=i+1;j<READ_TIMES;j++)
        {
            if(buf[i]>buf[j])//升序排列
            {
                temp=buf[i];
                buf[i]=buf[j];
                buf[j]=temp;
            }
        }
    }
    sum=0;
    for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
        sum+=buf[i];
    temp=sum/(READ_TIMES-2*LOST_VAL);
    return temp;
}
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
uint8_t touch_read_xy(uint16_t *x,uint16_t *y)
{
    uint16_t xtemp,ytemp;
    xtemp=touch_dead_xory(CMD_RDX);
    ytemp=touch_dead_xory(CMD_RDY);
    //if(xtemp<100||ytemp<100)return 0;//读数失败
    *x=xtemp;
    *y=ytemp;
    return 1;//读数成功
}
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 50 //误差范围
uint8_t touch_read_xy2(uint16_t *x,uint16_t *y)
{
    uint16_t x1,y1;
    uint16_t x2,y2;
    uint8_t flag;
    flag=touch_read_xy(&x1,&y1);
    if(flag==0)
        return(0);
    flag=touch_read_xy(&x2,&y2);
    if(flag==0)
        return(0);
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
        &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else
        return 0;
}

//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t touch_scan(uint8_t tp)
{
    if(PEN==0)//有按键按下
    {
        if(tp)
        {
            touch_read_xy2(&pen_dat.X,&pen_dat.Y);//读取物理坐标
        }
        else if(touch_read_xy2(&pen_dat.X,&pen_dat.Y))//读取屏幕坐标
        {
            pen_dat.X0=touch_dat.xfac*(pen_dat.X)+touch_dat.xoff;
            pen_dat.Y0=touch_dat.yfac*(pen_dat.Y)+touch_dat.yoff;
        }
        if((pen_dat.Key_Sta&Key_Down)==0)//之前没有被按下
        {
            pen_dat.Key_Sta=Key_Down|TP_Key_Down;
        }
    }
    else
    {
        if(pen_dat.Key_Sta&Key_Down)//之前是被按下的
        {
            pen_dat.Key_Sta &= ~(Key_Down);//标记按键松开
        }
    }
    return pen_dat.Key_Sta;//返回当前的触屏状态
}

//////////////////////////////////////////////////
//与LCD部分有关的函数
//画一个触摸点
//用来校准用的
void touch_draw_Coordinate(uint8_t x,uint16_t y,uint16_t Color)
{
    LCD_DrawLine(x-12,y,x+13,y,Color);//横线
    LCD_DrawLine(x,y-12,x,y+13,Color);//竖线
    LCD_DrawPoint(x+1,y+1,Color);
    LCD_DrawPoint(x-1,y+1,Color);
    LCD_DrawPoint(x+1,y-1,Color);
    LCD_DrawPoint(x-1,y-1,Color);
    LCD_Draw_Circle(x,y,6,Color);//画中心圈
}
//画一个大点
//2*2的点
void touch_draw_point(uint8_t x,uint16_t y,uint16_t Color)
{
    LCD_DrawPoint(x,y,Color);//中心点
    LCD_DrawPoint(x+1,y,Color);
    LCD_DrawPoint(x,y+1,Color);
    LCD_DrawPoint(x+1,y+1,Color);
}

void touch_load_draw_ui(void)
{
    LCD_Clear(WHITE);//清屏
    LCD_Fill(0,0,30,20,BLUE);
    LCD_Fill(30,0,60,20,RED);
    LCD_Fill(60,0,90,20,YELLOW);
    LCD_Fill(90,0,120,20,BLACK);
    LCD_Fill(120,0,150,20,GREEN);
    LCD_Fill(150,0,180,20,GRAY);
    LCD_Fill(180,0,210,20,BROWN);
    POINT_COLOR=BLUE;//设置字体为蓝色
    touch_show_string(216,0,"CLR",POINT_COLOR);//显示清屏区域
  //POINT_COLOR=RED;//设置画笔蓝色
}

//计算两坐标点间距离
uint16_t Calculation_distance(signed short *point1,signed short *point2)
{
    u32 tem1,tem2;
    tem1=abs(point1[0]-point2[0]);//x1-x2
    tem2=abs(point1[1]-point2[1]);//y1-y2
    return sqrt(tem1*tem1+tem2*tem2);//得到两点距离的距离
}

///触摸屏校准，计数水平，垂直，对角线误差
#define Calibration_Error 3
uint8_t touch_Calibration_Calculation(void)
{
    uint16_t d1,d2;
    float fac;
    //水平方向距离
    d1=Calculation_distance(Coordinate_xy[0],Coordinate_xy[1]);
    d2=Calculation_distance(Coordinate_xy[2],Coordinate_xy[3]);
    if((d1+d2)!=0)
    {
        fac=abs(d1-d2)*100/(d1+d2);
    }
    if((fac>Calibration_Error)||d1==0||d2==0)
        return 0;
    ///垂直线距离
    d1=Calculation_distance(Coordinate_xy[0],Coordinate_xy[2]);
    d2=Calculation_distance(Coordinate_xy[1],Coordinate_xy[3]);
    if((d1+d2)!=0)
    {
        fac=abs(d1-d2)*100/(d1+d2);
    }
    if((fac>Calibration_Error)||d1==0||d2==0)
        return 0;
    //对角线相等
    d1=Calculation_distance(Coordinate_xy[1],Coordinate_xy[2]);
    d2=Calculation_distance(Coordinate_xy[0],Coordinate_xy[3]);
    if((d1+d2)!=0)
    {
        fac=abs(d1-d2)*100/(d1+d2);
    }
    if((fac>Calibration_Error)||d1==0||d2==0)
        return 0;
    return 1;
}


//计算校准参数

void touch_Calibration(void)
{
    //水平方向LCD两点的实际距离DisplaySample[1].x-DisplaySample[0].x，触摸屏上的距离Coordinate_xy[1][0]-Coordinate_xy[0][0]

    touch_dat.xfac=(float)(DisplaySample[1].x-DisplaySample[0].x)/(Coordinate_xy[1][0]-Coordinate_xy[0][0]);
    touch_dat.xoff=((DisplaySample[1].x+DisplaySample[0].x)-touch_dat.xfac*(Coordinate_xy[1][0]+Coordinate_xy[0][0]))/2;//得到xoff

    touch_dat.yfac=(float)(DisplaySample[2].y-DisplaySample[0].y)/(Coordinate_xy[2][1]-Coordinate_xy[0][1]);//得到yfac
    touch_dat.yoff=((DisplaySample[2].y+DisplaySample[0].y)-touch_dat.yfac*(Coordinate_xy[2][1]+Coordinate_xy[0][1]))/2;//得到yoff
}

//触摸屏校准代码
//得到四个校准参数
void touch_adjust(void)
{
    uint8_t  cnt=0;
    uint8_t flag;
    cnt=0;
    BACK_COLOR =WHITE;
    LCD_Clear(WHITE);//清屏
    POINT_COLOR=RED;//红色
    flag=0;

    touch_dat.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误

    loop:
    for( cnt=0;cnt<4;cnt++)
    {
        LCD_Clear(WHITE);//清屏
        if(flag==0)
        {
            touch_show_string(35,80,TP_REMIND_MSG_TBL,POINT_COLOR);
        }
        else
        {
            touch_show_string(35,80,TP_REMIND_MSG_TBL1,POINT_COLOR);
        }
        touch_draw_Coordinate(DisplaySample[cnt].x,DisplaySample[cnt].y,POINT_COLOR);
        while(1)
        {
            touch_scan(1);
            if((pen_dat.Key_Sta&0xc0)==TP_Key_Down)//按键按下了一次(此时按键松开了.)
            {                           //触摸屏被按下
                Coordinate_xy[cnt][0]=pen_dat.X;
                Coordinate_xy[cnt][1]=pen_dat.Y;
                pen_dat.Key_Sta &= ~(TP_Key_Down);//标记按键已经被处理过了.
                break;
            }
        }
    }
    if(!touch_Calibration_Calculation())
    {
        cnt=0;
        flag=1;
        goto loop;
    }
    touch_Calibration();

    POINT_COLOR=BLUE;
    touch_show_string(35,110,"Touch Screen Adjust OK!",POINT_COLOR);
    delay_ms(500);
    LCD_Clear(WHITE);
}

void touch_init(void)
{
    mcu_io_clk_enable(GPIOB_CLK|GPIOC_CLK|GPIOF_CLK);
    mcu_io_config(GPIOB, PIN1|PIN2, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_RES);
    mcu_io_config(GPIOB, PIN0, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_RES);
    mcu_io_config(GPIOC, PIN13, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_RES);
    mcu_io_config(GPIOF, PIN11, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_RES);
    if(FALSE == touch_load_param())
    {
        touch_adjust();
        touch_save_param();
    }
}

