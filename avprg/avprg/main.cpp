#include <iostream>
#include <opencv2\opencv.hpp>
#include "ColorSplitter.h"
#include "ShapeDetector.h"
#include "CollectionAggregator.h"
#include "Object.h"
#include <time.h>

using namespace cv;
using namespace std;

int verbinden(vector<vector<Point> > shapes);
int thresh = 150;
int max_thresh = 255;
long t = time(0);
ColorSplitter colorSplitter;
ShapeDetector shapeDetector;
CollectionAggregator aggregator;
std::list<Object> objects;
std::list<Object> result;
Mat src;


void debugOutput(CollectionAggregator);
void detect();


String imageDirectory = "C:\\Users\\Timmi\\Documents\\Visual Studio 2013\\Projects\\avprg\\test3.jpg";
bool isImage = false;
RNG rng(120);

int main(){

	VideoCapture cap(0);
	namedWindow("src", WINDOW_AUTOSIZE);

	//createTrackbar("Canny thresh:", "src", &thresh, max_thresh);


	if (!isImage){
		/*
		*VIDEO
		*/
		for (;;){
			//Load frame
			cap >> src;
			//detect shapes
			detect();

			if (waitKey(30) == 27) break;
		}
	}
	else{
		/*
		*IMAGE
		*/

		// Read the file
		src = imread(imageDirectory, 1);

		// Check for invalid input
		if (!src.data)
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;
		}


		while (true){
			//detect shapes
			detect();

			if (waitKey(50) == 27) break;
		}
	}

	return 0;
}



void detect(){
	Mat blue = colorSplitter.getImageChannel(src, "blue");
	objects = shapeDetector.getShapes("blue", blue, thresh);
	aggregator.append(objects);

	Mat red = colorSplitter.getImageChannel(src, "red");
	objects = shapeDetector.getShapes("red", red, thresh);
	aggregator.append(objects);

	Mat green = colorSplitter.getImageChannel(src, "green");
	objects = shapeDetector.getShapes("green", green, thresh);
	aggregator.append(objects);

	//Console output
	debugOutput(aggregator);


	aggregator.setNewCycle();


	imshow("src", src);
}


//Console output
void debugOutput(CollectionAggregator aggregator){

	//time in seconds to reload
	if (time(0) > t + 1){


		cout << "----------------FOUND OBJECTS----------------" << endl;
		result = aggregator.retrieve();

		list<Object>::iterator iter = result.begin();

		while (iter != result.end()) {
			Object& temp = *iter;
			cout << temp.getColor();
			cout << " ";
			cout << temp.getName();
			cout << " at ";
			int posX = temp.getPosition().x;
			int posY = temp.getPosition().y;
			cout << to_string(posX);
			cout << ",";
			cout << to_string(posY) << endl;
			iter++;
		}

		result.clear();
		t = time(0);
	}
}