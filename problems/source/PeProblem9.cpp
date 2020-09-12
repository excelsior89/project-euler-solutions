// Copyright 2020 Paul Robertson
//
// PeProblem9.cpp
//
// "Special Pythagorean triplet"

#include "PeProblem9.h"

using namespace std;

namespace pe {


// Simple search
// Return product abc if a triple (a,b,c) that makes the sum is found, otherwise return 0.
// Store the triple, if found, in <triple>.
static PeUint Method1(PeUint triple_sum, tuple<PeUint, PeUint, PeUint> &triple)
{
	// Based on Euclid's formula, the sum a + b + c
	// is equivalent to 2 * k * m * m + 2 * k * m * n.
	// We can search for the correct sum using this formula
	// instead of having to calculate a, b and c first.
	//
	// A _VERY_ loose upper bound on the search space is
	// k < triple_sum since, m < triple_sum, n < triple_sum

	// We get a glorious O(n^3) loop here
	for (PeUint k = 1; k < triple_sum; ++k) {
		for (PeUint n = 1; n < triple_sum; ++n) {
			for (PeUint m = n + 1; m < triple_sum; ++m) {
				// Check the sum
				if ((2 * k * m * m + 2 * k * m * n) == triple_sum) {
					// Set the triple
					get<0>(triple) = k * m * m - k * n * n;
					get<1>(triple) = 2 * k * m * n;
					get<2>(triple) = k * m * m + k * n * n;

					return get<0>(triple) * get<1>(triple) * get<2>(triple);
				}
			}
		}
	}

	return 0;
}

// Better searching bounds than Method 1
static PeUint Method2(PeUint triple_sum, tuple<PeUint, PeUint, PeUint> &triple)
{
	// The lowest Pythagorean triple sum is 12, so
	// we can quickly exit for a sum less than 12
	if (triple_sum < 12) {
		return 0;
	}

	// Based on Euclid's formula, the sum a + b + c
	// is equivalent to 2 * k * m * m + 2 * k * m * n.
	//
	// We know that m > n and all of k, m, n are > 0.
	// We can use this to determine some maximum values for (k,m,n).
	//
	// For k:
	// k = X / (2*m*(m+n))
	// If m = 2, n = 1 (min values of m, n):
	//   = X / (2*2*3)
	//   = X / 12 ---> max k
	//
	// n = (X - 2*k*m*m) / (2*k*m)
	// If m = 2, k = 1 (min values of m, k):
	//   = (X - 8) / 4 ---> max n
	//
	// For m:
	// 2*k*m*(m+n) = X
	//
	// If n, k = 1 (min values of n, k):
	// 2*m*(m+1) = X
	// 2*m*m + 2*m - X = 0
	// m = -1/2 +- sqrt(1/4 + X/2)
	// Choose +sqrt for max value.

	// We can also introduce some divisibility checks:
	// The sum of the triple must be equal to 2*k*m*(m+n).
	// We therefore know that a "valid" sum must be divisible by
	// 2, k, m and (m+n). We can use this to eliminate trial values
	// of k on the outer loop.
	// We could also use it to eliminate values of m on the innermost
	// loop but this would probably be as expensive as simply checking
	// the sum.

	for (PeUint k = 1;
		k < static_cast<PeUint>(ceil(float(triple_sum) / 12.0f)); ++k) {
		// Divisibility check
		if (triple_sum % (2 * k) != 0) {
			continue;
		}
		for (PeUint n = 1;
			n < static_cast<PeUint>(ceil(float(triple_sum - 8) / 4.0f)); ++n) {
			for (PeUint m = n + 1;
				m < static_cast<PeUint>(ceil(0.5f + 
					sqrtf(0.25f + float(triple_sum) / 2.0f))); ++m) {
				// Check the sum
				if ((2 * k * (m * m + m * n)) == triple_sum) {
					// Set the triple
					get<0>(triple) = k * (m * m - n * n);
					get<1>(triple) = 2 * k * m * n;
					get<2>(triple) = k * (m * m + n * n);

					// Cosmetic swap to ensure a < b < c
					// This doesn't affect the solution itself but
					// looks nice. Also worth noting here is that
					// this method can generate the same triple
					// more than once.
					if (get<0>(triple) > get<1>(triple)) {
						swap(get<0>(triple), get<1>(triple));
					}

					return get<0>(triple) * get<1>(triple) * get<2>(triple);
				}
			}
		}
	}

	return 0;
}


ostream &PeProblem9::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"A Pythagorean triplet is a set of three natural numbers, a < b < c," << endl <<
		"a^2 + b^2 = c^2. For example, 3^2 + 4^2 = 9 + 16 = 25 = 5^2." << endl <<
		"There exists exactly one Pythagorean triplet for which a + b + c = 1000." << endl <<
		"Find the product abc." << endl << endl;

	return os;
}


ostream &PeProblem9::DisplaySolution(ostream &os)
{
	const PeUint kTargetSum = 1000;
	tuple<PeUint, PeUint, PeUint> triple1(0,0,0), triple2(0,0,0);
	PeUint prod1 = 0, prod2 = 0;

	prod1 = Method1(kTargetSum, triple1);
	prod2 = Method2(kTargetSum, triple2);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << "Product: " << prod1 << " (" <<
			get<0>(triple1) << ", " <<
			get<1>(triple1) << ", " <<
			get<2>(triple1) << ")" << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"This uses Euclid's method to generate Pythagorean triples until a" << endl <<
		"valid one is found. This doesn't account for the possibility of" << endl <<
		"multiple solutions." << endl << endl <<
		"Method 1 solution: " << "Product: " << prod1 << " (" <<
			get<0>(triple1) << ", " <<
			get<1>(triple1) << ", " <<
			get<2>(triple1) << ")" << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"This uses Euclid's method, like Method 1, but attempts to put tighter" << endl <<
		"upper bounds on the search space by analysing the sum:" << endl <<
		"  a = k*(m^2 - n^2)" << endl <<
		"  b = 2*k*m*n" << endl <<
		"  c = k*(m^2 + n^2)" << endl <<
		"This gives a+b+c = 2*k*m*(m+n). We can set all three numbers to their" << endl <<
		"minimum valid values (k=1, m=2, n=1) to obtain the minimum valid sum of" << endl <<
		"2*1*2*(2+1) = 12. We can then find maximum possible values for each" << endl <<
		"parameter by holding the sum constant and the other two parameters at" << endl <<
		"their minima. This gives:" << endl <<
		"---- k ----" << endl <<
		"  2*k*2*(2+1) = sum" << endl <<
		"  k = sum / 12" << endl <<
		"---- n ----" << endl <<
		"  2*1*2*(2+n) = sum" << endl <<
		"  n = sum / 4 - 2" << endl <<
		"---- m ----" << endl <<
		"  2*1*m*(m+1) = sum" << endl <<
		"  m = -1 / 2 + sqrt(1 / 4 + sum / 2) [Choose +sqrt because searching for a maximum]" << endl << endl <<
		"We also can apply a divisibility check since we're only looking at" << endl <<
		"integer solutions. From the sum 2*k*m*(m+n), we know that a valid sum" << endl <<
		"must be divisible be 2, k, m and (m+n)." << endl << endl <<
		"Method 2 solution: " << "Product: " << prod2 << " (" <<
			get<0>(triple2) << ", " <<
			get<1>(triple2) << ", " <<
			get<2>(triple2) << ")" << endl << endl;

	return os;
}

ostream &PeProblem9::ProfileSolutions(int n_trials, ostream &os)
{
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	const PeUint kTargetSum = 1000;
	tuple<PeUint, PeUint, PeUint> triple1(0,0,0), triple2(0,0,0);
	PeUint prod1 = 0, prod2 = 0;

	clock_t start_time(clock());
	for (int i = 0; i < n_trials; ++i) {
		prod1 = Method1(kTargetSum, triple1);
	}
	clock_t method_1_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		prod2 = Method2(kTargetSum, triple2);
	}
	clock_t method_2_time = clock() - start_time;

	os << formatting::MethodHeader(1) << endl << endl <<
		"Time average over " << n_trials << " trials: " <<
		(long double)method_1_time / (long double)n_trials << endl << endl <<
		formatting::MethodHeader(2) << endl << endl <<
		"Time average over " << n_trials << " trials: " <<
		(long double)method_2_time / (long double)n_trials << endl << endl;

	return os;
}

}; // namespace pe