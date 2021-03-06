// Copyright 2020-2021 Paul Robertson
//
// PeProblem11.h
//
// "Largest product in a grid"

#pragma once

#include <algorithm>
#include <array>

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem11: public PeProblem
{
public:
	PeProblem11() {}
	virtual ~PeProblem11() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 11;
};
}; // namespace pe

