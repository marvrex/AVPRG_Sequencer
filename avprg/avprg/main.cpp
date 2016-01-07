#include <iostream>
#include <opencv2\opencv.hpp>
#include "ColorSplitter.h"
#include "ShapeDetector.h"

using namespace cv;
using namespace std;

void getShapes(Mat src, int thresh);
void drawShapeContours(String name, Mat src, int thresh);
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
bool isRectangleBackground(cv::Rect rectangle, Mat window);

int verbinden(vector<vector<Point> > shapes);
int thresh = 150;
int max_thresh = 255;

//debug code
String imageDirectory = "C:\\Users\\Timmi\\Documents\\Visual Studio 2013\\Projects\\avprg\\test3.jpg";
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

			//drawShapeContours(frame, thresh);

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

		while (true){
			Mat blue = colorSplitter.getImageChannel(src, "blue");
			shapeDetector.getShapes("blue", blue, thresh);

			Mat red = colorSplitter.getImageChannel(src, "red");
			shapeDetector.getShapes("red", red, thresh);

			Mat green = colorSplitter.getImageChannel(src, "green");
			shapeDetector.getShapes("green", green, thresh);

			if (waitKey(50) == 27){
				break;
			}
		}
			waitKey(0);	
	}

	return 0;
}