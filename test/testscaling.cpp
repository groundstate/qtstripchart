
#include <iostream>
#include <iomanip>
#include <string>

#include <cmath>
#include <cstdlib>

	bool autoScale_;
		double min_,max_;
		double firstTick_,lastTick_;
		double absMin_,absMax_;
		double tickSpacing_;
		int scalingExponent_;
		int nIntervals_;

		using namespace std;

string formatNumber(double x,int e,double ts,double *d)
{
	char buf[32];
	string r;
	int sf;
	
	if (e >= -1 && e < 4){
		*d = 1.0;
		int dprec = -round(log10(ts)); // significant figures after the DP
		cout << "dprec = " << dprec << endl;
		if(dprec > 0){  // have digits after the DP
			double tsmround= round(trunc((ts/pow(10,-dprec))));
			if (tsmround < ts)
				dprec++;
		}
		cout << "dprec = " << dprec << endl;
		std::cout << std::setiosflags(ios::fixed) << std::setprecision(dprec) << x << endl;
	}
	else
		*d = pow(10.0,e);
		
	
	return r;
}

void setTicks()
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
	
	cout << "SE= " << scalingExponent_ << endl;
	int intervalScaling = (int) ( log10(max_-min_) );
	cout << "IS= " << intervalScaling << endl;
	
	// Now use this to bring max_-min_ into the range [0,10) 
	nmin = floor(min_/pow(10.0,intervalScaling));
	nmax = ceil(max_/pow(10.0,intervalScaling)); // -ve numbers ?

	cout << "nmin= " << nmin << " nmax=" << nmax << endl;
	
	firstTick_=nmin*pow(10.0,intervalScaling);
	lastTick_ =nmax*pow(10.0,intervalScaling);

	// Some style rules need to be applied to the exponent
	// viz. numbers between 0.1 and 10000 don't have to be
	// written with a multiplier
	 
	//if (scalingExponent_ >= -1 && scalingExponent_ < 4)
	//	scalingExponent_=0;
	
	if (scalingExponent_ -intervalScaling > 1)
		scalingExponent_=0;
	cout << "SE= " << scalingExponent_ << endl;
	
	// Some style rules to do with the number of intervals to use
	if ((nmax-nmin) > 2) //If nmax - nmin > 2, all is OK */
		nIntervals_= (int) round(nmax-nmin);
	else if ((nmax - nmin) == 2)  // Use 4 intervals 
		nIntervals_=4;
	else if ((nmax - nmin) == 1) // Use 5 intervals 
		nIntervals_=5;

	tickSpacing_ = (lastTick_-firstTick_)/nIntervals_;
 
	std::cout << "BEF: " << firstTick_ << " " << lastTick_ <<  " exp= " << scalingExponent_ << " nInt= " << nIntervals_ << " ts= " << tickSpacing_ << std::endl;
	// The min tick and max tick are tracked separately because they may not be on
	// "integer" boundaries. So eg if min==firsTick it gets shifted. Also if the
	// first tick precedes min_ (which is usually the case) it gets shifted.
	

	if (min_>=firstTick_){
		int di = ceil((min_-firstTick_)/tickSpacing_);
		firstTick_+= di*tickSpacing_;
		nIntervals_ -= di;
		std::cout << "Adjust first tick " << di << std::endl;
	}
	if (max_<=lastTick_){  
		int di = ceil((lastTick_-max_)/ tickSpacing_);
		lastTick_ -= di*tickSpacing_;
		nIntervals_ -= di;
		std::cout << "Adjust last tick " << di << std::endl;
	}
	
	// A tweak
	// The preceding adjustments may have resulted in nIntervals == 1
	// which is a little sparse.

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
	
	std::cout << "AFT: " << firstTick_ << " " << lastTick_ <<  " exp= " << scalingExponent_ << " nInt= " << nIntervals_ << " ts= " << tickSpacing_ << std::endl;
}

int main(int argc, char **argv)
{
	min_=atof(argv[1]);
	max_=atof(argv[2]);
	setTicks();
	std::cout << std::setiosflags(ios::fixed) << std::setprecision(2) << firstTick_ << " " << lastTick_ << " " << scalingExponent_ << " " << nIntervals_ << " " << tickSpacing_ << std::endl;
	std::cout << firstTick_ << " " << lastTick_ << " "<<  firstTick_+tickSpacing_ << std::endl;
	
	double formattedNumber;
	formatNumber(firstTick_,scalingExponent_,tickSpacing_,&formattedNumber);
	formatNumber(lastTick_,scalingExponent_,tickSpacing_,&formattedNumber);
}
