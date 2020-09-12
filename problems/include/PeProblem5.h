// Copyright 2020 Paul Robertson
//
// PeProblem5.h
//
// "Smallest multiple"

#pragma once

#include <map>

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem5: public PeProblem
{
public:
	PeProblem5() {}
	virtual ~PeProblem5() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 5;
};
}; // namespace pe

