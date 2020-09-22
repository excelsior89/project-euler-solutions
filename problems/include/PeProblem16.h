// Copyright 2020 Paul Robertson
//
// PeProblem16.h
//
// "Power digit sum"

#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem16: public PeProblem
{
public:
	PeProblem16() {}

	virtual ~PeProblem16() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 16;
};
}; // namespace pe

