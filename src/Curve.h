#pragma once
#ifndef  CURVE_H
#define  CURVE_H
#include "allheader.h"
#include "Bresenham.h"

typedef struct node {
	GLdouble x, y;
	bool last;
	node(GLdouble X = 0.0f, GLdouble Y = 0.0f, bool l = false) :x(X), y(Y), last(l) {};
}Point;

class Bezier {
private:
	std::vector<GLdouble> Line;
	std::vector<GLdouble> AN;
	std::vector<GLdouble> Curve;
	void updateLine();
	void updatePoint(const float t);
public:
	std::vector<Point> P;
	std::vector<Point> Animation;
	unsigned int size;
	Bezier();
	bool hasExistedPoint(Point p);
	void addPoint(Point p);
	void deleteLastPoint();
	std::vector<GLdouble> &getLine();
	void getBezierCurve();
	bool isLast(Point p);
	void CalculateAnimationData(const float t);
	vector<GLdouble> &getCurve(){
		return Curve;
	}
	vector<GLdouble> &getAN() {
		return AN;
	}
};





#endif // ! CURVE_H

