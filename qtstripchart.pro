TEMPLATE = lib
CONFIG  = qt debug dll warn_on

TARGET = qtstripchart
target.path += /usr/local/lib

VERSION = 0.2.0
DESTDIR = ./
MOC_DIR = moc
OBJECTS_DIR=obj
INCLUDEPATH += ./include

DEPENDPATH=$$INCLUDEPATH


HEADERS     =  include/PlottingAttributes.h \
							 include/Axis.h \
							 include/AxisWidget.h \
							 include/DoubleInput.h \
							 include/DoubleInputAction.h \
							 include/Series.h \
							 include/KeyColour.h \
							 include/KeyLabel.h \
							 include/Key.h \
							 include/Plotter.h \
							 include/StripChart.h \
							 include/QtStripChart.h
				
SOURCES     =  src/Axis.cpp \
							 src/AxisWidget.cpp \
							 src/DoubleInput.cpp \
							 src/Series.cpp \
							 src/KeyColour.cpp \
							 src/KeyLabel.cpp \
							 src/Key.cpp \
							 src/Plotter.cpp \
							 src/StripChart.cpp
							 
headers.files += $$HEADERS
headers.path=/usr/local/include/qtstripchart

QT += core gui  xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INSTALLS += target headers
