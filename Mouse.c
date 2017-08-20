/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: Mouse.c
*Author: JH Yang
*Date:  2017/08/06
*Description: 设置鼠标
*Others: 包含三种鼠标图案
*Function List:
	1.void MouseInit(void);
	2.void Set_MouseRange(int ,int ,int ,int)；//左上右下
	3.void Get_MouseStatus(_Mouse);
	4.void Get_MouseBK(_Mouse);
	5.void Draw_MouseBK(_Mouse);
	6.void Draw_Mouse(_Mouse);
	7.unsigned int If_MousePress(int, int, int, int,_Mouse);
	8.void MouseReset();
	
*History:
	1.Date:
      Author:
      Modification:

**********************************************************/

#include "SVGA.h"
#include "Mouse.h"
#include "head.h"

extern _ModeInfo ModeInfo;

/*定义鼠标大小的长宽*/
#define MouseShapey  16
#define MouseShapex 10

/*一般样式鼠标--用于一般情形*/
const int MouseShape1[MouseShapey][MouseShapex] = {
		{1,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0},
		{1,2,1,0,0,0,0,0,0,0},
		{1,2,2,1,0,0,0,0,0,0},
		{1,2,2,2,1,0,0,0,0,0},
		{1,2,2,2,2,1,0,0,0,0},
		{1,2,2,2,2,2,1,0,0,0},
		{1,2,2,2,2,2,2,1,0,0},
		{1,2,2,2,2,2,2,2,1,0},
		{1,2,2,2,2,2,1,1,1,1},
		{1,2,2,1,2,2,1,0,0,0},
		{1,2,1,0,1,2,2,1,0,0},
		{1,1,0,0,1,2,2,1,0,0},
		{1,0,0,0,0,1,2,2,1,0},
		{0,0,0,0,0,1,2,2,1,0},
		{0,0,0,0,0,0,1,1,1,0},
	};

/*手型鼠标--用于图标选择*/
const int MouseShape2[MouseShapey][MouseShapex] = {
		{0,1,1,1,0,0,0,0,0,0},
		{0,1,2,1,0,0,0,0,0,0},
		{0,1,2,1,0,0,0,0,0,0},
		{0,1,2,1,0,0,0,0,0,0},
		{0,1,2,1,1,1,0,0,0,0},
		{0,1,2,1,2,1,1,1,0,0},
		{0,1,2,1,2,1,2,1,1,1},
		{1,1,2,2,2,1,2,1,2,1},
		{1,2,2,2,2,2,2,1,2,1},
		{1,2,2,2,2,2,2,2,2,1},
		{1,2,2,2,2,2,2,2,2,1},
		{1,1,2,2,2,2,2,2,2,1},
		{0,1,1,2,2,2,2,2,2,1},
		{0,0,1,1,2,2,2,2,1,1},
		{0,0,0,1,1,2,2,1,1,0},
		{0,0,0,0,1,1,1,1,0,0},
	};

/*输入鼠标--用于文本框选择*/
const int MouseShape3[MouseShapey][MouseShapex] = {
		{1,1,1,1,0,0,1,1,1,1},
		{1,1,1,1,0,0,1,1,1,1},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0},
		{1,1,1,1,0,0,1,1,1,1},
		{1,1,1,1,0,0,1,1,1,1},
	};

/*鼠标遮挡的背景缓存数组，全局变量，BK是background的缩写*/
int Mouse_BK[MouseShapey][MouseShapex] = {0};

/*************************************************
*Function: void MouseInit(void);
*Description: 初始化鼠标
*Calls: // 被本函数调用的函数清单
*Input: mode : 无;
*Output: 错误信息
*Return: a.ax 0000 不支持
*			  ffff 支持	
*Others: 无
*************************************************/
void MouseInit(void)
{

	union REGS uRegs;

	/*设置鼠标功能号*/
	uRegs.x.ax = 0;
	int86(0x33,&uRegs,&uRegs);

	/*返回错误信息*/
	if(uRegs.x.ax == 0x0000)
	{
		printf("The mouse failed to initialize!");
		delay(5000);
		exit(1);
	}
}

/*************************************************
*Function: void Set_MouseRange(int ,int ,int ,int);
*Description: 设置鼠标的移动范围
*Calls: // 被本函数调用的函数清单
*Input: mode : 无;
*Output: 错误信息
*Return: 无
*Others: 无
*************************************************/
void Set_MouseRange(int x1,int y1,int x2,int y2)
{
	union REGS uRegs;
	
	/*设置横坐标范围*/
	uRegs.x.ax = 7;
	uRegs.x.cx = x1;
	uRegs.x.dx = x2;
	int86(0x33, &uRegs, &uRegs);
	
	/*设置纵坐标范围*/
	uRegs.x.ax = 8;
	uRegs.x.cx = y1;
	uRegs.x.dx = y2;
	int86(0x33, &uRegs, &uRegs);
}

/*************************************************
*Function: void Get_MouseStatus(_Mouse);
*Description: 获取鼠标的相关信息
*Calls: // 被本函数调用的函数清单
*Input: mode : 无;
*Output: 鼠标的位置，以及按键信息；
*Return: 无
*Others: 无
*************************************************/
void Get_MouseStatus(_Mouse * Mouse)
{
	union REGS uRegs;
	
	/*设置功能号*/
	uRegs.x.ax = 3;
	int86(0x33, &uRegs, &uRegs);

	Mouse->x = uRegs.x.cx;
	Mouse->y = uRegs.x.dx;
	Mouse->button = uRegs.x.bx;
}

/*************************************************
*Function: void Get_MouseBK(_Mouse);
*Description: 获取目标位置鼠标的覆盖的背景
*Calls: // 被本函数调用的函数清单
*Input: mode : 鼠标信息;
*Output: 错误信息；
*Return: 无
*Others: 无
*************************************************/
void Get_MouseBK(_Mouse Mouse)
{
	int i,j;
	int (*MouseShape)[10];

	/*选择鼠标图形*/
	switch(Mouse.shape) {
		case 1:
			MouseShape = MouseShape1;
			break;
		case 2:
			MouseShape = MouseShape2;
			break;
		case 3:
			MouseShape = MouseShape3;
			break;
		default :
			printf("The shpae mouse does not exist!");
			delay(5000);
			exit(1);
	}

	/*获取并存储鼠标覆盖的图像*/
	for (i = 0; i < MouseShapex; i++)
	{
		for (j = 0; j < MouseShapey; j++)
		{
			// 0 无需画图直接跳过
			if (MouseShape[j][i] == 0)
				continue;

			// i 为列所以在y方向上 j 为行所以在x方向上
			Mouse_BK[j][i] = GetPixel64K(i + Mouse.x, j + Mouse.y);
		}
	}
}

/*************************************************
*Function: void Draw_MouseBK(_Mouse);
*Description: 在目标位置绘制存储的鼠标背景
*Calls: // 被本函数调用的函数清单
*Input: mode : 鼠标信息;
*Output: 错误信息；
*Return: 无
*Others: 无
*************************************************/
void Draw_MouseBK(_Mouse Mouse)
{
	int i,j;
	int (*MouseShape)[10];

	/*选择鼠标图形*/
	switch(Mouse.shape) {
		case 1:
			MouseShape = MouseShape1;
			break;
		case 2:
			MouseShape = MouseShape2;
			break;
		case 3:
			MouseShape = MouseShape3;
			break;
		default :
			printf("The shpae mouse does not exist!");
			delay(5000);
			exit(1);
	}

	/*获取并存储鼠标覆盖的图像*/
	for (i = 0; i < MouseShapex; i++)
	{
		for (j = 0; j < MouseShapey; j++)
		{

			// 0 无需画图直接跳过
			if (MouseShape[j][i] == 0)
				continue;

			// i 为列所以在y方向上 j 为行所以在x方向上
			PutPixel64K(i + Mouse.x, j + Mouse.y, Mouse_BK[j][i]);
		}
	}
}

/*************************************************
*Function: void Draw_Mouse(_Mouse);
*Description: 在目标位置绘制鼠标图形
*Calls: // 被本函数调用的函数清单
*Input: mode : 鼠标信息;
*Output: 错误信息， 鼠标图形；
*Return: 无
*Others: 无
*************************************************/
void Draw_Mouse(_Mouse Mouse)
{
	int i,j;
	int (*MouseShape)[10];

	/*选择鼠标图形*/
	switch(Mouse.shape) {
		case 1:
			MouseShape = MouseShape1;
			break;
		case 2:
			MouseShape = MouseShape2;
			break;
		case 3:
			MouseShape = MouseShape3;
			break;
		default :
			printf("The shpae mouse does not exist!");
			delay(5000);
			exit(1);
	}

	/*获取并存储鼠标覆盖的图像*/
	for (i = 0; i < MouseShapex; i++)
	{
		for (j = 0; j < MouseShapey; j++)
		{
			// 0 无需画图直接跳过
			if (MouseShape[j][i] == 0)
			{
				continue;
			}

			// i 为列所以在y方向上 j 为行所以在x方向上
			else if (MouseShape[j][i] == 1)
			{
					PutPixel64K (i + Mouse.x, j + Mouse.y, Black);
			}

			else if (MouseShape[j][i] == 2)
			{
					PutPixel64K (i + Mouse.x, j + Mouse.y, White);
			}
		}
	}
}

/*************************************************
*Function: unsigned char If_MousePress(int x1, int y1, int x2, int y2);
*Description: 判断鼠标是否在目标位置按下
*Calls: // 被本函数调用的函数清单
*Input: mode : 目标区域;
*Output: 无；
*Return: 返回按键信息：1 左键；
*					   2 右键；
*					   4 中键；
*					   0 无；
*					   //以上各数可以相加
*Others: 无
*************************************************/
unsigned int If_MousePress(int x1, int y1, int x2, int y2, _Mouse Mouse)
{
	//****若无传入鼠标状态则启用一下程序段
	/*初始化鼠标结构体*/
	// _Mouse Mouse = {0,0,0,1};

	/*获取鼠标状态*/
	// Get_MouseStatus(&Mouse);

	
	if ((Mouse.x >= x1) && (Mouse.y >= y1) && (Mouse.x <= x2) && (Mouse.y <= y2))
	{
		return Mouse.button;
	}

}

/*************************************************
*Function: void MouseReset();
*Description: 鼠标重置函数
*Calls: // 被本函数调用的函数清单
*Input: mode : 无
*Output: 无；
*Return: 错误信息
*Others: 无
*************************************************/
void MouseReset()
{
	_Mouse Mouse = {0,0,0,1};
	MouseInit();
	Set_MouseRange(0,0,ModeInfo.XRes - 1,ModeInfo.YRes - 1);
	Get_MouseStatus(&Mouse);
	Get_MouseBK(Mouse);
}