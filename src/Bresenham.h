#pragma once
#include "allheader.h"
using namespace std;
/*
	����Bresenham��ֱ��
	��x1,y1���루x2,y2��������ȷ��һ��ֱ�ߣ�
	vertex�洢��������м����������ֱ���ϵĵ㡣
*/
static void drawLineWithBresenham(const int x1, const int y1, const int x2, const int y2, vector<GLdouble> &vertex) {
	GLdouble dxy;							//б��
	int minx, miny, maxx, maxy;			//��minx,miny���洢��������x��С�ĵ㣬��maxx,maxy���洢x�ϴ�ĵ�
	int dx, dy, dyy;					//dx:x�����ϵľ���ֵ��,dy��y�����ϵľ���ֵ�dyy������б������ȷ��y����+1����-1	
	int rx, ry, rxx, ryy;				//��Bresenham�㷨ѭ����ʵ�ʵ��ӵ�x��y��
	int p0;								//p0�����жϻ��ڵ�ǰ�ĵ㣬��һ�����ѡȡ
	bool high = false;					//true����ֱ�ߵ�б�ʾ���ֵ����1������0<=&&>=1
	if (x1 == x2) dxy = 2;
	else dxy = (GLdouble)(y1 - y2) / (GLdouble)(x1 - x2);

	//�жϴ����������е�xֵ��С�ĵ�
	if (x1 <= x2) {
		minx = x1; miny = y1;
		maxx = x2; maxy = y2;
	}
	else {
		minx = x2; miny = y2;
		maxx = x1; maxy = y1;
	}

	//б�ʴ���1����ֱ�߹���45��ֱ��Ϊ�Գ��������ۣ�����x��y��ֵ����
	if (fabs(dxy) > 1) {
		high = true;
		dx = (int)fabs(y1 - y2);
		dy = (int)fabs(x1 - x2);
		rx = miny; ry = minx;
		rxx = maxy; ryy = maxx;
		//���б�ʾ���ֵ����1��б��С��0���������ۺ���Ҫ��x��С��ֵ����
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

	//��ʼ��p0
	p0 = 2 * dy - dx;
	//�ж�ry��ѭ�����������������ӻ������¼���
	dyy = dxy >= 0 ? 1 : -1;
	GLdouble a = 0.0;
	//Bresenhamѭ������
	while (rx != rxx+1) {
		//���high=true��˵��������45��ֱ�ߵķ��ۣ���Ҫ�����yֵѹ�붥���xͨ���������xֵѹ�붥���yͨ��
		if (high) {
			vertex.push_back((GLdouble)ry);//�����ŵ� - 1.0 - 1.0֮��
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
	���õ�Bresenham�㷨��Բ
	x��y��ԭ�㣻
	radiu���뾶��
	vertex���洢����ó���Բ�ϵĶ���
*/
/*static void drawCircleWithBresenham(const int x, const int y, const int radius,vector<GLdouble> &vertex) {
	//��������һ�������������������ڸõ�ԳƵĵ�
	const int dir[4][2] = {
		1,1,-1,1,-1,-1,1,-1
	};
	//��ʼ����һ����Ϊ��0���뾶��������ʾ��p0.
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
	����һ�����϶��µ�ɨ���߼��������������ͬһˮƽ���ϵ��ڵ�->ɨ�����������������ߵ�������֮��ĵ㣬
	ɨ�赽�������������еĵ�浽����verttex�С�
*/
/*static void drawTriangleWithFilling(const int (*triangle)[2],vector<float> &vertex) {
	//��¼�����δ��ϵ��µ���������
	float topx, middlex, bottomx;
	float topy, middley, bottomy;
	//��¼���ϵ������������ڴ���3*2�ľ����е�λ��
	int index1, index2, index3;
	bool equal1 = false, equal2 = false, equal3 = false;
	//����ֱ�ߵ�б�ʵĵ���
	float k1, k2, k3;

	//�ҳ�����������y�������ĵ�
	index1 = triangle[0][1] >= triangle[(triangle[1][1] >= triangle[2][1] ? 1 : 2)][1] ? 0 : (triangle[1][1] >= triangle[2][1] ? 1 : 2);
	//�ҳ�����������y������С�ĵ�
	index3 = triangle[0][1] < triangle[(triangle[1][1] < triangle[2][1] ? 1 : 2)][1] ? 0 : (triangle[1][1] < triangle[2][1] ? 1 : 2);
	index2 = 3 - index1 - index3;

	topx = (float)triangle[index1][0]; topy = (float)triangle[index1][1];
	middlex = (float)triangle[index2][0]; middley = (float)triangle[index2][1];
	bottomx = (float)triangle[index3][0]; bottomy = (float)triangle[index3][1];

	//�ж�����ֱ�����Ƿ��д�ֱ��ֱ��
	if (bottomy == topy) equal2 = true;
	else k2 = (bottomx - topx) / (bottomy - topy);
	if(middley==topy) equal1=true;
	else k1 = (middlex - topx) / (middley - topy);
	if (middley == bottomy) equal3 = true;
	else k3 = (bottomx - middlex) / (bottomy - middley);

	//��ˮƽɨ�������϶���ɨ�����Ӧ���������ڵĵ�
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


