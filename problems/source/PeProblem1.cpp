#include "PeProblem1.h"

using namespace std;

namespace pe {

// The brute force solution: find the sum of multiples of a or b from 1 to limit
// by checking every integer from 1 to limit for divisibility and adding it to
// the sum if it's divisible by a or b
static int Method1(int limit, int a, int b)
{
	int cumulative_sum = 0;

	for (int i = 0; i < limit; ++i) {
		if (((i % a) == 0) || ((i % b) == 0)) {
			cumulative_sum += i;
		}
	}

	return cumulative_sum;
}

// A less iterative solution
// The sum of multiples of a
static int Method2(int limit, int a, int b)
{
	// Decrement limit for use in the below calculations
	// since we want to sum up to but not including the limit
	--limit;

	int lcm_ab = math::Lcm(a, b),
		// Intentional integer division
		a_lim = limit / a, b_lim = limit / b,
		ab_lim = limit / lcm_ab;

	int sa = a * (a_lim * a_lim + a_lim) / 2,
		sb = b * (b_lim * b_lim + b_lim) / 2,
		sab = lcm_ab * (ab_lim * ab_lim + ab_lim) / 2;

	return sa + sb - sab;
}

PeProblem1::PeProblem1()
{
}


ostream &PeProblem1::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"If we list all the natural numbers below 10 that are multiples of 3 or 5, " << endl <<
		"we get 3, 5, 6 and 9. The sum of these multiples is 23." << endl << endl <<
		"Find the sum of all the multiples of 3 or 5 below 1000." << endl;

	return os;
}


ostream &PeProblem1::DisplaySolution(ostream &os)
{
	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << Method2(1000, 3, 5) << endl << endl <<
		formatting::MethodHeader(1) << endl << endl <<
		"Simple brute-force summation can easily compute the answer:" << endl <<
		"Loop over integers from 1 to 1000, check for divisibility by 3 or 5 "
		"and accumulate the sum." << endl << endl <<
		"Method 1 solution: " << Method1(1000, 3, 5) << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"A more elegant solution:" << endl <<
		"The sum of integers from 1 to N is equal to (N^2 + N)/2. " << 
		"The sum of multiples of of a number M from 1 to M*N is:" << endl <<
		"M*(N^2 + N)/2" << endl <<
		"The sum of multiples of of a number M from 1 to N is therefore:" << endl <<
		"M*(P^2 + P)/2, where P = floor(N/M)" << endl << endl <<
		"If we're interested in multiples of more than one number, using the" << endl <<
		"above formula cause overlap: numbers that are multiples of both the" << endl <<
		"base numbers will be included twice. Fortunately, these numbers are" << endl <<
		"all multiples of the lowest common multiple (lcm) of the base numbers." << endl <<
		"We can therefore subtract the sum of these lcm multiples from the sums" << endl <<
		"of the base multiples. For the problem, this means that we calculate" << endl <<
		"the sums of multiples of 3, 5 and lcm(3,5)=15:" << endl <<
		"S_3  = sum(3n)  for n = [1 ... floor(999/3)]  = 3*(333^2 + 333)/2" << endl <<
		"S_5  = sum(5n)  for n = [1 ... floor(999/5)]  = 5*(199^2 + 199)/2" << endl <<
		"S_15 = sum(15n) for n = [1 ... floor(999/15)] = 15*(66^2 + 66)/2" << endl <<
		"S = S_3 + S_5 - S_15 = 166833 + 99500 - 33165 = 233168" << endl << endl <<
		"Method 2 solution: " << Method2(1000, 3, 5) << endl << endl;

	return os;
}

ostream &PeProblem1::ProfileSolutions(int n_trials, ostream &os)
{
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	clock_t start_time(clock());

	for (int i = 0; i < n_trials; ++i) {
		Method1(1000, 3, 5);
	}

	clock_t method_1_time = clock() - start_time;

	start_time = clock();

	for (int i = 0; i < n_trials; ++i) {
		Method2(1000, 3, 5);
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