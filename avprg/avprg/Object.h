#ifndef OBJECT_H
#define OBJECT_H

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Object{

public:
	Object(string name, string color, Point position);
	
	string Object::getName();
	string Object::getColor();
	Point Object::getPosition();


private:
	string name;
	string color;
	Point position;
};


#endif