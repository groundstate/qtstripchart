//
// qtstripchart - a strip chart recorder library for Qt
//
// The MIT License (MIT)
//
// Copyright (c)  2014  Michael J. Wouters
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// Some benchmarking
// 4 instances, 1000 pt window
// 2 Hz update: 0.6% CPU/window 2% xserver 
// 20 Hz update: 4% CPU/window  1% xserver
// 3000 pt window
// 50 Hz update 11% CPU/window

#include <cmath>

#include <QtGui>
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>

#include "StripDemo.h"
#include "QtStripchart.h"

	
StripDemo::StripDemo()
{
	
	QWidget *w = new QWidget(this);
	QVBoxLayout * vb = new QVBoxLayout(w);
	vb->setContentsMargins(0,0,0,0);
	
	chart_= new StripChart(this);
	chart_->setTitle("");
	chart_->setFrameStyle(QFrame::Panel | QFrame::Raised);
	chart_->setLineWidth(3);
	//chart_->plotter()->setScrollingWindow(true);
	chart_->plotter()->setXWindow(0,200);
	
	vb->addWidget(chart_,1);
	
	key_ = new Key(Qt::Horizontal,this);
	vb->addWidget(key_,0);
	
	Series* sd = chart_->addSeries("data0");
	sd->setColour(QColor(255,0,255));
	key_->addSeries(sd);
	
	sd=chart_->addSeries("data1");
	key_->addSeries(sd);
	
	setCentralWidget(w);

	createActions();
	createMenus();

	demoTimer_ = new QTimer(this);
	connect(demoTimer_, SIGNAL(timeout()), this, SLOT(addData()));
	speed =50.0;
	tcurr = 0.0;
	
	connect(key_,SIGNAL(attributesChanged(Series *)),chart_->plotter(),SLOT(attributesChanged(Series *)));
	
}

StripDemo::~StripDemo()
{
}

//
// private slots:
//

void StripDemo::startDemo()
{
	demoTimer_->start(1000/speed);
}

void StripDemo::stopDemo()
{
	demoTimer_->stop();
}

void StripDemo::addData()
{
	int ids[2];
	double x[2],y[2];
	ids[0]=0;
	x[0]=tcurr;
	y[0]=sin(tcurr*3.141/1000.0);
	ids[1]=1;
	x[1]=tcurr;
	y[1]=0.7*cos(tcurr*3.141/1000.0);
	chart_->addPoints(ids,x,y,2);
	tcurr += 1.0;
}

void StripDemo::setSpeed()
{
	bool ok;
	int ret = QInputDialog::getInt(this, tr("Set update speed"),
                                          tr("Update speed (1 - 500 samples/s)"), 
																					speed,1,500,1,&ok);
	if (ok && (ret!=speed))
			speed=ret;
		 
	stopDemo();
	startDemo();
}


void StripDemo::createActions()
{
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	demoStartAct = new QAction(tr("Start"), this);
	connect(demoStartAct, SIGNAL(triggered()), this, SLOT(startDemo()));

	demoStopAct = new QAction(tr("Stop"), this);
	connect(demoStopAct, SIGNAL(triggered()), this, SLOT(stopDemo()));
	
	demoSetSpeedAct= new QAction(tr("Set update speed"), this);
	connect(demoSetSpeedAct, SIGNAL(triggered()), this, SLOT(setSpeed()));

}

void StripDemo::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAct);

	demoMenu = menuBar()->addMenu(tr("&Demo"));
	demoMenu->addAction(demoStartAct);
	demoMenu->addAction(demoStopAct );
	demoMenu->addAction(demoSetSpeedAct );
}
