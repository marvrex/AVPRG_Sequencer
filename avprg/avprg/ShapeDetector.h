#ifndef SHAPEDETECTOR_H
#define SHAPEDETECTOR_H

#include <opencv2\opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

class ShapeDetector {
public:
	ShapeDetector();
	~ShapeDetector();
	void getShapes(String name, Mat src, int thresh);
	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	bool isRectangleBackground(cv::Rect rectangle, Mat window);
};

#endif