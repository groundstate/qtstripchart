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

#include <sys/time.h>

#include <QDebug>

#include <cmath>
#include <cfloat>

#include "Axis.h"


Axis::Axis(QObject *parent):QObject(parent)
{
	init();
}
		
Axis::~Axis()
{
}

void Axis::range(double *rmin,double *rmax)
{
	*rmin=min_;
	*rmax=max_;
}

void Axis::setAutoScale(bool s)
{
	
	autoScale_=s; // FIXME autoscale does nothing
}

void Axis::ticks(double *minTick,double *maxTick,
								double *firstTick,double *lastTick,
								double *tickSpacing,int *numIntervals,
								int *scalingExponent)
{
	*minTick=min_;
	*maxTick=max_;
	*firstTick=firstTick_;
	*lastTick=lastTick_;
	*tickSpacing=tickSpacing_;
	*numIntervals=nIntervals_;
	*scalingExponent=scalingExponent_;
}

QString Axis::formatValue(double d)
{
	QString ret;
	if (usesTimeOfDay_)
	{
		int secs= ((int) rint(d)) % 86400;
		int hh = secs / 3600;
		int mm = (secs - hh*3600) / 60;
		int ss = secs - hh*3600 - mm*60;
		ret.sprintf("%02d:%02d:%02d",hh,mm,ss);
	}
	else
	{
		ret.sprintf("%g",d);
	}
	return ret;
}

void Axis::setUsesTimeOfDay(bool tod)
{
	usesTimeOfDay_=tod;
}



void Axis::setAxisStyle(AxisStyle style)
{
	style_=style;
}

//
//
//
void Axis::setRange(double min,double max)
{
	if (min < max){
		min_=min;
		max_=max;
	}
	else{
		max_=min;
		min_=max;
	}
	if (min < absMin_) min_ = absMin_;
	if (max > absMax_) max_ = absMax_;
	
	setTicks(); 

}

void Axis::setRangeLimits(double min,double max)
{
	// The data range will be clamped to these limits
	if (min < max){
		absMin_=min;
		absMax_=max;
	}
	else{
		absMin_=max;
		absMax_=min;
	}
	
}

void Axis::setMin(double min)
{
	// assuming inconsistent axis limits come from a UI event
	// shift the other axis limit to maintain the axis range
	if (min > max_){
		max_ = min + (max_-min_);
		if (max_>absMax_) max_=absMax_;
	}
	
	min_=min;
	if (min_ < absMin_) min_ = absMin_;
	setTicks();

}

void Axis::setMax(double max)
{
	
	if (max < min_)
	{
		min_=max-(max_-min_);
		if (min_ < absMin_) min_ = absMin_;
	}
	
	max_=max;
	if (max_ > absMax_) max_=absMax_;
	setTicks();
	
}

		
void Axis::init()
{
	
	style_=Linear;
	min_=0;
	max_=10.0;
	absMin_ = -DBL_MAX;
	absMax_ = DBL_MAX;
	
	usesTimeOfDay_=false;
	autoScale_=false;
	
	setTicks();
	
}


// void Axis::setTicks()
// {
// 
// 	// Zero exponent indicates do not use exponential multiplier in 
// 	// the label
// 	
// 	double nmin,nmax;
// 	
// 	///qDebug() << min_ << " " << max_ ;
// 	
// 	if (min_ == max_)
// 	{
// 		if (max_ !=0)
// 			scalingExponent_ = (int) (rint( log10(fabs(max_))));
// 		else
// 			scalingExponent_=0;
// 		min_ = min_ - 1E-31; // Not perfect ..
// 		max_ = max_ + 1E-31;
// 		nIntervals_ = 2;
// 		return;
// 	}
// 	
// 	// dx = dxmantissa * 10^{dxpow}
// 	scalingExponent_ = (int) (rint( log10(max_-min_) ));
// 
// 	// Now use this to bring max_-min_ into the range [0,10) 
// 	nmin = floor(min_/pow(10.0,scalingExponent_));
// 	nmax = ceil(max_/pow(10.0,scalingExponent_)); // -ve numbers ?
// 
// 	firstTick_=nmin*pow(10.0,scalingExponent_);
// 	lastTick_ =nmax*pow(10.0,scalingExponent_);
// 
// 	// Some style rules need to be applied to the exponent
// 	// viz. numbers between 0.1 and 10000 don't have to be
// 	// written with a multiplier
// 	 
// 	if (scalingExponent_ >= -1 && scalingExponent_ < 4)
// 		scalingExponent_=0;
// 
// 	// Some style rules to do with the number of intervals to use
// 	if ((nmax-nmin) > 2) //If nmax - nmin > 2, all is OK */
// 		nIntervals_= (int)(nmax-nmin);
// 	else if ((nmax - nmin) == 2)  // Use 4 intervals 
// 		nIntervals_=4;
// 	else if ((nmax - nmin) == 1) // Use 5 intervals 
// 		nIntervals_=5;
// 
// 	tickSpacing_ = (lastTick_-firstTick_)/nIntervals_;
//  
// 	// The min tick and max tick are tracked separately because they may not be on
// 	// "integer" boundaries. So eg if min==firsTick it gets shifted. Also if the
// 	// first tick precedes min_ (which is usually the case) it gets shifted.
// 	
// 
// 	if (min_>=firstTick_){
// 		int di = ceil((min_-firstTick_)/tickSpacing_);
// 		firstTick_+= di*tickSpacing_;
// 		nIntervals_ -= di;
// 	}
// 	if (max_<=lastTick_){  
// 		int di = ceil((lastTick_-max_)/ tickSpacing_);
// 		lastTick_ -= di*tickSpacing_;
// 		nIntervals_ -= di;
// 	}
// 	
// 	// A tweak
// 	// The preceding adjustments may have resulted in nIntervals == 1
// 	// which is a little sparse.
// 
// 	if (1== nIntervals_)
// 	{
// 		tickSpacing_ /= 2.0;
// 		nIntervals_=2;
// 		if (firstTick_ - tickSpacing_ > min_)
// 		{
// 			firstTick_ -=tickSpacing_;
// 			nIntervals_++;
// 		}
// 		if (lastTick_ + tickSpacing_ < max_)
// 		{
// 			lastTick_ += tickSpacing_;
// 			nIntervals_++;
// 		}
// 	}
// }


void Axis::setTicks()
{

	// Zero exponent indicates do not use exponential multiplier in 
	// the label
	
	double nmin,nmax;
	
	if (min_ == max_)
	{
		if (max_ !=0)
			scalingExponent_ = (int) (floor( log10(fabs(max_))));
		else
			scalingExponent_=0;
		min_ = min_ - 1E-31; // Not perfect ..
		max_ = max_ + 1E-31;
		nIntervals_ = 2;
		return;
	}
	
	if (fabs(min_) > fabs(max_)) 
		scalingExponent_ = (int) (rint( log10(fabs(min_)) ));
	else
		scalingExponent_ = (int) (rint( log10(fabs(max_)) ));
	
	//cout << "SE= " << scalingExponent_ << endl;
	int intervalScaling = (int) ( log10(max_-min_));
	//cout << "IS= " << intervalScaling << endl;
	
	// Now use this to bring max_-min_ into the range [0,10) 
	nmin = floor(min_/pow(10.0,intervalScaling));
	nmax = ceil(max_/pow(10.0,intervalScaling)); // -ve numbers ?

	firstTick_=nmin*pow(10.0,intervalScaling);
	lastTick_ =nmax*pow(10.0,intervalScaling);

	// Some style rules need to be applied to the exponent
	// viz. numbers between 0.1 and 10000 don't have to be
	// written with a multiplier
	 
	//if (scalingExponent_ >= -1 && scalingExponent_ < 4)
	//	scalingExponent_=0;
	
	if (scalingExponent_ -intervalScaling > 1 || fabs(scalingExponent_) <= 4)
		scalingExponent_=0;
	//cout << "SE= " << scalingExponent_ << endl;
	
	// Some style rules to do with the number of intervals to use
	if ((nmax-nmin) > 2) //If nmax - nmin > 2, all is OK */
		nIntervals_= (int) round(nmax-nmin);
	else if ((nmax - nmin) == 2)  // Use 4 intervals 
		nIntervals_=4;
	else if ((nmax - nmin) == 1) // Use 5 intervals 
		nIntervals_=5;

	tickSpacing_ = (lastTick_-firstTick_)/nIntervals_;
 
	//std::cout << "BEF: " << firstTick_ << " " << lastTick_ <<  " exp= " << scalingExponent_ << " nInt= " << nIntervals_ << " ts= " << tickSpacing_ << std::endl;
	// The min tick and max tick are tracked separately because they may not be on
	// "integer" boundaries. So eg if min==firsTick it gets shifted. Also if the
	// first tick precedes min_ (which is usually the case) it gets shifted.
	

	if (min_>=firstTick_){
		int di = ceil((min_-firstTick_)/tickSpacing_);
		firstTick_+= di*tickSpacing_;
		nIntervals_ -= di;
		//std::cout << "Adjust first tick " << di << std::endl;
	}
	if (max_<=lastTick_){  
		int di = ceil((lastTick_-max_)/ tickSpacing_);
		lastTick_ -= di*tickSpacing_;
		nIntervals_ -= di;
		//std::cout << "Adjust last tick " << di << std::endl;
	}
	
	// A tweak
	// The preceding adjustments may have resulted in nIntervals == 1
	// which is a little sparse.

	if (1== nIntervals_)
	{
		tickSpacing_ /= 2.0;
		nIntervals_=2;
		if (firstTick_ - tickSpacing_ > min_)
		{
			firstTick_ -=tickSpacing_;
			nIntervals_++;
		}
		if (lastTick_ + tickSpacing_ < max_)
		{
			lastTick_ += tickSpacing_;
			nIntervals_++;
		}
	}
	
	//std::cout << "AFT: " << firstTick_ << " " << lastTick_ <<  " exp= " << scalingExponent_ << " nInt= " << nIntervals_ << " ts= " << tickSpacing_ << std::endl;
}

