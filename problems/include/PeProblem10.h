#pragma once

#include "PeProblem.h"
#include "PeUtilities.h"

namespace pe {
class PeProblem10: public PeProblem
{
public:
	PeProblem10();

	virtual ~PeProblem10() {}

	virtual std::ostream &DisplayProblem(std::ostream &os = std::cout) override;
	virtual std::ostream &DisplaySolution(std::ostream &os = std::cout) override;
	virtual std::ostream &ProfileSolutions(int n_trials, std::ostream &os = std::cout) override;

private:
	static const int kProblemNumber = 10;
};
}; // namespace pe

