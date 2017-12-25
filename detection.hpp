#ifndef DETECTION_HPP
#define DETECTION_HPP
#pragma once

#include "cvimagewidget.hpp"

#include <QDebug>
#include <QString>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>

void openMatImage(cv::Mat& image, QString mainPath, QString imagePath) {
    // Open the image
    image = cv::imread(mainPath.toStdString() + imagePath.toStdString());
    if (image.empty()) {
        qDebug("Wrong image path!");
    }
}

void segregateBackground(cv::Mat& image) {
    // Make a negative image
    cv::subtract(cv::Scalar::all(255), image, image);
    // Use the Sobel Filter
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    cv::GaussianBlur(image, image, cv::Size(5,5), 0, 0, cv::BORDER_DEFAULT);
    cv::cvtColor(image, image, CV_BGR2GRAY );
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Sobel(image, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_x, abs_grad_x );
    cv::Sobel(image, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_y, abs_grad_y );
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, image);
    // Gaussian blur for smoothing
    cv::GaussianBlur(image, image, cv::Size(5,5), 0, 0, cv::BORDER_DEFAULT);
    // Use the threshold for a binary image
    cv::threshold(image, image, 15, 255, cv::THRESH_BINARY);
    // Use the local adaptive threshold to remove points out of the cluster
    cv::adaptiveThreshold(image, image, 255, CV_ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 5, 0);
    cv::cvtColor(image, image, CV_GRAY2BGR);
    //Create a mask from edges in the original image
    cv::Mat mask;
    cv::Canny(image, mask, 100, 200);
    cv::copyMakeBorder(mask, mask, 1, 1, 1, 1, cv::BORDER_REPLICATE);
    // Flood fill the background
    cv::floodFill(image, mask, cv::Point(0, 0), cv::Scalar(0, 0, 255));
    cv::floodFill(image, mask, cv::Point(image.cols - 1, 0), cv::Scalar(0, 0, 255));
    cv::floodFill(image, mask, cv::Point(0, image.rows - 1), cv::Scalar(0, 0, 255));
    cv::floodFill(image, mask, cv::Point(image.cols - 1, image.rows - 1), cv::Scalar(0, 0, 255));
}

void cutBackground(cv::Mat& image) {
    cv::Mat background;
    image.copyTo(background);
    segregateBackground(background);
    cv::Vec3b white(255, 255, 255);
    for (int i = 0; i < background.rows; i++) {
        for (int j = 0; j < background.cols; j++) {
            cv::Vec3b color = background.at<cv::Vec3b>(i, j);
            if ((color.val[0] == 0) && (color.val[1] == 0) && (color.val[2] == 255)) {
                image.at<cv::Vec3b>(i, j) = white;
            }
        }
    }
}

void detectObjects(cv::Mat& image, cv::Mat& cutImage, std::string xmlFile, std::string objectName, int minNeighbors, int width, int height) {
    // Load the trained model
    cv::CascadeClassifier objectDetector;
    if (!objectDetector.load(xmlFile)) {
        qDebug("Wrong xml file path!");
        return;
    }
    std::vector<cv::Rect> objects;
    // Start the detection
    objectDetector.detectMultiScale(cutImage, objects, 1.1, minNeighbors, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(width,height));
    // Start the labeling
    for (int i = 0; i < (int) objects.size(); i++) {
        cv::Point pt1(objects[i].x, objects[i].y);
        cv::Point pt2(objects[i].x + objects[i].width, objects[i].y + objects[i].width);
        // Draw a rectangle around the detected object and label it
        cv::rectangle(image, pt1, pt2, cv::Scalar(0, 0, 255), 5);
        cv::putText(image, objectName, pt1, CV_FONT_HERSHEY_PLAIN, 6, cv::Scalar(255, 0, 0), 5.0);
        cv::rectangle(cutImage, pt1, pt2, cv::Scalar(0, 0, 255), 5);
        cv::putText(cutImage, objectName, pt1, CV_FONT_HERSHEY_PLAIN, 6, cv::Scalar(255, 0, 0), 5.0);
    }
}

void updateDetectionImage(cv::Mat& image, cv::Mat& cutImage, QString mainPath) {
    // Detect the objects
    detectObjects(image, cutImage, mainPath.toStdString() + "/models/bottle.xml", "Bottle", 100, 500, 500);
    detectObjects(image, cutImage, mainPath.toStdString() + "/models/pen.xml", "Pen", 100, 400, 400);
    detectObjects(image, cutImage, mainPath.toStdString() + "/models/scissor.xml", "Scissor", 100, 400, 400);
    detectObjects(image, cutImage, mainPath.toStdString() + "/models/tape.xml", "Tape measure", 50, 300, 300);
}

#endif // DETECTION_HPP
