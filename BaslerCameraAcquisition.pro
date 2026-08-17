QT += core gui widgets

TARGET = BaslerCameraAcquisition
TEMPLATE = app
CONFIG += c++14
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    baslercamera.cpp

HEADERS += \
    mainwindow.h \
    baslercamera.h

FORMS += \
    mainwindow.ui

# OpenCV
# Override OPENCV_ROOT when OpenCV is installed elsewhere, e.g.:
#   export OPENCV_ROOT=/usr/local/opencv455
OPENCV_ROOT = $$(OPENCV_ROOT)
isEmpty(OPENCV_ROOT) {
    OPENCV_ROOT = /usr/local
}

INCLUDEPATH += \
    $$OPENCV_ROOT/include/opencv4

LIBS += \
    -L$$OPENCV_ROOT/lib \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_imgcodecs

# Basler pylon SDK
# Override PYLON_ROOT when needed, e.g.:
#   export PYLON_ROOT=/opt/pylon
PYLON_ROOT = $$(PYLON_ROOT)
isEmpty(PYLON_ROOT) {
    PYLON_ROOT = /opt/pylon
}

INCLUDEPATH += $$PYLON_ROOT/include

PYLON_CONFIG = $$PYLON_ROOT/bin/pylon-config
exists($$PYLON_CONFIG) {
    QMAKE_CXXFLAGS += $$system($$PYLON_CONFIG --cflags)
    LIBS += $$system($$PYLON_CONFIG --libs)
} else {
    # Fallback for older pylon installations.
    LIBS += \
        -L$$PYLON_ROOT/lib \
        -lpylonbase \
        -lpylonutility \
        -lGenApi_gcc_v3_1_Basler_pylon \
        -lGCBase_gcc_v3_1_Basler_pylon
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
