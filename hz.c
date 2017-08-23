/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName:  // hz.c
*Author:  //syc
*Version:  //版本
*Date:  //2017.8.18
*Description:  //输入汉字
*Others:  输入汉字必须为GBK编码汉字。其他编码不行！！！！！！！
*		  ！！！！！！！！！！！！！！！！！！！！！！！！！！
*Function List:  //主要函数列表，每条记录应包含函数名及功能简要说明
 	1.void Puthz16(char *s,int x,int y,int wd,int Px,int Py,char color);
 					*s为输入的汉字，int x，y是输入的x,y轴坐标, int wd 是汉字间的间距int Px,Py是x,y轴方向上的大小
 					char color 为字体颜色
*History:  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
    1.Date:
      Author:
      Modification:
    2.…………
**********************************************************/

#include "hz.h"
#include "Head.h"

/*************************************************
*Function: Puthz16
*Description: 该函数把字符串s中的内容输出到图形模式下的屏幕上
*Calls:  fseek,fopen
*Input:  x，y输出点左上角的坐标,wd为汉字之间的间距,px，py设置字体在x，y方向上的大小
*Output:  none
*Others: wd 一般设置为20
*		 若无放缩 px,py设置成1；
*************************************************/
void Puthz16(char *s,int x,int y,int wd,int Px,int Py,char color)
{
	FILE *fq;
	char buffer[32];
	register m,n,i,j,k;
	unsigned char qh,wh;
	unsigned long offset;

	/*打开汉子库*/
	if((fq = fopen("./hzk//hzk16","rb")) == NULL)
	{
		printf("can't open hzk16,please add it");
		getch();	
		exit(0);
	}

	while(*s)
	{
		/*计算汉字区码*/
		qh=*s - 0xa0;
		/*计算汉字位码*/
		wh=*(s + 1) - 0xa0;//0xa0;

		offset=(94 * (qh - 1) + (wh - 1)) * 32L;

		fseek(fq, offset, SEEK_SET);
		fread(buffer, 32, 1, fq);

		for(i = 0; i < 16; i++)
			for(n = 0; n < Px; n++)
				for( j = 0; j < 2; j++)
					for( k = 0; k < 8; k++)
						for(m = 0; m < Py; m++)
							if(((buffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
							{
								PutPixel64K(x + 8 * j * Py + k * Py + m, y + i * Px + n, color);
							}

		s += 2;
		x = x + wd;
	}

	fclose(fq);
}

