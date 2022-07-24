// Copyright 2020-2022 Paul Robertson
//
// PeProblem12.h
//
// "Highly divisible triangular number"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem12: public PeProblem
{
public:
	PeProblem12() {}
	virtual ~PeProblem12() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 12;
};
}; // namespace pe

