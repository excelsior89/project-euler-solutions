// Copyright 2020-2022 Paul Robertson
//
// PeProblem17.h
//
// "Number letter counts"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem17: public PeProblem
{
public:
	PeProblem17() {}

	virtual ~PeProblem17() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 17;
};
}; // namespace pe

