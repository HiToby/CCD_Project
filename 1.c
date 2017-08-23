/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: Debug.c
*Author: JH Yang
*Date:  2017/08/06
*Description: ²¿·Ö´úÂë¼ì²â³ÌÐò
*Others: Èç²»Ê¹ÓÃ ×¢ÊÍ#define Debug ¼´¿É
*Function List:
*History:
	1.Date : 8/17
      Author : JH Yang
      Modification : 0x117Ä£Ê½ÏÂµÄPutPixel64Kº¯Êý¼ì²â
	2.Date : 8/17
	  Author : JH Yang
	  Modification : VGAHIÄ£Ê½ÏÂÍ¼ÐÎ»æÖÆº¯ÊýÊ¾Àý
**********************************************************/

//#define Debug_VGA
#define Debug_SVGA

#include "Mouse.h"

#ifdef Debug_SVGA
	#include "SVGA.H"
	#define Debug
#endif

#ifdef Debug_VGA
	#include "VGA.H"
	#define Debug
#endif


#ifdef Debug
extern _ModeInfo ModeInfo;

int main(){

	//*********************************************************   0x117Ä£Ê½ÏÂ PutPixel64K ÒÔ¼° GetPixel64K º¯Êý¼ì²â
#if 0	
	int x,y,i,j,k,r,g,b;

	Set_SVGAMode(0x117);

	//	PutPixel64K  &  rgbcolor64K
	for (x = 0; x < 100; x++)
		for (y = 0; y < 100; y++)
		{
			PutPixel64K (x,y,rgbcolor64K(0x94,0x00,0xd3));
		//	PutPixel256(x,y,10);
		// getch();
		}

	//	GetPixel64K
	for (x = 200; x < 300; x++)
		for (y = 500; y < 600; y++)
		{
			PutPixel64K (x,y,GetPixel64K(x - 200, y - 500));
		//	PutPixel256(x,y,10);
		// getch();
		}

	getch(); //»­³öÍ¼ÐÎºóÐèÍ£Áô¹Û²ìÍ¼ÐÎ
	return 0;
#endif

	//**************************************************************  VGAHIÄ£Ê½ÏÂÍ¼ÐÎ»æÖÆº¯ÊýÊ¾Àý
#if 0	
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
#endif


	//***************************************************************  Êó±ê¼ì²â
#if 0	
	_Mouse Old_Mouse, New_Mouse = {0,0,0,1};
	int i,j; // Ñ­»·±äÁ¿

	Set_SVGAMode(0x117);

	//»æÖÆ±³¾°
	for (i = 0; i < ModeInfo.XRes ; i++)
		for (j = 0; j < ModeInfo.YRes ; j++)
		{
			PutPixel64K (i,j,rgbcolor64K(0x94,0x00,0xd3));
		//	PutPixel256(x,y,10);
		// getch();
		}

	MouseReset();
	// MouseInit();
	// Set_MouseRange(0,0,ModeInfo.XRes - 1,ModeInfo.YRes - 1);
	Get_MouseBK(New_Mouse);
	Get_MouseStatus(&New_Mouse);
	Old_Mouse = New_Mouse;

	while (1){
		// Get_MouseStatus  &  If_MousePress º¯Êý¼ì²â³ÌÐò¶Î£¬Ðè¹Ø±ÕSVGAÄ£Ê½
		// Get_MouseStatus(&New_Mouse);
		// printf("Mouse.x = %d ; Mouse.y = %d ; Mouse.button = %d", New_Mouse.x,New_Mouse.y,New_Mouse.button);
		// delay(20);
		// system("cls");
		// if(If_MousePress(0,0,10,10,New_Mouse) == 1)
			// exit(1);
	
		//
		// Draw_Mouse º¯Êý¼ì²â³ÌÐò¶Î
		// New_Mouse.x = 200;
		// New_Mouse.y = 200;
		// New_Mouse.shape = 1;
		// Draw_Mouse(New_Mouse);
		// Draw_MouseBK  &  Get_MouseBKº¯Êý¼ì²â³ÌÐò¶Î
		// for(New_Mouse.y = i = 50, j = 50; j <= 200; j++)
		// {
			// New_Mouse.x = j;
			// Draw_MouseBK(Old_Mouse);
			// Get_MouseBK(New_Mouse);
			// Draw_Mouse(New_Mouse);
			// delay(50);
		// }
		
		//×ÛºÏ¼ì²â³ÌÐò¶Î
		if(If_MousePress(0,0,50,50,New_Mouse) == 3)
			exit(1);
		Get_MouseStatus(&New_Mouse);
		if (New_Mouse.x == Old_Mouse.x && New_Mouse.y == Old_Mouse.y)
		{
			delay(100);
			Draw_MouseBK(Old_Mouse);
			// delay(500);
			Old_Mouse = New_Mouse;
			continue;
		}
		else if (New_Mouse.x >= 0 && New_Mouse.x <= 300)
		{
			New_Mouse.shape = 1;
			Draw_MouseBK(Old_Mouse);
			Get_MouseBK(New_Mouse);
			Draw_Mouse(New_Mouse);
			delay(30);
			Old_Mouse = New_Mouse;
			continue;
		}
		else if (New_Mouse.x >= 300 && New_Mouse.x <= 550)
		{
			New_Mouse.shape = 2;
			Draw_MouseBK(Old_Mouse);
			Get_MouseBK(New_Mouse);
			Draw_Mouse(New_Mouse);
			Old_Mouse = New_Mouse;
			delay(30);
			continue;
		}
		else if (New_Mouse.x >= 550 && New_Mouse.x < 1024)
		{
			New_Mouse.shape = 3;
			Draw_MouseBK(Old_Mouse);
			Get_MouseBK(New_Mouse);
			Draw_Mouse(New_Mouse);
			Old_Mouse = New_Mouse;
			delay(30);
			continue;
		}
	}
#endif

#if 0
	//*********************************************************   ÌùÍ¼Ïà¹Øº¯Êý¼ì²â
	int i,j;
	unsigned long Pixel;
	FILE *fp;

	Set_SVGAMode(0x117);
	Draw_Bmp64K(0,0,"./logo256.bmp");
	getch();
#endif


#if 1
	//*********************************************************    ºº×ÖÏÔÊ¾Ïà¹Øº¯Êý¼ì²â
	Set_SVGAMode(0x117);
	Puthz16("����", 100, 100, 20, 1, 1, 0xffff);
	getch();
#endif

}
#endif
