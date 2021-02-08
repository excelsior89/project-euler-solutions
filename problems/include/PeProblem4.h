// Copyright 2020-2021 Paul Robertson
//
// PeProblem4.h
//
// "Largest palindrome product"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem4: public PeProblem
{
public:
	PeProblem4() {}
	virtual ~PeProblem4() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 4;
};
}; // namespace pe

