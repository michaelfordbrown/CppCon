// SIMD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<iostream>
#include<random>
#include<string>
#include<algorithm>
#include<chrono>

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
			<< "\nTotal Elapsed Time: "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mStart).count()
			<< " ms"
			<< std::endl;
	}

	std::string ElapsedTimer()
	{
		std::string s;

		s = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mElapse).count());
		mElapse = Clock::now();

		return s;

	}

private:
	Clock::time_point mStart = Clock::now();
	Clock::time_point mElapse;
};


void simple_mad(float *a, float *b, float *c, float *result, const int length)
{
	std::cout << "simple_mad" << std::endl;
	for (int i = 0; i < length; i++)
	{
		result[i] = a[i] * b[i] + c[i];
	}
}

void optimization()
{
	const int length = 1024 * 1024 * 64;

	float *a = new float[length];
	float *b = new float[length];
	float *c = new float[length];
	float *result = new float[length];

	std::cout << "Optimization" << std::endl;

	std::mt19937_64 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(0, 1);

	//float rng = (rand() % 1000)1;

	for (size_t i = 0; i < length; i++)
	{
		a[i] = dist(rng);
		b[i] = dist(rng);		
		c[i] = dist(rng);
	}

	simple_mad(a, b, c, result, length);

	delete[] a;
	delete[] b;
	delete[] c;
	delete[] result;
}

int main()
{
	std::cout << "Optimization Code" << std::endl;

	ExecutionTimer timer;

	optimization();
    return 0;
}

