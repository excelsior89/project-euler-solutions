// Copyright 2020 Paul Robertson
//
// PeProblem.h
//
// Base class for Project Euler problem/solution display

#pragma once

#include <ctime>
#include <iostream>

#include "PeUtilities.h"

namespace pe {
class PeProblem
{
public:
	PeProblem() {}

	virtual ~PeProblem() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) = 0;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) = 0;

	// Base function for simple solution profiling (using system clock timing)
	// This base function simply prints out a formatted header for profiling.
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) = 0;

private:
	static const int kProblemNumber = 0;
};
}; // namespace pe

