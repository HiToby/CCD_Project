/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName:  Head.c
*Author:  JH Yang
*Version:  1.0
*Date:  8/15
*Description:  最底层的一些函数
*Others:  //其他内容说明
*Function List:  //主要函数列表，每条记录应包含函数名及功能简要说明
 	1.char far *farptr(char far *p, long length) (far指针重定位)
    2.…………
*History:  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
    1.Date:
      Author:
      Modification:
    2.…………
**********************************************************/

#include "Head.h"


/*************************************************
*Function: char far *farptr (char far *p, long length) 
*Description: far指针重定位
*Calls: 无
*Input: char far *p : 待操作的原始far指针
		long length : 指针变化的长度
*Output: 无
*Return: 返回新的far指针
*Others: // 其它说明
*************************************************/
char far *farptr (char far *p, long length) 
{
	// 如果length != 0，则启动C语言强制类型转换功能
	if (length) 
		return (char far *)((char huge *)p + length);
	else asm{
			MOV AX,WORD PTR p
			SHR AX,4
			ADD WORD PTR p+2,AX
			ADD WORD PTR p,0x0F
		}
		
	return(p);
}
