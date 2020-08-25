#include "PeProblem8.h"

using namespace std;

namespace pe {

// Brute force solution.
// Test every window over the whole sequence.
// Throws an exception if window_size is greater than the sequence length.
// Treats a window_size of 0 as the same as a window_size of 1.
static uint64_t Method1(const vector<uint64_t> &digit_sequence, size_t window_size)
{
	// Max product cannot be less than 0 so we're using unsigned types
	uint64_t max_product = 0;

	// Error if the window size is too big
	if (window_size > digit_sequence.size()) {
		throw runtime_error("Window size exceeds sequence length.");
	}

	// Make sure window size is at least 1
	if (window_size < 1) {
		window_size = 1;
	}

	// Two concurrent iterators at start and end of the window
	auto it1 = digit_sequence.begin();
	auto it2 = it1 + window_size - 1;

	size_t i1 = 0;
	size_t i2 = window_size - 1;

	// Iterate the window over the sequence
	for (; it2 != digit_sequence.end(); ++it1, ++it2, ++i1, ++i2) {

		// Product of window - remember to accumulate to it2 + 1 (to include it2 itself)
		uint64_t digit_product = accumulate(it1, it2 + 1, (uint64_t)1, multiplies<uint64_t>());

		// Keep the product if it's the largest so far
		if (digit_product > max_product) {
			max_product = digit_product;
		}
	}

	return max_product;
}

// A loose optimisation of above:
// If a zero is encountered, move the window to one past the zero
// and continue. This will skip _some_, but not necessarily all zeros
// as it doesn't check for any new zeros _within_ the sequence.
// Throws an exception if window_size is greater than the sequence length.
// Treats a window_size of 0 as the same as a window_size of 1.
static uint64_t Method2(const vector<uint64_t> &digit_sequence, size_t window_size)
{
	// Max product cannot be less than 0 so we're using unsigned types
	uint64_t max_product = 0;

	// Error if the window size is too big
	if (window_size > digit_sequence.size()) {
		throw runtime_error("Window size exceeds sequence length.");
	}

	// Make sure window size is at least 1
	if (window_size < 1) {
		window_size = 1;
	}

	// Two concurrent iterators at start and end of the window
	auto it1 = digit_sequence.begin();
	auto it2 = it1 + window_size - 1;

	// Iterate the window over the sequence
	for (; it2 != digit_sequence.end(); ++it1, ++it2) {
		// Check for an incoming zero
		if (*it2 != 0) {
			// Product of window - remember to accumulate to it2 + 1 (to include it2 itself)
			uint64_t digit_product = accumulate(it1, it2 + 1, (uint64_t)1, multiplies<uint64_t>());

			// Keep the product if it's the largest so far
			if (digit_product > max_product) {
				max_product = digit_product;
			}
		} else {
			// If the next digit in the sequence is a zero, shift the window
			// to start at this zero (and let the ++it1, ++it2 then shift
			// it one past this zero)

			// Exception check in case we're near the end, since we can't
			// move an iterator past the end
			try {
				it1 += window_size - 1;
				it2 += window_size - 1;
			} catch (const exception &) {
				continue;
			}
		}
	}

	return max_product;
}


// A different approach:
// Scan the list for zeros and record their indices.
// Once these indices are known, it's possible to determine which sections of the
// sequence are "worth" scanning as they contain no zeros for at least <window_size>
// digits.
// As before, throws an exception if window_size is greater than the sequence length.
// Treats a window_size of 0 as the same as a window_size of 1.
static uint64_t Method3(const vector<uint64_t> &digit_sequence, size_t window_size)
{
	// Max product cannot be less than 0 so we're using unsigned types
	uint64_t max_product = 0;

	// Error if the window size is too big
	if (window_size > digit_sequence.size()) {
		throw runtime_error("Window size exceeds sequence length.");
	}

	// Make sure window size is at least 1
	if (window_size < 1) {
		window_size = 1;
	}


	///////
	int previous_zero_index = -1;

	for (size_t i = 0; i < digit_sequence.size(); ++i) {
		if (digit_sequence[i] == 0) {
			// Find length of zero-free zpan
			size_t span_length = i - (unsigned)(previous_zero_index + 1);

			// If span is at least window_size long, find the product(s)
			if (span_length >= (unsigned)window_size) {

				// Window checking between the current zero
				// and the previous zero.
				for (size_t start_ind = previous_zero_index + 1, 
					end_ind = previous_zero_index + window_size;
					end_ind < i; ++start_ind, ++end_ind) {

					// Product of the window
					// Product of window - remember to accumulate to end_ind + 1
					// (to include end_ind itself)
					uint64_t digit_product = accumulate(digit_sequence.begin() + start_ind, 
						digit_sequence.begin() + end_ind + 1, (uint64_t)1, multiplies<uint64_t>());

					// Keep the product if it's the largest so far
					if (digit_product > max_product) {
						max_product = digit_product;
					}
				}
			}

			previous_zero_index = (int)i;
		}
	}

	return max_product;
}


PeProblem8::PeProblem8()
{
}


ostream &PeProblem8::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"The four adjacent digits in the 1000-digit number that have the greatest" << endl <<
		"product are 9 * 9 * 8 * 9 = 5832." << endl << endl <<
		"    73167176531330624919225119674426574742355349194934" << endl <<
		"    96983520312774506326239578318016984801869478851843" << endl <<
		"    85861560789112949495459501737958331952853208805511" << endl <<
		"    12540698747158523863050715693290963295227443043557" << endl <<
		"    66896648950445244523161731856403098711121722383113" << endl <<
		"    62229893423380308135336276614282806444486645238749" << endl <<
		"    30358907296290491560440772390713810515859307960866" << endl <<
		"    70172427121883998797908792274921901699720888093776" << endl <<
		"    65727333001053367881220235421809751254540594752243" << endl <<
		"    52584907711670556013604839586446706324415722155397" << endl <<
		"    53697817977846174064955149290862569321978468622482" << endl <<
		"    83972241375657056057490261407972968652414535100474" << endl <<
		"    82166370484403199890008895243450658541227588666881" << endl <<
		"    16427171479924442928230863465674813919123162824586" << endl <<
		"    17866458359124566529476545682848912883142607690042" << endl <<
		"    24219022671055626321111109370544217506941658960408" << endl <<
		"    07198403850962455444362981230987879927244284909188" << endl <<
		"    84580156166097919133875499200524063689912560717606" << endl <<
		"    05886116467109405077541002256983155200055935729725" << endl <<
		"    71636269561882670428252483600823257530420752963450" << endl << endl <<
		"Find the thirteen adjacent digits in the 1000-digit number that have" << endl <<
		"the greatest product. What is the value of this product?" << endl << endl;

	return os;
}


ostream &PeProblem8::DisplaySolution(ostream &os)
{
	// This problem can easily exceed integer max values!
	// The max product of N sequential digits is 9^N, so to avoid overflow the
	// window size must be less than bit_depth/log2(9).
	// For uint8: < 2  (9^2 < 2^8, 9^3 > 2^8)
	// For uint16: < 5  (9^5 < 2^16, 9^6 > 2^16)
	// For uint32: < 10  (9^10 < 2^32, 9^11 > 2^32)
	// For uint64: < 20  (9^20 < 2^64, 9^21 > 2^64)
	//
	// If for some reason you're seeing this on a system than supports 128, 256
	// or higher integer bit depths, this limit roughly doubles for a doubling
	// in bit depth (although not exactly: 512/log2(9) ~= 161.518, so you could
	// get away with a window size of 161, rather than 160...)

	const unsigned kWindowSize = 13;
	const vector<uint64_t> kNumber({
		7,3,1,6,7,1,7,6,5,3,1,3,3,0,6,2,4,9,1,9,2,2,5,1,1,9,6,7,4,4,2,6,5,7,4,7,
		4,2,3,5,5,3,4,9,1,9,4,9,3,4,9,6,9,8,3,5,2,0,3,1,2,7,7,4,5,0,6,3,2,6,2,3,
		9,5,7,8,3,1,8,0,1,6,9,8,4,8,0,1,8,6,9,4,7,8,8,5,1,8,4,3,8,5,8,6,1,5,6,0,
		7,8,9,1,1,2,9,4,9,4,9,5,4,5,9,5,0,1,7,3,7,9,5,8,3,3,1,9,5,2,8,5,3,2,0,8,
		8,0,5,5,1,1,1,2,5,4,0,6,9,8,7,4,7,1,5,8,5,2,3,8,6,3,0,5,0,7,1,5,6,9,3,2,
		9,0,9,6,3,2,9,5,2,2,7,4,4,3,0,4,3,5,5,7,6,6,8,9,6,6,4,8,9,5,0,4,4,5,2,4,
		4,5,2,3,1,6,1,7,3,1,8,5,6,4,0,3,0,9,8,7,1,1,1,2,1,7,2,2,3,8,3,1,1,3,6,2,
		2,2,9,8,9,3,4,2,3,3,8,0,3,0,8,1,3,5,3,3,6,2,7,6,6,1,4,2,8,2,8,0,6,4,4,4,
		4,8,6,6,4,5,2,3,8,7,4,9,3,0,3,5,8,9,0,7,2,9,6,2,9,0,4,9,1,5,6,0,4,4,0,7,
		7,2,3,9,0,7,1,3,8,1,0,5,1,5,8,5,9,3,0,7,9,6,0,8,6,6,7,0,1,7,2,4,2,7,1,2,
		1,8,8,3,9,9,8,7,9,7,9,0,8,7,9,2,2,7,4,9,2,1,9,0,1,6,9,9,7,2,0,8,8,8,0,9,
		3,7,7,6,6,5,7,2,7,3,3,3,0,0,1,0,5,3,3,6,7,8,8,1,2,2,0,2,3,5,4,2,1,8,0,9,
		7,5,1,2,5,4,5,4,0,5,9,4,7,5,2,2,4,3,5,2,5,8,4,9,0,7,7,1,1,6,7,0,5,5,6,0,
		1,3,6,0,4,8,3,9,5,8,6,4,4,6,7,0,6,3,2,4,4,1,5,7,2,2,1,5,5,3,9,7,5,3,6,9,
		7,8,1,7,9,7,7,8,4,6,1,7,4,0,6,4,9,5,5,1,4,9,2,9,0,8,6,2,5,6,9,3,2,1,9,7,
		8,4,6,8,6,2,2,4,8,2,8,3,9,7,2,2,4,1,3,7,5,6,5,7,0,5,6,0,5,7,4,9,0,2,6,1,
		4,0,7,9,7,2,9,6,8,6,5,2,4,1,4,5,3,5,1,0,0,4,7,4,8,2,1,6,6,3,7,0,4,8,4,4,
		0,3,1,9,9,8,9,0,0,0,8,8,9,5,2,4,3,4,5,0,6,5,8,5,4,1,2,2,7,5,8,8,6,6,6,8,
		8,1,1,6,4,2,7,1,7,1,4,7,9,9,2,4,4,4,2,9,2,8,2,3,0,8,6,3,4,6,5,6,7,4,8,1,
		3,9,1,9,1,2,3,1,6,2,8,2,4,5,8,6,1,7,8,6,6,4,5,8,3,5,9,1,2,4,5,6,6,5,2,9,
		4,7,6,5,4,5,6,8,2,8,4,8,9,1,2,8,8,3,1,4,2,6,0,7,6,9,0,0,4,2,2,4,2,1,9,0,
		2,2,6,7,1,0,5,5,6,2,6,3,2,1,1,1,1,1,0,9,3,7,0,5,4,4,2,1,7,5,0,6,9,4,1,6,
		5,8,9,6,0,4,0,8,0,7,1,9,8,4,0,3,8,5,0,9,6,2,4,5,5,4,4,4,3,6,2,9,8,1,2,3,
		0,9,8,7,8,7,9,9,2,7,2,4,4,2,8,4,9,0,9,1,8,8,8,4,5,8,0,1,5,6,1,6,6,0,9,7,
		9,1,9,1,3,3,8,7,5,4,9,9,2,0,0,5,2,4,0,6,3,6,8,9,9,1,2,5,6,0,7,1,7,6,0,6,
		0,5,8,8,6,1,1,6,4,6,7,1,0,9,4,0,5,0,7,7,5,4,1,0,0,2,2,5,6,9,8,3,1,5,5,2,
		0,0,0,5,5,9,3,5,7,2,9,7,2,5,7,1,6,3,6,2,6,9,5,6,1,8,8,2,6,7,0,4,2,8,2,5,
		2,4,8,3,6,0,0,8,2,3,2,5,7,5,3,0,4,2,0,7,5,2,9,6,3,4,5,0
	});

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << Method3(kNumber, kWindowSize) << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"A simple solution: Have a \"window\" of adjacent digits and move it" << endl <<
		"along the number. Compute the product of each window and keep track" << endl <<
		"of the maximum product found so far. By the time the window reaches" << endl <<
		"the end, the maximum product in the number sequence will be found." << endl << endl <<
		"Method 1 solution: " << Method1(kNumber, kWindowSize) << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"A loose optimiation of method 1: Do the same window method, but if the" << endl <<
		"next digit encountered is a zero, shift the window by window_size to" << endl <<
		"entirely skip over this zero. This will skip some zeros, but will still" << endl <<
		"calculate products with zeros _inside_ the window (i.e. ones that end" << endl <<
		"up inside the window after a window_size shift)." << endl << endl <<
		"Method 2 solution: " << Method2(kNumber, kWindowSize) << endl << endl <<

		formatting::MethodHeader(3) << endl << endl <<
		"A different approach: Iterate over the list and keep track of the index" << endl <<
		"of the last zero found. If the current digit is also zero, check the" << endl <<
		"length of the span between the current and previous zero. If greater" << endl <<
		"than the window size, do the \"window shifting\" product search from" << endl <<
		"methods 1 & 2 within the current span. This should skip all the zeros" << endl <<
		"in the number and only check non-zero products." << endl << endl <<
		"Method 3 solution: " << Method3(kNumber, kWindowSize) << endl << endl;

	return os;
}

ostream &PeProblem8::ProfileSolutions(int n_trials, ostream &os)
{
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	const unsigned kWindowSize = 13;
	const vector<uint64_t> kNumber({
		7,3,1,6,7,1,7,6,5,3,1,3,3,0,6,2,4,9,1,9,2,2,5,1,1,9,6,7,4,4,2,6,5,7,4,7,
		4,2,3,5,5,3,4,9,1,9,4,9,3,4,9,6,9,8,3,5,2,0,3,1,2,7,7,4,5,0,6,3,2,6,2,3,
		9,5,7,8,3,1,8,0,1,6,9,8,4,8,0,1,8,6,9,4,7,8,8,5,1,8,4,3,8,5,8,6,1,5,6,0,
		7,8,9,1,1,2,9,4,9,4,9,5,4,5,9,5,0,1,7,3,7,9,5,8,3,3,1,9,5,2,8,5,3,2,0,8,
		8,0,5,5,1,1,1,2,5,4,0,6,9,8,7,4,7,1,5,8,5,2,3,8,6,3,0,5,0,7,1,5,6,9,3,2,
		9,0,9,6,3,2,9,5,2,2,7,4,4,3,0,4,3,5,5,7,6,6,8,9,6,6,4,8,9,5,0,4,4,5,2,4,
		4,5,2,3,1,6,1,7,3,1,8,5,6,4,0,3,0,9,8,7,1,1,1,2,1,7,2,2,3,8,3,1,1,3,6,2,
		2,2,9,8,9,3,4,2,3,3,8,0,3,0,8,1,3,5,3,3,6,2,7,6,6,1,4,2,8,2,8,0,6,4,4,4,
		4,8,6,6,4,5,2,3,8,7,4,9,3,0,3,5,8,9,0,7,2,9,6,2,9,0,4,9,1,5,6,0,4,4,0,7,
		7,2,3,9,0,7,1,3,8,1,0,5,1,5,8,5,9,3,0,7,9,6,0,8,6,6,7,0,1,7,2,4,2,7,1,2,
		1,8,8,3,9,9,8,7,9,7,9,0,8,7,9,2,2,7,4,9,2,1,9,0,1,6,9,9,7,2,0,8,8,8,0,9,
		3,7,7,6,6,5,7,2,7,3,3,3,0,0,1,0,5,3,3,6,7,8,8,1,2,2,0,2,3,5,4,2,1,8,0,9,
		7,5,1,2,5,4,5,4,0,5,9,4,7,5,2,2,4,3,5,2,5,8,4,9,0,7,7,1,1,6,7,0,5,5,6,0,
		1,3,6,0,4,8,3,9,5,8,6,4,4,6,7,0,6,3,2,4,4,1,5,7,2,2,1,5,5,3,9,7,5,3,6,9,
		7,8,1,7,9,7,7,8,4,6,1,7,4,0,6,4,9,5,5,1,4,9,2,9,0,8,6,2,5,6,9,3,2,1,9,7,
		8,4,6,8,6,2,2,4,8,2,8,3,9,7,2,2,4,1,3,7,5,6,5,7,0,5,6,0,5,7,4,9,0,2,6,1,
		4,0,7,9,7,2,9,6,8,6,5,2,4,1,4,5,3,5,1,0,0,4,7,4,8,2,1,6,6,3,7,0,4,8,4,4,
		0,3,1,9,9,8,9,0,0,0,8,8,9,5,2,4,3,4,5,0,6,5,8,5,4,1,2,2,7,5,8,8,6,6,6,8,
		8,1,1,6,4,2,7,1,7,1,4,7,9,9,2,4,4,4,2,9,2,8,2,3,0,8,6,3,4,6,5,6,7,4,8,1,
		3,9,1,9,1,2,3,1,6,2,8,2,4,5,8,6,1,7,8,6,6,4,5,8,3,5,9,1,2,4,5,6,6,5,2,9,
		4,7,6,5,4,5,6,8,2,8,4,8,9,1,2,8,8,3,1,4,2,6,0,7,6,9,0,0,4,2,2,4,2,1,9,0,
		2,2,6,7,1,0,5,5,6,2,6,3,2,1,1,1,1,1,0,9,3,7,0,5,4,4,2,1,7,5,0,6,9,4,1,6,
		5,8,9,6,0,4,0,8,0,7,1,9,8,4,0,3,8,5,0,9,6,2,4,5,5,4,4,4,3,6,2,9,8,1,2,3,
		0,9,8,7,8,7,9,9,2,7,2,4,4,2,8,4,9,0,9,1,8,8,8,4,5,8,0,1,5,6,1,6,6,0,9,7,
		9,1,9,1,3,3,8,7,5,4,9,9,2,0,0,5,2,4,0,6,3,6,8,9,9,1,2,5,6,0,7,1,7,6,0,6,
		0,5,8,8,6,1,1,6,4,6,7,1,0,9,4,0,5,0,7,7,5,4,1,0,0,2,2,5,6,9,8,3,1,5,5,2,
		0,0,0,5,5,9,3,5,7,2,9,7,2,5,7,1,6,3,6,2,6,9,5,6,1,8,8,2,6,7,0,4,2,8,2,5,
		2,4,8,3,6,0,0,8,2,3,2,5,7,5,3,0,4,2,0,7,5,2,9,6,3,4,5,0
	});

	clock_t start_time(clock());
	for (int i = 0; i < n_trials; ++i) {
		Method1(kNumber, kWindowSize);
	}
	clock_t method_1_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		Method2(kNumber, kWindowSize);
	}
	clock_t method_2_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		Method3(kNumber, kWindowSize);
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