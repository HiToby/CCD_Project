/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: Mouse.h
*Author: JH Yang
*Date:  2017/08/06
*Description: 设置鼠标
*Others: NONE
*Function List:
	1.extern void MouseInit(void);
	2.extern void Set_MouseRange(int ,int ,int ,int)；
	3.extern void Get_MouseStatus(_Mouse);
	4.extern void Get_MouseBK(_Mouse);
	5.extern void Draw_MouseBK(_Mouse);
	6.extern void Draw_Mouse(_Mouse);
	7.extern unsigned int If_MousePress(int, int, int, int,_Mouse);
	8.extern void MouseReset();

*History:
	1.Date:
      Author:
      Modification:

**********************************************************/
#ifndef _MOUSE_H_
#define _MOUSE_H_

/*Mouse结构体*/
typedef struct _mouse
{
	int x ,y;		//x,y 分别为鼠标的横纵坐标
	int button;		//第零位为鼠标左键信息 1
					//第一位为鼠标右键信息 2 
					//第二位为鼠标中键信息 4
					//0为未按下， 1为按下
	int shape;		//鼠标图形信息
} _Mouse;
/*警告！！！！！！！！！！！！！！！！
*_Mouse在使用时如果为首次打开Dosbox
*button初始化值为1
*但如果第二次及以上调用时
*button初始值为1，需按下按键后才能恢复正常
*若鼠标按键调用出现问题，先检查此处
*/


extern void MouseInit(void);
extern void Set_MouseRange(int ,int ,int ,int);
extern void Get_MouseStatus(_Mouse*);
extern void Get_MouseBK(_Mouse);
extern void Draw_MouseBK(_Mouse);
extern void Draw_Mouse(_Mouse);
extern unsigned int If_MousePress(int, int, int, int,_Mouse);
extern void MouseReset();

#endif