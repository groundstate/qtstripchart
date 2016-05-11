//
// qtstripchart - a stripchart widget libary for Qt
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

#include <iostream>

#include <QAction>
#include <QColorDialog>
#include <QLayout>
#include <QLabel>
#include <QMenu>

#include "KeyColour.h"
#include "KeyLabel.h"
#include "PlottingAttributes.h"
#include "Series.h"

KeyLabel::KeyLabel(Series *sd,QWidget *parent):
	QFrame(parent)
{
	
	setContextMenuPolicy(Qt::CustomContextMenu);
	
	sd_ = sd;
	PlottingAttributes &pa = sd->plottingAttributes();
	
	setFrameStyle(QFrame::Panel | QFrame::Raised);
	setLineWidth(3);
	
	QHBoxLayout *hb = new QHBoxLayout(this);
	hb->setContentsMargins(0,0,0,0);
	
	c_= new KeyColour(pa.colour,this);
	hb->addWidget(c_,0);
	
	l_ = new QLabel(sd->name(),this);
	l_->setContentsMargins(0,0,0,0);
	hb->addWidget(l_,1);
	
	connect(this,SIGNAL(customContextMenuRequested ( const QPoint &)),
		this,SLOT(createContextMenu(const QPoint &)));
	
}

KeyLabel::~KeyLabel()
{
	
}

//
// Private slots
//

void KeyLabel::setPlotStyle(QAction *a)
{
	PlotStyles::Enum ps ;
	if (a->text() == "Lines")
	{
		ps=PlotStyles::Lines;
	}
	else if (a->text() == "Points")
	{
		ps=PlotStyles::Points;
	}
	else if (a->text() == "Lines+points")
	{
		ps=PlotStyles::LinesPoints;
	}
	
	PlottingAttributes &pa = sd_->plottingAttributes();
	
	if (ps != pa.plotStyle)
	{
			pa.plotStyle = ps;
			emit attributesChanged(sd_);
	}
}

void KeyLabel::setColour(bool)
{
	PlottingAttributes &pa = sd_->plottingAttributes();
	
	QColorDialog *dlg = new QColorDialog(pa.colour, this);
	dlg->setWindowFlags(Qt::Popup);
	if (dlg->exec() == QDialog::Accepted)
	{
		
		if (dlg->selectedColor() != pa.colour)
		{
			
			pa.colour = dlg->selectedColor();
			emit attributesChanged(sd_);
			c_->setColour(pa.colour);
		}
	}
	
	delete dlg;
}

void KeyLabel::setLineStyle(QAction *a)
{
	LineStyles::Enum lt;
	if (a->text() == "Solid")
	{
		lt = LineStyles::Solid;
	}
	else if (a->text() == "Dashed")
	{
		lt = LineStyles::Dashed;
	}
	PlottingAttributes &pa = sd_->plottingAttributes();
	
	if (lt != pa.lineStyle)
	{
		pa.lineStyle = lt;
		emit attributesChanged(sd_);	
	}
	
}

void KeyLabel::setLineThickness(QAction *)
{
}

void KeyLabel::setPointStyle(QAction *a)
{
	PointStyles::Enum ps;
	if (a->text() == "Box")
	{
		ps = PointStyles::Box;
	}
	else if (a->text() == "Cross")
	{
		ps = PointStyles::Cross;
	}
	else if (a->text() == "Dot")
	{
		ps = PointStyles::Dot;
	}
	
	PlottingAttributes &pa = sd_->plottingAttributes();
	
	if (ps != pa.pointStyle)
	{
		pa.pointStyle = ps;
		emit attributesChanged(sd_);	
	}
}

void KeyLabel::toggleVisibility(bool)
{
	PlottingAttributes &pa = sd_->plottingAttributes();
	pa.visible= !pa.visible;
	emit attributesChanged(sd_);	
}

//
// Private methods
//


void KeyLabel::createContextMenu(const QPoint &)
{
	QMenu *menu= new QMenu();
	
	//
	PlottingAttributes &pa = sd_->plottingAttributes();
	
	QMenu *sub = menu->addMenu(tr("Plot style"));
	QActionGroup *group = new QActionGroup(sub);
	group->setExclusive(true);
	
	QAction *action = sub->addAction("Lines");
	action->setCheckable(true);
	action->setChecked(pa.plotStyle==PlotStyles::Lines);
	group->addAction(action);
	
	action = sub->addAction("Points");
	action->setCheckable(true);
	action->setChecked(pa.plotStyle==PlotStyles::Points);
	group->addAction(action);
	
	action = sub->addAction("Lines+points");
	action->setCheckable(true);
	action->setChecked(pa.plotStyle==PlotStyles::LinesPoints);
	group->addAction(action);
	
	connect(sub,SIGNAL(triggered(QAction *)),this,SLOT(setPlotStyle(QAction *)));
	
	//
	
	sub = menu->addMenu(tr("Line style"));
	group = new QActionGroup(sub);
	group->setExclusive(true);
	
	action = sub->addAction(tr("Solid"));
	action->setCheckable(true);
	action->setChecked(pa.lineStyle==LineStyles::Solid);
	group->addAction(action);
	
	action = sub->addAction(tr("Dashed"));
	action->setCheckable(true);
	action->setChecked(pa.lineStyle==LineStyles::Dashed);
	group->addAction(action);
	
	connect(sub,SIGNAL(triggered(QAction *)),this,SLOT(setLineStyle(QAction *)));
	
	sub = menu->addMenu(tr("Point style"));
	group = new QActionGroup(sub);
	group->setExclusive(true);
	
	action = sub->addAction(tr("Cross"));
	action->setCheckable(true);
	action->setChecked(pa.pointStyle==PointStyles::Cross);
	group->addAction(action);
	
	action = sub->addAction(tr("Box"));
	action->setCheckable(true);
	action->setChecked(pa.pointStyle==PointStyles::Box);
	group->addAction(action);
	
	action = sub->addAction(tr("Dot"));
	action->setCheckable(true);
	action->setChecked(pa.pointStyle==PointStyles::Dot);
	group->addAction(action);
	
	connect(sub,SIGNAL(triggered(QAction *)),this,SLOT(setPointStyle(QAction *)));
	
	action = menu->addAction(tr("Line colour"));
	connect(action,SIGNAL(triggered(bool)),this,SLOT(setColour(bool)));
	
	//menu.addAction("Line thickness");
	
	action = menu->addAction(tr("Visible"));
	action->setCheckable(true);
	action->setChecked(pa.visible);
	connect(action,SIGNAL(triggered(bool)),this,SLOT(toggleVisibility(bool)));
	
	menu->exec(QCursor::pos());	
	delete menu;
	
}