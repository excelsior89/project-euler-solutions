#include "PeUtilities.h"

using namespace std;

namespace pe {
namespace formatting {
// Standard header strings for problems, solutions and methods
string ProblemHeader(int problem_number)
{
	return kHeading1Dashes + string(" Problem ") +
		to_string(problem_number) +
		string(" ") + kHeading1Dashes;
}

string ProfileHeader(int solution_number)
{
	return kHeading1Dashes + string(" Profiling Problem ") +
		to_string(solution_number) +
		string(" ") + kHeading1Dashes;
}

string SolutionHeader(int solution_number)
{
	return kHeading1Dashes + string(" Solution ") +
		to_string(solution_number) +
		string(" ") + kHeading1Dashes;
}

string MethodHeader(int method_number)
{
	return kHeading2Dashes + string(" Method ") +
		to_string(method_number) +
		string(" ") + kHeading2Dashes;
}
};

namespace math {
// Helper recursion function for CollatzGraphIterations().
// This function adds <value>'s current <depth> to the map <iteration_counts>,
// then calls itself for 2*<value> and, if mod(value,6)=4, also for
// (<value>-1)/3.
void CollatzRecursion(const unsigned depth_limit, const unsigned value,
	const unsigned depth, unordered_map<unsigned, unsigned> &iteration_counts)
{
	// Update the map
	iteration_counts[value] = depth;

	// Check for recursion depth limit
	if (depth < depth_limit) {
		// Every value follows the "2n" path
		CollatzRecursion(depth_limit, 2 * value, depth + 1, iteration_counts);

		// Some values also follow an additional path for odd parity
		// determined by mod(value,6)=4. However, 4 itself is a special
		// case and will loop (4->2->1->4->2->1) indefinitely, so we need
		// to check for the value 4 itself
		if ((value != 4) && (value % 6 == 4)) {
			CollatzRecursion(depth_limit, (value - 1) / 3, depth + 1, iteration_counts);
		}
	}
}

// Generate a map of numbers to Collatz iterations for each number to reach 1.
// Collatz iterations refers to the sequence defined in the Collatz Conjecture
// (wiki link: https://en.wikipedia.org/wiki/Collatz_conjecture):
//    n -> n/2 (n is even)
//    n -> 3n + 1 (n is odd)
// The generation of this map is limited by the depth (maximum number of
// iterations) to search for. It essentially operates "in reverse" to the above
// mapping by starting at 1 and mapping to 2n, and (n-1)/3 if mod(n,6) is 4.
// This search operates recursively up to the specified depth limit.
unordered_map<unsigned, unsigned> CollatzGraphIterations(const unsigned depth_limit)
{
	// Map to store iteration counts, initialised with the base case of
	// 1 needing 0 iterations.
	unordered_map<unsigned, unsigned> iteration_count({{1, 0}});

	// Recursion to find iteration counts up to limit using helper function
	CollatzRecursion(depth_limit, 2, 1, iteration_count);

	return iteration_count;
}

// Construct the sequence formed by performing the iterative mapping defined in
// the Collatz Conjecture (https://en.wikipedia.org/wiki/Collatz_conjecture):
//    n -> n/2 (n is even)
//    n -> 3n + 1 (n is odd)
// This values in the sequence can greatly exceed the starting value.
// For this reason, the returned sequence uses long long (64 bit) integers,
// even though the starting value is limited to only regular (32 bit) integers.
vector<long long unsigned> CollatzSequence(const unsigned starting_value)
{
	// Record the maximum length sequence.
	// Doing this separately once the maximum length starting number
	// has already been found, otherwise we'd be resizing and writing
	// a lot to the sequence vector.
	vector<long long unsigned> sequence;

	// Simple "blind" guess of memory needs
	unsigned reserved_length = 100;
	sequence.reserve(reserved_length);

	long long unsigned tmp_number =
		static_cast<long long unsigned>(starting_value);
	size_t i = 0;

	// The iteration itself
	while (tmp_number != 1) {
		if (math::IsEven(tmp_number)) {
			tmp_number /= 2;
		} else {
			tmp_number = 3 * tmp_number + 1;
		}

		// Trying to be a bit smarter about memory allocation
		if (i > sequence.size()) {
			reserved_length *= 2;
			sequence.reserve(reserved_length);
		}

		sequence.push_back(tmp_number);
		++i;
	}

	return sequence;
}

// Return a vector containing all factors of <trial_number>,
// from 1...trial_number.
// This is done by simply trialing each integer from 1...sqrt(trial_number)
// and noting that factors come in pairs:
// n = a * b
// if a <= sqrt(n) then b >= sqrt(n)
// b = n / a
vector<unsigned> Factors(unsigned trial_number)
{
	// Collect the factors in two arrays, since there's always
	// pairs of factors (except for squares). The trivial factors
	// of 1 and trial_number are inserted straight away.
	vector<unsigned> lower_factors({1});
	vector<unsigned> upper_factors({trial_number});

	// Test integers from 2...sqrt(trial_number) to find lower factors
	// For any lower factor, the upper factor is trial_number / lower_factor
	// Store each new factor at the end of its respective array.
	// This stores the lower factors is ascending order, while the
	// upper factors will be stored in descending order
	for (unsigned i = 2; i <= (unsigned)(sqrt((double)trial_number)); ++i) {
		if (trial_number % i == 0) {
			lower_factors.push_back(i);
			upper_factors.push_back(trial_number / i);
		}
	}

	// Check for an integer square root, which will occur at the end of
	// both arrays. Remove this repeated factor from the upper_factors array
	// if it exists.
	if (lower_factors.back() == upper_factors.back()) {
		upper_factors.pop_back();
	}

	// Using reverse iterators to flip the order, append the upper_factors array
	// to the lower_factors array.
	copy(upper_factors.rbegin(), upper_factors.rend(), back_inserter(lower_factors));

	return lower_factors;
}

// O(1) calculation of the Nth Fibonacci number
// Inaccurate for large N as it uses floating point arithmetic
// Note that N > 46 will overflow standard 32 bit int
int FibonacciDirect(int n)
{
	return (int)round(pow(kPhi, n) / sqrt(5.0));
}

// O(log N) calculation of the Nth Fibonacci number
// Works with integers so should be exact
// Note that N > 46 will overflow standard 32 bit int,
// so this function will return -1 in these cases.
#define FIB_MAX 64
static int ff[64] = {0}; // Overflow past max just in case
int FibonacciExact(int n)
{
	// Sanity check
	if (n > 46)
		return -1;

    // Base cases
    if (n == 0)
        return 0;
    if (n == 1 || n == 2)
        return (ff[n] = 1);

    // If Nth Fibonacci number is already computed
    if (ff[n])
        return ff[n];

    int k = (n & 1) ? (n + 1) / 2 : n / 2;

    // Applying above formula [Note value n&1 is 1
    // if n is odd, else 0.

	int fk = FibonacciExact(k);
	int fkm1 = FibonacciExact(k - 1);
    ff[n] = (n & 1) ?
		(fk * fk + fkm1 * fkm1) :
           (2 * fkm1 + fk) * fk;

    return ff[n];
}
#undef FIB_MAX


// Greatest common divisor using Euler's algorithm
int Gcd(int a, int b)
{
	// Alternative for a while (true)
	// This could also be implemented using recursion,
	// I've gone with looping here since the code is
	// similarly clear (IMO) and this uses less of the
	// call stack.
    for (;;) {
        if (a == 0) {
			return b;
		}
        b %= a;

        if (b == 0) {
			return a;
		}
        a %= b;
    }
}


// Generate array of primes up to <limit> using the
// Sieve of Eratosthenes method
vector<unsigned> GeneratePrimesEratosthenes(unsigned limit)
{
	// Quick exits for first few cases
	if (limit < 2) {
		return vector<unsigned>();
	} else if (limit == 2) {
		return vector<unsigned>({2});
	} else if (limit <= 4) { // Catch both 3 and 4
		return vector<unsigned>({2, 3});
	} else if (limit <= 6) { // Catch both 5 and 6
		return vector<unsigned>({2, 3, 5});
	} else if (limit <= 10) { // Catch 7-10
		return vector<unsigned>({2, 3, 5, 7});
	} else { // Ok, time to do actual calculation...

		// First, an estimate for the size of the array.
		// We're not worried about an exact estimate, just establishing
		// a reasonable upper bound.
		// The following estimate comes from:
		//   Rosser, J. Barkley; Schoenfeld, Lowell (1962).
		//   "Approximate formulas for some functions of prime numbers".
		//   Illinois J. Math. 6: 64–94. doi:10.1215/ijm/1255631807
		unsigned num_primes =
			(unsigned)(1.25506 * ((double)limit / log((double)limit)));

		// Set up the primes array
		vector<unsigned> primes_array;
		primes_array.reserve(num_primes);

		// Set up an array of flags
		vector<bool> is_prime(limit + 1, true);
		is_prime[0] = false;
		is_prime[1] = false;

		// Incrementally mark off numbers as
		// not prime by counting up to the limit
		// in multiples of primes i.e.
		// starting at 2, mark off 4, 6, 8 etc...
		// then starting at 3, mark off 3, 6, 9, 12...
		// then starting at 5 (skip 4 because it's already been marked non-prime)
		// then starting at 7 and so on
		unsigned p = 2;
		unsigned p2 = 4;
		unsigned j = p2;

		while (p2 <= limit) { // Only check to sqrt(limit)
			j = p2;
			primes_array.push_back(p);
			// Mark off multiples of prime
			while (j <= limit) {
				is_prime[j] = false;
				j += p;
			}

			// Find the next number still marked as prime
			do {
				++p;
			} while (!is_prime[p]);

			// Take square
			p2 = p * p;
		}

		// Find remaining primes greater than sqrt(limit)
		for (unsigned i = p; i <= limit; ++i) {
			if (is_prime[i]) {
				primes_array.push_back(i);
			}
		}

		return primes_array;
	}
}


// Generate array of primes up to <limit> using the
// Sieve of Sundaram method
vector<unsigned> GeneratePrimesSundaram(unsigned limit)
{
	// Quick exits for first few cases
	if (limit < 2) {
		return vector<unsigned>();
	} else if (limit == 2) {
		return vector<unsigned>({2});
	} else if (limit <= 4) { // Catch both 3 and 4
		return vector<unsigned>({2, 3});
	} else if (limit <= 6) { // Catch both 5 and 6
		return vector<unsigned>({2, 3, 5});
	} else if (limit <= 10) { // Catch 7-10
		return vector<unsigned>({2, 3, 5, 7});
	} else { // Ok, time to do actual calculation...

		// First, an estimate for the size of the array.
		// We're not worried about an exact estimate, just establishing
		// a reasonable upper bound.
		// The following estimate comes from:
		//   Rosser, J. Barkley; Schoenfeld, Lowell (1962).
		//   "Approximate formulas for some functions of prime numbers".
		//   Illinois J. Math. 6: 64–94. doi:10.1215/ijm/1255631807
		unsigned num_primes =
			(unsigned)(1.25506 * ((double)limit / log((double)limit)));

		// Set up the primes array
		vector<unsigned> primes_array;
		primes_array.reserve(num_primes);
		primes_array.push_back(2);

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

		for (unsigned i = 1; i < base_candidates.size(); ++i) {
			if (base_candidates[i]) {
				primes_array.push_back(2 * i + 1);
			}
		}

		return primes_array;
	}
}


// Generate a Pythagorean triple determined by two integers (m, n) such that m > n.
// If m <= n, they will be swapped. This method uses Euclid's formula:
//		a = m^2 - n^2, b = 2*m*n, c = m^2 + n^2
tuple<unsigned, unsigned, unsigned> GeneratePythagoreanEuclidTriple(
	unsigned m, unsigned n, unsigned k)
{
	if ((k == 0) || (m == 0) || (n == 0)) {
		return tuple<unsigned, unsigned, unsigned>(0, 0, 0);
	}

	// Ensure m > n
	if (m <= n) {
		unsigned tmp;
		tmp = m;
		m = n;
		n = tmp;
	}

	unsigned a = m * m - n * n,
		b = 2 * m * n,
		c = m * m + n * n;

	return tuple<unsigned, unsigned, unsigned>(k * a, k * b, k * c);
}


// Test if three integers (a,b,c) are a Pythagorean triple
bool IsPythagoreanTriple(unsigned a, unsigned b, unsigned c)
{
	return (c * c == a * a + b * b);
}


// Lowest common multiple
// Lcm(a,b) = a*b/Gcd(a,b)
int Lcm(int a, int b)
{
    int temp = Gcd(a, b);

    return temp ? (a / temp * b) : 0;
}


// Find the prime factors of <trial_number> using the
// Wheel Factorisation method with a basis of (2,3,5).
// Note this is not the most efficient algorithm out there,
// but it's reasonably simple and has a decent runtime for smaller
// numbers.
vector<unsigned> PrimeFactors(unsigned trial_number)
{
	// Bigger bases could be used for larger data types, but we'll
	// just stick with this basis for now
	const vector<unsigned> basis({2, 3, 5});

	// Vector to store factors
	// N.B. could be modified to only store the maximum
	vector<unsigned> factors;


	// Trial division by the basis primes
	for (size_t i = 0; i < basis.size(); ++i) {
		while (trial_number % basis[i] == 0) {
			factors.push_back(basis[i]);
			trial_number /= basis[i];
		}
	}

	// Check if we need to do any more factorisation
	if (trial_number == 1) {
		return factors;
	}

	// The modulo (product) of the basis primes
	unsigned basis_mod = 30;

	// Increments for the wheel spokes (hard coded since the basis is hard coded)
	vector<unsigned> increments({4, 2, 4, 2, 4, 6, 2, 6});
	size_t i = 0; // Increment index

	// Starting point for the wheel
	unsigned k = 7;

	// If our current trial number lies in the first rotation of the wheel then it
	// must be a smaller prime
	if (trial_number < k * k) {
		while (k <= trial_number) {
			if (trial_number % k == 0) {
				factors.push_back(k);
				trial_number /= k;
			} else {
				k += increments[i];
				++i;

				// Once we reach the last increment, go back to the first
				// (the "revolutions" of the wheel)
				if (i >= increments.size()) {
					i = 0;
				}
			}
		}

		return factors;
	}

	// Now we test k until k^2 exceeds the trial numbers
	// We increment k using the increment steps in the increments array
	// which is more efficient than simply checking every integer

	while (k * k <= trial_number) {
		if (trial_number % k == 0) {
			factors.push_back(k);
			trial_number /= k;
		} else {
			k += increments[i];
			++i;

			// Once we reach the last increment, go back to the first
			// (the "revolutions" of the wheel)
			if (i >= increments.size()) {
				i = 0;
			}
		}
	}

	// Capture any lingering factor
	if (trial_number > 1) {
		factors.push_back(trial_number);
	}

	return factors;
}


// Reverse an integers digits, useful for testing palindromes
int ReverseDigits(int num)
{
    int rev_num = 0;
    while (num > 0) {
        rev_num = 10 * rev_num + num % 10;
        num /= 10; // Integer division
    }
    return rev_num;
}

// The nth pyramid number, the sum of integers from 1 to n
// Uses the analytic formula sum = (n*(n+1))/2
int SumOfOneToN(int n)
{
	return n * (n + 1) / 2;
}

// The sum of squared integers: 1^2 + 2^2 + ... n^2
// Uses the analytic formula sum = (n*(n+1)*(2*n+1))/6
int SumOfSquaresOneToN(int n)
{
	return n * (n + 1) * (2 * n + 1) / 6;
}

// The sum of squared integers: 1^3 + 2^3 + ... n^3
// Uses the analytic formula sum = (n*n*(n+1)*(n+1))/4
int SumOfCubesOneToN(int n)
{
	// This is the same polynomial in Horner form
	return n * n * (1 + n * (2 + n)) / 4;
}

};
}; // namespace pe