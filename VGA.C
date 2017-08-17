/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: VGA.c
*Author: JH Yang
*Date:  2017/08/17
*Description: 设置自带的图形系统
*Others: NONE
*Function List:
	1.void Set_VGAMode(void)

*History:
	1.Date:
      Author:
      Modification:

**********************************************************/

#include "VGA.h"

/*************************************************
*Function: void Set_VGAMode(void);
*Description: 设置并初始化显示模式，返回VBE特定信息
*Calls: // 被本函数调用的函数清单
*Input: mode : none;
*Output: none
*Return: 无
*Others: 无
*************************************************/
void Set_VGAMode(void)
{
	int dirver = VGA;
	int mode = VGAHI,i,j,x,y;
	initgraph(&dirver,&mode,"../BORLANDC/BGI");
}
