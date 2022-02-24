// Copyright 2020-2022 Paul Robertson
//
// PeProblem20.cpp
//
// "Factorial digit sum"

#include "PeProblem20.h"

#include "PeBigInt.h"


using namespace std;

namespace pe {

// The underlying solution to this problem is fairly obvious:
// calculate the factorial and then add up its digits.
// It is hopefully also fairly obvious that this is outside the range
// of normal integer arithmetic, so we'll need to use another approach:
// the BigInt class developed for earlier problems!
static PeUint Method1(PeUint n)
{
	// Initialise at 1 (which also captures 0! = 1)
	PeBigInt factorial_calc(1);

	// Iterate up to n and multiply each time to calculate the factorial
	for (PeUint i = 2; i <= n; ++i) {
		factorial_calc *= i;
		cout << PeUint(factorial_calc.sumDigits()) << endl;
	}

	// Cast to PeUint for returning - this will limit the sum of the digits
	// to the 64 bit PeUint range, but if we've calculated a number whose
	// digits _sum_ to >(2^64-1), we might be dealing with other problems
	// before this becomes an issue...
	return PeUint(factorial_calc.sumDigits());
}

ostream &PeProblem20::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"n! means n * (n - 1) * ... * 3 * 2 * 1" << endl << endl <<

		"For example, 10! = 10 * 9 * ... * 3 * 2 * 1 = 3628800," << endl <<
		"and the sum of the digits in the number 10! is" << endl <<
		"3 + 6 + 2 + 8 + 8 + 0 + 0 = 27." << endl << endl <<

		"Find the sum of the digits in the number 100!" << endl << endl;

	return os;
}

ostream &PeProblem20::DisplaySolution(ostream &os)
{
	PeUint n = 100;
	auto answer1 = Method1(n);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << answer1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"The underlying solution here is fairly obvious:" << endl <<
		"simply calculate the factorial and then add up its digits." << endl <<
		"It's hopefully also obvious that this will lead to numbers" << endl <<
		"larger than can be represented by a 64 bit unsigned integer..." << endl <<
		"However, we can use our custom Big Int class to implement" << endl <<
		"this method!" << endl << endl <<
		"Method 1 solution: " << answer1 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 100

PROFILE_SOLUTIONS(PeProblem20, Method1)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe