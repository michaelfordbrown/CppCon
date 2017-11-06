// SimpleModelFailure.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<iostream>
#include<random>
#include<string>
#include<algorithm>
#include<array>
#include<chrono>
#include<math.h>

using namespace std;

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

private:
	Clock::time_point mStart = Clock::now();
	Clock::time_point mElapse;
};

int main()
{
	std::cout << "Simple Hardware Model Failure" << std::endl;

	ExecutionTimer timer;

	const int n = 100;

	int array[n][n];

	// row major traversal:

	for (int s = 0; s < 2; s++)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				switch (s)
				{
					case 1:
					{
						array[i][j] += (int)(std::sqrt(std::hash<int>() (j*n + i))) % 1000;
						break;
					}
					default:
					{
						array[i][j] = 1000;
						break;
					}
				}

			}
		}
		std::cout << "Row Major Elapsed Time (" << s << "): " << timer.ElapsedTimer() << " ns" << std::endl;
	}
	cout << endl;


	// column major traversal:

	for (int s = 0; s < 2; s++)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				switch (s)
				{
				case 1:
				{
					array[j][i] += (int)(std::sqrt(std::hash<int>() (j*n + i)))%1000;
					break;
				}
				default:
				{
					array[j][i] = 1000;
					break;
				}
				}

			}
		}
		std::cout << "Row Major Elapsed Time (" << s << "): " << timer.ElapsedTimer() << " ns" << std::endl;
	}
	cout << endl;

    return 0;
}

