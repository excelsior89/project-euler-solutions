#include "PeProblem6.h"

using namespace std;

namespace pe {

// Brute force method
// Work out the squared sum and sum of squares iteratively
// then return the difference
static int Method1(int n)
{
	// Collect sums in these variables
	int sum_of_int = 0, sum_of_squares = 0;

	// Iteratve from 1 to n, adding i and i^2
	// to the two sums
	for (int i = 1; i <= n; ++i) {
		sum_of_int += i;
		sum_of_squares += i*i;
	}

	// Difference of the square of sum and sum of squares
	return sum_of_int*sum_of_int - sum_of_squares;
}

// This method uses and analytic formula derived from the
// established formulae for these two summation sequences:
//  (sum(1..n))^2 - sum(1^2...n^2)
//  = (n^4+2*n^3+n^2)/4 - (2*n^3+3*n^2+n)/6
//  = (3*n^4+2*n^3-3*n^2-2*n)/12
static int Method2(int n)
{
	// Horner form of the above polynomial
	return n * (-2 + n * (-3 + n * (2 + 3 * n))) / 12;
}


PeProblem6::PeProblem6()
{
}


ostream &PeProblem6::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"The sum of the squares of the first ten natural numbers is:" << endl <<
		"1^2+2^2+...+10^2=385" << endl <<
		"The square of the sum of the first ten natural numbers is:" << endl <<
		"(1+2+...+10)^2=55^2=3025" << endl <<
		"Hence the difference between the sum of the squares of the" << endl <<
		"first ten natural numbers and the square of the sum is 3025-385=2640." << endl <<
		"Find the difference between the sum of the squares of the first " << endl <<
		"one hundred natural numbers and the square of the sum." << endl << endl;

	return os;
}


ostream &PeProblem6::DisplaySolution(ostream &os)
{
	const int kN = 100;
	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << Method2(kN) << endl << endl <<
		formatting::MethodHeader(1) << endl << endl <<
		"This method finds sum and sum of squares iteratively from 1 to" << endl <<
		"the supplied number, the returns the difference:" << endl <<
		"(sum(1..n))^2 - sum(1^2...n^2)" << endl << endl <<
		"Method 1 solution: " << Method1(kN) << endl << endl <<


		formatting::MethodHeader(2) << endl << endl <<
		"This method makes of use of two analytic formulae:" << endl <<
		"The sum of squares and sum of integers (a.k.a. triangle numbers)." << endl <<
		"The sum of integers 1...n is given by the formula:" << endl <<
		"  sum(1..n) = (n^2+n)/2" << endl <<
		"The sum of squares 1^2...n^2 is given by the formula:" << endl <<
		"  sum(1^2..n^2) = (n*(n+1)*(2*n+1))/6" << endl <<
		"We can therefore find a polynomial to express the answer:" << endl <<
		"  (sum(1..n))^2 - sum(1^2..n^2)" << endl <<
		"  ((n^2+n)/2)^2 - (n*(n+1)*(2*n+1))/6" << endl <<
		"  (n^4+2*n^3+n^2)/4 - (2*n^3+3*n^2+n)/6" << endl <<
		"  3*(n^4+2*n^3+n^2)/12 - 2*(2*n^3+3*n^2+n)/12" << endl <<
		"  ((3*n^4+6*n^3+3*n^2) - (4*n^3+6*n^2+2*n))/12" << endl <<
		"  (3*n^4+2*n^3-3*n^2-2*n)/12" << endl << endl <<
		"Method 2 solution: " << Method2(kN) << endl << endl;

	return os;
}

ostream &PeProblem6::ProfileSolutions(int n_trials, ostream &os)
{
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	const int kN = 100;

	clock_t start_time(clock());
	for (int i = 0; i < n_trials; ++i) {
		Method1(kN);
	}
	clock_t method_1_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		Method2(kN);
	}
	clock_t method_2_time = clock() - start_time;

	os << formatting::MethodHeader(1) << endl << endl <<
		"Time average over " << n_trials << " trials: " <<
		(long double)method_1_time / (long double)n_trials << endl << endl <<
		formatting::MethodHeader(2) << endl << endl <<
		"Time average over " << n_trials << " trials: " <<
		(long double)method_2_time / (long double)n_trials << endl << endl;

	return os;
}

}; // namespace pe