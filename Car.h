/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: Car.h
*Author: SYC
*Date:  2017/08/30
*Description: ��ͨ��ظ��ֽ�ͷ�嶨��
*Others: ��
*Function List:
*History:  
    1.Date:
      Author:
      Modification:
    2.
**********************************************************/

#ifndef _Car_H_
#define _Car_H_

/************************�����ӡ����·����Ϣ************************/
extern void Output_OneCar();
extern void Output_Crossroad();
/************************��������************************/
extern _CAR *Creat_Car(_CAR *phead,int num);
extern void Destory_Car(_CAR *phead);
extern void Output_Car(_CAR *phead);
#endif