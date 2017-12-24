#include "customform.hpp"

CustomForm::CustomForm() : QWidget() {
    okButton = new QPushButton("Choose the image file", this);
    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(changeFile()));
    mainPathField = new QLineEdit;
    filePathField = new QLineEdit;
    mainPathString = "";
    filePathString = "";
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("The main directory path:", mainPathField);
    formLayout->addRow("The image path:", filePathField);
    QVBoxLayout* principalLayout = new QVBoxLayout;
    principalLayout->addLayout(formLayout);
    principalLayout->addWidget(okButton);
    setLayout(principalLayout);
}

QString CustomForm::getMainPath() {
    return mainPathString;
}

void CustomForm::setMainPath(QString mainPath) {
    mainPathString = mainPath;
    mainPathField->setText(mainPathString);
}

QString CustomForm::getFilePath() {
    return filePathString;
}

void CustomForm::setFilePath(QString filePath) {
    filePathString = filePath;
    filePathField->setText(filePathString);
}

void CustomForm::changeFile() {
    mainPathString = mainPathField->text();
    filePathString = filePathField->text();
}
