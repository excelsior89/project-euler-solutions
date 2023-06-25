// Copyright 2020-2023 Paul Robertson
//
// PeProblemSelector.cpp
//
// Problem selector helper class

#include "PeProblemSelector.h"

#include "PeProblem001.h"
#include "PeProblem002.h"
#include "PeProblem003.h"
#include "PeProblem004.h"
#include "PeProblem005.h"
#include "PeProblem006.h"
#include "PeProblem007.h"
#include "PeProblem008.h"
#include "PeProblem009.h"
#include "PeProblem010.h"
#include "PeProblem011.h"
#include "PeProblem012.h"
#include "PeProblem013.h"
#include "PeProblem014.h"
#include "PeProblem015.h"
#include "PeProblem016.h"
#include "PeProblem017.h"
#include "PeProblem018.h"
#include "PeProblem019.h"
#include "PeProblem020.h"
#include "PeProblem021.h"

namespace pe
{
PeProblemSelector::PeProblemSelector()
{
    problems_.push_back(std::make_unique<PeProblem001>());
    problems_.push_back(std::make_unique<PeProblem002>());
    problems_.push_back(std::make_unique<PeProblem003>());
    problems_.push_back(std::make_unique<PeProblem004>());
    problems_.push_back(std::make_unique<PeProblem005>());
    problems_.push_back(std::make_unique<PeProblem006>());
    problems_.push_back(std::make_unique<PeProblem007>());
    problems_.push_back(std::make_unique<PeProblem008>());
    problems_.push_back(std::make_unique<PeProblem009>());
    problems_.push_back(std::make_unique<PeProblem010>());
    problems_.push_back(std::make_unique<PeProblem011>());
    problems_.push_back(std::make_unique<PeProblem012>());
    problems_.push_back(std::make_unique<PeProblem013>());
    problems_.push_back(std::make_unique<PeProblem014>());
    problems_.push_back(std::make_unique<PeProblem015>());
    problems_.push_back(std::make_unique<PeProblem016>());
    problems_.push_back(std::make_unique<PeProblem017>());
    problems_.push_back(std::make_unique<PeProblem018>());
    problems_.push_back(std::make_unique<PeProblem019>());
    problems_.push_back(std::make_unique<PeProblem020>());
    problems_.push_back(std::make_unique<PeProblem021>());
}

std::ostream& PeProblemSelector::DisplayProblem(size_t problem_number, std::ostream& os)
{
    if ( (problem_number > 0) && (problem_number <= problems_.size()) ) {
        --problem_number;
        problems_[problem_number]->DisplayProblem(os);
        problems_[problem_number]->DisplaySolution(os);
    } else {
        os << "Problem number " << problem_number << " not found." << std::endl;
    }

    return os;
}

std::ostream& PeProblemSelector::ProfileProblem(size_t problem_number, int profile_n_trials, std::ostream& os)
{
    if ( (problem_number > 0) && (problem_number <= problems_.size()) ) {
        --problem_number;
        problems_[problem_number]->ProfileSolutions(profile_n_trials, os);
    } else {
        os << "Problem number " << problem_number << " not found." << std::endl;
    }

    return os;
}
}; // namespace pe