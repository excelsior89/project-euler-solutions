#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "PeProblem.h"

namespace pe {
class PeProblemSelector
{
public:
	PeProblemSelector();

	virtual ~PeProblemSelector() {}

	std::ostream &DisplayProblem(size_t problem_number, std::ostream &os = std::cout);
	std::ostream &ProfileProblem(size_t problem_number, int profile_n_trials,
		std::ostream &os = std::cout);

private:
	std::vector<std::unique_ptr<PeProblem>> problems_;
};
}; // namespace pe

