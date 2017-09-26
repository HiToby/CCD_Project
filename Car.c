/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: Car.c
*Author: SYC
*Date:  2017/09/24
*Description: 汽车基本操作
*Others: 无
*Function List:
*History:  
    1.Date:
      Author:
      Modification:
    2.
**********************************************************/
#include "head.h"
#include "Path.h"
#include "Traffic.h"
#include "Car.h"

/*************************************************
*Function: void Output_OneCar(_CAR *car)
*Description:输出一辆汽车信息
*Calls: 1 . stdio.h
		2 . 
*Input: 
*Output:
*Return: 
*Others: 
*************************************************/
void Output_OneCar(_CAR *car)
{
	
	printf("output the information of cars\n");
	printf("Location of the car: (%d,%d)\n",car->x,car->y);
	printf("output length and width : %d %d \n",car->length,car->width);
	printf("pattern and arc : %d,%d \n",car->pattern,car->arc);
	printf("speed and sight : %d,%d\n",car->v,car->sight);
	printf("drive_flag and lane_flag : %s,%s\n",car->drive_flag,car->lane_flag);
	printf("begin: %s,end: %s\n",car->begin,car->end);
	printf("path : %s\n",car->path);
	printf("lable: %s\n",car->lable);
	printf("lane_cur: %s , lane_aim: %s",car->lane_cur,car->lane_aim);
	
}
/*************************************************
*Function: void Output_Crossroad(_Crossroad *road)
*Description:输出道路信息
*Calls: 1 . stdio.h
		2 . 
*Input: 头节点	汽车数目
*Output:头节点
*Return: 
*Others: 
*************************************************/
void Output_Crossroad(_Crossroad *road)
{	
	
	int i,j;
	printf("output the information of roads\n");
	printf("crossing num : %d,crossing arc : %d, crossing type :%d\n",road->num,road->arc,road->type);
	for(i=0;i<Road_NumMax;i++)
	 {
		for(j=0;j<RoadType_Num;j++)
	 	 {
	 	 	// Output_Car();
	 	 }
	 }
}

/*************************************************
*Function: _CAR *Creat_Car(_CAR *phead,int num)
*Description:创建汽车
*Calls: 1 . random
		2 . 
*Input: 头节点	汽车数目
*Output:头节点
*Return: 
*Others: 汽车路径输入时间复杂度高，需要优化
*************************************************/
_CAR *Creat_Car(_CAR *phead,int num)
{
    int Car[7][3][2]={
	{{91,0},    {115,0},   {139,0}},					//0
	{{358,0},   {382,0},   {406,0}},					//1
	{{689,0},   {713,0},   {737,0}},					//2
	{{213,768}, {189,768}, {165,768}},				//9
	{{1024,364},{1024,388},{1024,412}},				//10
	{{0,703},   {0,679},   {0,655}},					//13
	{{1024,581}, {1024,605},{1024,629}}				//14
	};

	_CAR  *pnew;
	_CAR  *pend;
	int i;
	int Car_Start=0;
	int Car_End=0;
	int randroad=0;
	int path[7];
	randomize();

	//phead=NULL;
	do{
		/******内存是否分配成功检测********/
		Car_Start=random(7);
        Car_End=random(7);
        randroad=random(3);
        while(Car_Start==Car_End)
	    {
		  Car_End=random(7);
	    }//随机出车的初始位置
	    Get_Path(path,Car_Start,Car_End);
		if((pnew=(_CAR *)malloc(sizeof(_CAR)))==NULL)
		{
			printf("No room \n");
			exit(1);
		}
		pnew->pattern=(char)random(4);
		pnew->begin=Car_Start;
		pnew->end = Car_End;
		pnew->x=Car[Car_Start][randroad][0];
	    pnew->y=Car[Car_Start][randroad][1];
	    for(i=0;path[i]>=0;i++)
	    {
	    	pnew->car_path[i] = path[i];
	    }//时间复杂度高，需要优化
	    
	    num--;//控制车辆数目
	    if(phead == NULL)
	    {
	    	phead = pnew;
	    	pend = phead;
	    }
	    else
	    {
	    	pend ->next = pnew;
	    	pnew ->prev = pend;
	    	pend = pnew;

	    }
	}while(num>0);
	pend -> next =NULL;
	free(pnew);
	return phead;

}
_CAR *Insert_Car(_CAR *phead, _CAR *pnew)
{
	_CAR *pcur;
	

}
_CAR *Delete_Car(_CAR *phead,unsigned n)
{
	
}
/*************************************************
*Function: void Destory_Car(_CAR *phead)
*Description:删除所有车辆
*Calls: 1 . free
		2 . 
*Input: 头节点	
*Output:
*Return: 
*Others: 节约内存，释放整个链
*************************************************/
void Destory_Car(_CAR *phead)
{
	_CAR *pcur;
	_CAR *pnext;
	pcur = phead;
	for(pnext = pcur -> next;pcur ==NULL; pcur =pnext,pnext = pnext ->next)
		free(pcur);
}
/*************************************************
*Function:void Output_Car(_CAR *phead)
*Description:输出所有车辆信息
*Calls: 1 . stdio.h
		2 . 
*Input: 头节点	
*Output:
*Return: 
*Others: 
*************************************************/
void Output_Car(_CAR *phead)
{
	_CAR *pcur;
	int i;
	pcur = phead;

	while (pcur != NULL)
	{
		printf("output the information of cars\n");
		printf("	Location of the car: (%d,%d)\n",pcur->x,pcur->y);
		printf("	output length and width : %d %d \n",pcur->length,pcur->width);
		printf("	pattern and arc : %d,%d \n",pcur->pattern,pcur->arc);
		printf("	speed and sight : %d,%d\n",pcur->v,pcur->sight);
		printf("	drive_flag and lane_flag : %c,%c\n",pcur->drive_flag,pcur->lane_flag);
		printf("	begin: %c,end: %c\n",pcur->begin,pcur->end);
		printf("	car_path : ");
		for(i=0;i<7;i++)
		{
	 	printf("%d ",pcur->car_path[i]);
		}
		//printf("path : %s\n",pcur->path);
		printf("\n    lable: %c\n",pcur->lable);
		printf("	lane_cur: %c , lane_aim: %c",pcur->lane_cur,pcur->lane_aim);
		pcur = pcur -> next;
	}
	
	printf("\n");
}