/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: SVGA.h
*Author: JH Yang
*Date:  2017/08/06
*Description: 图形界面初始化
*Others: 
*Function List:
    1.extern void Set_SVGAMode(unsigned int);
    2.extern unsigned int Get_SVGAMode(void);
    3.extern void Set_ViewPage(unsigned int);
    4.extern unsigned int Get_ViewPage(void);
    
    5.extern long rgbcolor64K (unsigned char r,unsigned char g, unsigned char b);
    6.extern void PutPixel64K (int x, int y, unsigned long color);
    7.extern unsigned long GetPixel64K (int x, int y);

*History:
	1.Date:
      Author:
      Modification:

**********************************************************/

#ifndef _SVGA_H_
#define _SVGA_H_

typedef struct _modeinfo//VBE模式信息共256字节
{
    short ModeAttr;           //模式的属性
    char  WinAAttr;
    char  WinBAttr;            //窗口A,B的属性

    short WinGran;             //位面大小(窗口粒度),以KB为单位
    short WinSize;             //窗口大小,以KB为单位
    short WinASeg; 
    short WinBSeg;             //窗口A,B的起始段址
    char  far *BankFunc;       //换页调用入口指针

    short charsPerScanLine;    //每条水平扫描线所占的字节数
    short XRes, YRes;          //水平,垂直方向的分辨率
    char  XCharSize;
    char  YCharSize;           //字符的宽度和高度
    char  NumberOfplanes;      //位平面的个数
    char  BitsPerPixel;        //每像素的位数
    char  NumberOfBanks;       //CGA逻辑扫描线分组数
    char  MemoryModel;         //显示内存模式
    char  BankSize;            //CGA每组扫描线的大小
    char  NumberOfImagePages;  //可同时载入的最大满屏图像数
    char  reserve1;            //为页面功能保留

/*//对直接写颜色模式的定义区域
    char  RedMaskSize;         //红色所占的位数
    char  RedFieldPosition;    //红色的最低有效位位置
    char  GreenMaskSize;       //绿色所占位数
    char  GreenFieldPosition;  //绿色的最低有效位位置
    char  BlueMaskSize;        //蓝色所占位数
    char  BlueFieldPosition;   //蓝色最低有效位位置
    char  RsvMaskSize;         //保留色所占位数
    char  RsvFieldPosition;    //保留色的最低有效位位置
    char  DirectColorModeInfo; //直接颜色模式属性

//以下为VBE2.0版本以上定义
    char  far *PhyBasePtr;     //可使用的大的帧缓存时为指向其首址的32位物理地址
    long  OffScreenMenOffset;  //帧缓存首址的32位偏移量
    short OffScreenMemSize;    //可用的,连续的显示缓冲区,以KB为单位

//以下为VBE3.0版以上定义
    short LincharsPerScanLine;   //线形缓冲区中每条扫描线的长度,以字节为单位
    char  BnkNumberOfImagePages; //使用窗口功能时的显示页面数
    char  LinNumberOfImagePages; //使用大的线性缓冲区时的显示页面数
    char  LinRedMaskSize;        //使用大的线性缓冲区时红色所占位数
    char  LinRedFieldPosition;   //使用大的线性缓冲区时红色最低有效位位置
    char  LinGreenMaskSize;      //使用大的线性缓冲区时绿色所占的位数
    char  LinGreenFieldPosition; //使用大的线性缓冲区时绿色最低有效位位置
    char  LinBlueMaskSize;       //使用大的线性缓冲区时蓝色所占的位数
    char  LinBlueFieldPosition;  //使用大的线性缓冲区时蓝色最低有效位位置
    char  LinRsvMaskSize;        //使用大的线性缓冲区时保留色所占位数
    char  LinRsvFieldPosition;   //使用大的线性缓冲区时保留色最低有效位位置
    char  reserve2[194];         //保留*/
}_ModeInfo;

/*********************SVGA相关函数************************/
extern void Set_SVGAMode(unsigned int);
extern unsigned int Get_SVGAMode(void);
extern void Set_ViewPage(unsigned int);
extern unsigned int Get_ViewPage(void);

/*********************画图相关函数************************/
extern long rgbcolor64K (unsigned char r,unsigned char g, unsigned char b);
extern void PutPixel64K (int x, int y, unsigned long color);
extern unsigned long GetPixel64K (int x, int y);

#endif