SOURCES += \
    main.cpp \
    customform.cpp

QT += widgets

INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_ml \
        -lopencv_video \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_objdetect \
        -lopencv_contrib \
        -lopencv_legacy \
        -lopencv_flann \
        -lopencv_nonfree \

RESOURCES +=

HEADERS += \
    cvimagewidget.hpp \
    detection.hpp \
    customform.hpp
