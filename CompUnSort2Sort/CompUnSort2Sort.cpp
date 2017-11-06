// CompUnSort2Sort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

#include<chrono>




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

	string ElapsedTimer()
	{
		string s;

		s = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - mElapse).count());
		mElapse = Clock::now();

		return s;
			
	}

	void ExecutionTimer::ResetTimer()
	{
		mElapse = Clock::now();
	}

private:
	Clock::time_point mStart = Clock::now();
	Clock::time_point mElapse;
};


ExecutionTimer timer;
int fc = 0;


int function_count(bool bSort)
{
	int c = 0;

	/*
	* Sequence container that encapsulates dynamic size arrays, elements are stored contiguously
	*/
	std::vector<float> v(32768);
	std::vector<float> d(32768);

	//std::vector<float> v(100000);
	//std::vector<float> d(100000);
	
	/*
	* Generate values for range with function,
	* Assigns the value returned by successive calls to gen to the elements in the range [first,last)
	*/
	std::generate(v.begin(),
		v.end(),
		/*
		* Lambda expression that randomly (pseudo-random, 0 to RAND_MAX) returns 1 or -1
		*/
		[] {return (rand() % 2) ? 1 : -1; });

	std::generate(d.begin(),
		d.end(),
		/*
		* Lambda expression that randomly (pseudo-random, 0 to RAND_MAX) returns 1 or -1
		*/
		[] {return (rand() % 2) ? 1 : -1; });

	/*
	 * Sorts the elements in the range [first, last) in ascending order
	 */
	if (bSort)
	{
		std::sort(v.begin(), v.end());
	}
	else
	{
		std::sort(d.begin(), d.end());
	}

	/*
	 * count_if, Returns the number of elements in the range [first, last) satisfying specific criteria.
	 * 
	 * Lambda returning number of +1's
	 */

	timer.ResetTimer();

	c = std::count_if(v.begin(),
								   v.end(),
								   [](float x) { return x > 0; });

	std::cout << c << "\t\t" << timer.ElapsedTimer() << "ns\t\t";
	if (bSort)
		cout << endl;

	return c;
}

int main()
{
	cout << "Counting Performance Test\n";

	cout << "Unsorted: \t\t\t Sorted:\n";
	cout << "Count\t\tTime\t\tCount\t\tTime\n";
	for (int i = 0; i < 25; i++)
	{
		fc = function_count(false);
		fc = function_count(true);

	}
	cout << endl;

	return 0;
}
