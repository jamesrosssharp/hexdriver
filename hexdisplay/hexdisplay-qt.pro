include(/usr/lib/x86_64-linux-gnu/qt5/examples/gui/analogclock/../rasterwindow/rasterwindow.pri)

# work-around for QTBUG-13496
CONFIG += no_batch

SOURCES += \
    hexdisplay-qt.cpp \
	sharedmem.h

target.path = ./hexdisplay-qt
INSTALLS += target
