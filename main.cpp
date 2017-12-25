#include "cvimagewidget.hpp"
#include "customform.hpp"
#include "detection.hpp"

#include <QDialog>
#include <QApplication>
#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <ctime>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QMainWindow window;
    QMainWindow cutWindow;
    window.setWindowTitle("Final objects identified");
    cutWindow.setWindowTitle("Removed background");
    QString mainPathString = "";
    QString filePathString = "";
    int maxRows = 600;
    cv::Mat image;
    cv::Mat cutImage;

    // Check the path
    if (argc < 3) { // Number of arguments, the first argument (argv[0]) is the running instruction
        qDebug("Add the main path of the directory and the specific path of the image!");
        return -1;
     }
    mainPathString = argv[1];
    filePathString = argv[2];
    QFile imageFile(mainPathString + filePathString);
    if (!imageFile.exists()) {
        qDebug() << "The " << argv[1] << argv[2] << "image file could not be read!";
        return -2;
    }
    qDebug().nospace() << "Running the program on the" << argv[1] << argv[2] << " image file...";

    // Open the image
    openMatImage(image, mainPathString, filePathString);
    image.copyTo(cutImage);

    // Background image
    std::clock_t begin_time1 = std::clock();
    cutBackground(cutImage);

    // Detect the objects
    std::clock_t begin_time2 = std::clock();
    updateDetectionImage(image, cutImage, mainPathString);
    std::clock_t begin_time3 = std::clock();

    // Exceution time
    double time1 = 1000.0 * (begin_time2 - begin_time1) / CLOCKS_PER_SEC;
    double time2 = 1000.0 * (begin_time3 - begin_time2) / CLOCKS_PER_SEC;
    double time3 = time1 + time2;
    qDebug() << "Execution time of the background cutting:" << time1 << "ms";
    qDebug() << "Execution time of the object detection by the model:" << time2 << "ms";
    qDebug() << "Total execution time:" << time3 << "ms";

    // Show the transformed frames
    if (image.rows > maxRows) {
        double scale = (double) maxRows / image.rows;
        int cols = image.cols * scale, rows = image.rows * scale;
        cv::Size size(cols, rows);
        cv::resize(image, image, size);
        cv::resize(cutImage, cutImage, size);
    }

    // Form for image file
    QVBoxLayout* principalLayout = new QVBoxLayout;
    CustomForm* fileForm = new CustomForm();
    fileForm->setMainPath(mainPathString);
    fileForm->setFilePath(filePathString);
    principalLayout->addWidget(fileForm);
    // Create the image widget
    CVImageWidget* imageWidget = new CVImageWidget();
    principalLayout->addWidget(imageWidget);
    QWidget* centerWindow = new QWidget();
    centerWindow->setLayout(principalLayout);
    window.setCentralWidget(centerWindow);
    imageWidget->showImage(image);
    // Create the image widget
    CVImageWidget* imageWidget2 = new CVImageWidget();
    cutWindow.setCentralWidget(imageWidget2);
    imageWidget2->showImage(cutImage);

    cutWindow.show();
    window.show();

    return app.exec();
}



