// Copyright 2020-2022 Paul Robertson
//
// PeProblem3.cpp
//
// "Largest prime factor"

#include "PeProblem3.h"

#include <numeric>

using namespace std;

namespace pe {
// The simple solution: test dividing by 2, then
// check odd numbers up to sqrt(trial_number).
// return the last number to successfully divide
// the trial number.
static PeUint Method1(PeUint trial_number)
{
	PeUint factor = 1;

	if ((trial_number % 2) == 0) {
		factor = 2;
		while ((trial_number % factor) == 0) {
			trial_number /= factor;
		}
	}

	PeUint trial_sqrt = (PeUint)ceil(sqrt((double)trial_number));

	for (PeUint i = 3; ((i < trial_sqrt) && (i < trial_number)); i += 2) {
		if ((trial_number % i) == 0) {
			factor = i;
			while ((trial_number % factor) == 0) {
				trial_number /= factor;
			}
		}
	}

	return (factor > trial_number ? factor : trial_number);
}

// An improvement on Method1 using wheel factorisation
static PeUint Method2(PeUint trial_number)
{
	/* AN INTERESTING SIDE NOTE BUT NOT PRACTICALLY USEFUL

	// A heuristic for estimating the basis: the basis should consist of
	// primes 1...n, where n is the length of the number's hexidecimal representation


	// If the number's hex length is 4 or more (note check leaves a wide floating point margin)
	// then estimate the nth prime to use when calculating the basis using the upper bound
	// found in: Rosser, Barkley (1941). "Explicit bounds for some functions of prime numbers".
	// American Journal of Mathematics. 63 (1): 211–232. doi:10.2307/2371291

	// Get the length of the number in hex
	double log16_trial = (PeUint)ceil(log2((double)(trial_number)) / 4.0);


	if (log16_trial > 3.5) {
		basis_limit = (PeUint)(ceil(log16_trial*(log(log16_trial*log(log16_trial)))));
	}

	// However, don't go overboard on how many primes to use in the basis,
	// since this isn't the *main* challenge here...
	basis_limit = (basis_limit > 17) ? 17 : basis_limit;

	*/

	// A very crude heuristic:
	// For larger numbers it might be worth the additional time and space
	// to expand the basis to include 7 and 11.
	// This means a wheel modulo of 2310 with an increment vector of
	// size 339. Far fewer checks than just testing odd numbers, but
	// a bigger memory footprint with additional computational time in
	// working out the basis increments, so possibly not worth it.

	// Minimum basis is {2, 3, 5}
	PeUint basis_limit = 5;

	if (trial_number > 1e12) {
		// Sieve function generates up to but not including the limit,
		// set to one above the desired limit.
		basis_limit = 12;
	}
	

	// Note that we could extend the basis limit above past 19, or probably
	// tabulate the estimate of the nth prime for the limited number of lengths,
	// but this is more here as a matter of curiosity. Also consider that for
	// much bigger numbers, this isn't really the algorithm to use for
	// factorisation in general.


	// Generate the trial basis
	const auto basis = math::GeneratePrimesSundaram(basis_limit);

	// Vector to store factors
	// N.B. could be modified to only store the maximum
	vector<PeUint> factors;

	// The modulo (product) of the basis primes
	PeUint basis_mod = accumulate(basis.begin(), basis.end(),
		PeUint(1), multiplies<PeUint>());

	// Trial division by the basis primes
	for (size_t i = 0; i < basis.size(); ++i) {
		while (trial_number % basis[i] == 0) {
			factors.push_back(basis[i]);
			trial_number /= basis[i];
		}
	}

	// Check if we need to do any more factorisation
	if (trial_number == 1) {
		return factors.back();
	}

	// Get the increments for the wheel spokes by first finding all primes up to the modulo
	auto increments = math::GeneratePrimesSundaram(basis_mod + basis.back());

	//... then removing the primes already in the basis
	increments.erase(increments.begin(), increments.begin() + basis.size());

	// ...then completing the "wheel" by appending the value of
	// the first coprime + the product of the basis
	increments.push_back(increments.front() + basis_mod);

	//... then finding the increments between them
	adjacent_difference(increments.begin(), increments.end(), increments.begin());

	// Record the starting point of the wheel spokes
	// (conveniently the adjacent difference function leaves this element unchanged)
	PeUint k = increments.front();
	//... then remove it from the increments array
	increments.erase(increments.begin());


	// Now we test k until k^2 exceeds the trial numbers
	// We increment k using the increment steps in the increments array
	// which is more efficient than simply checking every integer
	size_t i = 0;

	while (k * k <= trial_number) {
		if (trial_number % k == 0) {
			factors.push_back(k);
			trial_number /= k;
		} else {
			k += increments[i];
			++i;

			// Once we reach the last increment, go back to the first
			// (the "revolutions" of the wheel)
			if (i >= increments.size()) {
				i = 0;
			}
		}
	}

	// The largest factor could be the divisor or the result
	return (factors.back() > trial_number ? factors.back() : trial_number);
}


ostream &PeProblem3::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"The prime factors of 13195 are 5, 7, 13 and 29." << endl <<
		"What is the largest prime factor of the number 600851475143 ?" << endl << endl;

	return os;
}


ostream &PeProblem3::DisplaySolution(ostream &os)
{
	const PeUint kTrialNumber = 600851475143;
	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << Method1(kTrialNumber) << endl << endl <<
		formatting::MethodHeader(1) << endl << endl <<
		"A simple approach:" << endl <<
		"Find the prime factors by dividing by 2, then checking odd numbers from 3 to sqrt(n)." << endl <<
		"Since we're only finding the maximum factor, we can discard any lower factors." << endl << endl <<
		"Method 1 solution: " << Method1(kTrialNumber) << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"An improved approach:" << endl <<
		"Use the Wheel Factorisation method, where we use a small basis of primes to" << endl <<
		"form a \"wheel\" of trial factors, thereby skipping checking more known" << endl <<
		"non-prime integers. The most basic case of this is the idea of skipping" << endl <<
		"all even numbers above 2. The next level is to consider {2, 3} as the basis." << endl <<
		"Since 2x3 = 6, we find all coprimes from (3+1) to (3+6):" << endl <<
		"\t = {5, 7}" << endl <<
		"In effect we've eliminated all multiples of 2 and 3 from (3 + 1) to (3 + 2x3)." << endl <<
		"Our trial divisors will be the basis {2, 3} followed by the coprimes {5, 7} and " << endl <<
		"additional \"turns of the wheel\" on {5, 7} by adding 6 to each entry in turn: " << endl <<
		"\t5, 7, 11, 13, 17, 19, 23, 25, 29, 31..." << endl <<
		"Note how this pattern never hits any multiples of 2 or 3. Compare with just " << endl <<
		"testing odd numbers:" << endl <<
		"\t5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31..." << endl <<
		"The first basis only checks 10 of the 14 odd numbers up to 31, however this" << endl <<
		"still tests every possible prime factor." << endl << endl <<
		"Method 2 solution: " << Method2(kTrialNumber) << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 600851475143

PROFILE_SOLUTIONS(PeProblem3, Method1, Method2)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe