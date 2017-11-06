// DoSomeMultiplications.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<iostream>
#include<vector>
#include<iterator>
#include<algorithm>
#include<string>
#include<chrono>
#include<thread>
#include<atomic>
#include<typeinfo>
#include<cmath>
#include<cfloat>

class ExecutionTimer
{
public:
	using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock,
		std::chrono::steady_clock>;

	ExecutionTimer() {
		mElapse = mStart;
	};

	~ExecutionTimer()
	{
		std::cout
			<< "\nElapsed Time:\t\t"
			<< std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - mStart).count()
			<< " ns"
			<< std::endl;
	}

	std::string ElapsedTimer()
	{
		std::string s;

		s = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - mElapse).count());
		mElapse = Clock::now();

		return s;

	}

	void ResetTimer()
	{
		mElapse = Clock::now();
	}

private:
	Clock::time_point mStart = Clock::now();
	Clock::time_point mElapse;
};

float do_some_multiplications(float x)
{
	for (int i = 0; i < 10000; ++i)
		x *= 0.999f;

	return x;
}

int main()
{
	ExecutionTimer timer;

	/*
	 * 1.0/0.0 is an infinity value (inf)
	 * 0.0/0.0 is a Not-A-Number (NaN)
	 */

	float numberdata[5];

	numberdata[0] = (float)1.0;
	numberdata[1] = INFINITY;
	numberdata[2] = NAN;
	numberdata[3] = -0.0;

	/*
	 * denormal numbers or denormalized numbers (now often called subnormal numbers) 
	 * fill the underflow gap around zero in floating-point arithmetic. 
	 * Any non-zero number with magnitude smaller than the smallest normal number is 'subnormal'.
	 * Such numbers are represented using leading zeros in the significand.
	 *
	 * The significand (or mantissa) of an IEEE floating point number is the part of a floating-point 
	 * number that represents the significant digits. 
	 */
	numberdata[4] = (float) 1e-39; //denormal 

	for (int i = 0; i < 5; i++)
	{
		std::cout << "Denormals" << std::endl;

		timer.ResetTimer();
		do_some_multiplications(numberdata[i]);
		std::cout << "Time for: " << numberdata[i] << "\t\t" << timer.ElapsedTimer() << std::endl;
	}

    return 0;
}

