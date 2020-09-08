#include "PeProblem10.h"

using namespace std;

namespace pe {

// Use the Sieve of Eratosthenes to generate primes until the summation limit
// is met. We're using 64 bit integers since the problem statement could start
// pushing us towards quite large values.
static uint64_t Method1(uint64_t limit)
{
	uint64_t sum = 0;

	// Get the array of primes
	auto primes_vec = math::GeneratePrimesEratosthenes((unsigned)limit);

	// STL accumulate to calulcate the sum
	sum = accumulate(primes_vec.begin(), primes_vec.end(), (uint64_t)0);
	
	return sum;
}

// For this method we're doing a couple of improvements:
// We're using the Sieve of Sundaram instead of the Sieve of Eratosthenes.
// We're also directly using the algorithm and modifying it to record only
// the sum instead of an array of primes, which avoids using the space for
// the array and uses only the loops for the sieve algorithm, saving an
// extra summation loop.
static uint64_t Method2(unsigned limit)
{
	if (limit < 2) {
		return 0;
	} else if (limit == 2) {
		return 2;
	} else {
		// 2, being the only even prime, gets specially included first
		uint64_t primes_sum = 2;

		// Set up an array of flags
		vector<bool> base_candidates(limit / 2, true);

		unsigned k = limit / 3; // Integer division

		// Sieve of Sundaram
		// Mark off odd integers of the form i + j + 2ij
		for (unsigned j = 1; j < k; ++j) {
			for (unsigned i = 1; i <= j; ++i) {
				if (i + j + 2 * i * j >= base_candidates.size()) {
					break;
				}
				base_candidates[i + j + 2 * i * j] = false;
			}
		}

		// Collect the sum for all remaining "true" candidates, which
		// must be prime
		for (unsigned i = 1; i < base_candidates.size(); ++i) {
			if (base_candidates[i]) {
				primes_sum += (uint64_t)(2 * i + 1);
			}
		}

		return primes_sum;
	}
}


PeProblem10::PeProblem10()
{
}


ostream &PeProblem10::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17." << endl <<
		"Find the sum of all the primes below two million." << endl << endl;

	return os;
}


ostream &PeProblem10::DisplaySolution(ostream &os)
{
	const uint64_t kTargetSum = 2000000;

	auto sum1 = Method1(kTargetSum);
	auto sum2 = Method2(kTargetSum);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << sum1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"Here we call the Sieve of Eratosthenes method we implemented for an" << endl <<
		"earlier problem, the simply sum the returned array of primes." << endl << endl <<
		"Method 1 solution: " << sum1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"In this implementation we use the Sieve of Sundaram (an improvement" << endl <<
		"over the Sieve of Eratosthenes), but we change the algorithm to record" << endl <<
		"the sum directly rather than first record an array of primes, saving" << endl <<
		"on memory space and removing an additional for loop." << endl << endl <<
		"Method 2 solution: " << sum2 << endl << endl;

	return os;
}

ostream &PeProblem10::ProfileSolutions(int n_trials, ostream &os)
{
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	const uint64_t kTargetSum = 1000000;
	uint64_t sum1 = 0, sum2 = 0;

	clock_t start_time(clock());
	for (int i = 0; i < n_trials; ++i) {
		sum1 = Method1(kTargetSum);
	}
	clock_t method_1_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		sum2 = Method2(kTargetSum);
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