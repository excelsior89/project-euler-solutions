// Copyright 2020-2022 Paul Robertson
//
// PeProblem10.cpp
//
// "Summation of primes"

#include "PeProblem10.h"

using namespace std;

namespace pe {

// Use the Sieve of Eratosthenes to generate primes until the summation limit
// is met. We're using 64 bit integers since the problem statement could start
// pushing us towards quite large values.
static PeUint Method1(PeUint limit)
{
	PeUint sum = 0;

	// Get the array of primes
	auto primes_vec = math::GeneratePrimesEratosthenes(limit);

	// STL accumulate to calulcate the sum
	sum = accumulate(primes_vec.begin(), primes_vec.end(), PeUint(0));
	
	return sum;
}

// For this method we're doing a couple of improvements:
// We're using the Sieve of Sundaram instead of the Sieve of Eratosthenes.
// We're also directly using the algorithm and modifying it to record only
// the sum instead of an array of primes, which avoids using the space for
// the array and uses only the loops for the sieve algorithm, saving an
// extra summation loop.
static PeUint Method2(PeUint limit)
{
	if (limit < 2) {
		return 0;
	} else if (limit == 2) {
		return 2;
	} else {
		// 2, being the only even prime, gets specially included first
		PeUint primes_sum = 2;

		// Set up an array of flags
		vector<bool> base_candidates(limit / 2, true);

		PeUint k = limit / 3; // Integer division

		// Sieve of Sundaram
		// Mark off odd integers of the form i + j + 2ij
		for (PeUint j = 1; j < k; ++j) {
			for (PeUint i = 1; i <= j; ++i) {
				if (i + j + 2 * i * j >= base_candidates.size()) {
					break;
				}
				base_candidates[i + j + 2 * i * j] = false;
			}
		}

		// Collect the sum for all remaining "true" candidates, which
		// must be prime
		for (PeUint i = 1; i < base_candidates.size(); ++i) {
			if (base_candidates[i]) {
				primes_sum += (PeUint)(2 * i + 1);
			}
		}

		return primes_sum;
	}
}


ostream &PeProblem10::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17." << endl <<
		"Find the sum of all the primes below two million." << endl << endl;

	return os;
}


ostream &PeProblem10::DisplaySolution(ostream &os)
{
	const PeUint kTargetSum = 2000000;

	auto sum1 = Method1(kTargetSum);
	auto sum2 = Method2(kTargetSum);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << sum1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"Here we call the Sieve of Eratosthenes method we implemented for an" << endl <<
		"earlier problem, the simply sum the returned array of primes." << endl << endl <<
		"Method 1 solution: " << sum1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"In this implementation we use the Sieve of Sundaram (an improvement" << endl <<
		"over the Sieve of Eratosthenes), but we change the algorithm to record" << endl <<
		"the sum directly rather than first record an array of primes, saving" << endl <<
		"on memory space and removing an additional for loop." << endl << endl <<
		"Method 2 solution: " << sum2 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 1000000

PROFILE_SOLUTIONS(PeProblem10, Method1, Method2)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe