#include "Curve.h"

Bezier::Bezier() {
	size = 0;
}

//检测点是否已经在vector中存在
bool Bezier::hasExistedPoint(Point p) {
	printf("check existence\n");
	for (unsigned int i = 0; i < size; i++) {
		double dis = fabs((P[i]).x - p.x);
		double dis1 =fabs((P[i]).y - p.y);
		if (dis < DIS && dis1 < DIS) return true;
		else continue;
	}
	return false;
}

//向P中添加新的点
void Bezier::addPoint(Point p) {
	printf("add point trying\n");
	if (size < P.size()) {
		(P[size]).x = p.x;
		(P[size]).y = p.y;
		(P[size]).last = p.last;
		if (size > 0) (P[size - 1]).last = false;
	}
	else if (size == 0) P.push_back(Point(p.x, p.y, p.last));
	else {
		(P[size - 1]).last = false;
		P.push_back(Point(p.x, p.y, p.last));
	}
	size++;
	printf("size++:%d\n", size);
}

//删除最后一个添加的点
void Bezier::deleteLastPoint() {
	if (size > 1) {
		size--;
		(P[size-1]).last = true;
	}
	else if (size == 1) size=0;
	else size = 0;
	printf("size--:%d\n", size);
}

//返回有点计算得到的顶点
std::vector<GLdouble> &Bezier::getLine() {
	updateLine();
	return this->Line;
}

//更新直线的顶点数据
void Bezier::updateLine() {
	Line.clear();
	if (size == 0) return;
	for (unsigned int i = 0; i < size - 1; i++) {
		int x1=(int)((P[i]).x+0.5),y1= (int)((P[i]).y + 0.5);
		int x2 = (int)((P[i + 1]).x + 0.5), y2 = (int)((P[i + 1]).y + 0.5);
		drawLineWithBresenham(x1,y1,x2,y2,Line);
	}
}

//计算Bezier曲线的顶点
void Bezier::getBezierCurve() {
	Curve.clear();
	float dt = 1 / (float)DOTNUM;
	if (size <= 1) return;
	unsigned int n = size - 1;
	for (float t = 0; t <= 1; t += dt) {
		GLdouble xx = 0, yy = 0;
		for (unsigned int i = 0; i <= n; i++) {
			double fre = returnCin(n, i);
			fre *= pow(t, i);
			fre *= pow(1 - t, n - i);
			xx += (P[i]).x*fre;
			yy += (P[i]).y*fre;
		}
		Curve.push_back(xx);
		Curve.push_back(yy);
		Curve.push_back((GLdouble)0.0);
	}
}

//判断一个顶点是否在顶点数组的最后一位
bool Bezier::isLast(Point p) {
	Point pp = P[size - 1];
	if (fabs(pp.x - p.x) <= DIS && fabs(pp.y - p.y) <= DIS) {
		return pp.last;
	}
	else return false;
}

//计算动画数据
void Bezier::CalculateAnimationData(const float t) {
	updatePoint(t);
}

//计算随t变化的动态点。
void Bezier::updatePoint(const float t) {
	//printf("updatePoint\n");
	//vector<Point> p;
	Animation.clear();
	int allsize = size;
	int step = 0;
	bool st = true, sst = true;
	if (allsize - 1 < 1) return;
	else {
		while (allsize >= 1) {
			if (st) {
				for (int i = 0; i < allsize - 1; i++) {
					GLdouble x1 = (P[i]).x , y1 = (P[i]).y ;
					GLdouble x2 = (P[i + 1]).x , y2 = (P[i + 1]).y;
					GLdouble posx = x1 + (x2 - x1)*t, posy = y1 + (y2 - y1)*t;
					Animation.push_back(Point(posx, posy, false));
				}
				st = false;
			}
			else {
				if (sst) sst = false;
				else step++;
				for (int i = 0; i < allsize - 1; i++) {
					GLdouble x1 = (Animation[step]).x, y1 = (Animation[step]).y;
					GLdouble x2 = (Animation[step + 1]).x , y2 = (Animation[step + 1]).y;
					GLdouble posx = x1 + (x2 - x1)*t, posy = y1 + (y2 - y1)*t;
					Animation.push_back(Point(posx, posy, false));
					step++;
				}
			}
			allsize--;
		}
	}
	//printf("updatePoint over\n");
}