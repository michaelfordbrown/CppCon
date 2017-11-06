// SharingBetweenCores.cpp : Defines the entry point for the console application.
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

private:
	Clock::time_point mStart = Clock::now();
	Clock::time_point mElapse;
};


void workatomic(std::atomic<int>& a, const int m)
{
	for (int i = 0; i < m; ++i)
		a = (a + 1) % INT_MAX;
	//std::cout << "Under WORK Procedure a = \t" << a << std::endl;
}

void work(int& a, const int m)
{
	for (int i = 0; i < m; ++i)
		a = (a + 1) % INT_MAX;
	//std::cout << "Under WORK Procedure a = \t" << a << std::endl;
}

void threadtest(const int m)
{
	std::atomic<int> a; a = 0;

	std::thread t1([&]() { workatomic(a, m); });
	std::thread t2([&]() { workatomic(a, m); });
	std::thread t3([&]() { workatomic(a, m); });
	std::thread t4([&]() { workatomic(a, m); });

	t1.join(); t2.join(); t3.join(); t4.join();

	std::cout << "Sum of:\t" << a << "\tfor a count of:\t" << (4*m);
}

void nonconcurrenttest(const int m)
{
	std::atomic<int> a; a = 0;

	std::thread t1([&]() { workatomic(a, m); });
	t1.join();

	std::thread t2([&]() { workatomic(a, m); });
	t2.join();

	std::thread t3([&]() { workatomic(a, m); });
	t3.join();

	std::thread t4([&]() { workatomic(a, m); });
	t4.join();

	std::cout << "Sum of:\t" << a << "\tfor a count of:\t" << (4*m);
}

void nonthreadtest(const int m)
{
	int a; a = 0;

	work(a, m);
	work(a, m);
	work(a, m);
	work(a, m);

	std::cout << "Sum of:\t" << a << "\tfor a count of:\t" << (4*m);
}

int main()
{

	const int m = 300;
	std::cout << "Sharing Between Cores\n\n";

	ExecutionTimer timer;

	threadtest(m);
	std::cout << "\nMulti Thread Time:\t" << timer.ElapsedTimer() << " ns\n\n";

	nonconcurrenttest(m);
	std::cout << "\nSingle Thread Time:\t" << timer.ElapsedTimer() << " ns\n\n";

	nonthreadtest(m);
	std::cout << "\nNo Threads Time:\t" << timer.ElapsedTimer() << " ns\n\n";

    return 0;
}

