// Copyright 2020 Paul Robertson
//
// PeUtilities.h
//
// Print formatting, math and other utility functions

#pragma once

#include <ctime>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include "PeDefinitions.h"

namespace pe {
namespace formatting {

const std::string kHeading1Dashes("--------");
const std::string kHeading2Dashes("----");
const std::string kHeading3Dashes("--");

// Standard header strings for problems, solutions and methods
std::string ProblemHeader(int problem_number);
std::string ProfileHeader(int problem_number);
std::string SolutionHeader(int solution_number);
std::string MethodHeader(int method_number);

}; // namespace formatting

namespace math {
const double kPhi = 1.6180339887499;

const double kHalfPi = 1.5707963267949;
const double kPi = 3.14159265358979;
const double kTwoPi = 6.28318530717959;

// Generate a map of numbers to Collatz iterations for each number to reach 1.
// Collatz iterations refers to the sequence defined in the Collatz Conjecture
// (wiki link: https://en.wikipedia.org/wiki/Collatz_conjecture):
//    n -> n/2 (n is even)
//    n -> 3n + 1 (n is odd)
// The generation of this map is limited by the depth (maximum number of
// iterations) to search for. It essentially operates "in reverse" to the above
// mapping by starting at 1 and mapping to 2n, and (n-1)/3 if mod(n,6) is 4.
// This search operates recursively up to the specified depth limit.
std::unordered_map<PeUint, PeUint> CollatzGraphIterations(const PeUint depth_limit);

// Construct the sequence formed by performing the iterative mapping defined in
// the Collatz Conjecture (https://en.wikipedia.org/wiki/Collatz_conjecture):
//    n -> n/2 (n is even)
//    n -> 3n + 1 (n is odd)
// This values in the sequence can greatly exceed the starting value.
// For this reason, the returned sequence uses long long (64 bit) integers,
// even though the starting value is limited to only regular (32 bit) integers.
std::vector<PeUint> CollatzSequence(const PeUint starting_value);

// Return a vector containing all factors of <trial_number>,
// from 1...trial_number. O(sqrt(n)).
std::vector<PeUint> Factors(PeUint trial_number);

// O(1) calculation of the Nth Fibonacci number
// May be inaccurate for large N as it uses floating point arithmetic
// Note that N > 46 will overflow standard 32 bit int
PeUint FibonacciDirect(PeUint n);

// O(log N) calculation of the Nth Fibonacci number
// Works with integers so should be exact
// Note that N > 46 will overflow standard 32 bit int,
// so this function will return -1 in these cases.
PeUint FibonacciExact(PeUint n);

// Greatest common divisor
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type Gcd(T a, T b)
{
	// Alternative for a while (true)
	// This could also be implemented using recursion,
	// I've gone with looping here since the code is
	// similarly clear (IMO) and this uses less of the
	// call stack.
    for (;;) {
        if (a == 0) {
			return b;
		}
        b %= a;

        if (b == 0) {
			return a;
		}
        a %= b;
    }
}

// Generate array of primes up to <limit> using the
// Sieve of Eratosthenes method
std::vector<PeUint> GeneratePrimesEratosthenes(PeUint limit);

// Generate array of primes up to <limit> using the
// Sieve of Sundaram method
std::vector<PeUint> GeneratePrimesSundaram(PeUint limit);

// Generate a Pythagorean triple determined by two integers (m, n) such that m > n.
// If m <= n, they will be swapped. This method uses Euclid's formula:
//		a = m^2 - n^2, b = 2*m*n, c = m^2 + n^2
std::tuple<PeUint, PeUint, PeUint> GeneratePythagoreanEuclidTriple(
	PeUint m, PeUint n, PeUint k = 1);

// Parity checks
inline bool IsEven(PeInt n) {
	return !(n & 1);
}

inline bool IsEven(PeUint n) {
	return !(n & 1);
}

inline bool IsOdd(PeInt n) {
	return (n & 1);
}

inline bool IsOdd(PeUint n) {
	return (n & 1);
}

// Test if three integers (a,b,c) are a Pythagorean triple
bool IsPythagoreanTriple(PeUint a, PeUint b, PeUint c);

// Lowest common multiple
PeUint Lcm(PeUint a, PeUint b);

// Find "N choose K", sometimes also written nCk, the number of possible
// combinations of k items from a set of n items.
// Using PeUint (64 bit unsigned) this will first overflow n = 67, k = 32.
// Values of n < 67 won't overflow for any valid k.
PeUint NChooseK(PeUint n, PeUint k);

// Find the prime factors of <trial_number>
// Wheel Factorisation method with a basis of (2,3,5).
std::vector<PeUint> PrimeFactors(PeUint trial_number);

// Reverse an integers digits, useful for testing palindromes
PeUint ReverseDigits(PeUint num);

// The nth pyramid number, the sum of integers from 1 to n
// Uses the analytic formula sum = (n*(n+1))/2
PeUint SumOfOneToN(PeUint n);

// The sum of squared integers: 1^2 + 2^2 + ... n^2
// Uses the analytic formula sum = (n*(n+1)*(2*n+1))/6
PeUint SumOfSquaresOneToN(PeUint n);

// The sum of squared integers: 1^3 + 2^3 + ... n^3
// Uses the analytic formula sum = (n*n*(n+1)*(n+1))/4
PeUint SumOfCubesOneToN(PeUint n);
};

namespace profiling {

// Do a simple "time trial" of a function by running it for a fixed number of
// trials with the same inputs to give a crude assessment of its relative
// performance. Display the average time (using ctime's clock()) per trial on
// the console.
// This uses variadic templating to achieve more general use. An example call:
//
//		TimeProfileFunction<PeUint, PeUint>(100, 1, std::cout, Method1, 20);
//
// This wold call "Method1(20)" 100 times and write the resulting time,
// with a formatted header indicating this is method 1, to the stream std::cout.
template<typename ReturnType, typename ...InputTypes>
void TimeProfileFunction(int method_number,
							int number_of_trials,
							std::ostream &os,
							ReturnType (profile_func)(InputTypes...),
							InputTypes ...inputs)
{
	// Function timing itself
	clock_t start_time(clock());

	for (int i = 0; i < number_of_trials; ++i) {
		profile_func(inputs...);
	}

	clock_t time_taken = clock() - start_time;

	// Formatted output display
	os << formatting::MethodHeader(method_number) << endl << endl <<
		"Time average over " << number_of_trials << " trials: " <<
		static_cast<long double>(time_taken) / static_cast<long double>(number_of_trials) << 
		endl << endl;
}

}; // namespace profiling

}; // namespace pe

