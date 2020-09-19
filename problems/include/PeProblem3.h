// Copyright 2020 Paul Robertson
//
// PeProblem3.h
//
// "Largest prime factor"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem3: public PeProblem
{
public:
	PeProblem3() {}
	virtual ~PeProblem3() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 3;
};
}; // namespace pe

