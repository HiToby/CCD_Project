/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName:  Map.c
*Author:  YJH
*Version:  //版本
*Date:  2017.8.29
*Description:  //用于主要说明此程序文件完成的主要功能
                //与其他模块或函数的接口、输出值、取值范围、
                //含义及参数间的控制、顺序、独立及依赖关系
*Others:  //其他内容说明
*Function List:  //主要函数列表，每条记录应包含函数名及功能简要说明
 	1. void Draw_Arc(int centerx,int centery,int radius ,int color,int num);	
    2.…………
*History:  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
    1.Date:
      Author:
      Modification:
    2.…………
**********************************************************/

#include "svga.h"

/************************************************
*Function: void Draw_Arc(int, int, int, int)
*Description: 64K色条件下，采用bresenham画圆方法
*Calls: PutPixel64K(int int int)
*Input: centerx,centery 为圆心
*		radius 为半径
*		color 为直线颜色

*Output: 错误信息
*Return: 无
*Others: 画π/4圆弧，num =1时，为右上，依次向下。最后转到左上
！！！！！！！！！！！！！！！本函数亟待优化
*************************************************/
void Draw_Arc(int centerx,int centery,int radius ,int color,int num)
{
	int x,y,d;
	if (!radius) return;
	y = radius;
	d = 3 - radius << 1;
	if(num==1)
		{
			for (x = 0; x <= y; x++)
			{
			  	PutPixel64K(centerx + x,centery - y,color);//4
				if (d < 0) d += x * 4 + 6;
				else d += (x - y--) * 4 + 10;
			}
		}
	else if(num==2)
		{
			for (x = 0; x <= y; x++)
			{
	    		PutPixel64K(centerx + y,centery - x,color);//6	
				if (d < 0) d += x * 4 + 6;
				else d += (x - y--) * 4 + 10;
			}
		}
	else if(num==3)
		{
			for (x = 0; x <= y; x++)
			{  
		    	PutPixel64K(centerx + y,centery + x,color);//5
				if (d < 0) d += x * 4 + 6;
				else d += (x - y--) * 4 + 10;
			}
		}
	else if(num==4)
		{
			for (x = 0; x <= y; x++)
			{
				PutPixel64K(centerx + x,centery + y,color);//1				
				if (d < 0) d += x * 4 + 6;
				else d += (x - y--) * 4 + 10;
			}
		}
		else if(num==5)
		{
			for (x = 0; x <= y; x++)
			{
		    	PutPixel64K(centerx - x,centery + y,color);//4
				if (d < 0) d += x * 4 + 6;
				else d += (x - y--) * 4 + 10;
			}
		}
	 else if(num==6)
		{
			for (x = 0; x <= y; x++)
			{

		    PutPixel64K(centerx - y,centery + x,color);//8
				if (d < 0) d += x * 4 + 6;
				else d += (x - y--) * 4 + 10;
			}
		}
		
		
		else if(num==7)
		{
			for (x = 0; x <= y; x++)
			{
		    	PutPixel64K(centerx - y,centery - x,color);//7
				if (d < 0) d += x * 4 + 6;
				else d += (x - y--) * 4 + 10;
			}
		}
	 else if(num==8)
		{
			for (x = 0; x <= y; x++)
 			{
	    		PutPixel64K(centerx - x,centery - y,color);//3
				if (d < 0) d += x * 4 + 6;
				else d += (x - y--) * 4 + 10;
			}
		}
}

/*************************************************
*Function: void Draw_DotLine(int, int, int, int, int)
*Description: // 函数功能、性能等的描述
*Calls: // 被本函数调用的函数清单
*Input: // 输入参数说明，包括每个参数的
	   // 用、取值说明及参数间关系。
*Output: // 对输出参数的说明。
*Return: // 函数返回值的说明
*Others: // 其它说明
*************************************************/
void Draw_DotLine(int left, int top, int right, int bottom, int color)
{
	int d;
	int dx,dy,dx2,dy2;
	int xinc,yinc;
	int dxy;
	int half;
	int flag;
	dx = abs(right - left);
	dx2 = dx << 1;
	dy = abs(bottom - top);
	dy2 = dy << 1;
	xinc = (right > left) ? 1 : (right == left ? 0 : -1);
	yinc = (bottom > top) ? 1 : (bottom == top ? 0 : -1);
	flag = 1;
	PutPixel64K(left, top, color);
	if (dx >= dy)
	{
		d = dy2 - dx;
		dxy = dy2 - dx2;
		while (dx--)
		{
			if (d <= 0) d += dy2;
			else {
				d += dxy;
				top += yinc;
			}
			if (flag < 10)
			{
				PutPixel64K(left += xinc, top, color);
				flag++;
			} else if (flag < 20)
			{
				flag = (flag + 1) % 20;
				left += xinc;
			} 
						
		}
	} else {
		d = dx2 - dy;
		dxy = dx2 - dy2;
		while (dy--)
		{
			if (d <= 0) d += dx2;
			else {
				d += dxy;
				left += xinc;
			}
			if (flag < 10)
			{
				PutPixel64K(left, top += yinc, color);
				flag++;
			} else if (flag < 20)
			{
				flag = (flag + 1) % 20;
				top += yinc;
			} 
		}
	}
}