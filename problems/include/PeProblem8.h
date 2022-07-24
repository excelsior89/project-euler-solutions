// Copyright 2020-2022 Paul Robertson
//
// PeProblem8.h
//
// "Largest product in a series"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem8: public PeProblem
{
public:
	PeProblem8() {}
	virtual ~PeProblem8() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 8;
};
}; // namespace pe

