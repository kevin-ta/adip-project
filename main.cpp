#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    // Open image
    Mat image = imread("img/B1/B1_1.jpg");

    // Load the trained models
    CascadeClassifier bottleDetector, penDetector, scissorDetector, tapeDetector;
    bottleDetector.load("models/bottle.xml");
    penDetector.load("models/pen.xml");
    scissorDetector.load("models/scissor.xml");
    tapeDetector.load("models/tape.xml");

    std::vector<Rect> bottles, pens, scissors, tapes;
    
    // Starting the detection
    bottleDetector.detectMultiScale(image, bottles, 1.1, 100, 0 | CV_HAAR_SCALE_IMAGE, Size(500,500));
    penDetector.detectMultiScale(image, pens, 1.1, 100, 0 | CV_HAAR_SCALE_IMAGE, Size(400,400));
    scissorDetector.detectMultiScale(image, scissors, 1.1, 100, 0 | CV_HAAR_SCALE_IMAGE, Size(400,400));
    tapeDetector.detectMultiScale(image, tapes, 1.1, 50, 0 | CV_HAAR_SCALE_IMAGE, Size(300,300));
    
    // Starting the labeling
    for (int i = 0; i < (int) bottles.size(); i++) {
        Point pt1(bottles[i].x, bottles[i].y);
        Point pt2(bottles[i].x + bottles[i].width, bottles[i].y + bottles[i].width);
        // Draw a rectangle around the detected bottle
        rectangle(image, pt1, pt2, Scalar(0, 0, 255), 2);
        putText(image, "Bottle", pt1, FONT_HERSHEY_PLAIN, 10, Scalar(255, 0, 0), 2.0);
    }
    for (int i = 0; i < (int) pens.size(); i++) {
        Point pt1(pens[i].x, pens[i].y);
        Point pt2(pens[i].x + pens[i].width, pens[i].y + pens[i].width);
        // Draw a rectangle around the detected pen
        rectangle(image, pt1, pt2, Scalar(0, 0, 255), 2);
        putText(image, "Pen", pt1, FONT_HERSHEY_PLAIN, 10, Scalar(255, 0, 0), 2.0);
    }
    for (int i = 0; i < (int) scissors.size(); i++) {
        Point pt1(scissors[i].x, scissors[i].y);
        Point pt2(scissors[i].x + scissors[i].width, scissors[i].y + scissors[i].width);
        // Draw a rectangle around the detected scissor
        rectangle(image, pt1, pt2, Scalar(0, 0, 255), 2);
        putText(image, "Scissor", pt1, FONT_HERSHEY_PLAIN, 10, Scalar(255, 0, 0), 2.0);
    }
    for (int i = 0; i < (int) tapes.size(); i++) {
        Point pt1(tapes[i].x, tapes[i].y);
        Point pt2(tapes[i].x + tapes[i].width, tapes[i].y + tapes[i].width);
        // Draw a rectangle around the detected tape
        rectangle(image, pt1, pt2, Scalar(0, 0, 255), 2);
        putText(image, "Tape measure", pt1, FONT_HERSHEY_PLAIN, 10, Scalar(255, 0, 0), 2.0);
    }

    // Show the transformed frame
    int cols = image.cols/3, rows = image.rows/3;
    Size size(cols, rows);
    cv::resize(image, image, size);
    cvNamedWindow("Final objects identified", WINDOW_NORMAL);
    cv::resizeWindow("Final objects identified", cols, rows);
    imshow("Final objects identified", image);
    cv::waitKey(0);

    return 0;
}
