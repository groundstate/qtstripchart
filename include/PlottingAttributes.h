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

#ifndef __PLOTTING_ATTRIBUTES_H_
#define __PLOTTING_ATTRIBUTES_H_

#include <QColor>
#include <QString>

namespace PlotStyles
{
	enum Enum {Lines,Points,LinesPoints};
}

namespace LineStyles
{
	enum Enum {Solid,Dashed};
}

namespace PointStyles
{
	enum Enum {Cross,Box,Dot};
}

class PlottingAttributes
{
	public:
		
		PlottingAttributes(){
			thickness=1;
			colour = QColor(255,255,0);
			plotStyle = PlotStyles::Lines;
			lineStyle = LineStyles::Solid;
			pointStyle = PointStyles::Cross;
			visible=true;
		}
		
		PlottingAttributes(PlottingAttributes *pa){
			thickness=pa->thickness;
			colour = pa->colour;
			plotStyle = pa->plotStyle;
			lineStyle = pa->lineStyle;
			pointStyle = PointStyles::Cross;
			visible=pa->visible;
		}
		
		int thickness;
		QColor colour;
		PlotStyles::Enum plotStyle;
		LineStyles::Enum lineStyle;
		PointStyles::Enum pointStyle;
		bool visible;
};

#endif