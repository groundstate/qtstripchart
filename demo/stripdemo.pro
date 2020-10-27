TEMPLATE = app
CONFIG  = qt debug  warn_on
TARGET =stripdemo
VERSION = 0.1.0
DESTDIR = ./
INCLUDEPATH += ../include
# LIBS += -L../ -lqtstripchart

HEADERS     =  StripDemo.h 
				
SOURCES     =  Main.cpp \
							 StripDemo.cpp

unix:LIB_SRC= ../src
unix:LIB_H  = ../include

HEADERS     +=  ../include/PlottingAttributes.h \
							 ../include/Axis.h \
							 ../include/AxisWidget.h \
							 ../include/DoubleInput.h \
							 ../include/DoubleInputAction.h \
							 ../include/Series.h \
							 ../include/KeyColour.h \
							 ../include/KeyLabel.h \
							 ../include/Key.h \
							 ../include/Plotter.h \
							 ../include/StripChart.h \
							 ../include/QtStripChart.h
				
SOURCES     +=  ../src/Axis.cpp \
							../src/AxisWidget.cpp \
							 ../src/DoubleInput.cpp \
							 ../src/Series.cpp \
							 ../src/KeyColour.cpp \
							 ../src/KeyLabel.cpp \
							 ../src/Key.cpp \
							 ../src/Plotter.cpp \
							 ../src/StripChart.cpp
							 
QT += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 

QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg
