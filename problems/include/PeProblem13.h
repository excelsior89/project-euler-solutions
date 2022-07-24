// Copyright 2020-2022 Paul Robertson
//
// PeProblem13.h
//
// "Large sum"

#pragma once

#include "PeProblem.h"

#include "PeBigInt.h"

#include <array>

namespace pe {
class PeProblem13: public PeProblem
{
public:
	PeProblem13() {}

	virtual ~PeProblem13() {}

	std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 13;
};
}; // namespace pe

