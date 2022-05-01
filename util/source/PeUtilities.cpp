// Copyright 2020-2022 Paul Robertson
//
// PeUtilities.cpp
//
// Print formatting, math and other utility functions

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
void CollatzRecursion(const PeUint depth_limit, const PeUint value,
	const PeUint depth, unordered_map<PeUint, PeUint> &iteration_counts)
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
unordered_map<PeUint, PeUint> CollatzGraphIterations(const PeUint depth_limit)
{
	// Map to store iteration counts, initialised with the base case of
	// 1 needing 0 iterations.
	unordered_map<PeUint, PeUint> iteration_count({{1, 0}});

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
vector<PeUint> CollatzSequence(const PeUint starting_value)
{
	// Record the maximum length sequence.
	// Doing this separately once the maximum length starting number
	// has already been found, otherwise we'd be resizing and writing
	// a lot to the sequence vector.
	vector<PeUint> sequence;

	// Simple "blind" guess of memory needs
	PeUint reserved_length = 100;
	sequence.reserve(reserved_length);

	PeUint tmp_number =
		static_cast<PeUint>(starting_value);
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

// Calculate the digit sum of the of <num>: first, add the digits of <num>.
// If this sum is greater than 10, add the digits of the sum to form a new sum.
// Repeat this process until left with a number less than 10. For example:
//     num = 123456789
//     sum = 1+2+3+4+5+6+7+8+9 = 45
//     45 >= 10, so sum = 4+5 = 9
//     9 < 10, so the digital sum of 123456789 is 9
PeUint DigitalSum(PeUint num)
{
	PeUint sum = 0;
	PeUint tmp_num = num;

	// Outer loop: calculate sum of digits, check if sum > 9 and repeat with
	// the number replaced by the previous sum until sum <= 9
	do {
		sum = 0;

		// Internal loop to calculate sum of digits for the current number
		while (tmp_num > 0) {
			sum += (tmp_num % 10);
			tmp_num /= 10; // Integer division
		}
		tmp_num = sum;
	} while (sum > 9);

	return sum;
}

// Return a vector containing all factors of <trial_number>,
// from 1...trial_number.
// This is done by simply trialing each integer from 1...sqrt(trial_number)
// and noting that factors come in pairs:
// n = a * b
// if a <= sqrt(n) then b >= sqrt(n)
// b = n / a
vector<PeUint> Factors(PeUint trial_number)
{
	// Collect the factors in two arrays, since there's always
	// pairs of factors (except for squares). The trivial factors
	// of 1 and trial_number are inserted straight away.
	vector<PeUint> lower_factors({1});
	vector<PeUint> upper_factors({trial_number});

	// Test integers from 2...sqrt(trial_number) to find lower factors
	// For any lower factor, the upper factor is trial_number / lower_factor
	// Store each new factor at the end of its respective array.
	// This stores the lower factors is ascending order, while the
	// upper factors will be stored in descending order
	for (PeUint i = 2; i <= (PeUint)(sqrt((double)trial_number)); ++i) {
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
PeUint FibonacciDirect(PeUint n)
{
	return static_cast<PeUint>(round(pow(kPhi, n) / sqrt(5.0)));
}

// O(log N) calculation of the Nth Fibonacci number
// Works with integers so should be exact
// Note that N > 46 will overflow standard 32 bit int,
// so this function will return -1 in these cases.
static PeUint ff[64] = {0}; // Overflow past max just in case
PeUint FibonacciExact(PeUint n)
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

    PeUint k = (n & 1) ? (n + 1) / 2 : n / 2;

    // Applying above formula [Note value n&1 is 1
    // if n is odd, else 0.

	PeUint fk = FibonacciExact(k);
	PeUint fkm1 = FibonacciExact(k - 1);
    ff[n] = (n & 1) ?
		(fk * fk + fkm1 * fkm1) :
           (2 * fkm1 + fk) * fk;

    return ff[n];
}


// Generate array of primes up to <limit> using the
// Sieve of Eratosthenes method
vector<PeUint> GeneratePrimesEratosthenes(PeUint limit)
{
	// Quick exits for first few cases
	if (limit < 2) {
		return vector<PeUint>();
	} else if (limit == 2) {
		return vector<PeUint>({2});
	} else if (limit <= 4) { // Catch both 3 and 4
		return vector<PeUint>({2, 3});
	} else if (limit <= 6) { // Catch both 5 and 6
		return vector<PeUint>({2, 3, 5});
	} else if (limit <= 10) { // Catch 7-10
		return vector<PeUint>({2, 3, 5, 7});
	} else { // Ok, time to do actual calculation...

		// First, an estimate for the size of the array.
		// We're not worried about an exact estimate, just establishing
		// a reasonable upper bound.
		// The following estimate comes from:
		//   Rosser, J. Barkley; Schoenfeld, Lowell (1962).
		//   "Approximate formulas for some functions of prime numbers".
		//   Illinois J. Math. 6: 64–94. doi:10.1215/ijm/1255631807
		PeUint num_primes =
			(PeUint)(1.25506 * ((double)limit / log((double)limit)));

		// Set up the primes array
		vector<PeUint> primes_array;
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
		PeUint p = 2;
		PeUint p2 = 4;
		PeUint j = p2;

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
		for (PeUint i = p; i <= limit; ++i) {
			if (is_prime[i]) {
				primes_array.push_back(i);
			}
		}

		return primes_array;
	}
}


// Generate array of primes up to <limit> using the
// Sieve of Sundaram method
vector<PeUint> GeneratePrimesSundaram(PeUint limit)
{
	// Quick exits for first few cases
	if (limit < 2) {
		return vector<PeUint>();
	} else if (limit == 2) {
		return vector<PeUint>({2});
	} else if (limit <= 4) { // Catch both 3 and 4
		return vector<PeUint>({2, 3});
	} else if (limit <= 6) { // Catch both 5 and 6
		return vector<PeUint>({2, 3, 5});
	} else if (limit <= 10) { // Catch 7-10
		return vector<PeUint>({2, 3, 5, 7});
	} else { // Ok, time to do actual calculation...

		// First, an estimate for the size of the array.
		// We're not worried about an exact estimate, just establishing
		// a reasonable upper bound.
		// The following estimate comes from:
		//   Rosser, J. Barkley; Schoenfeld, Lowell (1962).
		//   "Approximate formulas for some functions of prime numbers".
		//   Illinois J. Math. 6: 64–94. doi:10.1215/ijm/1255631807
		PeUint num_primes =
			(PeUint)(1.25506 * ((double)limit / log((double)limit)));

		// Set up the primes array
		vector<PeUint> primes_array;
		primes_array.reserve(num_primes);
		primes_array.push_back(2);

		// Set up an array of flags
		vector<bool> base_candidates(limit / 2, true);

		PeUint k = limit / 3; // Integer division

		// Sieve of Sundaram
		// Mark off odd integers of the form i + j + 2ij
		for (PeUint j = 1; j < k; ++j) {
			for (PeUint i = 1; i <= j; ++i) {
				if (i + j + 2 * i * j >= base_candidates.size()) {
					break;
				}
				base_candidates[i + j + 2 * i * j] = false;
			}
		}

		for (PeUint i = 1; i < base_candidates.size(); ++i) {
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
tuple<PeUint, PeUint, PeUint> GeneratePythagoreanEuclidTriple(
	PeUint m, PeUint n, PeUint k)
{
	if ((k == 0) || (m == 0) || (n == 0)) {
		return tuple<PeUint, PeUint, PeUint>(0, 0, 0);
	}

	// Ensure m > n
	if (m <= n) {
		PeUint tmp;
		tmp = m;
		m = n;
		n = tmp;
	}

	PeUint a = m * m - n * n,
		b = 2 * m * n,
		c = m * m + n * n;

	return tuple<PeUint, PeUint, PeUint>(k * a, k * b, k * c);
}


// Test if three integers (a,b,c) are a Pythagorean triple
bool IsPythagoreanTriple(PeUint a, PeUint b, PeUint c)
{
	return (c * c == a * a + b * b);
}


// Lowest common multiple
// Lcm(a,b) = a*b/Gcd(a,b)
PeUint Lcm(PeUint a, PeUint b)
{
    PeUint temp = Gcd(a, b);

    return temp ? (a / temp * b) : 0;
}


// Find "N choose K", sometimes also written nCk, the number of possible
// combinations of k items from a set of n items.
// Using PeUint (64 bit unsigned) this will first overflow at n = 67, k = 32.
// Values of n < 67 won't overflow for any valid k.
PeUint NChooseK(PeUint n, PeUint k)
{
	// Sanity check
	if (k > n) {
		return 0;
	}

	// Some quick exits
	
	// nC0 = nCn = 1
	if ((k == 0) || (k == n)) {
		return 1;
	}

	// nC1 = nC(n-1) = n
	if ((k == 1) || (k == (n - 1))) {
		return n;
	}

    // num holds the value of n! (the numerator)
    // den holds the value of k! (the denominator)
    PeUint num = 1, den = 1;


    // Since nCk = nC(n-k), we choose the smaller value of k and n-k to reduce
	// the chances of overflow and shorten the while loop
    if (k > (n - k)) {
        k = n - k;
	}

	// Loop down from k...1 to iteratively calculate the numerator and denominator
    while (k > 0) {
        num *= n;
        den *= k;

        // Divide both numerator and denominator by their GCD.
		// This keeps the values smaller which further reduces the chances
		// of overflow
        PeUint m = Gcd(num, den);

        num /= m;
        den /= m;

        --n;
        --k;
    }

    return num;
}

// Return "radix buckets" of a number n with a given radix.
// For radix = 10, this will simply be a vector with each separate
// digit of n as one element of the vector e.g.
//   n = 1234, radix = 10 returns {4, 3, 2, 1}
// Note the "reverse order" which means element indices correspond
// to the radix power for each bucket. For the above example,
// we see that ind(4) = 0, i.e. 4 is in the 10^0 (ones) bucket,
// while ind(3) = 1, i.e. 3 is in the 10^1 (tens) bucket.
// Radix can be set to other values too.
// Setting n = 1234, radix = 100 returns {43, 21}.
vector<PeUint> NumberToRadixBuckets(PeUint n, PeUint radix)
{
	vector<PeUint> buckets;

	while (n >= radix) {
		buckets.push_back(n % radix);
		n /= radix; // Integer division
	}
	buckets.push_back(n);

	return buckets;
}


// Find the prime factors of <trial_number> using the
// Wheel Factorisation method with a basis of (2,3,5).
// Note this is not the most efficient algorithm out there,
// but it's reasonably simple and has a decent runtime for smaller
// numbers.
vector<PeUint> PrimeFactors(PeUint trial_number)
{
	// Bigger bases could be used for larger data types, but we'll
	// just stick with this basis for now
	const vector<PeUint> basis({2, 3, 5});

	// Vector to store factors
	// N.B. could be modified to only store the maximum
	vector<PeUint> factors;


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
	PeUint basis_mod = 30;

	// Increments for the wheel spokes (hard coded since the basis is hard coded)
	vector<PeUint> increments({4, 2, 4, 2, 4, 6, 2, 6});
	size_t i = 0; // Increment index

	// Starting point for the wheel
	PeUint k = 7;

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
PeUint ReverseDigits(PeUint num)
{
    PeUint rev_num = 0;
    while (num > 0) {
        rev_num = 10 * rev_num + num % 10;
        num /= 10; // Integer division
    }
    return rev_num;
}

// Calculate the sum of the digits of <num>. This is not the same as the
// "digital sum" common in number theory since it only does the first pass
// of that calculation.
PeUint SumDigits(PeUint num)
{
	PeUint sum = 0;

	while (num > 0) {
		sum += (num % 10);
		num /= 10; // Integer division
	}

	return sum;
}

// The nth pyramid number, the sum of integers from 1 to n
// Uses the analytic formula sum = (n*(n+1))/2
PeUint SumOfOneToN(PeUint n)
{
	return n * (n + 1) / 2;
}

// The sum of squared integers: 1^2 + 2^2 + ... n^2
// Uses the analytic formula sum = (n*(n+1)*(2*n+1))/6
PeUint SumOfSquaresOneToN(PeUint n)
{
	return n * (n + 1) * (2 * n + 1) / 6;
}

// The sum of squared integers: 1^3 + 2^3 + ... n^3
// Uses the analytic formula sum = (n*n*(n+1)*(n+1))/4
PeUint SumOfCubesOneToN(PeUint n)
{
	// This is the same polynomial in Horner form
	return n * n * (1 + n * (2 + n)) / 4;
}

};
}; // namespace pe