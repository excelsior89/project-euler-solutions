#pragma once

#include <ctime>
#include <iostream>

namespace pe {
class PeProblem
{
public:
	PeProblem();

	virtual ~PeProblem() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) = 0;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) = 0;

	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) = 0;

private:
	static const int kProblemNumber = 0;
};
}; // namespace pe

