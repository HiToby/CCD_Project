/**********************************************************
*Copyright(c) Automation Excellence Class
*FileName: SVGA.C
*Author: JH Yang
*Date:  2017/08/06
*Description: 图形界面初始化
*Others: 
*Function List:
    1. void Set_SVGAMode(unsigned int);
    2. unsigned int Get_SVGAMode(void);
    3. void Set_ViewPage(unsigned int);
    4. unsigned int Get_ViewPage(void);

    5. long rgbcolor64K (unsigned char r,unsigned char g, unsigned char b);
    6. void PutPixel64K (int x, int y, unsigned long color);
    7. void PutPixel64K_Xor (int x, int y, unsigned long color);
    8. unsigned long GetPixel64K (int x, int y);
    9. void PutPixel256 (int x, int y, unsigned long color);

    10. void Get_BmpFileHeader(FILE*, _BITMAPFileHeader*);
    11. void Get_BmpInfoHeader(FILE*, _BITMAPInfoHeader*);
    12. void Draw_Bmp64K(int, int, const char *);
    13. void Draw_Bmp64K_Xor(int, int, const char *);

    14. void Draw_Line(int, int, int, int, int);
    15. void Draw_Circle(int, int, int, int);

*History:
	1.Date: 2017/8/26
      Author:YJH
      Modification:
      		1.放弃原有的计算page来计算页面方法，改用y >> 5(64K) y >> 4(256) 来计算新页面
      		2.贴图函数放弃putpixel方法画点，改为直接写显存画点

	2.Date: 2017/8/27
	  Author: YJH
	  Modification:
	  		添加了画圆以及画线函数。算法采用bresenman算法
	
**********************************************************/
#include "SVGA.H"

//显示模式相关信息
 _ModeInfo ModeInfo;
 //像素点颜色定义
typedef struct BmpPixel {
	unsigned char B;
	unsigned char G;
	unsigned char R;
}_BmpPixel;

/*********************SVGA相关函数************************/
/**********************************************************
	在dos.h中REGS的定义如下：
	struct WORDREGS
	{
		unsigned int ax, bx, cx, dx, si, di, cflag, flags;
	};

	struct BYTEREGS
	{
		unsigned char al, ah, bl, bh, cl, ch, dl, dh;
	};

	union REGS
	{
		struct WORDREGS x;
		struct BYTEREGS h;
	};
这样al对应ax的低八位，ah对应ax的高八位。
调用时需要查表决定各入口参数取值,获取返回值表示的信息。
**********************************************************/

/**********************************************************
ax  说 明
   0x4f00  读SVGA卡信息
   0x4f01  读显示模式信息
   0x4f02  设置显示模式
   0x4f03  读当前显示模式
   0x4f04  存储或恢复SVGA的视频状态
   0x4f05  控制内存页区域切换
   0x4f06  设置或读取逻辑扫描线宽度
   0x4f07  设置或读取视频内存与屏幕的初始对应位置
   0x4f08  设置或读取DAC各原色有效位数
显示模式号bx：
		模式号		分辨率		颜色数		颜色位定义
		0x101		640*480		256				-
		0x103		800*600		256				-
		0x104		1024*768	16				-
		0x105		1024*768	256				-
		0x110		640*480		32K			1:5:5:5
		0x111		640*480		64K			5:6:5
		0x112		640*480		16.8M		8:8:8
		0x113		800*600		32K			1:5:5:5
		0x114		800*600		64K			5:6:5
		0x115		800*600		16.8M		8:8:8
		0x116		1024*768	32K			1:5:5:5
		0x117		1024*768	64K			5:6:5
		0x118		1024*768	16.8M		8:8:8
**********************************************************/

/*************************************************
*Function: void Set_SVGAMode(unsinged int mode);
*Description: 设置并初始化显示模式，返回VBE特定信息
*Calls: // 被本函数调用的函数清单
*Input: mode : SVGA模式号;
*Output: 错误信息
*Return: 无
*Others: 无
*************************************************/
void Set_SVGAMode(unsigned int mode){

	union REGS u_Regs;

	/*寄存器功能号设置*/
	u_Regs.x.ax = 0x4f02;
	u_Regs.x.bx = mode;
	int86(0x10, &u_Regs, &u_Regs);

	if (mode == 0x117 || mode == 0x105 || mode == 0x116 || mode == 0x104) {
		ModeInfo.XRes = 1024;
		ModeInfo.YRes = 768;
	}
	if (mode == 0x114 || mode == 0x113 || mode == 0x115) {
		ModeInfo.XRes = 800;
		ModeInfo.YRes = 600;
	}
	if (mode == 0x110 || mode == 0x111 || mode == 0x112) {
		ModeInfo.XRes = 640;
		ModeInfo.YRes = 480;
	}

	/*ax != 0x004f意味着初始化失败，输出错误信息见上,下同*/
	if (u_Regs.x.ax != 0x004f)
	{
		printf("Error in setting SVGA mode!\nError code:0x%x\n", u_Regs.x.ax);
		delay(5000);
		exit(1);
	}
}


/*************************************************
*Function: unsigned int Get_SVGAMode();
*Description: 检测当前SVGA的模式号
*Calls: // 被本函数调用的函数清单
*Input:  无
*Output: 错误信息
*Return: 返回当前SVGA模式的模式号
*Others: 无
*************************************************/
unsigned int Get_SVGAMode(){

	union REGS u_Regs;

	/*寄存器功能号设置*/
	u_Regs.x.ax = 0x4f03;
	int86(0x10,&u_Regs,&u_Regs);

	//返回错误信息
	if (u_Regs.x.ax != 0x004f)
	{
		printf("Error in getting SVGA mode!\nError code:0x%x\n", u_Regs.x.ax);
		delay(5000);
		exit(1);
	}
	
	return u_Regs.x.bx;
}


/*************************************************
*Function: void Set_ViewPage(unsigned int)
*Description: 设置显示页面
*Calls: // 被本函数调用的函数清单
*Input: Page : 页面号
*Output: // 对输出参数的说明。
*Return: // 函数返回值的说明
*Others: // 其它说明
*************************************************/
void Set_ViewPage(unsigned int Page)
{
	union REGS u_Regs;

	/*上一次的页面号,用于减少切换次数,是使用次数很多的重要变量*/
	static unsigned char s_old_page = 0;
	
	/*标志数，用于判断是否是第一次换页*/
	static int s_flag = 0;

	/*寄存器功能号设置*/
	u_Regs.x.ax = 0x4f05;
	u_Regs.x.bx = 0;

	/*如果是第一次换页*/
	if (s_flag == 0) {
		u_Regs.x.dx = Page;
		s_old_page = Page;
		int86(0x10,&u_Regs,&u_Regs);
		s_flag++;
	}

	/*如果不是第一次换页，切所换页面与当前页面不同*/
	else if (s_old_page != Page) { 
		u_Regs.x.dx = Page;
		s_old_page = Page;
		int86(0x10,&u_Regs,&u_Regs);
	}
}


/*************************************************
*Function: unsigned int Get_ViewPage(void)
*Description: 获取当前页面号
*Calls: // 被本函数调用的函数清单
*Input: 无
*Output: // 对输出参数的说明。
*Return: 当前页面号
*Others: // 其它说明
*************************************************/
unsigned int Get_ViewPage(void)
{

	union REGS u_Regs;

	/*寄存器功能号设置*/
	u_Regs.x.ax = 0x4f05;
	u_Regs.x.bx = 1;

	int86(0x10,&u_Regs,&u_Regs);

	return u_Regs.x.dx;
}


/*********************画图相关函数************************/


/*************************************************
*Function: long rgbcolor64K(unsigned char r,unsigned char g, unsigned char b);
*Description: 得到64K高色彩下的颜色值
*Calls: // 被本函数调用的函数清单
*Input: r,g,b : 分别为颜色值红绿蓝的颜色值
*Output: // 对输出参数的说明。
*Return: 返回64K高色彩下的颜色值
*Others: // 其它说明
*************************************************/
long rgbcolor64K(unsigned char r,unsigned char g, unsigned char b)
{
	return (((((r >> 3) << 6) | (g >> 2)) << 5) | (b >> 3));
}


/*************************************************
*Function:  void PutPixel64K (int x, int y, unsigned long color)
*Description: 设置64K高色彩模式下画点函数
*Calls: // 被本函数调用的函数清单
*Input: x,y : 表示在x,y下画点
		color : 表示x,y所画点的颜色值
*Output: // 对输出参数的说明。
*Return: 无
*Others: 再调用此函数时，如需先扫x方向请将x循环放在y循环后面
*************************************************/
void PutPixel64K (int x, int y, unsigned long color) 
{

	/*显存指针*/
	unsigned int far * const VideoBuffer = (unsigned int far *)0xA0000000L;

	/*页面号*/
	unsigned char Page;

	/*显存位置偏移量*/
	long Pos;

	/*判断是否超出屏幕*/
	if (x >= ModeInfo.XRes || x < 0 || y >= ModeInfo.YRes || y < 0)
		return;

	/*计算偏移量的大小*/
	Pos = (((long)y << 10) + (long)x);

	//Page = Pos >> 15; //32K个点为一页，代替除以32K算法。因为显存指针为char型。所以多除2

	Set_ViewPage(y >> 5);

	VideoBuffer[Pos] = color;
}

/*************************************************
*Function:  void PutPixel64K_Xor (int x, int y, unsigned long color)
*Description: 设置64K高色彩模式下异或画点画点函数
*Calls: // 被本函数调用的函数清单
*Input: x,y : 表示在x,y下画点
		color : 表示x,y所画点的颜色值
*Output: // 对输出参数的说明。
*Return: 无
*Others: 再调用此函数时，如需先扫x方向请将x循环放在y循环后面
*************************************************/
void PutPixel64K_Xor (int x, int y, unsigned long color) 
{

	/*显存指针*/
	unsigned int far * const VideoBuffer = (unsigned int far *)0xA0000000L;

	/*页面号*/
	unsigned char Page;

	/*显存位置偏移量*/
	long Pos;

	/*判断是否超出屏幕*/
	if (x >= ModeInfo.XRes || x < 0 || y >= ModeInfo.YRes || y < 0)
		return;

	/*计算偏移量的大小*/
	Pos = (((long)y << 10) + (long)x);

	//Page = Pos >> 15; //32K个点为一页，代替除以32K算法。因为显存指针为char型。所以多除2

	Set_ViewPage(y >> 5);

	if (color != 0x0000)
		VideoBuffer[Pos] ^= color;
}

/*************************************************
*Function: unsigned long GetPixel64K (int x, int y);
*Description: 获取64K高色彩模式下画点函数
*Calls: // 被本函数调用的函数清单
*Input: x,y : 表示在x,y下画点
*Output: // 对输出参数的说明。
*Return: color : 表示x,y所画点的颜色值
*Others: // 其它说明
*************************************************/
unsigned long GetPixel64K (int x, int y)
{

	/*显存指针*/
	unsigned int far * const VideoBuffer = (unsigned int far *)0xA0000000L;

	/*页面号*/
	unsigned char Page;

	/*显存位置偏移量*/
	long Pos;

	/*判断是否超出屏幕*/
	if (x >= ModeInfo.XRes || x < 0 || y >= ModeInfo.YRes || y < 0)
		return;

	/*计算偏移量的大小*/
	Pos = (((long)y << 10) + (long)x);

	//Page = Pos >> 15; //32K个点为一页，代替除以32K算法

	Set_ViewPage(y >> 5);

	return VideoBuffer[Pos] ;
}

/*************************************************
*Function: void PutPixel256 (int x, int y, unsigned long color);
*Description: 获取256色彩模式下画点函数
*Calls: // 被本函数调用的函数清单
*Input: x,y : 表示在x,y下画点
*Output: // 对输出参数的说明。
*Return: None
*Others: 使用说明
*		 由于256色需要设置调色板
*		 {
*		 	outportb(0x03c8,k);
*		 	outportb(0x03c9,r >> 2);
*		 	outportb(0x03c9,g >> 2);
*		 	outportb(0x03c9,b >> 2);
*		 }
*		 如上所示修改调色板才可调用
*		 且每页只可存在一个调色板
*************************************************/
void PutPixel256 (int x, int y, unsigned long color)
{

	/*显存指针*/
	unsigned char far * const VideoBuffer = (unsigned char far *)0xA0000000L;

	/*页面号*/
	unsigned char Page;

	/*显存位置偏移量*/
	long Pos;

	/*判断是否超出屏幕*/
 	if (x >= ModeInfo.XRes || x < 0 || y >= ModeInfo.YRes || y < 0)
		return;

	/*计算偏移量的大小*/
	Pos = (((long)y << 10) + ((long)x));

	//Page = Pos >> 16; //64K个点为一页，代替除以64K算法

	Set_ViewPage(y >> 6);

	VideoBuffer[Pos] = color;
}

/*************************************************
*Function: void Get_BmpFileHeader(FILE*, _BITMAPFileHeader*);
*Description: 获取bmp位图文件的文件头
*Calls: // 被本函数调用的函数清单
*Input: fp : bmp位图文件指针
*		BmpFH : 文件头指针
*Output: // 对输出参数的说明。
*Return: 无
*Others: fread读取数据块无法达到要求暂不使用
*************************************************/
void Get_BmpFileHeader(FILE *fp,_BITMAPFileHeader *BmpFH)
{

	/*文件指针移至文件首*/
	rewind(fp);

	/*读取位图文件头*/
	fread(BmpFH, sizeof(_BITMAPFileHeader), 1, fp);
}

/*************************************************
*Function: void Get_BmpInfoHeader(FILE*, _BITMAPInfoHeader*);
*Description: 获取bmp位图文件的信息头
*Calls: // 被本函数调用的函数清单
*Input: fp : bmp位图文件指针
*		BmpIH : : 信息头指针
*Output: // 对输出参数的说明。
*Return: 无
*Others: fread读取数据块无法达到要求暂不使用
*************************************************/
void Get_BmpInfoHeader(FILE *fp, _BITMAPInfoHeader *BmpIH){

	/*文件指针移至文件首*/
	rewind(fp);

	/*文件指针移至信息头对应位置*/
	fseek(fp, 14L, SEEK_SET); 
	//偏移量大小不可使用sizeof(_BITMAPInfoHeader)
	//结构体sizeof存在字节对齐问题

	/*读取位图文件头*/
	fread(BmpIH, sizeof(_BITMAPInfoHeader), 1, fp);
}

/*************************************************
*Function: void Draw_Bmp64K(int x,int y,const char * BmpPath)
*Description: bmp位图文件在64K色下贴图
*Calls: // 被本函数调用的函数清单
*Input: x,y : 贴图左上角的坐标
*		BmpPath : 所贴图片的路径
*Output: 错误信息
*Return: 无
*Others: // 其它说明
*************************************************/
void Draw_Bmp64K(int x,int y,const char * BmpPath)
{
	_BmpPixel * buffer;
	FILE *fp;
	_BITMAPFileHeader BmpFH;
	_BITMAPInfoHeader BmpIH;
	long color;
	int LineBytes,i,j,ret;

	/*显存指针*/
	unsigned int far * const VideoBuffer = (unsigned int far *)0xA0000000L;		

	/*页面号*/
	unsigned char Page;		

	/*显存位置偏移量*/
	long Pos;		


	/*打开bmp文件*/
	fp = fopen(BmpPath, "rb");
	if (fp == NULL)
	{
		printf("Error Bmp Path!!!\n");
		delay(5000);
		exit(1);
	}
	rewind(fp);

	/*检测是否为bmp文件*/
	//fseek(fp,0,SEEK_SET);
	//ret = fread(&BmpFH.bfType,sizeof(BmpFH.bfType),1,fp);

	//暂时不检测是否为bmp图片。！！！！！ 严禁放入非法图片
	//if (BmpFH.bfType != "BM") 
	//{
	//	printf("The Picture is not BITMAP!!!\n");
		// delay(5000);
		// exit(1);
	// }

	/*检测是否为24位色图片*/
	fseek(fp,28L,SEEK_SET);
	fread(&BmpIH.biBitCount,2,1,fp);

	if (BmpIH.biBitCount != 24) 
	{
		printf("The BITMAP BitCount is not 24!!!\n");
		delay(5000);
		exit(1);
	}

	/*检测是否采用压缩算法*/
	fseek(fp,30L,SEEK_SET);
	fread(&BmpIH.biCompression,4,1,fp);

	if (BmpIH.biCompression != 0)
	{
		printf("The BITMAP has Compression!!!\n");
		delay(5000);
		exit(1);
	}

	/*检测图片大小是否超出屏幕范围*/
	fseek(fp,18L,SEEK_SET);
	fread(&BmpIH.biWidth,4,1,fp);
	fread(&BmpIH.biHeight,4,1,fp);


	if (BmpIH.biWidth > ModeInfo.XRes || BmpIH.biHeight > ModeInfo.YRes)
	{
		printf("The BITMAP is too big!!!\n");
		delay(5000);
		exit(1);
	}

	LineBytes = (BmpIH.biWidth * BmpIH.biBitCount / 8 + 3) / 4 * 4; // / 4 * 4---先整除4在乘以4
	
	/*开辟缓存一行像素的空间*/
	if ((buffer = (_BmpPixel*)malloc(LineBytes)) == 0)
	{
		printf("Error malloc!!!\n");
		delay(5000);
		exit(1);
	}

	/*移动文件指针至图片数据开始处*/
	fseek(fp,10L,SEEK_SET);
	fread(&BmpFH.bfOffBits,4,1,fp);
	fseek(fp,BmpFH.bfOffBits,SEEK_SET);

	for (j = BmpIH.biHeight - 1; j >= 0; j--)
	{
		//读取一行像素点的信息
		fread(buffer,LineBytes,1,fp);

		//处理一行像素点信息
		for (i = 0; i < BmpIH.biWidth; i++)
		{
			color =  rgbcolor64K(buffer[i].R,buffer[i].G,buffer[i].B);
			// PutPixel64K(i + x, j + y, rgbcolor64K(buffer[i].R,buffer[i].G,buffer[i].B));

			/*判断是否超出屏幕*/
			if ((x + i) >= ModeInfo.XRes || (x + i) < 0 || (y + j) >= ModeInfo.YRes || (y + j) < 0)
				continue;		

			/*计算偏移量的大小*/
			Pos = (((long)(y + j) << 10) + (long)(x + i));	

			//Page = Pos >> 15; //32K个点为一页，代替除以32K算法。因为显存指针为char型。所以多除2		
			Set_ViewPage((y + j) >> 5);	

			VideoBuffer[Pos] = color;
		}
	}
	
	free(buffer);
	fclose(fp);
}

/*************************************************
*Function: Draw_Bmp64K_Xor(int x,int y,const char * BmpPath)
*Description: bmp位图文件在64K色下异或贴图
*Calls: // 被本函数调用的函数清单
*Input: x,y : 贴图左上角的坐标
*		BmpPath : 所贴图片的路径
*Output: 错误信息
*Return: 无
*Others: // 其它说明
*************************************************/
void Draw_Bmp64K_Xor(int x,int y,const char * BmpPath)
{
	_BmpPixel * buffer;
	FILE *fp;
	_BITMAPFileHeader BmpFH;
	_BITMAPInfoHeader BmpIH;
	long color;
	int LineBytes,i,j,ret;

	/*显存指针*/
	unsigned int far * const VideoBuffer = (unsigned int far *)0xA0000000L;		

	/*页面号*/
	unsigned char Page;		

	/*显存位置偏移量*/
	long Pos;	

	/*打开bmp文件*/
	fp = fopen(BmpPath, "rb");
	if (fp == NULL)
	{
		printf("Error Bmp Path!!!\n");
		delay(5000);
		exit(1);
	}
	rewind(fp);

	/*检测是否为bmp文件*/
	//fseek(fp,0,SEEK_SET);
	//ret = fread(&BmpFH.bfType,sizeof(BmpFH.bfType),1,fp);

	//暂时不检测是否为bmp图片。！！！！！ 严禁放入非法图片
	//if (BmpFH.bfType != "BM") 
	//{
	//	printf("The Picture is not BITMAP!!!\n");
		// delay(5000);
		// exit(1);
	// }

	/*检测是否为24位色图片*/
	fseek(fp,28L,SEEK_SET);
	fread(&BmpIH.biBitCount,2,1,fp);

	if (BmpIH.biBitCount != 24) 
	{
		printf("The BITMAP BitCount is not 24!!!\n");
		delay(5000);
		exit(1);
	}

	/*检测是否采用压缩算法*/
	fseek(fp,30L,SEEK_SET);
	fread(&BmpIH.biCompression,4,1,fp);

	if (BmpIH.biCompression != 0)
	{
		printf("The BITMAP has Compression!!!\n");
		delay(5000);
		exit(1);
	}

	/*检测图片大小是否超出屏幕范围*/
	fseek(fp,18L,SEEK_SET);
	fread(&BmpIH.biWidth,4,1,fp);
	fread(&BmpIH.biHeight,4,1,fp);


	if (BmpIH.biWidth > ModeInfo.XRes || BmpIH.biHeight > ModeInfo.YRes)
	{
		printf("The BITMAP is too big!!!\n");
		delay(5000);
		exit(1);
	}

	LineBytes = (BmpIH.biWidth * BmpIH.biBitCount / 8 + 3) / 4 * 4; // / 4 * 4---先整除4在乘以4
	
	/*开辟缓存一行像素的空间*/
	if ((buffer = (_BmpPixel*)malloc(LineBytes)) == 0)
	{
		printf("Error malloc!!!\n");
		delay(5000);
		exit(1);
	}

	/*移动文件指针至图片数据开始处*/
	fseek(fp,10L,SEEK_SET);
	fread(&BmpFH.bfOffBits,4,1,fp);
	fseek(fp,BmpFH.bfOffBits,SEEK_SET);

	for (j = BmpIH.biHeight - 1; j >= 0; j--)
	{
		//读取一行像素点的信息
		fread(buffer,LineBytes,1,fp);

		//处理一行像素点信息
		for (i = 0; i < BmpIH.biWidth; i++)
		{
			color =  rgbcolor64K(buffer[i].R,buffer[i].G,buffer[i].B);
			//if (color != 0x0000)
			//	PutPixel64K_Xor(i + x, j + y, color);

			/*判断是否超出屏幕*/
			if ((x + i) >= ModeInfo.XRes || (x + i) < 0 || (y + j) >= ModeInfo.YRes || (y + j) < 0)
				continue;		

			/*计算偏移量的大小*/
			Pos = (((long)(y + j) << 10) + (long)(x + i));	

			//Page = Pos >> 15; //32K个点为一页，代替除以32K算法。因为显存指针为char型。所以多除2		
			Set_ViewPage((y + j) >> 5);	

			if (color != 0x0000)
				VideoBuffer[Pos] ^= color;
		}
	}

	free(buffer);
	fclose(fp);
}

/*************************************************
*Function: void Draw_Line(int, int, int, int, int)
*Description: 64K色条件下，采用bresenham画线方法
*Calls: // 被本函数调用的函数清单
*Input: right,top,left,bottom 为直线的起点终点
*		color 为直线颜色
*Output: 错误信息
*Return: 无
*Others: // 其它说明
*************************************************/
void Draw_Line(int left, int top, int right, int bottom, int color)
{
	int d;
	int dx,dy,dx2,dy2;
	int xinc,yinc;
	int dxy;
	int half;
	dx = abs(right - left);
	dx2 = dx << 1;
	dy = abs(bottom - top);
	dy2 = dy << 1;
	xinc = (right > left) ? 1 : (right == left ? 0 : -1);
	yinc = (bottom > top) ? 1 : (bottom == top ? 0 : -1);

	PutPixel64K(left, top, color);
	if (dx >= dy)
	{
		d = dy2 - dx;
		dxy = dy2 - dx2;
		while (dx--)
		{
			if (d <= 0) d += dy2;
			else {
				d += dxy;
				top += yinc;
			}
			PutPixel64K(left += xinc, top, color);
		}
	} else {
		d = dx2 - dy;
		dxy = dx2 - dy2;
		while (dy--)
		{
			if (d <= 0) d += dx2;
			else {
				d += dxy;
				left += xinc;
			}
			PutPixel64K(left, top += yinc, color);
		}
	}
}

/*************************************************
*Function: void Draw_Circle(int, int, int, int)
*Description: 64K色条件下，采用bresenham画圆方法
*Calls: // 被本函数调用的函数清单
*Input: centerx,centery 为圆心
*		radius 为半径
*		color 为直线颜色
*Output: 错误信息
*Return: 无
*Others: // 其它说明
*************************************************/
void Draw_Circle(int centerx, int centery, int radius, int color)
{
	int x,y,d;
	if (!radius) return;
	y = radius;
	d = 3 - radius << 1;
	for (x = 0; x <= y; x++)
	{
		PutPixel64K(centerx + x,centery + y,color);
		PutPixel64K(centerx + x,centery - y,color);
		PutPixel64K(centerx - x,centery - y,color);
		PutPixel64K(centerx - x,centery + y,color);
		PutPixel64K(centerx + y,centery + x,color);
		PutPixel64K(centerx + y,centery - x,color);
		PutPixel64K(centerx - y,centery - x,color);
		PutPixel64K(centerx - y,centery + x,color);

		if (d < 0) d += x * 4 + 6;
		else d += (x - y--) * 4 + 10;
	}
}