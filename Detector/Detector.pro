QT += core
QT -= gui

TARGET = Detector
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Classifier.cpp \
    FeatureGenerator.cpp \
    ImageProcessing.cpp \
    Net.cpp


unix: !macx {
CONFIG   += c++11

INCLUDEPATH += /usr/local/include \
    /usr/local/include/opencv \
    /usr/local/include/opencv2


LIBS += -L/usr/local/lib \
`pkg-config --cflags --libs opencv`

}

macx: {

INCLUDEPATH += /usr/local/include \
    /usr/local/include/opencv \
    /usr/local/include/opencv2

LIBS += -L/usr/local/lib \
-lopencv_core.2.4.12 \
-lopencv_imgproc.2.4.12 \
-lopencv_features2d.2.4.12 \
-lopencv_highgui.2.4.12 \
-lopencv_contrib.2.4.12 \
-lopencv_flann.2.4.12 \
-lopencv_imgproc.2.4.12 \
-lopencv_legacy.2.4.12 \
-lopencv_ml.2.4.12 \
-lopencv_nonfree.2.4.12 \
-lopencv_objdetect.2.4.12 \
-lopencv_photo.2.4.12 \
-lopencv_stitching.2.4.12 \
-lopencv_superres.2.4.12 \
-lopencv_ocl.2.4.12 \
-lopencv_gpu.2.4.12 \
-lopencv_video.2.4.12 \
-lopencv_videostab.2.4.12 \
-lopencv_calib3d.2.4.12
}

win32: {

INCLUDEPATH += C:/opencv/build/include \
C:/opencv/build/include/opencv \
C:/opencv/build/include/opencv2

LIBS += -LC:/opencv/build/x64/vc12/lib \
-lopencv_core2410 \
-lopencv_imgproc2410 \
-lopencv_features2d2410 \
-lopencv_highgui2410 \
-lopencv_contrib2410 \
-lopencv_flann2410 \
-lopencv_legacy2410 \
-lopencv_ml2410 \
-lopencv_nonfree2410 \
-lopencv_objdetect2410 \
-lopencv_photo2410 \
-lopencv_stitching2410 \
-lopencv_superres2410 \
-lopencv_ocl2410 \
-lopencv_gpu2410 \
-lopencv_video2410 \
-lopencv_videostab2410 \
-lopencv_calib3d2410 \


}

HEADERS += \
    Classifier.h \
    FeatureGenerator.h \
    ImageProcessing.h \
    Net.h

