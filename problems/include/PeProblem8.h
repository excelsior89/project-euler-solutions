// Copyright 2020-2022 Paul Robertson
//
// PeProblem8.h
//
// "Largest product in a series"

#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem8: public PeProblem
{
public:
	PeProblem8() {}
	virtual ~PeProblem8() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 8;
};
}; // namespace pe

