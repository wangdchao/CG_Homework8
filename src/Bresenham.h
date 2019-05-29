#pragma once
#include "allheader.h"
using namespace std;
/*
	采用Bresenham画直线
	（x1,y1）与（x2,y2）两个点确定一条直线；
	vertex存储计算过程中计算出的属于直线上的点。
*/
static void drawLineWithBresenham(const int x1, const int y1, const int x2, const int y2, vector<GLdouble> &vertex) {
	GLdouble dxy;							//斜率
	int minx, miny, maxx, maxy;			//（minx,miny）存储两个点中x较小的点，（maxx,maxy）存储x较大的点
	int dx, dy, dyy;					//dx:x方向上的绝对值差,dy：y方向上的绝对值差，dyy：依据斜率正负确定y分量+1还是-1	
	int rx, ry, rxx, ryy;				//在Bresenham算法循环中实际叠加的x与y。
	int p0;								//p0用于判断基于当前的点，下一个点的选取
	bool high = false;					//true：则直线的斜率绝对值大于1，否则0<=&&>=1
	if (x1 == x2) dxy = 2;
	else dxy = (GLdouble)(y1 - y2) / (GLdouble)(x1 - x2);

	//判断传入两个点中的x值较小的点
	if (x1 <= x2) {
		minx = x1; miny = y1;
		maxx = x2; maxy = y2;
	}
	else {
		minx = x2; miny = y2;
		maxx = x1; maxy = y1;
	}

	//斜率大于1，则将直线关于45度直线为对称轴做翻折，即将x、y的值互换
	if (fabs(dxy) > 1) {
		high = true;
		dx = (int)fabs(y1 - y2);
		dy = (int)fabs(x1 - x2);
		rx = miny; ry = minx;
		rxx = maxy; ryy = maxx;
		//如果斜率绝对值大于1且斜率小于0，则做翻折后需要将x较小的值互换
		if (dxy < 0) {
			rx = maxy; ry = maxx;
			rxx = miny; ryy = minx;
		}
	}
	else {
		dy = (int)fabs(y1 - y2);
		dx = (int)fabs(x1 - x2);
		rx = minx; ry = miny;
		ryy = maxy; rxx = maxx;
	}

	//初始化p0
	p0 = 2 * dy - dx;
	//判定ry在循环过程中是向上增加还是向下减少
	dyy = dxy >= 0 ? 1 : -1;
	GLdouble a = 0.0;
	//Bresenham循环迭代
	while (rx != rxx+1) {
		//如果high=true，说明经过了45度直线的翻折，需要将结果y值压入顶点的x通道，将结果x值压入顶点的y通道
		if (high) {
			vertex.push_back((GLdouble)ry);//点缩放到 - 1.0 - 1.0之间
			vertex.push_back((GLdouble)rx);
			vertex.push_back(a);
		}
		else {
			
			vertex.push_back((GLdouble)rx);
			vertex.push_back((GLdouble)ry);
			vertex.push_back(a);
		}
		if (p0 >= 0) {
			p0 += 2 * dy - 2*dx;
			rx++; ry += dyy;
		}
		else {
			p0 += 2 * dy;
			rx++;
		}
	}
}

static double returnCin(unsigned int n, unsigned int i) {
	unsigned int dev = n - i;
	if (n == 0) n = 1;
	else {
		for (unsigned int d = n - 1; d >= 1; d--) {
			n *= d;
		}
	}
	if (i == 0) i = 1;
	else {
		for (unsigned int d = i - 1; d >= 1; d--) {
			i *= d;
		}
	}
	if (dev == 0) dev = 1;
	else {
		for (unsigned int d = dev - 1; d >= 1; d--) {
			dev *= d;
		}
	}
	return (double)n / ((double)i*(double)dev);
}

/*
	采用的Bresenham算法画圆
	x，y：原点；
	radiu：半径；
	vertex：存储计算得出的圆上的顶点
*/
/*static void drawCircleWithBresenham(const int x, const int y, const int radius,vector<GLdouble> &vertex) {
	//用来依据一个点计算出其他几个关于该点对称的点
	const int dir[4][2] = {
		1,1,-1,1,-1,-1,1,-1
	};
	//初始化第一个点为（0，半径），并出示化p0.
	int x0 = 0, y0 = radius;
	int p0 = 3 - 2 * radius;
	while (x0 <= y0) {
		int x1 = y0, y1 = x0;
		for (int i = 0; i < 4; i++) {
			float tempx = (GLdouble)(x0 * dir[i][0] + x) / MAXWIDTH;
			float tempy = (GLdouble)(y0 * dir[i][1] + y) / MAXWIDTH;
			vertex.push_back(tempx);
			vertex.push_back(tempy);
			vertex.push_back(0.0f);
			tempx = (GLdouble)(x1 * dir[i][0] + x) / MAXWIDTH;
			tempy = (GLdouble)(y1 * dir[i][1] + y) / MAXWIDTH;
			vertex.push_back(tempx);
			vertex.push_back(tempy);
			vertex.push_back(0.0f);
		}
		if (p0 >= 0) {
			p0 += 4 * (x0 - y0) + 10;
			y0--;
		}
		else {
			p0 += 4 * x0 + 6;
		}
		x0++;
	}
}*/


/*
	采用一条自上而下的扫描线检测三角形两条边同一水平线上的内点->扫描线与三角形两条边的两交点之间的点，
	扫描到的所有三角形中的点存到向量verttex中。
*/
/*static void drawTriangleWithFilling(const int (*triangle)[2],vector<float> &vertex) {
	//记录三角形从上到下的三个顶点
	float topx, middlex, bottomx;
	float topy, middley, bottomy;
	//记录从上到下三个顶点在传入3*2的矩阵中的位置
	int index1, index2, index3;
	bool equal1 = false, equal2 = false, equal3 = false;
	//三条直线的斜率的倒数
	float k1, k2, k3;

	//找出三个顶点中y分量最大的点
	index1 = triangle[0][1] >= triangle[(triangle[1][1] >= triangle[2][1] ? 1 : 2)][1] ? 0 : (triangle[1][1] >= triangle[2][1] ? 1 : 2);
	//找出三个顶点中y分量最小的点
	index3 = triangle[0][1] < triangle[(triangle[1][1] < triangle[2][1] ? 1 : 2)][1] ? 0 : (triangle[1][1] < triangle[2][1] ? 1 : 2);
	index2 = 3 - index1 - index3;

	topx = (float)triangle[index1][0]; topy = (float)triangle[index1][1];
	middlex = (float)triangle[index2][0]; middley = (float)triangle[index2][1];
	bottomx = (float)triangle[index3][0]; bottomy = (float)triangle[index3][1];

	//判断三条直线中是否有垂直的直线
	if (bottomy == topy) equal2 = true;
	else k2 = (bottomx - topx) / (bottomy - topy);
	if(middley==topy) equal1=true;
	else k1 = (middlex - topx) / (middley - topy);
	if (middley == bottomy) equal3 = true;
	else k3 = (bottomx - middlex) / (bottomy - middley);

	//以水平扫描线自上而下扫描出相应的三角形内的点
	for(int i=topy-1;i>bottomy;i--) {
		float vx1, vx2;
		int vx11, vx22;
		if(i>middley) {
			vx1 = k1 * (i - topy) + topx;
			vx2 = k2 * (i - topy) + topx;
		}
		else {
			vx1 = k2 * (i - topy) + topx;
			vx2 = k3 * (i - bottomy) + bottomx;
		}
		if(vx1<=vx2) {
			vx11 = ceil(vx1), vx22 = floor(vx2);
		}
		else {
			vx11 = ceil(vx2), vx22 = floor(vx1);
		}

		while(vx11<=vx22) {
			vertex.push_back((float)vx11 / MAXWIDTH);
			vertex.push_back((float)i / MAXWIDTH);
			vertex.push_back(0.0f);
			vx11++;
		}
	}
}*/

/*

return n!/(i!(n-i)!)

*/


