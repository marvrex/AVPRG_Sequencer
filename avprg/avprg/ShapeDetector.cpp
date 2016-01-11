#include <math.h>
#include <opencv2\opencv.hpp>
#include "ShapeDetector.h"
#include "Object.h"
#include <list>  

using namespace cv;
using namespace std;

void getShapes(Mat src, int thresh);
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
void removeDuplicates(std::list<Object>);
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
bool isRectangleBackground(cv::Rect rectangle, Mat window);

ShapeDetector::ShapeDetector()
{};

ShapeDetector::~ShapeDetector()
{};

std::list<Object> ShapeDetector::getShapes(String color, Mat src, int thresh) {
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	//convert to gray, blur and apply a threshold

	GaussianBlur(gray, gray, Size(9, 9), 2, 2);
	threshold(gray, gray, thresh, 255, CV_THRESH_BINARY);

	//detect edges
	Mat edges = gray.clone();
	Canny(gray, edges, 100, 200, 3, true);

	//opening um nur eine linie zu bekommen
	dilate(edges, edges, Mat(), Point(-1, -1), 1);
	erode(edges, edges, Mat(), Point(-1, -1), 1);

	//contours and their hierarchy
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	//find contours and draw them to a third image (contoursImg)
	findContours(gray, contours, hierarchy, RETR_LIST, CV_CHAIN_APPROX_NONE);
	Mat contoursImg = Mat::zeros(src.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(125, 125, 125);
		drawContours(contoursImg, contours, i, Scalar(125, 125, 125), 1, 8, hierarchy, 0, Point());
	}

	//approximate contours and write them to result
	Mat result = src.clone();

	//approx will contain the vertices of our objects
	vector<cv::Point> approx;

	objects.clear();


	for (int i = 0; i < contours.size(); i++) {
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

		//skip small and non-convex shapes
		if (std::fabs(cv::contourArea(contours[i])) < 1000 || !cv::isContourConvex(approx))
			continue;

		//Triangle
		if (approx.size() == 3) {
			setLabel(result, "TRI", contours[i]);
			cv::Point position = getPosition(contours[i]);
			Object o("TRI", color, position);
			objects.push_back(o);

		}
		//rectangle and hexagon
		else if (approx.size() >= 4 && approx.size() <= 6) {
			//number ov vertices
			int vertices = approx.size();

			//get degree of all corners
			std::vector<double> cos;
			for (int j = 2; j < vertices + 1; j++) {
				cos.push_back(angle(approx[j%vertices], approx[j - 2], approx[j - 1]));
			}

			//sort ascending the angles
			std::sort(cos.begin(), cos.end());

			//lowest and highest angle?
			double minCos = cos.front();
			double maxCos = cos.back();

			if (vertices == 4 && minCos >= -0.1 && maxCos <= 0.3) {
				cv::Rect rectangle = cv::boundingRect(contours[i]);
				double ratio = std::abs(1 - (double)rectangle.width / rectangle.height);

				//shape detection will recognize the white background as a rectangle, we want to remove it from our collection of shapes
				if (isRectangleBackground(rectangle, result))
					continue;

				//write our rectangle
				setLabel(result, "RECT", contours[i]);
				cv::Point position = getPosition(contours[i]);
				Object o("RECT", color, position);
				objects.push_back(o);
			}
			else if (vertices == 5 && minCos >= -0.34 && maxCos <= -0.27){
				setLabel(result, "PENTA", contours[i]);
				cv::Point position = getPosition(contours[i]);
				Object o("PENTA", color, position);
				objects.push_back(o);
			}
			else if (vertices == 6 && minCos >= -0.55 && maxCos <= -0.45){
				setLabel(result, "HEXA", contours[i]);
				cv::Point position = getPosition(contours[i]);
				Object o("HEXA", color, position);
				objects.push_back(o);
			}
		}
		else {
			//if the shape isnt a triangle, rectangle or hexagon, it should be a circle
			double area = cv::contourArea(contours[i]);
			cv::Rect rect = cv::boundingRect(contours[i]);
			int radius = rect.width / 2;

			if (std::abs(1 - ((double)rect.width / rect.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * std::pow((float)radius, (float)2)))) <= 0.2) {
				setLabel(result, "CIRCLE", contours[i]);
				cv::Point position = getPosition(contours[i]);
				Object o("CIRCLE", color, position);
				objects.push_back(o);
			}
		}

		//end of loop
	}
	imshow(color, result);

	return objects;
}


/*
* get centered position
*/
Point ShapeDetector::getPosition(vector<Point> contour){
	cv::Rect r = cv::boundingRect(contour);
	cv::Point pt(r.x + (r.width / 2), r.y + (r.height / 2));
	return pt;
}



/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
double ShapeDetector::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
 * Helper function to display text in the center of a contour
 */
void ShapeDetector::setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}


//not needed anymore?
bool ShapeDetector::isDuplicate(std::list<Object> objects, cv::Point newPosition) {
	int tempX;
	int tempY;

	//allocate iterator
	list<Object>::iterator iter = objects.begin();
	while (iter != objects.end()) {
		Object& temp = *iter;
		tempX = temp.getPosition().x;
		tempY = temp.getPosition().y;

		if (newPosition.x == tempX &&
			newPosition.y == tempY)
			return true;
	}

	return false;
}

/*
 * DEPRECATED:
 * This is a bit of a hack:
 * the white background will always be 2 pixels smaller in height and width.
 */
bool ShapeDetector::isRectangleBackground(cv::Rect rectangle, Mat window) {
	Size rectSize = rectangle.size();
	Size windowSize = window.size();
	windowSize.height -= 2;
	windowSize.width -= 2;

	return rectSize == windowSize;
}