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
#define Debug_SVGA

#include "Mouse.h"
#include "hz.h"
#include "Key.h"
//#include "Map.h"
#include "Traffic.h"

#ifdef Debug_SVGA
	#include "SVGA.H"
	#define Debug
#endif

#ifdef Debug_VGA
	#include "VGA.H"
	#define Debug
#endif

#ifdef Debug
int main() {

_Crossroad Intersection[Node_Num];
int i,j,k;

#if 1
	//*********************************************************  交通初始化测试程序
	TrafficInit(Intersection);
	for (i = 0; i < Node_Num; i++)
	{
		printf("Intersection Num: %d;\n",i);
		printf("Road Num: %d;\n",Intersection[i].num);
		printf("Intersection Arc:%d;\n",Intersection[i].arc);
		for (j = 0; j < Intersection[i].num; j++)
			for (k = 0; k < RoadType_Num; k++)
			{
				printf("Road Num : %d;	RoadType Num : %d\n",j,k);
				Output_Car(&Intersection[i].Road[j][k]);
				getch();
			}
	}
#endif 
}
#endif