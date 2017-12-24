#ifndef CUSTOMFORM_HPP
#define CUSTOMFORM_HPP
#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QFormLayout>
#include <QVBoxLayout>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

class CustomForm: public QWidget {
  Q_OBJECT
  public:
    CustomForm();
    QString getMainPath();
    void setMainPath(QString mainPath);
    QString getFilePath();
    void setFilePath(QString filePath);

  public slots:
    void changeFile();

  private:
    QPushButton *okButton;
    QLineEdit* mainPathField;
    QLineEdit* filePathField;
    QString mainPathString;
    QString filePathString;
};

#endif // CUSTOMFORM_HPP
