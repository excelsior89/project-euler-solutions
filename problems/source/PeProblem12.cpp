// Copyright 2020 Paul Robertson
//
// PeProblem12.cpp
//
// "Highly divisible triangular number"

#include "PeProblem12.h"

using namespace std;

namespace pe {

// As usual, a brute force method to begin:
// Generate successive triangle/pyramid numbers and factorise
// them until we find one with more factors than the target
static PeUint Method1(const PeUint target_divisor_count)
{
	PeUint divisor_count = 1; // Stores the length of the factors vector
	PeUint target_number = 1; // The current triangle number
	PeUint i_triangle = 1; // Used to generate the triangle number

	// Break this loop once we have more factors than the
	// target requirement
	while (divisor_count < target_divisor_count) {
		// Generate next triangle number
		++i_triangle;
		target_number = math::SumOfOneToN(i_triangle);

		// Find its factors and count how many there are
		const auto factor_vec = math::Factors(target_number);
		divisor_count = static_cast<PeUint>(factor_vec.size());
	}

	return target_number;
}

// The same method as above but with a few optimisations to
// the code such as iteratively updating a pyramid/triangle
// number as a cumulative sum rather than calculating it from
// scratch at each time step.
static PeUint Method2(const PeUint target_divisor_count)
{
	PeUint divisor_count = 1; // Stores the length of the factors vector
	PeUint target_number = 1; // The current triangle number
	PeUint i_triangle = 1; // Used to generate the triangle number

	// Break this loop once we have more factors than the
	// target requirement
	while (divisor_count < target_divisor_count) {

		// Next pyramid/triangle number
		target_number += ++i_triangle;

		// Reset divisor count
		divisor_count = 2;

		// Test integers from 2...sqrt(target_number) to find lower factors
		// For any lower factor, the upper factor is trial_number / lower_factor
		// We only need to count factors so we can just increment the factor count
		// by 2 for each new lower factor found.
		PeUint uint_sqrt = static_cast<PeUint>(sqrt(double(target_number)));

		for (PeUint i = 2; i <= uint_sqrt; ++i) {
			if (target_number % i == 0) {
				divisor_count += 2;
			}

			// A special check for an integer square root is needed, as this only
			// counts as one factor rather than two.
			// Note we need to verify that the target number is actually a
			// square number, since the "uint_sqrt" value we use is the integer
			// cast of the (possibly non-integer) square root.
			if ((i == uint_sqrt) && (target_number % uint_sqrt == 0)) {
				--divisor_count;
			}
		}
	}

	return target_number;
}

// Every triangle number is the product of two consecutive integers divided by two.
// Since consecutive integers are coprime, the number of factors of their product
// will be the product of the number of factors of each integer.
// We can use this to reduce the size of the factorisation problem, particularly
// since the two integer factors (n) and (n+1) will not be "trivial" factors i.e.
// for large numbers they won't be e.g. 2, 3 etc.
static PeUint Method3(const PeUint target_divisor_count)
{
	// Quick exit for a target of 0 or 1
	if (target_divisor_count < 2) {
		return 1;
	}

	// The factor count doesn't work properly for n == 2
	// since it's assuming both factors are greater than 1
	// This could be fixed with some extra logic within the
	// while loop, but it's probably more efficient to just
	// treat n == 2 as a special case and exit here
	if (target_divisor_count == 2) {
		return 3;
	}


	PeUint divisor_count = 2; // Stores the length of the factors vector
	PeUint i_triangle = 2; // Used to generate the triangle number

	PeUint tri_factor1 = 1, tri_factor2 = 3,
		tri_div_count1 = 1, tri_div_count2 = 2;

	// Break this loop once we have more factors than the
	// target requirement
	while (divisor_count < target_divisor_count) {

		// Find the two factors of the triangle number
		if (i_triangle % 2 == 0) {
			tri_factor1 = i_triangle / 2;
			tri_factor2 = i_triangle + 1;
		} else {
			tri_factor1 = i_triangle;
			tri_factor2 = (i_triangle + 1) / 2;
		}

		// Reset divisor counts
		tri_div_count1 = 2, tri_div_count2 = 2, divisor_count = 2;

		// Do the same factorisation as from Method 2 on both
		// the factors and record separate divisor counts

		// Note this time we're checking i * i <= number instead
		// of i <= sqrt(number) - this makes the logic for checking
		// for a square number slightly simpler.
		// It might be more efficient since i * i should be much faster
		// to compute than sqrt(), however we do need to compute i * i
		// every iteration so for larger numbers this _may_ no longer
		// be more efficient.


		// Factor 1
		for (PeUint i = 2; i * i <= tri_factor1; ++i) {
			if (tri_factor1 % i == 0) {
				tri_div_count1 += 2;
			}
			// Check for square
			if (i * i == tri_factor1) {
				--tri_div_count1;
			}
		}
		
		// Factor 2
		for (PeUint i = 2; i * i <= tri_factor2; ++i) {
			if (tri_factor2 % i == 0) {
				tri_div_count2 += 2;
			}
			// Check for square
			if (i * i == tri_factor2) {
				--tri_div_count2;
			}
		}

		// Since these two factors are coprime, the number of divisors
		// for the resulting triangle number is the product of the number
		// of divisors for each factor
		divisor_count = tri_div_count1 * tri_div_count2;

		// Increment index
		++i_triangle;
	}

	return tri_factor1 * tri_factor2;
}


ostream &PeProblem12::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
	"The sequence of triangle numbers is generated by adding the natural numbers." << endl <<
	"So the 7th triangle number would be 1 + 2 + 3 + 4 + 5 + 6 + 7 = 28." << endl <<
	"The first ten terms would be: 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ..." << endl << endl <<

	"Let us list the factors of the first seven triangle numbers:" << endl << endl <<

	" 1: 1" << endl <<
	" 3: 1,3" << endl <<
	" 6: 1,2,3,6" << endl <<
	"10: 1,2,5,10" << endl <<
	"15: 1,3,5,15" << endl <<
	"21: 1,3,7,21" << endl <<
	"28: 1,2,4,7,14,28" << endl << endl <<

	"We can see that 28 is the first triangle number to have over five divisors." << endl << endl <<

	"What is the value of the first triangle number to have over five hundred divisors?" << endl << endl;

	return os;
}


ostream &PeProblem12::DisplaySolution(ostream &os)
{
	// Searching for 2000 divisors gives T_n near 2*10^9,
	// so much higher than that will probably lead to overflow
	const PeUint kTargetDivisorCount = 500;
	auto target_triangle_number1 = Method1(kTargetDivisorCount);
	auto target_triangle_number2 = Method2(kTargetDivisorCount);
	auto target_triangle_number3 = Method3(kTargetDivisorCount);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << target_triangle_number1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"As usual, a brute force method to begin with... We simply generate" << endl <<
		"each successive triangle/pyramid number and count all its factors." << endl <<
		"Once we find a number with more factors than the target amount," << endl <<
		"we terminate." << endl << endl <<
		"Method 1 solution: " << target_triangle_number1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"The same method as for Method1, but with some minor code optimisations." << endl <<
		"Instead of calculating the nth triangle number from scratch, we update" << endl <<
		"a cumulative sum each iteration. We also forgo saving all the" << endl <<
		"factors of each triangle number and instead simply counts the" << endl <<
		"existence of each factor (avoiding needing to create a potentially" << endl <<
		"large vector of factors)." << endl << endl <<
		"Method 2 solution: " << target_triangle_number2 << endl << endl <<

		formatting::MethodHeader(3) << endl << endl <<
		"This method uses a bit more math to speed up the problem. Since we're" << endl <<
		"only concerned with finding the number of divisors, as with Method 2 we" << endl <<
		"can skip recording each divisor and simply count them." << endl << endl <<

		"We also can use the formula for the nth triangle number: T_n = (1/2)*n*(n+1)" << endl <<
		"to see that every T_n has n and n+1 as two of its divisors. Since n and n+1" << endl <<
		"are consecutive integers, they are coprime" << endl <<
		"(Quick proof: Let x=GCD(n,n+1). Then x divides n+1-n=1. Therefore x=1)." << endl << endl <<

		"If n has a divisors and n+1 has b divisors, n*(n+1) will have a*b divisors" << endl <<
		"since n and n+1 are coprime. Dividing the even value of n,n+1 by 2 will" << endl <<
		"maintain coprimality and gives us two divisors which we can factorise" << endl <<
		"separately. We then simply multiply these two divisor counts together" << endl <<
		"to find the total number of divisors for T_n. As in the first two methods," << endl <<
		"we terminate once we find T_n with more divisors than the target amount." << endl << endl <<
		"Method 3 solution: " << target_triangle_number3 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 300

PROFILE_SOLUTIONS(PeProblem12, Method1, Method2, Method3)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe