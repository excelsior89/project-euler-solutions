// Copyright 2020-2022 Paul Robertson
//
// PeProblem18.h
//
// "Maximum path sum I"

#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem18: public PeProblem
{
public:
	PeProblem18() {}

	virtual ~PeProblem18() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 18;
};
}; // namespace pe

