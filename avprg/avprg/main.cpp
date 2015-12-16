#include <iostream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

void getShapes(Mat src, int thresh);
int verbinden(vector<vector<Point> > shapes);
int thresh = 150;
int max_thresh = 255;

int main(){

	bool isImage = true;


	VideoCapture cap(0);
	Mat frame;
	namedWindow("src", WINDOW_AUTOSIZE);


	if (!isImage){
		for (;;){



			//Load frame
			cap >> frame;


			//getShapes(frame);



			if (waitKey(30) >= 0) break;
		}
	}
	else{

		Mat src = imread("C:\\Users\\Timmi\\Documents\\Visual Studio 2013\\Projects\\avprg\\test.jpg", 1);   // Read the file

		if (!src.data)                              // Check for invalid input
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;
		}


		imshow("src", src);
		createTrackbar(" Canny thresh:", "src", &thresh, max_thresh);

		
		//while (true){
			getShapes(src, thresh);
		//	if (waitKey(50) == 27){
		//		break;
		//	}
		//}
			waitKey(0);



		

		
	}

	return 0;
}



void getShapes(Mat src, int thresh){

	namedWindow("result", WINDOW_AUTOSIZE);
	namedWindow("contours_drawing", WINDOW_AUTOSIZE);



	Mat gray, src_copy;
	Mat result(src.rows, src.cols, CV_8UC1, Scalar(0, 0, 0));
	Mat contours_drawing(src.rows, src.cols, CV_8UC1, Scalar(0, 0, 0));

	//Convert src to GRAY
	cvtColor(src, gray, CV_BGR2GRAY);


	threshold(gray, gray, 245, 255, 0);
	GaussianBlur(gray, gray, Size(9, 9), 2, 2);


	Mat canny_output;
	vector<vector<Point> > contours;
	vector<vector<Point> > shapes;
	vector<Vec4i> hierarchy;


	// Detect edges using canny
	Canny(gray, canny_output, thresh, thresh*2, 3);

	//dilate um nur eine linie zu bekommen
	dilate(canny_output, canny_output, Mat(), Point(-1, -1), 1); 
	erode(canny_output, canny_output, Mat(), Point(-1, -1), 1);

	

	// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++){
		drawContours(contours_drawing, contours, i, Scalar(125, 125, 125), 1, 8, hierarchy, 0, Point());
	}
	imshow("contours_drawing", contours_drawing);


	shapes.resize(contours.size());
	int anzahlQuadrate = 0;
	int anzahlDreiecke = 0;


	//gehe alle konturen durch
	for (size_t i = 0; i < contours.size(); i++){

		//nimm aus der kontur alle punkte und mache daraus eine figur
		approxPolyDP(contours[i], shapes[i], 1, true);


		//zeichnen der figur
		cout << "Figur gefunden mit " + to_string(shapes[i].size()) + " Ecken.";
		cout << "\n";
		for (int j = 0; j < shapes[i].size(); j++){
			if (j == (shapes[i].size() - 1)){
				line(result, shapes[i][j], shapes[i][0], Scalar(255, 255, 255), 1, 8, 0);
			}
			else{
				line(result, shapes[i][j], shapes[i][j + 1], Scalar(255, 255, 255), 1, 8, 0);
			}
		}
	}


	
	imshow("result", result);
	

}