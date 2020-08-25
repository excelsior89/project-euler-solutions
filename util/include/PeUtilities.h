#pragma once

#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>



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


// Manipulators for changing text colour
namespace ansi {
// Reset colour to default
template <class CharT, class Traits>
constexpr
std::basic_ostream<CharT, Traits> &Reset(std::basic_ostream<CharT, Traits> &os)
{
	return os << "\033[0m";
}

// Make text red
template <class CharT, class Traits>
constexpr
std::basic_ostream<CharT, Traits> &TextRed(std::basic_ostream<CharT, Traits> &os)
{
	return os << "\033[31m";
}

} // namespace ansi

};

namespace math {
const double kPhi = 1.6180339887499;

const double kHalfPi = 1.5707963267949;
const double kPi = 3.14159265358979;
const double kTwoPi = 6.28318530717959;

// Return a vector containing all factors of <trial_number>,
// from 1...trial_number. O(sqrt(n)).
std::vector<unsigned> Factors(unsigned trial_number);

// O(1) calculation of the Nth Fibonacci number
// May be inaccurate for large N as it uses floating point arithmetic
// Note that N > 46 will overflow standard 32 bit int
int FibonacciDirect(int n);

// O(log N) calculation of the Nth Fibonacci number
// Works with integers so should be exact
// Note that N > 46 will overflow standard 32 bit int,
// so this function will return -1 in these cases.
int FibonacciExact(int n);

// Generate array of primes up to <limit> using the
// Sieve of Eratosthenes method
std::vector<unsigned> GeneratePrimesEratosthenes(unsigned limit);

// Generate array of primes up to <limit> using the
// Sieve of Sundaram method
std::vector<unsigned> GeneratePrimesSundaram(unsigned limit);

// Generate a Pythagorean triple determined by two integers (m, n) such that m > n.
// If m <= n, they will be swapped. This method uses Euclid's formula:
//		a = m^2 - n^2, b = 2*m*n, c = m^2 + n^2
std::tuple<unsigned, unsigned, unsigned> GeneratePythagoreanEuclidTriple(
	unsigned m, unsigned n, unsigned k = 1);

// Greatest common divisor
int Gcd(int a, int b);

// Test if three integers (a,b,c) are a Pythagorean triple
bool IsPythagoreanTriple(unsigned a, unsigned b, unsigned c);

// Lowest common multiple
int Lcm(int a, int b);

// Find the prime factors of <trial_number>
// Wheel Factorisation method with a basis of (2,3,5).
std::vector<unsigned> PrimeFactors(unsigned trial_number);

// Reverse an integers digits, useful for testing palindromes
int ReverseDigits(int num);

// The nth pyramid number, the sum of integers from 1 to n
// Uses the analytic formula sum = (n*(n+1))/2
int SumOfOneToN(int n);

// The sum of squared integers: 1^2 + 2^2 + ... n^2
// Uses the analytic formula sum = (n*(n+1)*(2*n+1))/6
int SumOfSquaresOneToN(int n);

// The sum of squared integers: 1^3 + 2^3 + ... n^3
// Uses the analytic formula sum = (n*n*(n+1)*(n+1))/4
int SumOfCubesOneToN(int n);
};

namespace profiling {
const int kNumberOfTrials = 1000;
}; // namespace profiling

}; // namespace pe

