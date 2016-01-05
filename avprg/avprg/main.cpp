#include <iostream>
#include <opencv2\opencv.hpp>

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

		
		while (true){
			Mat hsv;


			/*
			*BLUE
			*/
			Mat mask_blue;
			Mat blue(src.size().height, src.size().width, CV_8UC3, Scalar(255, 255, 255));			
			//convert to HSV
			cvtColor(src, hsv, CV_BGR2HSV);
			//define color range in HSV
			Scalar lowerBlue = Scalar(85, 100, 100);
			Scalar upperBlue = Scalar(130, 255, 255);
			//Threshold the HSV image to get a mask
			inRange(hsv, lowerBlue, upperBlue, mask_blue);
			//apply mask
			src.copyTo(blue, mask_blue);
			//call function
			drawShapeContours("blue", blue, thresh);

			/*
			*RED
			*/
			Mat mask_red;
			Mat mask_red1;
			Mat mask_red2;
			Mat red(src.size().height, src.size().width, CV_8UC3, Scalar(255, 255, 255));
			//convert to HSV
			cvtColor(src, hsv, CV_BGR2HSV);
			//define color range in HSV
			Scalar lowerRed1 = Scalar(0, 100, 100);
			Scalar upperRed1 = Scalar(10, 255, 255);
			Scalar lowerRed2 = Scalar(160, 100, 100);
			Scalar upperRed2 = Scalar(180, 255, 255);
			//Threshold the HSV image to get a mask
			inRange(hsv, lowerRed1, upperRed1, mask_red1);
			inRange(hsv, lowerRed2, upperRed2, mask_red2);
			bitwise_or(mask_red1, mask_red2, mask_red);
			//apply mask
			src.copyTo(red, mask_red);
			//call function
			drawShapeContours("red", red, thresh);

			/*
			*GREEN
			*/
			Mat mask_green;
			Mat green(src.size().height, src.size().width, CV_8UC3, Scalar(255, 255, 255));
			//convert to HSV
			cvtColor(src, hsv, CV_BGR2HSV);
			//define color range in HSV
			Scalar lowerGreen = Scalar(40, 100, 100);
			Scalar upperGreen = Scalar(84, 255, 255);
			//Threshold the HSV image to get a mask
			inRange(hsv, lowerGreen, upperGreen, mask_green);
			//apply mask
			src.copyTo(green, mask_green);
			//call function
			drawShapeContours("green", green, thresh);




			if (waitKey(50) == 27){
				break;
			}
		}
			waitKey(0);	
	}

	return 0;
}

void drawShapeContours(String name, Mat src, int thresh) {
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

	//show contoursImg
	//imshow("contours", contoursImg);

	//approximate contours and write them to result
	Mat result = src.clone();

	//approx will contain the vertices of our objects
	vector<cv::Point> approx;
	int triangleCount = 0;
	int rectCount = 0;
	int circleCount = 0;

	for (int i = 0; i < contours.size(); i++) {
		cv::approxPolyDP(cv::Mat(contours[i]),approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02,true);

		//skip small and non-convex shapes
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx) )
			continue;

		//Triangle
		if (approx.size() == 3) {
			setLabel(result, "TRI", contours[i]);
			triangleCount++;
		}
		//rectangle and hexagon
		else if (approx.size() >= 4 && approx.size() <= 6) {
			//number ov vertices
			int vertices = approx.size();

			//get degree of all corners
			std::vector<double> cos;
			for (int j = 2; j < vertices+1; j++) {
				cos.push_back(angle(approx[j%vertices], approx[j-2], approx[j-1]));
			}
			
			//sort ascending the angles
			std::sort(cos.begin(), cos.end());

			//lowest and highest angle?
			double minCos = cos.front();
			double maxCos = cos.back();

			if (vertices == 4 && minCos >= -0.1 && maxCos <= 0.3) {
				cv::Rect rectangle = cv::boundingRect(contours[i]);
				double ratio = std::abs(1 - (double) rectangle.width / rectangle.height);

				//shape detection will recognize the white background as a rectangle, we want to remove it from our collection of shapes
				if (isRectangleBackground(rectangle, result))
					continue;

				//write our rectangle
				setLabel(result,"RECT", contours[i]);
				rectCount++;
			} else if (vertices == 5 && minCos >= -0.34 && maxCos <= -0.27)
				setLabel(result, "PENTA", contours[i]);
			else if (vertices == 6 && minCos >= -0.55 && maxCos <= -0.45)
				setLabel(result, "HEXA", contours[i]);
		}
		else {
			//if the shape isnt a triangle, rectangle or hexagon, it should be a circle
			double area = cv::contourArea(contours[i]);
			cv::Rect rect = cv:: boundingRect(contours[i]);
			int radius = rect.width / 2;

			
		 if (std::abs(1 - ((double)rect.width / rect.height)) <= 0.2 &&
			 std::abs(1 - (area / (CV_PI * std::pow((float) radius, (float)2)))) <= 0.2) {
				setLabel(result, "CIRCLE", contours[i]);
				circleCount++;
			}
		}

	//end of loop
	}
	imshow(name, result);
}

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
 * Helper function to display text in the center of a contour
 */
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

/*
 * This is a bit of a hack:
 * the white background will always be 2 pixels smaller in height and width.
 */
bool isRectangleBackground(cv::Rect rectangle, Mat window) {
	Size rectSize = rectangle.size();
	Size windowSize = window.size();
	windowSize.height -= 2;
	windowSize.width -= 2;

	return rectSize == windowSize;
}

