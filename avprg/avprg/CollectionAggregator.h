#pragma once
#ifndef COLLECTIONAGGREGATOR_H
#define COLLECTIONAGGREGATOR_H

#include <opencv2\opencv.hpp>
#include "Object.h"

using namespace cv;
using namespace std;

class CollectionAggregator{

public:
	CollectionAggregator();
	~CollectionAggregator();
	void CollectionAggregator::append(std::list<Object> objects);
	std::list<Object> CollectionAggregator::retrieve();
private:
	std::list<Object> objects;
	bool newCycle;
	bool isDuplicate(Mat img, std::list<Object> objects);
};


#endif