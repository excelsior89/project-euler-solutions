// Copyright 2020 Paul Robertson
//
// PeProblem16.h
//
// "Power digit sum"

#include "PeProblem16.h"

#include "PeBigInt.h"

using namespace std;

namespace pe {

// As always, a brute force solution to begin. We can make use of the PeBigInt
// class we implemented earlier to directly calculate the required power, then
// calculate the sum of its digits.
static PeUint Method1(PeUint base, PeUint power)
{
	// Set up bigint with the base
	PeBigInt big_power(base);

	// Raise to power
	big_power.power(power);
	
	// Collect digit sum and cast to PeUint (not checking for overflow here)
	return PeUint(big_power.sumDigits());
}

static PeUint Method2(PeUint base, PeUint power)
{
	return 0;
}

static PeUint Method3(PeUint base, PeUint power)
{
	return 0;
}


ostream &PeProblem16::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
	"2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26." << endl <<
	"What is the sum of the digits of the number 2^1000?" << endl << endl;

	return os;
}


ostream &PeProblem16::DisplaySolution(ostream &os)
{
	const PeUint kBase = 2, kPower = 1000;
	auto answer1 = Method1(kBase, kPower);
	auto answer2 = Method2(kBase, kPower);
	auto answer3 = Method3(kBase, kPower);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << answer1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"" << endl <<
		"" << endl << endl <<
		"Method 1 solution: " << answer1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"" << endl <<
		"" << endl << endl <<
		"Method 2 solution: " << answer2 << endl << endl <<

		formatting::MethodHeader(3) << endl << endl <<
		"" << endl <<
		"" << endl << endl <<
		"Method 3 solution: " << answer3 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint, PeUint
#define PROFILE_ARGS_ 2, 1000

PROFILE_SOLUTIONS(PeProblem16, Method1, Method2, Method3)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe