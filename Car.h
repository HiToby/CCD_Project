/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: Car.h
*Author: SYC
*Date:  2017/08/30
*Description: 交通相关各种接头体定义
*Others: 无
*Function List:
*History:  
    1.Date:
      Author:
      Modification:
    2.
**********************************************************/

#ifndef _Car_H_
#define _Car_H_

/************************输出打印车和路的信息************************/
extern void Output_OneCar();
extern void Output_Crossroad();
/************************车辆函数************************/
extern _CAR *Creat_Car(_CAR *phead,int num);
extern void Destory_Car(_CAR *phead);
extern void Output_Car(_CAR *phead);
#endif