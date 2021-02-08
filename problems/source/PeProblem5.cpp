// Copyright 2020-2021 Paul Robertson
//
// PeProblem5.cpp
//
// "Smallest multiple"

#include "PeProblem5.h"

using namespace std;

namespace pe {

// Iteratively find the lowest common multiple
// from 1 to n
static PeUint Method1(PeUint n)
{
	PeUint current_lcm = 1;

	for (PeUint i = 1; i < n; ++i) {
		current_lcm = math::Lcm(current_lcm, i);
	}
		
	return current_lcm;
}

// Highest index prime factors
// Find the prime factors of each number
// from 1 to n and select the highest index of each,
// then compute the product
static PeUint Method2(PeUint n)
{
	// Quick return
	if (n == 1) {
		return 1;
	}

	// Store the indices in a map
	// We know that 2, at least, will be a factor here
	map<PeUint, PeUint> factor_indices({{2, 1}});

	// Iterate over each number from 2 to n
	for (PeUint i = 2; i <= n; ++i) {
		auto factors = math::PrimeFactors(i);

		// If i is prime, we know it won't have occured yet in the factor
		// indices, so we can add it to factor_indices with an index of 1
		if ((factors.size() == 1)) {
			factor_indices[factors.front()] = 1;
			continue;
		}

		// Otherwise, we need to do a bit more...

		PeUint ind = 1, current_factor = factors.front();
		
		// Tally factors into index form and update the map of factor indices
		for (auto i_this_fact = factors.begin() + 1;
			i_this_fact != factors.end(); ++i_this_fact) {
			// If the current factor is the same as the previous factor, increase the
			// index count by 1
			if (*i_this_fact == current_factor) {
				++ind;
			} else {
				// If the current factor is not the same as the next factor
				// add the index of the current factor to factor indices, provided
				// it's greater than this factor's index
				// E.g. if we've found 3^3, we want to add factor_indices[3] = 3,
				// but only if factor_indices[3] is not already greater than 3
				if (factor_indices.find(current_factor) != factor_indices.end()) {
					if (factor_indices.at(current_factor) < ind) {
						factor_indices[current_factor] = ind;
					}
				} else { // Factor not in indices map yet, add it
					factor_indices[current_factor] = ind;
				}

				// Reset index count and current factor
				ind = 1;
				current_factor = *i_this_fact;
			}
		}

		// Catch the last group from the above loop
		if (factor_indices.find(current_factor) != factor_indices.end()) {
			if (factor_indices.at(current_factor) < ind) {
				factor_indices[current_factor] = ind;
			}
		} else { // Factor not in indices map yet, add it
			factor_indices[current_factor] = ind;
		}
	}


	// Find the product of the max index factors
	PeUint lcm_product = 1;

	for (const auto &fact : factor_indices) {
		// Could use pow() here but this may be slightly faster
		for (PeUint i = 0; i < fact.second; ++i) {
			lcm_product *= fact.first;
		}
	}

	return lcm_product;
}

// Also using highest index prime factors, but instead of factorising each
// number up to n, we can observe that between 1 and n we will have an index
// of every prime up to n. The index will be the highest power of each prime
// that is less than n. E.g. for n = 10, we know we'll have primes {2,3,5,7}.
// The highest index of each prime is:
//   2^3 = 8 (because 2^4 = 16 > 10)
//   3^2 = 9 (3^3 = 27 > 10)
//   5^1 = 5 (5^2 = 25 > 10)
//   7^1 = 7 (7^2 = 49 > 10)
// Hence the LCM of digits 1...10 is 8*9*5*7 = 2520
static PeUint Method3(PeUint n)
{
	PeUint lcm_value = 1;

	// Find primes less than n
	auto primes_to_n = math::GeneratePrimesSundaram(n);

	// Find the maximum power of each prime such that p^k <= n,
	// where p is the prime number
	for (auto i = primes_to_n.begin(); i != primes_to_n.end(); ++i) {
		auto i_trial = *i;

		while (i_trial < n) {
			i_trial *= *i;
		}

		*i = i_trial / *i;
	}
	
	// Return the product of all the powers of primes using std::acculumate with
	// std::multiples as the predicate
	return accumulate(primes_to_n.begin(), primes_to_n.end(), PeUint(1), multiplies<PeUint>());
}


ostream &PeProblem5::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"2520 is the smallest number that can be divided by each of the numbers" << endl <<
		"from 1 to 10 without any remainder. What is the smallest positive number " << endl <<
		"that is evenly divisible by all of the numbers from 1 to 20?" << endl << endl;

	return os;
}


ostream &PeProblem5::DisplaySolution(ostream &os)
{
	const PeUint kN = 20;
	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << Method1(kN) << endl << endl <<
		formatting::MethodHeader(1) << endl << endl <<
		"This method finds the lowest common multiple iteratively from 1 to" << endl <<
		"the supplied number i.e lcm_n = Lcm(n, lcm_(n-1))." << endl << endl <<
		"Method 1 solution: " << Method1(kN) << endl << endl <<


		formatting::MethodHeader(2) << endl << endl <<
		"The lowest common multiple of a set of numbers is the product of" << endl <<
		"the highest index of each prime factor present in the set of all" << endl <<
		"the numbers' prime factors. E.g. to find the LCM of {4,5,6}:" << endl <<
		"  Find factors: {{2^2}, {5^1}, {2^1, 3^1}}" << endl <<
		"  The set of all prime factors: {2,3,5}" << endl <<
		"  The highest index of each prime factor: {2^2,3^1,5^1}" << endl <<
		"  The LCM is therefore: 2^2 * 3^1 * 5^1} = 4*3*5 = 60" << endl << endl <<
		"Method 2 solution: " << Method2(kN) << endl << endl <<


		formatting::MethodHeader(3) << endl << endl <<
		"Similar to method 2, except that instead of factorising each number" << endl <<
		"up to n, we instead use the fact that every prime up to n will factorise" << endl <<
		"every other number up to n. The maximum power of any of these primes will" << endl <<
		"be the highest power such that p^k <= n. The LCM is, like in method 2," << endl <<
		"is then the product of all these powers of primes." << endl << endl <<
		"Method 3 solution: " << Method3(kN) << endl << endl;
	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 20

PROFILE_SOLUTIONS(PeProblem5, Method1, Method2, Method3)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe