// Copyright 2020-2022 Paul Robertson
//
// PeProblem7.h
//
// "10001st prime"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem7: public PeProblem
{
public:
	PeProblem7() {}
	virtual ~PeProblem7() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 7;
};
}; // namespace pe

