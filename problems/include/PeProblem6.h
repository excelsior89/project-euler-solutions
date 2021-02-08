// Copyright 2020-2021 Paul Robertson
//
// PeProblem6.h
//
// "Sum square difference"

#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem6: public PeProblem
{
public:
	PeProblem6() {}
	virtual ~PeProblem6() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 6;
};
}; // namespace pe

