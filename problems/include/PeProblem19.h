// Copyright 2020-2022 Paul Robertson
//
// PeProblem19.h
//
// "Counting Sundays"

#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem19: public PeProblem
{
public:
	PeProblem19() {}

	virtual ~PeProblem19() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 19;
};
}; // namespace pe

