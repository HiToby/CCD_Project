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
    7.extern void PutPixel64K_Xor (int x, int y, unsigned long color);
    8.extern unsigned long GetPixel64K (int x, int y);
    9.extern void PutPixel256 (int x, int y, unsigned long color);

    10.extern void Get_BmpFileHeader(FILE*, _BITMAPFileHeader*);
    11.extern void Get_BmpInfoHeader(FILE*, _BITMAPInfoHeader*);
    12.extern void Draw_Bmp64K(int, int, const char *);
    13.extern void Draw_Bmp64K_Xor(int, int, const char *);

*History:
	1.Date:
      Author:
      Modification:

**********************************************************/

#ifndef _SVGA_H_
#define _SVGA_H_

#include "Head.h"

/********************相关结构体定义***********************/
//VBE模式信息共256字节
typedef struct _modeinfo
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
}_ModeInfo;

//位图文件头BITMAPFILEHEADER，是一个结构体类型。
typedef struct tagBITMAPFILEHEADER      //该结构的长度是固定的，为14个字节。
{  
        unsigned int  bfType;                //2bytes-位图文件类型，对"*.bmp"文件而言，必须是0x424D，即字符串“BM”。
        unsigned long int    bfSize;         //4bytes-位图文件大小，包括这14个字节。
        unsigned int  bfReserved1;           //2bytes-Windows保留字，暂不用。
        unsigned int  bfReserved2;           //2bytes-Windows保留字，暂不用。
        unsigned long int    bfOffBits;      //4bytes-从文件头到实际的位图数据的偏移字节数.
                                             //bitmap文件前3个部分(文件头、信息头、颜色表)的长度之和
}_BITMAPFileHeader;  

//位图信息头BITMAPINFOHEADER，一个结构体类型的数据结构
typedef struct tagBITMAPINFOHEADER           //该结构的长度也是固定的，为40个字节。
{
    unsigned long int    biSize;             //4bytes-本结构的长度，为40个字节。
    long int             biWidth;            //4bytes-位图的宽度，以像素为单位。
    long int             biHeight;           //4bytes-位图的高度，以像素为单位。
    unsigned int         biPlanes;           //2bytes-目标设备的级别，必须是1。
    unsigned int         biBitCount;         //2bytes-每个像素所占的位数（bit）。
                                             //其值必须为1（黑白图像）、4（16色图）、8（256色）、24（真彩色图）。
                                             //新的BMP格式支持32位色。
    unsigned long int    biCompression;      //4bytes-位图压缩类型。这里只讨论未经压缩的情况，即BI_RGB
    unsigned long int    biSizeImage;        //4bytes-实际的位图数据占用的字节数。
    long int             biXPelsPerMeter;    //4bytes-指定目标设备的水平分辨率，单位是像素/米。
    long int             biYPelsPerMeter;    //4bytes-指定目标设备的垂直分辨率，单位是像素/米。
    unsigned long int    biClrUsed;          //4bytes-位图实际用到的颜色数。
                                             //如果该值为零，则用到的颜色数为2的biBitCount次幂。
    unsigned long int    biClrImportant;     //4bytes-位图显示过程中重要的颜色数。
                                             //如果该值为零，则认为所有的颜色都是重要的。
}_BITMAPInfoHeader;

/********************颜色以及调色板***********************/
#define White 0xffff
#define Black 0x0000

//256色--画图调色板
typedef struct
{
    unsigned char R;    /*红色分量，RED缩写*/
    unsigned char G;    /*绿色分量，GREEN缩写*/
    unsigned char B;    /*蓝色分量，BLUE缩写*/
} COLORS256;

//bmp位图对应颜色表
typedef struct tagRGBQUAD               
{
    unsigned char   rgbBlue;            //该颜色的蓝色分量。
    unsigned char   rgbGreen;           //该颜色的绿色分量。
    unsigned char   rgbRed;             //该颜色的红色分量。
    unsigned char   rgbReserved;        //保留字节，暂不用。
}_RGBQUAD;

/*********************SVGA相关函数************************/
extern void Set_SVGAMode(unsigned int);
extern unsigned int Get_SVGAMode(void);
extern void Set_ViewPage(unsigned int);
extern unsigned int Get_ViewPage(void);

/*********************画图相关函数************************/
extern long rgbcolor64K (unsigned char r,unsigned char g, unsigned char b);
extern void PutPixel64K (int x, int y, unsigned long color);
extern void PutPixel64K_Xor (int x, int y, unsigned long color);
extern unsigned long GetPixel64K (int x, int y);
extern void PutPixel256 (int x, int y, unsigned long color);

/*******************bmp位图相关函数***********************/
extern void Get_BmpFileHeader(FILE *, _BITMAPFileHeader *);
extern void Get_BmpInfoHeader(FILE *, _BITMAPInfoHeader *);
extern void Draw_Bmp64K(int, int, const char *);
extern void Draw_Bmp64K_Xor(int, int, const char *);

#endif