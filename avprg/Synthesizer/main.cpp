#include <iostream>
#include <opencv2\opencv.hpp>
#include <time.h>
#include "soundobject.h"
#include <stdlib.h>
#include "node.h"
#include "iostream"
#include <comdef.h>
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
long t2 = time(0);
ColorSplitter colorSplitter;
ShapeDetector shapeDetector;
CollectionAggregator aggregator;
std::list<Object> objects;
std::list<Object> result;
Mat src;



void debugOutput(CollectionAggregator);
void detect();
void playSound();
int colorToVolume(String);
int shapeToInstrument(String);
int i = 0;
int highest_channel = -1;
std::list<Object> sounds;
SoundObject *o;


String imageDirectory = "C:\\Users\\Timmi\\Desktop\\AVPRG_Sequencer\\test3.jpg";
bool isImage = false;
RNG rng(120);



int main(int argc, char *argv[])
{

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	
	VideoCapture cap(0);
	namedWindow("src", WINDOW_AUTOSIZE);
	namedWindow("test", WINDOW_AUTOSIZE);
	Mat test(300, 300, CV_8UC3, Scalar(0));

	o = new SoundObject();
	
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
	
}






int colorToVolume(String color){
	if (color.compare("RED") == 0){
		return 20;
	}
	else if (color.compare("BLUE") == 0){
		return 40;

	}
	else {
		return 80;
	}
	return 127;
}

//1, 12, 45

int shapeToInstrument(String shape){
	if (shape.compare("TRI") == 0){
		return 1;
	}
	else if (shape.compare("RECT") == 0){
		return 1;
	}
	else if (shape.compare("PENTA") == 0){
		return 1;
	}
	else if (shape.compare("HEXA") == 0){
		return 1;
	}
	else{
		return 1;
	}
	return 1;
}






void playSound(){

	sounds.clear();
	o->deleteAll();

	//get sounds
	sounds = aggregator.retrieve();
	

	list<Object>::iterator iter = sounds.begin();

	while (iter != sounds.end()) {
		Object& temp = *iter;
		o->addNode(temp.getPosition().x, temp.getPosition().y, colorToVolume(temp.getColor()), shapeToInstrument(temp.getName()), i);
		i++;
		iter++;
	}
	
		
	o->play();
	i = 0;

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


	playSound();


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