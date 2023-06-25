// Copyright 2020-2023 Paul Robertson
//
// PeProblemSelector.h
//
// Problem selector helper class

#pragma once

#include "PeProblem.h"

#include <iostream>
#include <string>
#include <vector>

namespace pe
{
class PeProblemSelector
{
public:
    PeProblemSelector();

    virtual ~PeProblemSelector() {}

    std::ostream& DisplayProblem(size_t problem_number, std::ostream& os = std::cout);
    std::ostream& ProfileProblem(size_t problem_number, int profile_n_trials, std::ostream& os = std::cout);
private:
    std::vector<std::unique_ptr<PeProblem>> problems_;
};
}; // namespace pe
