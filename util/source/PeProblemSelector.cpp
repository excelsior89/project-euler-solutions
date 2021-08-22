// Copyright 2020-2021 Paul Robertson
//
// PeProblemSelector.cpp
//
// Problem selector helper class

#include "PeProblemSelector.h"

#include "PeProblem1.h"
#include "PeProblem2.h"
#include "PeProblem3.h"
#include "PeProblem4.h"
#include "PeProblem5.h"
#include "PeProblem6.h"
#include "PeProblem7.h"
#include "PeProblem8.h"
#include "PeProblem9.h"
#include "PeProblem10.h"
#include "PeProblem11.h"
#include "PeProblem12.h"
#include "PeProblem13.h"
#include "PeProblem14.h"
#include "PeProblem15.h"
#include "PeProblem16.h"
#include "PeProblem17.h"
#include "PeProblem18.h"
#include "PeProblem19.h"

using namespace std;

namespace pe {
PeProblemSelector::PeProblemSelector()
{
	problems_.push_back(make_unique<PeProblem1>());
	problems_.push_back(make_unique<PeProblem2>());
	problems_.push_back(make_unique<PeProblem3>());
	problems_.push_back(make_unique<PeProblem4>());
	problems_.push_back(make_unique<PeProblem5>());
	problems_.push_back(make_unique<PeProblem6>());
	problems_.push_back(make_unique<PeProblem7>());
	problems_.push_back(make_unique<PeProblem8>());
	problems_.push_back(make_unique<PeProblem9>());
	problems_.push_back(make_unique<PeProblem10>());
	problems_.push_back(make_unique<PeProblem11>());
	problems_.push_back(make_unique<PeProblem12>());
	problems_.push_back(make_unique<PeProblem13>());
	problems_.push_back(make_unique<PeProblem14>());
	problems_.push_back(make_unique<PeProblem15>());
	problems_.push_back(make_unique<PeProblem16>());
	problems_.push_back(make_unique<PeProblem17>());
	problems_.push_back(make_unique<PeProblem18>());
	problems_.push_back(make_unique<PeProblem19>());
}


ostream &PeProblemSelector::DisplayProblem(size_t problem_number, ostream &os)
{
	if ((problem_number > 0) &&  (problem_number <= problems_.size())) {
		--problem_number;
		problems_[problem_number]->DisplayProblem(os);
		problems_[problem_number]->DisplaySolution(os);
	} else {
		os << "Problem number " << problem_number << " not found." << endl;
	}

	return os;
}

ostream &PeProblemSelector::ProfileProblem(size_t problem_number, int profile_n_trials,
	ostream &os)
{
	if ((problem_number > 0) &&  (problem_number <= problems_.size())) {
		--problem_number;
		problems_[problem_number]->ProfileSolutions(profile_n_trials, os);
	} else {
		os << "Problem number " << problem_number << " not found." << endl;
	}

	return os;
}
}; // namespace pe