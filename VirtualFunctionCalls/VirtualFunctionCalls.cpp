// VirtualFunctionCalls.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<iostream>
#include<vector>
#include<iterator>
#include<algorithm>
#include<string>
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
			<< "\nElapsed Time:\t\t"
			<< std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - mStart).count()
			<< " ms"
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

struct Mammal
{
	virtual float getSomeNumber() const noexcept { return 1.0; }
};

struct Dog : public Mammal
{
	float getSomeNumber() const noexcept override { return 2.0; }
};

struct Cat : public Mammal
{
	float getSomeNumber() const noexcept override { return 2.0; }
};


int main()
{
	std::vector<Mammal*> zoo;
	std::vector<Mammal*> park;

	std::fill_n(std::back_inserter(zoo), 10000, new Mammal);
	std::fill_n(std::back_inserter(zoo), 10000, new Dog);
	std::fill_n(std::back_inserter(zoo), 10000, new Cat);

	std::fill_n(std::back_inserter(park), 10000, new Mammal);
	std::fill_n(std::back_inserter(park), 10000, new Dog);
	std::fill_n(std::back_inserter(park), 10000, new Cat);

	std::random_shuffle(zoo.begin(), zoo.end()); // or not!

	float sumZoo = 0;
	float sumPark = 0;

	ExecutionTimer timer;


	for (auto* animal : park)
		sumPark += animal->getSomeNumber();

	std::cout << "Normal Time (Park):\t" << timer.ElapsedTimer() << std::endl;

	for (auto* animal : zoo)
		sumZoo += animal->getSomeNumber();
	
	std::cout << "Random Time (Zoo):\t" << timer.ElapsedTimer() << std::endl;

	
    return 0;
}

