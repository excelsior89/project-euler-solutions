// Copyright 2020-2022 Paul Robertson
//
// PeProblem6.h
//
// "Sum square difference"

#pragma once

#include "PeProblem.h"

namespace pe {
class PeProblem6: public PeProblem
{
public:
	PeProblem6() {}
	virtual ~PeProblem6() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 6;
};
}; // namespace pe

