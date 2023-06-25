// Copyright 2020-2023 Paul Robertson
//
// main.cpp
//
// Entry point for the Project Euler solutions project

#include "PeProblemSelector.h"

#include <iostream>

int main(int argc, char* argv[])
{
    pe::PeProblemSelector selector;

    int         problem_number = 0, profile_n_trials = 0;
    bool        use_profiling = false;
    std::string profile_response;

    std::cout << "---- Project Euler Solutions ----" << std::endl << std::endl;

    // Loop until exit
    for ( ;; ) {
        problem_number   = 0;
        profile_n_trials = 0;
        use_profiling    = false;

        std::cout << "Pick a problem (1-700, enter 0 to exit): ";
        std::cin >> problem_number;

        // Exit if we enter 0 as the problem number
        if ( problem_number == 0 ) {
            return 0;
        }

        // Do we want to find the solution, or profile the methods used?
        std::cout << "Do profiling? Y/N: ";
        std::cin >> profile_response;

        if ( (profile_response == std::string("Y")) || (profile_response == std::string("y")) ) {
            use_profiling = true;
            std::cout << "Profiling will be used." << std::endl;
            std::cout << "Enter number of trials to use in profiling: ";
            std::cin >> profile_n_trials;
        } else {
            std::cout << "Profiling will not be used." << std::endl;
        }

        std::cout << std::endl;

        // Display or profile as determined above
        if ( use_profiling ) {
            selector.ProfileProblem(problem_number, profile_n_trials);
        } else {
            selector.DisplayProblem(problem_number);
        }

        std::cout << std::endl;
    }

    return 0;
}
