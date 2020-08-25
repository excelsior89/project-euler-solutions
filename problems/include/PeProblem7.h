#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem7: public PeProblem
{
public:
	PeProblem7();

	virtual ~PeProblem7() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 7;
};
}; // namespace pe

