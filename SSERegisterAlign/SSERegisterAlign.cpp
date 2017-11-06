// SSERegisterAlign.cpp : Defines the entry point for the console application.
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

void multiplyAdd(float* buffer1, float* buffer2, float factor, size_t size)
{
	std::cout << "x add buff1 addr: " << &buffer1 << " buff2 addr: " << &buffer2 << std::endl;

	for (size_t i = 0; i < size; ++i)
		/*
		mulps	xmm1, xmm2
		addps	xmm1, xmm0
		movups	XMMWORD PTR [ecx+edx*4], xmm1
		mov	eax, DWORD PTR _buffer2$GSCopy$[ebp]
		mov	ecx, DWORD PTR _buffer1$GSCopy$[ebp]
		movups	xmm1, XMMWORD PTR [eax+edx*4+16]
		movups	xmm0, XMMWORD PTR [ecx+edx*4+16]
		mulps	xmm1, xmm2
		addps	xmm1, xmm0
		movups	XMMWORD PTR [ecx+edx*4+16], xmm1
		add	edx, 8
		cmp	edx, edi
		jb	SHORT $LL4@multiplyAd
		$LN21@multiplyAd:
		cmp	edx, esi
		jae	$LN30@multiplyAd
		movss	xmm1, DWORD PTR _factor$[ebx]
		mov	eax, esi
		sub	eax, edx
		*/
		buffer1[i] += buffer2[i] * factor;
}

using namespace std;

void test()
{
	ExecutionTimer timer;

	const int size = 16384;

	auto b1 = new float[size];
	auto b2 = new float[size];

	/*std::cout << "b1 type: " << typeid(b1).name() << std::endl;
	std::cout << "b2 type: " << typeid(b2).name() << std::endl;

	std::cout << "b1 address: " << &b1 << std::endl;
	std::cout << "b2 address: " << &b2 << std::endl;*/

	// fill with data  . . .

	b1[0] = 0;
	b2[0] = 10;

	for (int i = 1; i < size; i++)
	{
		b1[i] = b1[i-1] + 5;
		b2[i] = 10;
	}
	
	timer.ResetTimer();
	multiplyAdd(b1, b2, 0.00001f, size);
	std::cout << "SSE Reg Aligned Time:\t" << timer.ElapsedTimer() << std::endl;

	/*for (int i = 0; i < size; i = (i + 1000))
	{
		std::cout << "b1[" << i << "] = " << b1[i] << "\t";
		std::cout << "b2[" << i << "] = " << b2[i] << std::endl;
	}*/

	b1[0] = 0;
	b2[0] = 10;

	for (int i = 1; i < size; i++)
	{
		b1[i] = b1[i - 1] + 5;
		b2[i] = 10;
	}

	timer.ResetTimer();
	multiplyAdd(b1 + 1, b2 + 2, 0.00001f, size - 2);
	std::cout << "SSE Reg Unaligned Time:\t" << timer.ElapsedTimer() << std::endl;
	/* for (int i = 0; i < size; i = (i + 1000))
	{
		std::cout << "b1[" << i << "] = " << b1[i] << "\t";
		std::cout << "b2[" << i << "] = " << b2[i] << std::endl;
	}
	*/

	std::cout << endl;
}

int main()
{
	std::cout << "Multiply-Add with Two Float Buffers (SSE Register Alignment)\n";

	test();

    return 0;
}

