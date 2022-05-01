// Copyright 2020-2022 Paul Robertson
//
// PeProblem21.cpp
//
// "Amicable Numbers"

#include "PeProblem21.h"


using namespace std;

namespace pe {

static PeUint Method1(PeUint n)
{
	return 0;
}


static PeUint Method2(PeUint n)
{
	return 0;
}


ostream &PeProblem21::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"" << endl <<
		"" << endl << endl <<

		"" << endl <<
		"" << endl <<
		"" << endl <<
		"" << endl << endl <<

		"" << endl << endl <<

		"" << endl << endl <<

		"" << endl <<
		"" << endl <<
		"" << endl <<
		"" << endl << endl;

	return os;
}

ostream &PeProblem21::DisplaySolution(ostream &os)
{
	const PeUint kLimit = 10000;

	auto answer1 = Method1(kLimit);
	auto answer2 = Method2(kLimit);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << answer1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"" << endl <<
		"" << endl <<
		"" << endl <<
		"" << endl << endl <<
		"Method 1 solution: " << answer1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"" << endl <<
		"" << endl <<
		"" << endl <<
		"" << endl <<
		"" << endl << endl <<
		"Method 2 solution: " << answer2 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 10000

PROFILE_SOLUTIONS(PeProblem21, Method1, Method2)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe