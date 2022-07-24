// Copyright 2020-2022 Paul Robertson
//
// PeProblem20.h
//
// "Factorial digit sum"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem20: public PeProblem
{
public:
	PeProblem20() {}

	virtual ~PeProblem20() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 20;
};
}; // namespace pe

