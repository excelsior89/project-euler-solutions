// Copyright 2020-2022 Paul Robertson
//
// PeProblem15.h
//
// "Lattice paths"

#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem15: public PeProblem
{
public:
	PeProblem15() {}
	virtual ~PeProblem15() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 15;
};
}; // namespace pe

