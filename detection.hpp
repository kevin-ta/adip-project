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

void detectObjects(cv::Mat& image, std::string xmlFile, std::string objectName,
                          int minNeighbors, int width, int height) {
    // Load the trained model
    cv::CascadeClassifier objectDetector;
    if (!objectDetector.load(xmlFile)) {
        qDebug("Wrong xml file path!");
        return;
    }
    std::vector<cv::Rect> objects;
    // Start the detection
    objectDetector.detectMultiScale(image, objects, 1.1, minNeighbors, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(width,height));
    // Start the labeling
    for (int i = 0; i < (int) objects.size(); i++) {
        cv::Point pt1(objects[i].x, objects[i].y);
        cv::Point pt2(objects[i].x + objects[i].width, objects[i].y + objects[i].width);
        // Draw a rectangle around the detected object and label it
        cv::rectangle(image, pt1, pt2, cv::Scalar(0, 0, 255), 5);
        cv::putText(image, objectName, pt1, CV_FONT_HERSHEY_PLAIN, 6, cv::Scalar(255, 0, 0), 5.0);
    }
}

void updateDetectionImage(cv::Mat& image, QString mainPath, QString imagePath) {
    // Open the image
    image = cv::imread(mainPath.toStdString() + imagePath.toStdString());
    if (image.empty()) {
        qDebug("Wrong image path!");
        return;
    }

    // Detect the objects
    detectObjects(image, mainPath.toStdString() + "/models/bottle.xml", "Bottle", 100, 500, 500);
    detectObjects(image, mainPath.toStdString() + "/models/pen.xml", "Pen", 100, 400, 400);
    detectObjects(image, mainPath.toStdString() + "/models/scissor.xml", "Scissor", 100, 400, 400);
    detectObjects(image, mainPath.toStdString() + "/models/tape.xml", "Tape measure", 50, 300, 300);
}

#endif // DETECTION_HPP
