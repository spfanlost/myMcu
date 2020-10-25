/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途

**********************************************************************************/

#include "common.h"
#include "math.h"
#include "drv_lcd.h"
#include "mcu.h"
#include "mcu_sys.h"
#include "mcu_isr.h"
#include "touch.h"


Pen_Parameters Pen_data;//定义笔实体
Touch_Adjust_Parameters Touch_date;//触摸屏参数校准

//X,Y方向与屏幕相同
u8 CMD_CHX=0XD0;
u8 CMD_CHY=0X90;
//X,Y方向与屏幕相反
// u8 CMD_CHX=0X90;
// u8 CMD_CHY=0XD0;
char* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
char* TP_REMIND_MSG_TBL1="Touch screen calibration failure,Please re calibrate the touch screen.";

signed short Coordinate_xy[4][2];//校准时，坐标缓存值


Coordinate DisplaySample[4] =
{
    { 20,  20 },
    { 220, 20},
    { 20, 300},
    { 220, 300}
};



//默认为touchtype=0的数据.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 312
#define STR_WIDTH		8		/* 字符宽度 */
#define STR_HEIGHT		16		/* 字符高度 */

void TP_ShowString(u16 x,u16 y,const u8 *pstr,u16 Color)
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
void TP_Write_Byte(u8 num)
{
	u8 count=0;
	for(count=0;count<8;count++)
	{
		if(num&0x80)TDIN_High;
		else TDIN_Low;
		num<<=1;
		TCLK_Low;
		delay_us(1);
		TCLK_High;		//上升沿有效
	}
}
//SPI读数据
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据
u16 TP_Read_AD(u8 CMD)
{
	u8 count=0;
	u16 Num=0;
	TCLK_Low;		//先拉低时钟
	TDIN_Low; 	//拉低数据线
	TCS_Low;		//选中触摸屏IC
	TP_Write_Byte(CMD);//发送命令字
	delay_us(6);//ADS7846的转换时间最长为6us
	TCLK_Low;
	delay_us(1);
	TCLK_High;	//给1个时钟，清除BUSY
	delay_us(1);
	TCLK_Low;
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效
	{
		Num<<=1;
		TCLK_Low;	//下降沿有效
	delay_us(1);
 		TCLK_High;
 		if(DOUT)Num++;
	}
	Num>>=4;   	//只有高12位有效.
	TCS_High;		//释放片选
	return(Num);
}
//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 5 	//读取次数
#define LOST_VAL 1	  	//丢弃值
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);
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
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;
}
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);
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
u8 TP_Read_XY2(u16 *x,u16 *y)
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;
    flag=TP_Read_XY(&x1,&y1);
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);
    if(flag==0)return(0);
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
u8 Read_TP(u8 tp)
{
    if(PEN==0)//有按键按下
    {
        LOG_INFO("PEN==0\r\n");
        if(tp)
        {
            TP_Read_XY2(&Pen_data.X,&Pen_data.Y);//读取物理坐标
        }
        else if(TP_Read_XY2(&Pen_data.X,&Pen_data.Y))//读取屏幕坐标
        {
            Pen_data.X0=Touch_date.xfac*(Pen_data.X)+Touch_date.xoff;
            Pen_data.Y0=Touch_date.yfac*(Pen_data.Y)+Touch_date.yoff;
        }
        if((Pen_data.Key_Sta&Key_Down)==0)//之前没有被按下
        {
            Pen_data.Key_Sta=Key_Down|TP_Key_Down;
        }
    }
    else
    {
        if(Pen_data.Key_Sta&Key_Down)//之前是被按下的
        {
            Pen_data.Key_Sta = ~((uint32_t)1<<7);//标记按键松开
        }
    }
    return Pen_data.Key_Sta;//返回当前的触屏状态
}

//////////////////////////////////////////////////
//与LCD部分有关的函数
//画一个触摸点
//用来校准用的
void Draw_Touch_Coordinate(u8 x,u16 y,u16 Color)
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
void Draw_Big_Circle(u8 x,u16 y,u16 Color)
{
	LCD_DrawPoint(x,y,Color);//中心点
	LCD_DrawPoint(x+1,y,Color);
	LCD_DrawPoint(x,y+1,Color);
	LCD_DrawPoint(x+1,y+1,Color);
}

void Load_TFT_surface(void)
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
	TP_ShowString(216,0,"CLR",POINT_COLOR);//显示清屏区域
  //POINT_COLOR=RED;//设置画笔蓝色
}

//计算两坐标点间距离
u16 Calculation_distance(signed short *point1,signed short *point2)
{
  u32 tem1,tem2;
	tem1=abs(point1[0]-point2[0]);//x1-x2
	tem2=abs(point1[1]-point2[1]);//y1-y2
  return sqrt(tem1*tem1+tem2*tem2);//得到两点距离的距离
}

///触摸屏校准，计数水平，垂直，对角线误差
#define Calibration_Error 3
u8 touch_Calibration_Calculation(void)
{
    u16 d1,d2;
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

    Touch_date.xfac=(float)(DisplaySample[1].x-DisplaySample[0].x)/(Coordinate_xy[1][0]-Coordinate_xy[0][0]);
    Touch_date.xoff=((DisplaySample[1].x+DisplaySample[0].x)-Touch_date.xfac*(Coordinate_xy[1][0]+Coordinate_xy[0][0]))/2;//得到xoff

    Touch_date.yfac=(float)(DisplaySample[2].y-DisplaySample[0].y)/(Coordinate_xy[2][1]-Coordinate_xy[0][1]);//得到yfac
    Touch_date.yoff=((DisplaySample[2].y+DisplaySample[0].y)-Touch_date.yfac*(Coordinate_xy[2][1]+Coordinate_xy[0][1]))/2;//得到yoff
}

//触摸屏校准代码
//得到四个校准参数
void Touch_Adjust(void)
{
    u8  cnt=0;
    u8 flag;
	cnt=0;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//清屏
	POINT_COLOR=RED;//红色
	flag=0;

	Touch_date.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误

	loop:
	for( cnt=0;cnt<4;cnt++)
    {
        LCD_Clear(WHITE);//清屏
        if(flag==0)
        {
            TP_ShowString(35,80,TP_REMIND_MSG_TBL,POINT_COLOR);
        }
        else
        {
            TP_ShowString(35,80,TP_REMIND_MSG_TBL1,POINT_COLOR);
        }
        Draw_Touch_Coordinate(DisplaySample[cnt].x,DisplaySample[cnt].y,POINT_COLOR);
        while(1)
        {
            Read_TP(1);
            if((Pen_data.Key_Sta&0xc0)==TP_Key_Down)//按键按下了一次(此时按键松开了.)
            {							//触摸屏被按下
                Coordinate_xy[cnt][0]=Pen_data.X;
                Coordinate_xy[cnt][1]=Pen_data.Y;
                Pen_data.Key_Sta&=~(1<<6);//标记按键已经被处理过了.
                break;
            }
            // }
        }
    }
    if(!touch_Calibration_Calculation())
    {
        cnt=0;
        flag=1;
        goto loop;
    }
    //计算结果
    touch_Calibration();

    POINT_COLOR=BLUE;
    LCD_Clear(WHITE);//清屏

    TP_ShowString(35,110,"Touch Screen Adjust OK!",POINT_COLOR);//校正完成
    delay_ms(400);
    LCD_Clear(WHITE);//清屏
}

//触摸屏初始化
//返回值:0,没有进行校准
//       1,进行过校准
void TP_Init(void)
{
    mcu_io_clk_enable(GPIOB_CLK|GPIOC_CLK|GPIOF_CLK);
    mcu_io_config(GPIOB, PIN1|PIN2, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
    mcu_io_config(GPIOB, PIN0, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
    mcu_io_config(GPIOC, PIN13, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
    mcu_io_config(GPIOF, PIN11, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
    Touch_Adjust();  	//屏幕校准
}
