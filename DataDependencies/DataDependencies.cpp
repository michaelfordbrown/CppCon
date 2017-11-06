// DataDependencies.cpp : Defines the entry point for the console application.
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

	void ResetTimer()
	{
		mElapse = Clock::now();
	}

private:
	Clock::time_point mStart = Clock::now();
	Clock::time_point mElapse;
};

unsigned atoui(const char* c)
{
	unsigned result = 0;

	while (*c >= '0' && *c <= '9')
		result = result * 10 + (*c - '0');

	return result;
}

void bad()
{
	int a[1000] = { 5 };
	int b[1000] = { 10 };
	int c[1000] = { 20 };



	for (int i = 0; i <= 998; ++i)
	{
		/*
		* Assembly: No Optimization
		mov	eax, DWORD PTR _i$1[ebp]
		mov	ecx, DWORD PTR _a$[ebp+eax*4]
		mov	edx, DWORD PTR _i$1[ebp]
		add	ecx, DWORD PTR _b$[ebp+edx*4]
		mov	eax, DWORD PTR _i$1[ebp]
		mov	DWORD PTR _a$[ebp+eax*4], ecx

		*/

		/*
		* Assembly: Level 3 Optimization (No RTC)
		mov	ecx, DWORD PTR _b$[ebp+eax]
		add	DWORD PTR _a$[ebp+eax], ecx
		*/

		a[i] += b[i];

		/*
		* Assembly: No Optimization
		mov	eax, DWORD PTR _i$1[ebp]
		mov	ecx, DWORD PTR _b$[ebp+eax*4+4]
		mov	edx, DWORD PTR _i$1[ebp]
		add	ecx, DWORD PTR _c$[ebp+edx*4]
		mov	eax, DWORD PTR _i$1[ebp]
		mov	DWORD PTR _b$[ebp+eax*4+4], ecx
		*/

		/*
		* Assembly: Level 3 Optimization (No RTC)
		mov	ecx, DWORD PTR _c$[ebp+eax]
		add	DWORD PTR _b$[ebp+eax+4], ecx
		add	eax, 4
		cmp	eax, 3992				; 00000f98H
		jle	SHORT $LL8@bad
		*/
				b[i + 1] += c[i];

		/*
		jmp	SHORT $LN2@bad
		$LN3@bad:
		*/
	}

	std::cout << "a[0] = " << a[0] << std::endl;
	std::cout << "a[997] = " << a[997] << std::endl;
	std::cout << "a[998] = " << a[998] << std::endl;
	std::cout << "a[999] = " << a[999] << std::endl;

	std::cout << "b[0] = " << b[0] << std::endl;
	std::cout << "b[997] = " << b[997] << std::endl;
	std::cout << "b[998] = " << b[998] << std::endl;
	std::cout << "b[999] = " << b[999] << std::endl;

	std::cout << "c[0] = " << c[0] << std::endl;
	std::cout << "c[997] = " << c[997] << std::endl;
	std::cout << "c[998] = " << c[998] << std::endl;
	std::cout << "c[999] = " << c[999] << std::endl;
}

void better()
{
	int a[1000] = { 5 };
	int b[1000] = { 10 };
	int c[1000] = { 20 };

	a[0] += b[0];

	for (int i = 1; i <= 997; ++i)
	{
		/* 
		 * Here the number of iterations has been reduced from (0 - 999) to (1 - 997)
		 *
		 * Now current iteration does depend upon previous there can use
		 * loop vectorisation:
		 *
		 * 
		 */

		/*
		 * Assembly: No Optimization
		mov	eax, DWORD PTR _i$1[ebp]
		mov	ecx, DWORD PTR _b$[ebp+eax*4+4]
		mov	edx, DWORD PTR _i$1[ebp]
		add	ecx, DWORD PTR _c$[ebp+edx*4]
		mov	eax, DWORD PTR _i$1[ebp]
		mov	DWORD PTR _b$[ebp+eax*4+4], ecx
		*/

		/*
		* Assembly: Level 3 Optimization (No RTC)
		movups	xmm0, XMMWORD PTR _b$[ebp+eax*4+4]
		movups	xmm1, XMMWORD PTR _c$[ebp+eax*4]
		paddd	xmm1, xmm0
		movups	xmm0, XMMWORD PTR _a$[ebp+eax*4+4]
		movups	XMMWORD PTR _b$[ebp+eax*4+4], xmm1
		*/

		b[i + 1] += c[i];

		/*
		 * Assembly: No Optimization
		mov	eax, DWORD PTR _i$1[ebp]
		mov	ecx, DWORD PTR _a$[ebp+eax*4+4]
		mov	edx, DWORD PTR _i$1[ebp]
		add	ecx, DWORD PTR _b$[ebp+edx*4+4]
		mov	eax, DWORD PTR _i$1[ebp]
		mov	DWORD PTR _a$[ebp+eax*4+4], ecx
		*/

		/*
		* Assembly: Level 3 Optimization (No RTC)
		paddd	xmm1, xmm0
		movups	xmm0, XMMWORD PTR _b$[ebp+eax*4+20]
		movups	XMMWORD PTR _a$[ebp+eax*4+4], xmm1
		movups	xmm1, XMMWORD PTR _c$[ebp+eax*4+16]
		paddd	xmm1, xmm0
		movups	xmm0, XMMWORD PTR _a$[ebp+eax*4+20]
		movups	XMMWORD PTR _b$[ebp+eax*4+20], xmm1
		paddd	xmm1, xmm0
		movups	XMMWORD PTR _a$[ebp+eax*4+20], xmm1
		add	eax, 8
		cmp	eax, 992				; 000003e0H
		jle	SHORT $LL4@better
		 */

		a[i + 1] += b[i + 1];

		/*
		* Assembly: No Optimization
		jmp	SHORT $LN2@better
		$LN3@better:
		*/
	}

	b[999] += c[998];

	std::cout << "a[0] = " << a[0] << std::endl;
	std::cout << "a[997] = " << a[997] << std::endl;
	std::cout << "a[998] = " << a[998] << std::endl;
	std::cout << "a[999] = " << a[999] << std::endl;

	std::cout << "b[0] = " << b[0] << std::endl;
	std::cout << "b[997] = " << b[997] << std::endl;
	std::cout << "b[998] = " << b[998] << std::endl;
	std::cout << "b[999] = " << b[999] << std::endl;

	std::cout << "c[0] = " << c[0] << std::endl;
	std::cout << "c[997] = " << c[997] << std::endl;
	std::cout << "c[998] = " << c[998] << std::endl;
	std::cout << "c[999] = " << c[999] << std::endl;
}

void best()
{
	int a[1000] = { 5 };
	int b[1000] = { 10 };
	int c[1000] = { 20 };

	a[1] += b[1];

	for (int i = 1; i <= 997; ++i)
	{
		b[i + 1] += c[i];
		a[i + 1] += b[i + 1];
	}

	b[999] += c[998];

	std::cout << "a[0] = " << a[0] << std::endl;
	std::cout << "a[997] = " << a[997] << std::endl;
	std::cout << "a[998] = " << a[998] << std::endl;
	std::cout << "a[999] = " << a[999] << std::endl;

	std::cout << "b[0] = " << b[0] << std::endl;
	std::cout << "b[997] = " << b[997] << std::endl;
	std::cout << "b[998] = " << b[998] << std::endl;
	std::cout << "b[999] = " << b[999] << std::endl;

	std::cout << "c[0] = " << c[0] << std::endl;
	std::cout << "c[997] = " << c[997] << std::endl;
	std::cout << "c[998] = " << c[998] << std::endl;
	std::cout << "c[999] = " << c[999] << std::endl;
}

int main()
{
	std::cout << "Data Dependencies\n";

	ExecutionTimer timer;

	/*std::cout << "\nBad Function:\n";
	timer.ResetTimer();
	bad();
	std::cout << "\nBad Time: \t\t" << timer.ElapsedTimer() << " ns" << std::endl;*/

	std::cout << "\nBetter Function:\n";
	timer.ResetTimer();
	better();
	std::cout << "\nBetter Time: \t\t" << timer.ElapsedTimer() << " ns" << std::endl;

    return 0;
}

