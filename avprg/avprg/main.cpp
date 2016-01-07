#include <iostream>
#include <opencv2\opencv.hpp>
#include "ColorSplitter.h"
#include "ShapeDetector.h"
#include "CollectionAggregator.h"
#include "Object.h"

using namespace cv;
using namespace std;

int verbinden(vector<vector<Point> > shapes);
int thresh = 150;
int max_thresh = 255;

//debug code
String imageDirectory = "C:\\dev\\git\\AVPRG_Sequencer\\avprg\\test3.jpg";
RNG rng(120);

int main(){

	bool isImage = true;


	VideoCapture cap(0);
	Mat frame;
	namedWindow("src", WINDOW_AUTOSIZE);


	if (!isImage){
		for (;;){
			//Load frame
			cap >> frame;

			//break
			if (waitKey(30) >= 0) break;
		}
	}
	else{
		//debug section
		Mat src = imread(imageDirectory, 1);   // Read the file

		if (!src.data)                              // Check for invalid input
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;
		}

		createTrackbar("Canny thresh:", "src", &thresh, max_thresh);
		imshow("src", src);

		ColorSplitter colorSplitter;
		ShapeDetector shapeDetector;
		CollectionAggregator aggregator;
		std::list<Object> objects;

		while (true){
			Mat blue = colorSplitter.getImageChannel(src, "blue");
			objects = shapeDetector.getShapes("blue", blue, thresh);
			aggregator.append(objects);

			Mat red = colorSplitter.getImageChannel(src, "red");
			objects = shapeDetector.getShapes("red", red, thresh);
			aggregator.append(objects);

			Mat green = colorSplitter.getImageChannel(src, "green");
			objects = shapeDetector.getShapes("green", green, thresh);
			aggregator.append(objects);

			cout << aggregator.retrieve().size() << endl;
			objects.clear();
			if (waitKey(50) == 27){
				break;
			}
		}
			waitKey(0);	
	}

	return 0;
}