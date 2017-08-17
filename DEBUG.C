/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: Debug.c
*Author: JH Yang
*Date:  2017/08/06
*Description: 部分代码检测程序
*Others: 如不使用 注释#define Debug 即可
*Function List:
*History:
	1.Date : 8/17
      Author : JH Yang
      Modification : 0x117模式下的PutPixel64K函数检测
	2.Date : 8/17
	  Author : JH Yang
	  Modification : VGAHI模式下图形绘制函数示例
**********************************************************/

//#define Debug_VGA
//#define Debug_SVGA

#ifdef Debug_SVGA
	#include "SVGA.H"
	#define Debug_SVGA
#endif

#ifdef Debug_VGA
	#include "VGA.H"
	#define Debug
#endif


#ifdef Debug

int main(){

	/**************************************   0x117模式下PutPixel64K函数检测
	int x,y,i,j;
	Set_SVGAMode(0x104);
	for (x = 10; x <= 100 ; x++)
		for (y = 10; y <= 100; y++)
		putpixel(x,y,0xffff);
	return 0;
	//*/

	/**************************************  VGAHI模式下图形绘制函数示例
	int x,y,i,j;
	int Cursor[16][10] = {
		{1,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0},
		{1,1,1,0,0,0,0,0,0,0},
		{1,1,1,1,0,0,0,0,0,0},
		{1,1,1,1,1,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0},
		{1,1,1,1,1,1,1,0,0,0},
		{1,1,1,1,1,1,1,1,0,0},
		{1,1,1,1,1,1,1,1,1,0},
		{1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,0,0,0,0,0},
		{1,1,1,0,1,0,0,0,0,0},
		{1,1,0,0,1,1,0,0,0,0},
		{1,0,0,0,0,1,1,0,0,0},
		{0,0,0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,1,0,0,0},
	};
	//Set_VGAmode();
	//Set_SVGAMode(0x102);
	cleardevice();
	setbkcolor(LIGHTGRAY);
	setviewport(20,100,570,450,1);
	x = 500; y = 200;

	for (i = 0; i < 16; i++)
		for (j = 0; j < 10; j++)
		{
			if (Cursor[i][j]!=0)
				putpixel(x+1,y+1,WHITE);
		}

	setfillstyle(1,WHITE);
	bar(100,100,400,120);
	setcolor(BLUE);
	circle(250,50,30);
	circle(250,10,8);
	circle(220,15,5);
	circle(280,15,5);
	setfillstyle(1,BLUE);
	floodfill(250,50,BLUE);
	floodfill(250,10,BLUE);
	floodfill(220,15,BLUE);
	floodfill(280,15,BLUE);
	settextstyle(1,0,3);
	setcolor(RED);
	outtextxy(150,30,"SEARCHING ENGINE");
	setfillstyle(1,LIGHTGRAY);
	bar(420,100,460,120);
	setcolor(WHITE);
	line(419,99,461,99);
	line(419,99,419,121);
	setcolor(DARKGRAY);
	line(461,121,461,99);
	line(461,121,419,121);
	setcolor(RED);
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	outtextxy(423,102,"search");
	outtextxy(125,130,"help");
	outtextxy(225,130,"more");
	outtextxy(325,130,"about");
	setcolor(DARKGRAY);
	line(102,101,102,119);
	getch();
	closegraph();
	getchar();
	return 0;
	//*/
}
#endif
