// Copyright 2020-2022 Paul Robertson
//
// PeProblem10.h
//
// "Summation of primes"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem10: public PeProblem
{
public:
	PeProblem10() {}
	virtual ~PeProblem10() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 10;
};
}; // namespace pe

