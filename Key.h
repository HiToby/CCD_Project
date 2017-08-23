/**********************************************************

*Copyright(c) Automation Excellence Class

*FileName:  // Key.h

*Author:  //syc

*Version:  //

*Date:  //2017.8.18

*Description:  //键盘宏定义，该文件定义了26个字母，F1，到F10，ESC，ENTER，TAB，上下左右键

                //其他函数使用键盘时可以直接调用

                //含义及参数间的控制、顺序、独立及依赖关系

*Others:  //

*History:  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介

    1.Date:

      Author:

      Modification:

    2.…………

**********************************************************/

#ifndef _Key_h
#define _Key_h

#define LEFT 0x4b00
#define RIGHT 0x4d00  
#define DOWN 0x5000
#define UP 0x4800 
#define HOME 0x4700 
#define END 0x4f00
#define ESC 0x11b
#define ENTER 0x1c0d
#define PAGEUP 0x4900
#define PAGEDOWN 0x5100  
#define F1 0x3b00
#define F2 0x3c00 
#define F3 0x3d00
#define F4 0x3e00 
#define F5 0x3f00 
#define F6 0x4000 
#define F7 0x4100 
#define F8 0x4200
#define F9 0x4300
#define F10 0x4400 
#define TAB 0xf09 
#define KEY_Q 0x1071
#define KEY_W 0x1177
#define KEY_E 0x1265
#define KEY_R 0x1372
#define KEY_T 0x1474
#define KEY_Y 0x1579
#define KEY_U 0x1675
#define KEY_I 0x1769
#define KEY_O 0x186f
#define KEY_P 0x1970
#define KEY_A 0x1e61
#define KEY_S 0x1f73
#define KEY_D 0x2064
#define KEY_F 0x2166
#define KEY_G 0x2267
#define KEY_H 0x2368
#define KEY_J 0x246a
#define KEY_K 0x256b
#define KEY_L 0x266c
#define KEY_Z 0x2c7a
#define KEY_X 0x2d78
#define KEY_C 0x2e63
#define KEY_V 0x2f76
#define KEY_B 0x3062
#define KEY_N 0x316e
#define KEY_M 0x326d
#define BREAKSPACE 0xe08



/*函数bioskey（0），bioskey（1），bioskey（2）的头文件*/
#include <bios.h>

#endif


// 目前测试结果。
// ctrl  - 0x0060
// shift - 改变后两位。即ascii 码
// alt   - 后两位清0