// Copyright 2020-2022 Paul Robertson
//
// PeProblem7.cpp
//
// "10001st prime"

#include "PeProblem7.h"

using namespace std;

namespace pe {

// Brute force solution
// Keep generating candidates and check divisibility against
// primes found so far
static PeUint Method1(PeUint n)
{
	// Sanity check
	if (n < 1) {
		return 0;

	// "Base case" quick exits
	} else if (n == 1) {
		return 2;
	} else if (n == 2) {
		return 3;
	} else {
		// Start with 2 and 3 as known primes
		vector<PeUint> primes_so_far({2, 3});

		// Start at 5 (and increment by 2)
		PeUint trial_number = 5, int_sqrt_trial_number = 3;
		bool is_prime = true;

		// Loop until the number of primes found is <n>
		while (primes_so_far.size() < (size_t)n) {
			is_prime = true;
			int_sqrt_trial_number = static_cast<PeUint>(ceil(sqrt((double)trial_number)));

			// Check divisibility with all primes found so far
			auto i = primes_so_far.begin();
			while (is_prime && (i != primes_so_far.end())) {
				// Make sure we don't bother checking above sqrt(trial_number)
				if (*i > int_sqrt_trial_number) {
					break;
				}

				// Check for divisibility and break if a divisor is found
				if (trial_number % *i == 0) {
					is_prime = false;
					break;
				}

				++i;
			}

			// If we broke out of the above loop without finding a divisior, the
			// number is a prime so we add it to the list
			if (is_prime) {
				primes_so_far.push_back(trial_number);
			}

			trial_number += 2;
		}

		return primes_so_far.back();
	}
}

// A different approach from above:
// Instead of testing candidates for primality until N primes are found,
// we instead use the Sieve of Sundaram with a limit known to be above
// the Nth prime and retrieve the Nth prime from the vector generated
static PeUint Method2(PeUint n)
{
	// Sanity check
	if (n < 1) {
		return 0;

	// "Base case" quick exits
	} else if (n == 1) {
		return 2;
	} else if (n == 2) {
		return 3;
	} else {
		double dn = static_cast<double>(n);
		PeUint limit = 0;
		if (n < 20) {
			// J. B. Rosser, The n-th prime is greater than n log n,
			// Proc. London Math. Soc. 45 (1939), 21–44.
			limit = static_cast<PeUint>(ceil(dn*(log(dn) + 2.0 * log(log(dn)))));
		} else {
			// J. B. Rosser and L. Schoenfeld, Approximate formulas for some
			// functions of prime numbers, Illinois J. Math. 6 (1962), 64–94.
			limit = static_cast<PeUint>(ceil(dn*(log(dn) + log(log(dn)) - 0.5)));
		}
		auto primes = math::GeneratePrimesSundaram(limit);
		return primes[n - 1];
	}
}


ostream &PeProblem7::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we" << endl <<
		"can see that the 6th prime is 13. What is the 10001st prime number?" << endl << endl;
	return os;
}


ostream &PeProblem7::DisplaySolution(ostream &os)
{
	const PeUint kN = 10001;

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << Method2(kN) << endl << endl <<
		formatting::MethodHeader(1) << endl << endl <<
		"Keep generating primes until the nth is found." << endl <<
		"This is effectively the same as simple factorisation:" << endl <<
		"starting with a trial number of 3, check the divisibility" << endl <<
		"of this trial number against all the primes found so far" << endl <<
		"up to sqrt(trial_number). If any of the primes divide it," << endl <<
		"it's not prime and we can discard it. If we make it past" << endl <<
		"sqrt(trial_number), we know this trial number is prime and" << endl <<
		"we can add it to the set of primes found so far." << endl <<
		"We end once we have a set of N primes." << endl << endl <<
		"Method 1 solution: " << Method1(kN) << endl << endl <<


		formatting::MethodHeader(2) << endl << endl <<
		"We use an upper bound for the nth prime number due to J.B. Rosser" << endl <<
		"and L. Schoenfeld and generate primes using the Sieve of Sundaram" << endl <<
		"up to this limit. This set will contain at least the N first primes," << endl <<
		"so we can simply return the Nth member of this set." << endl << endl <<
		"Method 2 solution: " << Method2(kN) << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 10001

PROFILE_SOLUTIONS(PeProblem7, Method1, Method2)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe