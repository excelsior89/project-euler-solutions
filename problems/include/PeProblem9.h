// Copyright 2020-2022 Paul Robertson
//
// PeProblem9.h
//
// "Special Pythagorean triplet"

#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem9: public PeProblem
{
public:
	PeProblem9() {}
	virtual ~PeProblem9() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 9;
};
}; // namespace pe

