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

HEADERS     +=  $$LIB_H/PlottingAttributes.h \
							 $$LIB_H/Axis.h \
							 $$LIB_H/AxisWidget.h \
							 $$LIB_H/DoubleInput.h \
							 $$LIB_H/DoubleInputAction.h \
							 $$LIB_H/Series.h \
							 $$LIB_H/KeyColour.h \
							 $$LIB_H/KeyLabel.h \
							 $$LIB_H/Key.h \
							 $$LIB_H/Plotter.h \
							 $$LIB_H/StripChart.h \
							 $$LIB_H/QtStripChart.h
				
SOURCES     +=  $$LIB_SRC/Axis.cpp \
							$$LIB_SRC/AxisWidget.cpp \
							 $$LIB_SRC/DoubleInput.cpp \
							 $$LIB_SRC/Series.cpp \
							 $$LIB_SRC/KeyColour.cpp \
							 $$LIB_SRC/KeyLabel.cpp \
							 $$LIB_SRC/Key.cpp \
							 $$LIB_SRC/Plotter.cpp \
							 $$LIB_SRC/StripChart.cpp
							 
QMAKE_CXXFLAGS += 

QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg