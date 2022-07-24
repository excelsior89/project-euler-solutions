// Copyright 2020-2022 Paul Robertson
//
// PeProblem14.h
//
// "Longest Collatz sequence"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem14: public PeProblem
{
public:
	PeProblem14() {}
	virtual ~PeProblem14() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 14;
};
}; // namespace pe

