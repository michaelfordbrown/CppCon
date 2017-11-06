// FalseSharing.cpp : Defines the entry point for the console application.
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

struct alignas (64) aligned_type
{
	std::atomic<int> a;
};

void workatomic(std::atomic<int>& a, const int m)
{
	for (int i = 0; i < m; ++i)
		a = (a + 1) % INT_MAX;
}

void workalign(aligned_type& at, const int m)
{
	for (int i = 0; i < m; ++i)
		at.a = (at.a + 1) % INT_MAX;
}

void work(int& a, const int m)
{
	for (int i = 0; i < m; ++i)
		a = (a + 1) % INT_MAX;
	
}

void testatomic(const int m)
{
	std::atomic<int> a; a = 0;
	std::atomic<int> b; b = 0;
	std::atomic<int> c; c = 0;
	std::atomic<int> d; d = 0;

	std::thread t1([&]() { workatomic(a, m); });
	std::thread t2([&]() { workatomic(b, m); });
	std::thread t3([&]() { workatomic(c, m); });
	std::thread t4([&]() { workatomic(d, m); });

	t1.join(); t2.join(); t3.join(); t4.join();
}

void nonconcurrenttest(const int m)
{
	std::atomic<int> a; a = 0;
	std::atomic<int> b; b = 0;
	std::atomic<int> c; c = 0;
	std::atomic<int> d; d = 0;

	std::thread t1([&]() { workatomic(a, m); });
	t1.join();

	std::thread t2([&]() { workatomic(b, m); });
	t2.join();

	std::thread t3([&]() { workatomic(c, m); });
	t3.join();

	std::thread t4([&]() { workatomic(d, m); });
	t4.join();

}

void testalign(const int m)
{

	aligned_type a, b, c, d;

	std::thread t1([&]() { workalign(a, m); });
	std::thread t2([&]() { workalign(b, m); });
	std::thread t3([&]() { workalign(c, m); });
	std::thread t4([&]() { workalign(d, m); });

	t1.join(); t2.join(); t3.join(); t4.join();
}

int main()
{

	const int m = 400;
	std::cout << "Sharing Between Cores\n";

	ExecutionTimer timer;

	testatomic(m);

	std::cout << "Atomic Time: \t\t" << timer.ElapsedTimer() << " ns" << std::endl;

	testalign(m);

	std::cout << "Aligned Time: \t\t" << timer.ElapsedTimer() << " ns" << std::endl;

	nonconcurrenttest(m);

	std::cout << "Normal Time: \t\t" << timer.ElapsedTimer() << " ns" << std::endl;

	return 0;
}

