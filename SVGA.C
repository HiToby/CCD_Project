/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: SVGA.C
*Author: JH Yang
*Date:  2017/08/06
*Description: 图形界面初始化
*Others: 
*Function List:
	1. void Set_SVGAMode(unsigned int);
    2. unsigned int Get_SVGAMode(void);
    3. void Set_ViewPage(unsigned int);
    4. unsigned int Get_ViewPage(void);
   
   	5. long rgbcolor64K (unsigned char r,unsigned char g, unsigned char b);
    6. void PutPixel64K (int x, int y, unsigned long color);
    7. unsigned long GetPixel64K (int x, int y);

*History:
	1.Date:
      Author:
      Modification:

**********************************************************/
#include "SVGA.H"
#include "Head.h"


/*显存指针*/
int far * const VideoBuffer = (int far *)0xA0000000L;

/*VBE模式信息*/
_ModeInfo ModeInfo;

/*********************SVGA相关函数************************/
/**********************************************************
	在dos.h中REGS的定义如下：
	struct WORDREGS
	{
		unsigned int ax, bx, cx, dx, si, di, cflag, flags;
	};

	struct BYTEREGS
	{
		unsigned char al, ah, bl, bh, cl, ch, dl, dh;
	};

	union REGS
	{
		struct WORDREGS x;
		struct BYTEREGS h;
	};
这样al对应ax的低八位，ah对应ax的高八位。
调用时需要查表决定各入口参数取值,获取返回值表示的信息。
**********************************************************/

/**********************************************************
ax  说 明
   0x4f00  读SVGA卡信息
   0x4f01  读显示模式信息
   0x4f02  设置显示模式
   0x4f03  读当前显示模式
   0x4f04  存储或恢复SVGA的视频状态
   0x4f05  控制内存页区域切换
   0x4f06  设置或读取逻辑扫描线宽度
   0x4f07  设置或读取视频内存与屏幕的初始对应位置
   0x4f08  设置或读取DAC各原色有效位数
显示模式号bx：
		模式号		分辨率		颜色数		颜色位定义
		0x101		640*480		256				-
		0x103		800*600		256				-
		0x104		1024*768	16				-
		0x105		1024*768	256				-
		0x110		640*480		32K			1:5:5:5
		0x111		640*480		64K			5:6:5
		0x112		640*480		16.8M		8:8:8
		0x113		800*600		32K			1:5:5:5
		0x114		800*600		64K			5:6:5
		0x115		800*600		16.8M		8:8:8
		0x116		1024*768	32K			1:5:5:5
		0x117		1024*768	64K			5:6:5
		0x118		1024*768	16.8M		8:8:8
**********************************************************/


/*************************************************
*Function: void Set_SVGAMode(unsinged int mode);
*Description: 设置并初始化显示模式，返回VBE特定信息
*Calls: // 被本函数调用的函数清单
*Input: mode : SVGA模式号;
*Output: 错误信息
*Return: 无
*Others: 无
*************************************************/
void Set_SVGAMode(unsigned int mode){

	union REGS u_Regs;

	/*寄存器功能号设置*/
	u_Regs.x.ax = 0x4f02;
	u_Regs.x.bx = mode;
	int86(0x10, &u_Regs, &u_Regs);

	if (mode == 0x117) {
		ModeInfo.XRes = 1024;
		ModeInfo.YRes = 768;
	}
	if (mode == 0x114) {
		ModeInfo.XRes = 800;
		ModeInfo.YRes = 600;
	}

	/*ax != 0x004f意味着初始化失败，输出错误信息见上,下同*/
	if (u_Regs.x.ax != 0x004f)
	{
		printf("Error in setting SVGA mode!\nError code:0x%x\n", u_Regs.x.ax);
		delay(5000);
		exit(1);
	}
}


/*************************************************
*Function: unsigned int Get_SVGAMode();
*Description: 检测当前SVGA的模式号
*Calls: // 被本函数调用的函数清单
*Input:  无
*Output: 错误信息
*Return: 返回当前SVGA模式的模式号
*Others: 无
*************************************************/
unsigned int Get_SVGAMode(){

	union REGS u_Regs;

	/*寄存器功能号设置*/
	u_Regs.x.ax = 0x4f03;
	int86(0x10,&u_Regs,&u_Regs);

	//返回错误信息
	if (u_Regs.x.ax != 0x004f)
	{
		printf("Error in getting SVGA mode!\nError code:0x%x\n", u_Regs.x.ax);
		delay(5000);
		exit(1);
	}
	
	return u_Regs.x.bx;
}


/*************************************************
*Function: void Set_ViewPage(unsigned int)
*Description: 设置显示页面
*Calls: // 被本函数调用的函数清单
*Input: Page : 页面号
*Output: // 对输出参数的说明。
*Return: // 函数返回值的说明
*Others: // 其它说明
*************************************************/
void Set_ViewPage(unsigned int Page)
{
	union REGS u_Regs;

	/*上一次的页面号,用于减少切换次数,是使用次数很多的重要变量*/
	static unsigned char s_old_page = 0;
	
	/*标志数，用于判断是否是第一次换页*/
	static int s_flag = 0;

	/*寄存器功能号设置*/
	u_Regs.x.ax = 0x4f05;
	u_Regs.x.bx = 0;

	/*如果是第一次换页*/
	if (s_flag == 0) {
		u_Regs.x.dx = Page;
		s_old_page = Page;
		int86(0x10,&u_Regs,&u_Regs);
		s_flag++;
	}

	/*如果不是第一次换页，切所换页面与当前页面不同*/
	else if (s_old_page != Page) { 
		u_Regs.x.dx = Page;
		s_old_page = Page;
		int86(0x10,&u_Regs,&u_Regs);
	}
}


/*************************************************
*Function: unsigned int Get_ViewPage(void)
*Description: 获取当前页面号
*Calls: // 被本函数调用的函数清单
*Input: 无
*Output: // 对输出参数的说明。
*Return: 当前页面号
*Others: // 其它说明
*************************************************/
unsigned int Get_ViewPage(void)
{

	union REGS u_Regs;

	/*寄存器功能号设置*/
	u_Regs.x.ax = 0x4f05;
	u_Regs.x.bx = 1;

	int86(0x10,&u_Regs,&u_Regs);

	return u_Regs.x.dx;
}


/*********************画图相关函数************************/


/*************************************************
*Function: long rgbcolor64K(unsigned char r,unsigned char g, unsigned char b);
*Description: 得到64K高色彩下的颜色值
*Calls: // 被本函数调用的函数清单
*Input: r,g,b : 分别为颜色值红绿蓝的颜色值
*Output: // 对输出参数的说明。
*Return: 返回64K高色彩下的颜色值
*Others: // 其它说明
*************************************************/
long rgbcolor64K(unsigned char r,unsigned char g, unsigned char b)
{
	return (((((r >> 3) << 6) + (g >> 3)) << 5) + (b >> 3));
}


/*************************************************
*Function:  void PutPixel64K (int x, int y, unsigned long color)
*Description: 设置64K高色彩模式下画点函数
*Calls: // 被本函数调用的函数清单
*Input: x,y : 表示在x,y下画点
		color : 表示x,y所画点的颜色值
*Output: // 对输出参数的说明。
*Return: 无
*Others: // 其它说明
*************************************************/
void PutPixel64K (int x, int y, unsigned long color) 
{

	/*页面号*/
	unsigned char Page;

	/*显存位置偏移量*/
	long Pos;

	/*判断是否超出屏幕*/
	if (x >= ModeInfo.XRes || x < 0 || y >= ModeInfo.YRes || y < 0)
		return;

	/*计算偏移量的大小*/
	Pos = (((long)y << 10) + (long)x);

	Page = Pos >> 16; //代替除以64k算法

	Set_ViewPage(Page);

	VideoBuffer[Pos] = color;
}

/*************************************************
*Function: unsigned long GetPixel64K (int x, int y);
*Description: 获取64K高色彩模式下画点函数
*Calls: // 被本函数调用的函数清单
*Input: x,y : 表示在x,y下画点
*Output: // 对输出参数的说明。
*Return: color : 表示x,y所画点的颜色值
*Others: // 其它说明
*************************************************/
unsigned long GetPixel64K (int x, int y)
{
	/*页面号*/
	unsigned char Page;

	/*显存位置偏移量*/
	long Pos;

	/*判断是否超出屏幕*/
	if (x >= ModeInfo.XRes || x < 0 || y >= ModeInfo.YRes || y < 0)
		return;

	/*计算偏移量的大小*/
	Pos = (((long)y << 10) + (long)x);

	Page = Pos >> 16; //代替除以64k算法

	Set_ViewPage(Page);

	return VideoBuffer[Page] ;
}