TEMPLATE = lib
CONFIG  = qt debug dll warn_on

TARGET = qtstripchart
target.path += /usr/local/lib

VERSION = 0.1.1
DESTDIR = ./
MOC_DIR = moc
OBJECTS_DIR=obj
INCLUDEPATH += ./include

DEPENDPATH=$$INCLUDEPATH

unix:LIB_SRC= src
unix:LIB_H  = include

HEADERS     =  $$LIB_H/PlottingAttributes.h \
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
				
SOURCES     =  $$LIB_SRC/Axis.cpp \
							$$LIB_SRC/AxisWidget.cpp \
							 $$LIB_SRC/DoubleInput.cpp \
							 $$LIB_SRC/Series.cpp \
							 $$LIB_SRC/KeyColour.cpp \
							 $$LIB_SRC/KeyLabel.cpp \
							 $$LIB_SRC/Key.cpp \
							 $$LIB_SRC/Plotter.cpp \
							 $$LIB_SRC/StripChart.cpp
							 
headers.files += $$HEADERS
headers.path=/usr/local/include/qtstripchart

QMAKE_CXXFLAGS += 

INSTALLS += target headers