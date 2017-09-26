/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: Traffic.h
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

#ifndef _Traffic_H_
#define _Traffic_H_

#define Node_Num  15 //只有15个结点，16为了数组定义
#define Road_NumMax 4 //定义最大路口数目
#define RoadType_Num 3 //定义车道种类数目
#define Road_Width 24 //定义车道宽   一个车道宽+一条白线宽
#define AtoR 0.01745329 // 角度转弧度乘数因子

/************************定义汽车结构体************************/
typedef struct Car{

	//1.中心点坐标
	int x;
	int y;
	//汽车存储的是车中心的xy坐标
	//头结点存储的是道路停车线的xy坐标		√

	// 2.汽车车身属性
	int length;				// 车长
							// 道路长度
	int width;				// 车身
							// 道路宽度
	unsigned char pattern;	// 样式
	int arc;				// 角度
							// 道路角度		√
	int v;					// 速度
	int sight;				// 可见状态 
							//0 为不可见  1 为可见   小车
							//0 为不可见  1 为可见   停车线    √

	//3.标识符
	char drive_flag;		// 汽车状态标识符
							// -2  首尾空节点标识符  √  -1 不可见镜像车辆标识符
							// 0 停止标识符              1直线行驶标识符
							// 2 转弯标识符
	char lane_flag;			// 0 不需要变道  1 需变道但是为变道
							// 2 变道始道    3 变道终道

	//路经相关
	char begin;				// 起始点
							// 道路起始结点
	char end;				// 目标点
							// 道路目标结点 √
	unsigned long path;
	char *car_path;			// 路径  每一字节表示一个目标路口标号
	unsigned char lable;	// 当前所在路口标号

	//车道相关
	unsigned char lane_cur;	// 当前车道
	unsigned char lane_aim;	// 目标车道
	
	//链表相关
	struct Car *prev;		// 前一车辆
	struct Car *next;		// 后一车辆
	struct Car *mirror;		// 镜像车辆
	
}_CAR;

/************************定义红绿灯结构体************************/
typedef struct {
	// 路灯标识符
	unsigned char rel_flag; // 路灯关联状态标识符
							// 0 为无关联  1 为有关联

	// 路灯标识符
	unsigned char lable[2][3]; 
		//红灯  -1    黄灯  0    绿灯  1
		//lable[0] 路灯1    lable[1] 路灯2
		//lable[][0]  左转灯    lable[][1]  直行灯    lable[][2]右转灯
		
	//路灯时间
	unsigned long time[2][3];
		//0x    00    00    00    00
		//      无   红灯  黄灯  绿灯
		//lable[0] 路灯1    lable[1] 路灯2
		//lable[][0]  左转灯    lable[][1]  直行灯    lable[][2]右转灯


}_TarLight;

/************************定义路口结构体(十字&丁字)************************/
typedef struct 
{
	//路口属性
	int num; 				//路口数目		√		
	int arc;				//路口角度		√
	int type;				//路口种类		√
							//0 为进入十字路口   1 为进入丁字路口   2 为进入45°弯道路口 3 为出地图路口

	//红绿灯相关
	_TarLight CarLight1;	//汽车红绿灯一
	_TarLight CarLight2;	//汽车红绿灯二

	//汽车车道相关
	_CAR Road[Road_NumMax][RoadType_Num];        //一个路口的车道
							//	Road[0]  N,NE车道     Road[1]  E,SE车道
							//	Road[2]  S,SW车道     Road[3]  W,NW车道
							//	若少于四个则为从N方向开始顺时针计算
							//	Road[][0]  左转车道    
							//	Road[][1]  直行车道
							//	Road[][2]  右转车道

}_Crossroad;

/************************初始化函数************************/
extern void TrafficInit(_Crossroad Intersection[]);
extern void RoadInit(_Crossroad * Inter,int i);
//extern void Output_Traffic();
#endif