// Copyright 2020-2022 Paul Robertson
//
// PeProblem16.h
//
// "Power digit sum"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem16: public PeProblem
{
public:
	PeProblem16() {}

	virtual ~PeProblem16() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 16;
};
}; // namespace pe

