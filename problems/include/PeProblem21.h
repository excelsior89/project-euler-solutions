// Copyright 2020-2022 Paul Robertson
//
// PeProblem21.h
//
// "Amicable Numbers"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem21: public PeProblem
{
public:
	PeProblem21() {}

	virtual ~PeProblem21() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 21;
};
}; // namespace pe

