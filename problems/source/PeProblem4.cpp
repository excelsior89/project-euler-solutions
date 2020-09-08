#include "PeProblem4.h"

using namespace std;

namespace pe {
// Brute force search, test products all pairs of n digit numbers
// to see if they're palindromes. Record the largest palindrome found
// so far. The search space is exhaustive, so the largest palindrome
// will be guaranteed to be found by the end of the search.
// Runtime is O(10^2n), so not good.
static int Method1(int digit_size)
{
	int start_value = (int)pow(10, digit_size) - 1;
	int end_value = (int)pow(10, digit_size - 1);

	// Initialise with smallest possible palindrome
	// 1[0...]1 * 1[0...]1
	// where 1[0...]1 is the requested number digits, e.g.:
	// 2 digits --> 11*11 = 121
	// 3 digits --> 101*101 = 10201
	// 4 digits --> 1001*1001 = 1002001
	int largest_palindrome = (end_value + 1) * (end_value + 1);

	// Loop over the trial pairs. Since multiplication is commutative,
	// we only need to check j = [i...end], which halves the number
	// of values we need to test.
	for (int i = start_value; i > end_value; --i) {
		for (int j = i; j > end_value; --j) {
			if (math::ReverseDigits(i * j) == i * j) {
				largest_palindrome = largest_palindrome < i * j ? i * j : largest_palindrome;
			}
		}
	}


	return largest_palindrome;
}

// Brute-force top-down approach:
// Generate palindromic numbers and factorise
static int Method2(unsigned digit_size)
{
	// We can generate palindromic numbers as:
	// 10^n*(abc...) + (...cba)
	// where abc... are the digits of an n-digit number
	// Example:
	// from 856 we generate:
	// 1000*856+658 = 856000+658 = 856658

	// Since we're trying to find the largest, we'll start from
	// 10^n - 1 = 9....9 and work down. This ensures that we test
	// the palindromes in descending order, so the first valid
	// one we find will be the largest and we can stop looking

	unsigned upper_ten_base = (unsigned)pow(10, digit_size);
	unsigned lower_ten_base = (unsigned)pow(10, digit_size - 1);

	for (unsigned i = upper_ten_base - 1; i > lower_ten_base; --i) {
		// Generate the palindrome number
		unsigned palindrome_number = upper_ten_base * i + math::ReverseDigits(i);

		// Factorise the palindrome number
		auto factors = math::Factors(palindrome_number);

		// Factors come in pairs, so we look at indices which are the same
		// distance from the start and end of the factors array for matched
		// pairs...
		for (size_t il = 1, iu = factors.size() - 2; il < iu; ++il, --iu) {
			// If we find a factor pair where the lower factor is >10^(n-1) and
			// the upper is <10^n then this number fulfils our requirement and we
			// can end the search
			if ((factors[il] > lower_ten_base) && (factors[iu] < upper_ten_base)) {
				return palindrome_number;
			}
		}
	}


	// Second attempt: if the above has failed to find anything, there is another
	// set of (smaller) palindrome numbers we can generate which contain one fewer
	// digit. These will have an odd number of digits and be of the form:
	// 10^(n-1)*(ab...xyz) + (yx...ba) i.e. the last digit of the generating
	// number is dropped.
	//
	// Example:
	// from 856 we generate:
	// 100*856+58 = 85600+58 = 85658
	for (unsigned i = upper_ten_base - 1; i > lower_ten_base; --i) {
		// Generate the palindrome number (note integer division)
		unsigned palindrome_number = lower_ten_base * i + math::ReverseDigits(i / 10);

		// Factorise the palindrome number
		auto factors = math::Factors(palindrome_number);

		// Factors come in pairs, so we look at indices which are the same
		// distance from the start and end of the factors array for matched
		// pairs...
		for (size_t il = 1, iu = factors.size() - 2; il < iu; ++il, --iu) {
			// If we find a factor pair where the lower factor is >10^(n-1) and
			// the upper is <10^n then this number fulfils our requirement and we
			// can end the search
			if ((factors[il] > lower_ten_base) && (factors[iu] < upper_ten_base)) {
				return palindrome_number;
			}
		}
	}

	// Default/worst case:
	// We know that 1[0...]1 * 1[0...]1 will always be the smallest
	// palindrome number fitting the criteria, so if the above has failed
	// to find anything then this must be the only possible answer
	return (lower_ten_base + 1) * (lower_ten_base + 1);
}


// Divisibility rules:
// Palindromic numbers have to be divisible by 11
// if they have an even number of digits:
//	(1[0...]1)*a + (1[0...]10)*b + (11[0...])*c
//	1[0...]1 is divisible by 11 if the number of 0s is even:
//	1[0...]{2k}1
//		= 10^(2k+1) + 1
//		= 10*(10^2)^k + 1
//		= 10*(99+1)^k + 1
//		= 10*(9*11+1)^k + 1
//		= 10*(9^k*11^k + C*9^(k-1)*11^(k-1) + ... + 1) + 1
//		= 10*9^k*11^k + 10*C*9^(k-1)*11^(k-1) + ... + 10 + 1
//		= 10*9^k*11^k + 10*C*9^(k-1)*11^(k-1) + ... + 11
//		= 11*(10*9^k*11^(k-1) + 10*C*9^(k-1)*11^(k-2) + ... + 1)
static int Method3(unsigned digit_size)
{
	// We can generate palindromic numbers as:
	// 10^n*(abc...) + (...cba)
	// where abc... are the digits of an n-digit number
	// Example:
	// from 856 we generate:
	// 1000*856+658 = 856000+658 = 856658

	// Since we're trying to find the largest, we'll start from
	// 10^n - 1 = 9....9 and work down. This ensures that we test
	// the palindromes in descending order, so the first valid
	// one we find will be the largest and we can stop looking

	unsigned upper_ten_base = (unsigned)pow(10, digit_size);
	unsigned lower_ten_base = (unsigned)pow(10, digit_size - 1);
	unsigned palindrome_number = 0;

	for (unsigned i = upper_ten_base - 1; i > lower_ten_base; --i) {
		// Generate the palindrome number
		palindrome_number = upper_ten_base * i + math::ReverseDigits(i);

		// Since we known palindrome numbers are divisible by 11 (for even digits),
		// we can search multiples of 11 as trial factors
		for (unsigned j = upper_ten_base - 10; j > lower_ten_base; j -= 11) {
			if (palindrome_number % j == 0) { // Found a factor

				// Does the n-digit factor have another n-digit number as
				// its co-factor?
				if ((palindrome_number / j < upper_ten_base) &&
					(palindrome_number >= lower_ten_base)) {
					return palindrome_number;
				}
			}
		}
	}

	// Default/worst case:
	// We know that 1[0...]1 * 1[0...]1 will always be the smallest
	// palindrome number fitting the criteria, so if the above has failed
	// to find anything then this must be the only possible answer
	return (lower_ten_base + 1) * (lower_ten_base + 1);
}

PeProblem4::PeProblem4()
{
}


ostream &PeProblem4::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"A palindromic number reads the same both ways. The largest palindrome" << endl <<
		"made from the product of two 2-digit numbers is 9009 = 91x99." << endl <<
		"Find the largest palindrome made from the product of two 3-digit numbers." << endl << endl;

	return os;
}


ostream &PeProblem4::DisplaySolution(ostream &os)
{
	const int kDigitSize = 3;
	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << Method1(kDigitSize) << endl << endl <<
		formatting::MethodHeader(1) << endl << endl <<
		"Brute-force bottom-up approach:" << endl <<
		"Test all 3-digit products and check which ones are palindromes. As a" << endl <<
		"*slight* optimisation, we note that a*b=b*a, so we can halve the search" << endl <<
		"space by each combination of 3-digit numbers once." << endl << endl <<
		"Method 1 solution: " << Method1(kDigitSize) << endl << endl <<


		formatting::MethodHeader(2) << endl << endl <<
		"Top-down approach:" << endl <<
		"Generate 6 digit palindromes in descending order and factorise them." << endl <<
		"Find the first (and therefore largest) one with two 3 digit numbers as" << endl <<
		"a pair of factors. As a fallback we can test 5 digit numbers as well." << endl <<
		"The factorisation here is not prime factorisation so we simply check" << endl <<
		"integers from 1...sqrt(n) for the trial number and record the pairs" << endl <<
		"of factors (b, a=n/b)." << endl << endl <<
		"Method 2 solution: " << Method2(kDigitSize) << endl << endl <<


		formatting::MethodHeader(3) << endl << endl <<
		"Top-down approach with an optimisation:" << endl <<
		"As before, generate 6 digit palindromes in descending order." << endl <<
		"Since palindrome numbers with even numbers of digits are divisible by 11," << endl <<
		"we can limit our search for 3 digit factors to multiples of 11." << endl <<
		"Once we find a 3 digit multiple of 11 whose cofactor is also 3 digits," << endl <<
		"we can return the palindrome number as we know this is the largest since" << endl <<
		"we've been testing them in descending order." << endl << endl <<
		"Method 3 solution: " << Method3(kDigitSize) << endl << endl;
	return os;
}

ostream &PeProblem4::ProfileSolutions(int n_trials, ostream &os)
{
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	const int kDigitSize = 3;

	clock_t start_time(clock());
	for (int i = 0; i < n_trials; ++i) {
		Method1(kDigitSize);
	}
	clock_t method_1_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		Method2(kDigitSize);
	}
	clock_t method_2_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		Method3(kDigitSize);
	}
	clock_t method_3_time = clock() - start_time;

	os << formatting::MethodHeader(1) << endl << endl <<
		"Time average over " << n_trials << " trials: " <<
		(long double)method_1_time / (long double)n_trials << endl << endl <<
		formatting::MethodHeader(2) << endl << endl <<
		"Time average over " << n_trials << " trials: " <<
		(long double)method_2_time / (long double)n_trials << endl << endl <<
		formatting::MethodHeader(3) << endl << endl <<
		"Time average over " << n_trials << " trials: " <<
		(long double)method_3_time / (long double)n_trials << endl << endl;

	return os;
}

}; // namespace pe