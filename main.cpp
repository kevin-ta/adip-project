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

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QMainWindow window;
    window.setWindowTitle("Final objects identified");
    QString mainPathString = "/home/jen/Documents/adip-project-qt/Ressources/";
    QString filePathString = "/B1/B1_1.jpg";
    int maxRows = 600;
    cv::Mat image;

    // Detect the objects
    updateDetectionImage(image, mainPathString, filePathString);

    // Show the transformed frame
    if (image.rows > maxRows) {
        double scale = (double) maxRows / image.rows;
        int cols = image.cols * scale, rows = image.rows * scale;
        cv::Size size(cols, rows);
        cv::resize(image, image, size);
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
    QObject::connect(fileForm, SIGNAL(updateImage()), imageWidget, SLOT(updateToDo()));

    window.show();

    return app.exec();
}



