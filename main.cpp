#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;
using namespace std;

int main()
{
    // Capture images from any camera connected to the system
    Mat image = imread("img/B1/B1_1.jpg");

    // Load the trained model
    CascadeClassifier bottleDetector;
    bottleDetector.load("models/bottle.xml");

    if (bottleDetector.empty()) {
        printf("Empty model.");
        return 0;
    }

    std::vector<Rect> bottles;

    // Detect bottle
    bottleDetector.detectMultiScale(image, bottles, 1.1, 30, 0 | CV_HAAR_SCALE_IMAGE, Size(150, 150));

    for (int i = 0; i < (int) bottles.size(); i++) {
        Point pt1(bottles[i].x, bottles[i].y);
        Point pt2(bottles[i].x + bottles[i].width, bottles[i].y + bottles[i].width);
        // Draw a rectangle around the detected bottle
        rectangle(image, pt1, pt2, Scalar(0, 0, 255), 2);
        putText(image, "Bottle", pt1, FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 0, 0), 2.0);
    }

    // Show the transformed frame
    int cols = image.cols/3, rows = image.rows/3;
    Size size(cols, rows);
    cv::resize(image, image, size);
    cvNamedWindow("Bottle detecting camera", WINDOW_NORMAL);
    cv::resizeWindow("Bottle detecting camera", cols, rows);
    imshow("Bottle detecting camera", image);
    cv::waitKey(0);

    return 0;
}

