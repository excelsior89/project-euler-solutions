// Copyright 2020-2022 Paul Robertson
//
// PeProblem21.cpp
//
// "Amicable Numbers"

#include "PeProblem21.h"


using namespace std;

namespace pe {

// The naive approach: check every pair of numbers up to (n - 1, n) by calculatingd
// the sum of divisors for both numbers and checking SumOfDivisors(a) == b and
// SumOfDivisors(b) == a. This requires (n-2)*(n-3)/2 iterations, with each
// iteration calling SumOfDivisors twice (note n-2 and n-3 since we start our
// loops at 3 and 2 instead of 1).
static PeUint Method1(PeUint n)
{
	// Quick exit for n = 1
	if (n < 2) {
		return 0;
	}

	// Sum of all amicable numbers
	PeUint overall_sum = 0;

	// Check each number pair up to (n - 2, n - 1)
	for (PeUint i = 3; i < n; ++i) {
		for (PeUint j = 2; j < i; ++j) {
			// If this is true, we have an amicable pair
			// Note the SumOfDivisors(n) function includes n itself in the sum,
			// so we need to make sure we subtract it.
			if ((math::SumOfDivisors(i) - i) == j && (math::SumOfDivisors(j) - j) == i) {
				overall_sum += i + j;
			}
		}
	}

	return overall_sum;
}

// We can improve Method 1 by removing the inner loop: if SumOfDivisors is
// less than the current number we're checking, then we've already checked the
// SumOfDivisors for that new number, so we don't need to re-check it here since
// the pair would already have been identified earlier. The reduces the number
// of iterations to n-3. With the check on j>i, it is also likely that many of
// these iterations will only need to call SumOfDivisors() once, further
// speeding things up.
static PeUint Method2(PeUint n)
{
	// Quick exit for n = 1
	if (n < 2) {
		return 0;
	}

	// Sum of all amicable numbers
	PeUint overall_sum = 0;

	// Calculate the sum of divisors for each number below n
	for (PeUint i = 3; i < n; ++i) {
		PeUint j = math::SumOfDivisors(i) - i;
		
		// If the sum of divisors is less than i, then we've already checked
		// this number so we can skip
		if (j > i && (math::SumOfDivisors(j) - j) == i) {
			// If this is true, we have an amicable pair
			overall_sum += i + j;
		}
	}

	return overall_sum;
}

// Like Method2, we need n-3 iterations, but we use a map to ensure we only call
// the expensive SumOfDivisors() function once per iteration. For lower numbers,
// (~ <1000) this will typically perform similarly to Method2. However, as n
// increases, the guarantee of no duplicated calls to SumOfDivisors shows
// increasing performance gains.
static PeUint Method3(PeUint n)
{
	// Quick exit for n = 1
	if (n < 2) {
		return 0;
	}

	// Sum of all amicable numbers
	PeUint overall_sum = 0;

	// Map of sums to numbers, initialised with the first entry
	unordered_map<PeUint, PeUint> sums_to_numbers_map;

	// Calculate divisor sum for every number up to the limit and store
	// in the map.
	for (PeUint i = 2; i < n; ++i) {
		// We need the sum excluding the number itself
		PeUint divisor_sum = math::SumOfDivisors(i) - i;

		// If we find i as a key in the map, then this number is the divisor sum of another number.
		// The i != divisor_sum check guards against perfect numbers e.g. 6.
		// Since we're doing a single, ascending pass from 2 to n, we only find an amicable pair
		// when the higher number of the pair is reached, so we avoid the risk of double-counting
		// any pairs.
		if (i != divisor_sum) {
			// Insert the divisor sum in the map
			sums_to_numbers_map[divisor_sum] = i;

			auto found_number = sums_to_numbers_map.find(i);

			if (found_number != sums_to_numbers_map.end()) {
				// We've found an amicable pair!
				if (found_number->second == divisor_sum) {
					overall_sum += i + divisor_sum;
				}
			}
		}
	}

	return overall_sum;
}


ostream &PeProblem21::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<

		"Let d(n) be defined as the sum of proper divisors of n (numbers less"  << endl <<
		"than n which divide evenly into n). If d(a) = b and d(b) = a,"  << endl <<
		"where a != b, then a and b are an amicable pair and each of a and b are"  << endl <<
		"called amicable numbers." << endl << endl <<

		"For example, the proper divisors of 220 are:" << endl <<
		"1, 2, 4, 5, 10, 11, 20, 22, 44, 55 and 110; therefore d(220) = 284." << endl <<
		"The proper divisors of 284 are 1, 2, 4, 71 and 142; so d(284) = 220." << endl << endl <<

		"Evaluate the sum of all the amicable numbers under 10000." << endl <<
		"(NOTE: Method1, the \"bad\" method, in this solution takes a while to run...)" << endl << endl;

	return os;
}


ostream &PeProblem21::DisplaySolution(ostream &os)
{
	const PeUint kLimit = 10000;

	auto answer1 = Method1(kLimit);
	auto answer2 = Method2(kLimit);
	auto answer3 = Method3(kLimit);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << answer1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"The naive approach: check every pair of numbers up to (n - 1, n) by" << endl <<
		"calculating the sum of divisors for both numbers and checking" << endl <<
		"SumOfDivisors(a) == b and SumOfDivisors(b) == a. This requires" << endl <<
		"(n-2)*(n-3)/2 iterations, with each iteration calling SumOfDivisors" << endl << 
		"twice (note n-2 and n-3 since we start our loops at 3 and 2 instead of 1)." << endl << endl <<
		"Method 1 solution: " << answer1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"We can improve Method 1 by removing the inner loop: if SumOfDivisors is less" << endl << 
		"than the current number we\'re checking, then we\'ve already checked the" << endl << 
		"SumOfDivisors for that new number, so we don\'t need to re-check it here" << endl << 
		"since the pair would already have been identified earlier. The reduces the" << endl << 
		"number of iterations to n-3. With the check on j>i, it is also likely that" << endl << 
		"many of these iterations will only need to call SumOfDivisors() once," << endl << 
		"further speeding things up." << endl << endl <<
		"Method 2 solution: " << answer2 << endl << endl <<

		formatting::MethodHeader(3) << endl << endl <<
		"Like Method2, we need n-3 iterations, but we use a map to ensure we only call" << endl << 
		"the expensive SumOfDivisors() function once per iteration. For lower numbers," << endl << 
		"(~ <1000) this will typically perform similarly to Method2. However, as n" << endl << 
		"increases, the guarantee of no duplicated calls to SumOfDivisors shows" << endl << 
		"increasing performance gains." << endl << endl <<
		"Method 3 solution: " << answer3 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 10000

PROFILE_SOLUTIONS(PeProblem21, Method1, Method2, Method3)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe