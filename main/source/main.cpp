#include <iostream>

#include "PeProblemSelector.h"

using namespace std;

using namespace pe;

int main(int argc, char *argv[])
{
	static PeProblemSelector selector;

	int problem_number = 0, profile_n_trials = 0;
	bool use_profiling = false;
	string profile_response;

	cout << "---- Project Euler Solutions ----" << endl << endl;

	for (;;) {
		problem_number = 0;
		profile_n_trials = 0;
		use_profiling = false;

		cout << "Pick a problem (1-700, enter 0 to exit): ";
		cin >> problem_number;
	
		// Exit if we enter 0 as the problem number
		if (problem_number == 0) {
			return 0;
		}

		// Do we want to find the solution, or profile the methods used?
		cout << "Do profiling? Y/N: ";
		cin >> profile_response;

		if ((profile_response == string("Y")) || (profile_response == string("y"))) {
			use_profiling = true;
			cout << "Profiling will be used." << endl;
			cout << "Enter number of trials to use in profiling: ";
			cin >> profile_n_trials;
		} else {
			cout << "Profiling will not be used." << endl;
		}
		
		cout << endl;

		// Display or profile as determined above
		if (use_profiling) {
			selector.ProfileProblem(problem_number, profile_n_trials);
		} else {
			selector.DisplayProblem(problem_number);
		}

		cout << endl;
	}

	return 0;
}