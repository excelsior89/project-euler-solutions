// Copyright 2020 Paul Robertson
//
// PeProblem14.cpp
//
// "Longest Collatz sequence"

#include "PeProblem14.h"

using namespace std;

namespace pe {

// Brute force: starting at 1, apply Collatz iterations to each integer up to
// the limit. Record the number of iterations needed to reach one for each
// starting integer and return the integer whose sequence contained the most
// terms.
static PeUint Method1(PeUint start_number_limit,
	vector<PeUint> &sequence)
{
	// Start at 1, with a sequence length of 1
	PeUint max_length_start_number = 1;
	PeUint max_sequence_length = 1;

	// Check every integer up to the limit
	for (PeUint i = 2; i < start_number_limit; ++i) {
		PeUint tmp_number = i;
		PeUint sequence_length = 1;

		// The Collatz iteration itself:
		// If even, divide by 2
		// If odd, multiply by 3 and add 1
		while (tmp_number != 1) {
			if (math::IsEven(tmp_number)) {
				tmp_number /= 2;
			} else {
				tmp_number = 3 * tmp_number + 1;
			}
			++sequence_length;
		}

		// Check if the new sequence is longer than the previous maximum
		if (sequence_length > max_sequence_length) {
			max_sequence_length = sequence_length;
			max_length_start_number = i;
		}
	}

	// Record the maximum length sequence (just for interest's sake)
	sequence = math::CollatzSequence(max_length_start_number);

	return max_length_start_number;
}


// In this method, instead of simply computing the sequence every time, we
// record the sequence lengths of the numbers already tested. If a sequence
// that we're testing reaches a number that has previously been tested, we
// already know how many more steps are required to reach 1, so we can stop
// searching and simply add on the number of steps from the previously tested
// number.
static PeUint Method2(PeUint start_number_limit,
	vector<PeUint> &sequence)
{
	// Start at 1, with a sequence length of 1
	PeUint max_length_start_number = 1;
	PeUint max_sequence_length = 1;

	// Record the lengths of the sequences for any numbers used so far
	// up to twice the start limit (a rather arbitrary choice but some
	// of the numbers reached in these sequenences can become orders of
	// magnitude larger and will rapidly use up available RAM).
	// We could use a map here but the cost of expanding the map, hashing
	// etc. over the large number of values is ultimately slower
	// than using a vector with direct index access and limited,
	// pre-allocated size.
	vector<PeUint> sequence_lengths(2 * start_number_limit, 0);
	sequence_lengths[1] = 1;

	// Check every integer up to the limit
	for (PeUint i = 2; i < start_number_limit; ++i) {
		PeUint tmp_number = i;
		PeUint sequence_length = 1;

		// The Collatz iteration itself:
		// If even, divide by 2
		// If odd, multiply by 3 and add 1
		while (tmp_number != 1) {
			if (math::IsEven(tmp_number)) {
				tmp_number /= 2;
			} else {
				tmp_number = 3 * tmp_number + 1;
			}

			// If we reach a known sequence length, we can append it
			// to the current sequence length and break. Note we need
			// to check that tmp_number is within the range of sequence
			// lengths that we've recorded.
			if (((tmp_number < sequence_lengths.size())) &&
				(sequence_lengths[tmp_number] > 0)) {
				sequence_length += sequence_lengths[tmp_number];
				break;
			}

			++sequence_length;
		}

		// Record the sequence length found for this number
		sequence_lengths[i] = sequence_length;

		// Check if the new sequence is longer than the previous maximum
		if (sequence_length > max_sequence_length) {
			max_sequence_length = sequence_length;
			max_length_start_number = i;
		}
	}

	// Record the maximum length sequence (just for interest's sake)
	sequence = math::CollatzSequence(max_length_start_number);

	return max_length_start_number;
}


// This method utilises a speedup that allows multiple iterations of the to be
// taken each calculation. This begins with a slight modification to the odd
// part of the formula: n -> (3n + 1)/2 i.e. adding in the next "divide by two"
// step, since if n is odd, 3n+1 must be even.
//
// This optimisation creates a classic time vs space tradeoff. Larger numbers
// of iterations per step require more precomputation and more storage space,
// but can speed up overall computation. The net result, from some empirical
// testing, seems to show a diminishing return with increasing k. Selecting
// k between 4-10 seems to give a (very crude) estimate of up to a 40% speed
// increase, with the increased procomputation time eventually starting to
// erode the speed gains of large iterations per step.
//
// Iterating k times on n = (2^k)*a + b can be calculated by (3^c)*a + d.
// Here, we split the number n into b, its k least significant bits, and
// (2^k)*a, the remaining bits. For example, 27 is 11011 in binary. For k=3,
// we would split 11011 into 11|011, giving b = 011 and a = 11.
// C and d are precalculated for all values up to 2^k-1. D is the result of
// iterating k times on b, and c is the number of odd values encountered on the
// way. For our example, b = 011 = 3.
// D is therefore:  3 -> 5 -> 8 -> 4 (3 iterations)
// C is therefore 2 (2 odd numbers, 3 and 5, were encountered).
//
// We can therefore calculate that after 3 iterations, 27 will become
// (3^2)*3 + 4 = 31. Checking: 27 -> 41 -> 62 -> 31
//
// Combining this with the caching from Method 2 should enable an even faster
// search of iteration lengths for numbers up to a given limit.
//
// We also use a "reverse" mapping to compute the numbers within k iterations
// of 1 to catch the ends of the iterations e.g. if the next term is 8 and we're
// using k=5, we'll miss the end point of 1 since that's only 4 steps away.
// By precomputing the values within 5 steps of 1, we instead identify 8 as
// being in this set and can terminate the iterations and find the correct
// sequence length.
static PeUint Method3(PeUint start_number_limit,
	vector<PeUint> &sequence, PeUint iterations_per_step = 5)
{
	// Some "pre-precomputed" arrays for lower iterations per step
	// A little bit of a "cheat" to extract more performance
	static const vector<vector<PeUint>> c_pre = {
		{ // 2
			0, 1, 1, 2
		},
		{ // 3
			0, 2, 1, 2, 1, 1, 2, 3
		},
		{ // 4
			0, 2, 2, 2, 1, 1, 2, 3, 1, 3, 1, 3, 2, 2, 3, 4
		},
		{ // 5
			0, 3, 2, 2, 2, 2, 2, 4, 1, 4, 1, 3, 2, 2, 3, 4,
			1, 2, 3, 3, 1, 1, 3, 3, 2, 3, 2, 4, 3, 3, 4, 5
		},
		{ // 6
			0, 3, 3, 3, 2, 2, 2, 4, 2, 4, 2, 3, 2, 2, 4, 4,
			1, 3, 4, 4, 1, 1, 3, 3, 2, 3, 2, 5, 3, 3, 4, 5,
			1, 4, 2, 2, 3, 3, 3, 5, 1, 5, 1, 4, 3, 3, 3, 5,
			2, 2, 3, 3, 2, 2, 4, 4, 3, 4, 3, 4, 4, 4, 5, 6
		},
		{ // 7
			0, 4, 3, 3, 3, 3, 3, 4, 2, 5, 2, 4, 2, 2, 4, 4,
			2, 3, 4, 4, 2, 2, 3, 3, 2, 4, 2, 6, 4, 4, 4, 6,
			1, 4, 3, 3, 4, 4, 4, 5, 1, 6, 1, 4, 3, 3, 3, 5,
			2, 3, 3, 3, 2, 2, 5, 5, 3, 4, 3, 4, 4, 4, 5, 6,
			1, 3, 4, 4, 2, 2, 2, 5, 3, 4, 3, 3, 3, 3, 5, 5,
			1, 4, 5, 5, 1, 1, 4, 4, 3, 3, 3, 5, 3, 3, 5, 5,
			2, 5, 2, 2, 3, 3, 3, 6, 2, 5, 2, 5, 4, 4, 4, 6,
			3, 2, 4, 4, 3, 3, 4, 4, 4, 5, 4, 5, 5, 5, 6, 7
		},
		{ // 8
			0, 4, 4, 4, 3, 3, 3, 5, 3, 5, 3, 4, 3, 3, 4, 4, 2, 3, 5, 5, 2, 2,
			4, 4, 2, 5, 2, 7, 4, 4, 4, 6, 2, 4, 3, 3, 4, 4, 4, 5, 2, 6, 2, 4,
			3, 3, 3, 6, 2, 4, 4, 4, 2, 2, 6, 6, 4, 5, 4, 4, 4, 4, 6, 6, 1, 3,
			4, 4, 3, 3, 3, 6, 4, 5, 4, 3, 4, 4, 5, 5, 1, 5, 6, 6, 1, 1, 4, 4,
			3, 4, 3, 6, 3, 3, 5, 5, 2, 5, 3, 3, 3, 3, 3, 7, 2, 5, 2, 5, 5, 5,
			5, 6, 3, 2, 4, 4, 3, 3, 4, 4, 4, 6, 4, 5, 5, 5, 6, 7, 1, 5, 3, 3,
			4, 4, 4, 4, 2, 6, 2, 5, 2, 2, 5, 5, 3, 4, 4, 4, 3, 3, 3, 3, 3, 4,
			3, 6, 5, 5, 5, 7, 1, 5, 4, 4, 5, 5, 5, 6, 1, 7, 1, 5, 4, 4, 4, 5,
			3, 3, 3, 3, 3, 3, 5, 5, 3, 4, 3, 5, 5, 5, 5, 7, 2, 4, 5, 5, 2, 2,
			2, 5, 3, 4, 3, 4, 3, 3, 6, 6, 2, 4, 5, 5, 2, 2, 5, 5, 4, 3, 4, 5,
			4, 4, 6, 6, 3, 6, 2, 2, 4, 4, 4, 6, 3, 6, 3, 6, 4, 4, 4, 7, 4, 3,
			5, 5, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 7, 8
		},
		{ // 9
			0, 5, 4, 4, 4, 4, 4, 5, 3, 5, 3, 4, 3, 3, 5, 5, 3, 3, 5, 5, 3, 3,
			4, 4, 3, 5, 3, 7, 4, 4, 4, 7, 2, 5, 3, 3, 5, 5, 5, 5, 2, 7, 2, 4,
			4, 4, 4, 7, 2, 5, 5, 5, 2, 2, 7, 7, 4, 5, 4, 5, 4, 4, 6, 6, 2, 4,
			4, 4, 3, 3, 3, 6, 4, 6, 4, 3, 4, 4, 5, 5, 2, 5, 6, 6, 2, 2, 4, 4,
			3, 5, 3, 7, 3, 3, 6, 6, 2, 5, 4, 4, 4, 4, 4, 7, 2, 6, 2, 6, 6, 6,
			6, 7, 4, 2, 5, 5, 4, 4, 4, 4, 4, 6, 4, 5, 6, 6, 6, 8, 1, 6, 3, 3,
			4, 4, 4, 5, 3, 7, 3, 5, 3, 3, 6, 6, 4, 5, 5, 5, 4, 4, 3, 3, 4, 5,
			4, 7, 5, 5, 5, 8, 1, 6, 5, 5, 6, 6, 6, 7, 1, 8, 1, 5, 4, 4, 4, 6,
			3, 4, 4, 4, 3, 3, 6, 6, 3, 5, 3, 6, 5, 5, 5, 7, 2, 4, 5, 5, 3, 3,
			3, 5, 3, 4, 3, 5, 3, 3, 7, 7, 2, 5, 5, 5, 2, 2, 5, 5, 5, 4, 5, 6,
			5, 5, 6, 6, 3, 7, 2, 2, 4, 4, 4, 7, 3, 6, 3, 6, 4, 4, 4, 8, 4, 3,
			6, 6, 4, 4, 5, 5, 5, 5, 5, 7, 6, 6, 7, 8, 1, 4, 5, 5, 3, 3, 3, 6,
			4, 6, 4, 5, 4, 4, 4, 4, 2, 4, 6, 6, 2, 2, 5, 5, 2, 6, 2, 8, 5, 5,
			5, 6, 3, 4, 4, 4, 4, 4, 4, 6, 3, 6, 3, 5, 3, 3, 3, 6, 3, 4, 4, 4,
			3, 3, 6, 6, 5, 6, 5, 4, 5, 5, 7, 7, 1, 3, 5, 5, 4, 4, 4, 7, 5, 5,
			5, 4, 5, 5, 6, 6, 1, 6, 7, 7, 1, 1, 5, 5, 4, 4, 4, 6, 4, 4, 5, 5,
			3, 6, 3, 3, 3, 3, 3, 8, 3, 5, 3, 5, 5, 5, 5, 6, 3, 3, 4, 4, 3, 3,
			5, 5, 5, 7, 5, 6, 5, 5, 7, 7, 2, 5, 4, 4, 5, 5, 5, 4, 2, 6, 2, 6,
			2, 2, 5, 5, 3, 4, 4, 4, 3, 3, 4, 4, 3, 4, 3, 6, 6, 6, 6, 7, 2, 5,
			4, 4, 5, 5, 5, 6, 2, 7, 2, 6, 5, 5, 5, 5, 4, 3, 3, 3, 4, 4, 5, 5,
			4, 4, 4, 5, 6, 6, 6, 8, 3, 5, 6, 6, 2, 2, 2, 6, 4, 5, 4, 4, 4, 4,
			6, 6, 3, 4, 6, 6, 3, 3, 6, 6, 4, 3, 4, 5, 4, 4, 7, 7, 4, 6, 3, 3,
			5, 5, 5, 6, 4, 7, 4, 7, 5, 5, 5, 7, 5, 4, 5, 5, 5, 5, 6, 6, 6, 6,
			6, 6, 7, 7, 8, 9
		},
		{ // 10
			0, 5, 5, 5, 4, 4, 4, 5, 4, 6, 4, 4, 4, 4, 5, 5, 3, 4, 5, 5, 3, 3,
			4, 4, 3, 6, 3, 8, 5, 5, 5, 8, 3, 6, 3, 3, 5, 5, 5, 6, 3, 7, 3, 5,
			4, 4, 4, 7, 3, 5, 5, 5, 3, 3, 7, 7, 4, 5, 4, 6, 4, 4, 7, 7, 2, 5,
			5, 5, 3, 3, 3, 7, 5, 7, 5, 3, 5, 5, 5, 5, 2, 5, 7, 7, 2, 2, 4, 4,
			4, 5, 4, 8, 4, 4, 7, 7, 2, 6, 5, 5, 5, 5, 5, 8, 2, 6, 2, 7, 7, 7,
			7, 8, 4, 2, 5, 5, 4, 4, 5, 5, 4, 7, 4, 6, 6, 6, 6, 8, 2, 6, 4, 4,
			4, 4, 4, 6, 3, 8, 3, 6, 3, 3, 6, 6, 4, 6, 6, 6, 4, 4, 3, 3, 4, 5,
			4, 7, 5, 5, 5, 9, 2, 7, 5, 5, 6, 6, 6, 8, 2, 8, 2, 5, 4, 4, 4, 7,
			3, 5, 5, 5, 3, 3, 7, 7, 3, 6, 3, 7, 6, 6, 6, 7, 2, 5, 5, 5, 4, 4,
			4, 6, 4, 4, 4, 5, 4, 4, 7, 7, 2, 6, 6, 6, 2, 2, 6, 6, 6, 5, 6, 6,
			6, 6, 7, 7, 4, 8, 2, 2, 5, 5, 5, 7, 4, 6, 4, 7, 4, 4, 4, 9, 4, 4,
			6, 6, 4, 4, 5, 5, 6, 6, 6, 8, 6, 6, 8, 8, 1, 5, 6, 6, 3, 3, 3, 7,
			4, 6, 4, 5, 4, 4, 5, 5, 3, 4, 7, 7, 3, 3, 5, 5, 3, 6, 3, 8, 6, 6,
			6, 6, 4, 4, 5, 5, 5, 5, 5, 6, 4, 7, 4, 6, 3, 3, 3, 7, 4, 5, 5, 5,
			4, 4, 7, 7, 5, 7, 5, 4, 5, 5, 8, 8, 1, 4, 6, 6, 5, 5, 5, 8, 6, 6,
			6, 5, 6, 6, 7, 7, 1, 7, 8, 8, 1, 1, 5, 5, 4, 5, 4, 6, 4, 4, 6, 6,
			3, 6, 4, 4, 4, 4, 4, 8, 3, 5, 3, 6, 6, 6, 6, 6, 3, 3, 5, 5, 3, 3,
			6, 6, 5, 8, 5, 6, 5, 5, 7, 7, 2, 5, 4, 4, 5, 5, 5, 4, 3, 6, 3, 6,
			3, 3, 5, 5, 3, 4, 4, 4, 3, 3, 5, 5, 3, 5, 3, 7, 7, 7, 7, 8, 2, 6,
			5, 5, 5, 5, 5, 6, 2, 7, 2, 7, 5, 5, 5, 6, 5, 4, 4, 4, 5, 5, 6, 6,
			5, 4, 5, 6, 6, 6, 6, 9, 3, 6, 7, 7, 2, 2, 2, 6, 4, 5, 4, 5, 4, 4,
			7, 7, 3, 4, 6, 6, 3, 3, 6, 6, 4, 4, 4, 5, 4, 4, 8, 8, 4, 6, 3, 3,
			6, 6, 6, 7, 4, 7, 4, 8, 5, 5, 5, 8, 5, 4, 5, 5, 5, 5, 7, 7, 6, 6,
			6, 6, 7, 7, 8, 9, 1, 6, 4, 4, 5, 5, 5, 6, 3, 5, 3, 5, 3, 3, 6, 6,
			4, 3, 6, 6, 4, 4, 5, 5, 4, 5, 4, 7, 4, 4, 4, 7, 2, 5, 4, 4, 6, 6,
			6, 5, 2, 8, 2, 4, 5, 5, 5, 8, 2, 6, 6, 6, 2, 2, 8, 8, 5, 6, 5, 5,
			5, 5, 6, 6, 3, 4, 4, 4, 4, 4, 4, 6, 4, 6, 4, 4, 4, 4, 6, 6, 3, 6,
			6, 6, 3, 3, 5, 5, 3, 6, 3, 7, 3, 3, 6, 6, 3, 5, 4, 4, 4, 4, 4, 7,
			3, 7, 3, 6, 6, 6, 6, 7, 5, 3, 6, 6, 5, 5, 4, 4, 5, 6, 5, 5, 7, 7,
			7, 9, 1, 7, 3, 3, 5, 5, 5, 5, 4, 7, 4, 5, 4, 4, 7, 7, 5, 5, 5, 5,
			5, 5, 4, 4, 5, 6, 5, 8, 6, 6, 6, 8, 1, 6, 6, 6, 7, 7, 7, 7, 1, 9,
			1, 6, 5, 5, 5, 6, 4, 4, 4, 4, 4, 4, 6, 6, 4, 5, 4, 6, 5, 5, 5, 8,
			3, 4, 6, 6, 3, 3, 3, 5, 3, 5, 3, 6, 3, 3, 8, 8, 3, 5, 5, 5, 3, 3,
			5, 5, 5, 4, 5, 7, 5, 5, 6, 6, 3, 7, 3, 3, 4, 4, 4, 8, 3, 7, 3, 6,
			5, 5, 5, 8, 5, 3, 7, 7, 5, 5, 6, 6, 5, 5, 5, 7, 7, 7, 7, 9, 2, 4,
			5, 5, 4, 4, 4, 6, 5, 7, 5, 6, 5, 5, 4, 4, 2, 5, 6, 6, 2, 2, 6, 6,
			2, 7, 2, 9, 5, 5, 5, 7, 3, 5, 4, 4, 4, 4, 4, 7, 3, 6, 3, 5, 4, 4,
			4, 6, 3, 4, 4, 4, 3, 3, 6, 6, 6, 6, 6, 5, 6, 6, 7, 7, 2, 3, 5, 5,
			4, 4, 4, 7, 5, 5, 5, 4, 5, 5, 6, 6, 2, 6, 7, 7, 2, 2, 6, 6, 5, 4,
			5, 7, 5, 5, 5, 5, 4, 7, 3, 3, 3, 3, 3, 9, 4, 6, 4, 5, 5, 5, 5, 7,
			4, 4, 4, 4, 4, 4, 5, 5, 6, 7, 6, 7, 6, 6, 8, 8, 3, 6, 5, 5, 6, 6,
			6, 5, 2, 7, 2, 7, 2, 2, 6, 6, 4, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 6,
			6, 6, 6, 7, 3, 5, 4, 4, 6, 6, 6, 7, 3, 8, 3, 6, 6, 6, 6, 5, 4, 3,
			3, 3, 4, 4, 5, 5, 4, 5, 4, 5, 7, 7, 7, 8, 4, 5, 6, 6, 3, 3, 3, 7,
			5, 6, 5, 4, 5, 5, 6, 6, 4, 5, 7, 7, 4, 4, 7, 7, 5, 3, 5, 6, 5, 5,
			7, 7, 5, 7, 4, 4, 5, 5, 5, 6, 5, 8, 5, 7, 6, 6, 6, 7, 6, 5, 6, 6,
			6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 10
		}
	};

	static const vector<vector<PeUint>> d_pre = {
		{ // 2
			0, 1, 2, 8
		},
		{ // 3
			0, 2, 1, 4, 2, 2, 8, 26
		},
		{ // 4
			0, 1, 2, 2, 1, 1, 4, 13, 2, 17, 2, 20, 8, 8, 26, 80
		},
		{ // 5
			0, 2, 1, 1, 2, 2, 2, 20, 1, 26, 1, 10, 4, 4, 13, 40,
			2, 5, 17, 17, 2, 2, 20, 20, 8, 22, 8, 71, 26, 26, 80, 242
		},
		{ // 6
			0, 1, 2, 2, 1, 1, 1, 10, 2, 13, 2, 5, 2, 2, 20, 20, 1, 8, 26, 26,
			1, 1, 10, 10, 4, 11, 4, 107, 13, 13, 40, 121, 2, 44, 5, 5, 17, 17,
			17, 152, 2, 161, 2, 56, 20, 20, 20, 182, 8, 7, 22, 22, 8, 8, 71,
			71, 26, 74, 26, 76, 80, 80, 242, 728
		},
		{ // 7
			0, 2, 1, 1, 2, 2, 2, 5, 1, 20, 1, 8, 1, 1, 10, 10, 2, 4, 13, 13, 2,
			2, 5, 5, 2, 17, 2, 161, 20, 20, 20, 182, 1, 22, 8, 8, 26, 26, 26,
			76, 1, 242, 1, 28, 10, 10, 10, 91, 4, 11, 11, 11, 4, 4, 107, 107,
			13, 37, 13, 38, 40, 40, 121, 364, 2, 14, 44, 44, 5, 5, 5, 137, 17,
			47, 17, 16, 17, 17, 152, 152, 2, 53, 161, 161, 2, 2, 56, 56, 20, 19,
			20, 175, 20, 20, 182, 182, 8, 188, 7, 7, 22, 22, 22, 593, 8, 202, 8,
			206, 71, 71, 71, 638, 26, 8, 74, 74, 26, 26, 76, 76, 80, 233, 80,
			236, 242, 242, 728, 2186
		},
		{ // 8
			0, 1, 2, 2, 1, 1, 1, 8, 2, 10, 2, 4, 2, 2, 5, 5, 1, 2, 20, 20, 1,
			1, 8, 8, 1, 26, 1, 242, 10, 10, 10, 91, 2, 11, 4, 4, 13, 13, 13,
			38, 2, 121, 2, 14, 5, 5, 5, 137, 2, 17, 17, 17, 2, 2, 161, 161, 20,
			56, 20, 19, 20, 20, 182, 182, 1, 7, 22, 22, 8, 8, 8, 206, 26, 71,
			26, 8, 26, 26, 76, 76, 1, 80, 242, 242, 1, 1, 28, 28, 10, 29, 10,
			263, 10, 10, 91, 91, 4, 94, 11, 11, 11, 11, 11, 890, 4, 101, 4,
			103, 107, 107, 107, 319, 13, 4, 37, 37, 13, 13, 38, 38, 40, 350,
			40, 118, 121, 121, 364, 1093, 2, 125, 14, 14, 44, 44, 44, 43, 5,
			395, 5, 134, 5, 5, 137, 137, 17, 47, 47, 47, 17, 17, 16, 16, 17,
			49, 17, 445, 152, 152, 152, 1367, 2, 155, 53, 53, 161, 161, 161,
			479, 2, 1457, 2, 164, 56, 56, 56, 167, 20, 19, 19, 19, 20, 20, 175,
			175, 20, 59, 20, 179, 182, 182, 182, 1640, 8, 62, 188, 188, 7, 7,
			7, 190, 22, 64, 22, 65, 22, 22, 593, 593, 8, 67, 202, 202, 8, 8,
			206, 206, 71, 23, 71, 209, 71, 71, 638, 638, 26, 647, 8, 8, 74, 74,
			74, 661, 26, 668, 26, 674, 76, 76, 76, 2051, 80, 26, 233, 233, 80,
			80, 236, 236, 242, 238, 242, 719, 728, 728, 2186, 6560
		},
		{ // 9
			0, 2, 1, 1, 2, 2, 2, 4, 1, 5, 1, 2, 1, 1, 8, 8, 2, 1, 10, 10, 2, 2,
			4, 4, 2, 13, 2, 121, 5, 5, 5, 137, 1, 17, 2, 2, 20, 20, 20, 19, 1,
			182, 1, 7, 8, 8, 8, 206, 1, 26, 26, 26, 1, 1, 242, 242, 10, 28, 10,
			29, 10, 10, 91, 91, 2, 11, 11, 11, 4, 4, 4, 103, 13, 107, 13, 4, 13,
			13, 38, 38, 2, 40, 121, 121, 2, 2, 14, 14, 5, 44, 5, 395, 5, 5, 137,
			137, 2, 47, 17, 17, 17, 17, 17, 445, 2, 152, 2, 155, 161, 161, 161,
			479, 20, 2, 56, 56, 20, 20, 19, 19, 20, 175, 20, 59, 182, 182, 182,
			1640, 1, 188, 7, 7, 22, 22, 22, 65, 8, 593, 8, 67, 8, 8, 206, 206,
			26, 71, 71, 71, 26, 26, 8, 8, 26, 74, 26, 668, 76, 76, 76, 2051, 1,
			233, 80, 80, 242, 242, 242, 719, 1, 2186, 1, 82, 28, 28, 28, 251,
			10, 29, 29, 29, 10, 10, 263, 263, 10, 89, 10, 269, 91, 91, 91, 820,
			4, 31, 94, 94, 11, 11, 11, 95, 11, 32, 11, 98, 11, 11, 890, 890, 4,
			101, 101, 101, 4, 4, 103, 103, 107, 35, 107, 314, 107, 107, 319,
			319, 13, 971, 4, 4, 37, 37, 37, 992, 13, 334, 13, 337, 38, 38, 38,
			3077, 40, 13, 350, 350, 40, 40, 118, 118, 121, 119, 121, 1079, 364,
			364, 1093, 3280, 2, 41, 125, 125, 14, 14, 14, 377, 44, 380, 44, 128,
			44, 44, 43, 43, 5, 44, 395, 395, 5, 5, 134, 134, 5, 404, 5, 3644,
			137, 137, 137, 410, 17, 46, 47, 47, 47, 47, 47, 422, 17, 425, 17,
			143, 16, 16, 16, 433, 17, 49, 49, 49, 17, 17, 445, 445, 152, 449,
			152, 50, 152, 152, 1367, 1367, 2, 17, 155, 155, 53, 53, 53, 1403,
			161, 157, 161, 53, 161, 161, 479, 479, 2, 485, 1457, 1457, 2, 2,
			164, 164, 56, 55, 56, 496, 56, 56, 167, 167, 20, 506, 19, 19, 19,
			19, 19, 4616, 20, 172, 20, 173, 175, 175, 175, 524, 20, 20, 59, 59,
			20, 20, 179, 179, 182, 1619, 182, 542, 182, 182, 1640, 1640, 8, 184,
			62, 62, 188, 188, 188, 62, 7, 562, 7, 566, 7, 7, 190, 190, 22, 64,
			64, 64, 22, 22, 65, 65, 22, 65, 22, 587, 593, 593, 593, 1777, 8,
			199, 67, 67, 202, 202, 202, 604, 8, 1822, 8, 611, 206, 206, 206,
			205, 71, 23, 23, 23, 71, 71, 209, 209, 71, 70, 71, 211, 638, 638,
			638, 5741, 26, 215, 647, 647, 8, 8, 8, 650, 74, 218, 74, 73, 74, 74,
			661, 661, 26, 74, 668, 668, 26, 26, 674, 674, 76, 25, 76, 226, 76,
			76, 2051, 2051, 80, 688, 26, 26, 233, 233, 233, 695, 80, 2096, 80,
			2105, 236, 236, 236, 2119, 242, 80, 238, 238, 242, 242, 719, 719,
			728, 722, 728, 724, 2186, 2186, 6560, 19682
		},
		{ // 10
			0, 1, 2, 2, 1, 1, 1, 2, 2, 8, 2, 1, 2, 2, 4, 4, 1, 2, 5, 5, 1, 1, 2,
			2, 1, 20, 1, 182, 8, 8, 8, 206, 2, 26, 1, 1, 10, 10, 10, 29, 2, 91,
			2, 11, 4, 4, 4, 103, 2, 13, 13, 13, 2, 2, 121, 121, 5, 14, 5, 44, 5,
			5, 137, 137, 1, 17, 17, 17, 2, 2, 2, 155, 20, 161, 20, 2, 20, 20,
			19, 19, 1, 20, 182, 182, 1, 1, 7, 7, 8, 22, 8, 593, 8, 8, 206, 206,
			1, 71, 26, 26, 26, 26, 26, 668, 1, 76, 1, 233, 242, 242, 242, 719,
			10, 1, 28, 28, 10, 10, 29, 29, 10, 263, 10, 89, 91, 91, 91, 820, 2,
			94, 11, 11, 11, 11, 11, 98, 4, 890, 4, 101, 4, 4, 103, 103, 13, 107,
			107, 107, 13, 13, 4, 4, 13, 37, 13, 334, 38, 38, 38, 3077, 2, 350,
			40, 40, 121, 121, 121, 1079, 2, 1093, 2, 41, 14, 14, 14, 377, 5, 44,
			44, 44, 5, 5, 395, 395, 5, 134, 5, 404, 137, 137, 137, 410, 2, 47,
			47, 47, 17, 17, 17, 143, 17, 16, 17, 49, 17, 17, 445, 445, 2, 152,
			152, 152, 2, 2, 155, 155, 161, 53, 161, 157, 161, 161, 479, 479, 20,
			1457, 2, 2, 56, 56, 56, 496, 20, 167, 20, 506, 19, 19, 19, 4616, 20,
			20, 175, 175, 20, 20, 59, 59, 182, 179, 182, 1619, 182, 182, 1640,
			1640, 1, 62, 188, 188, 7, 7, 7, 566, 22, 190, 22, 64, 22, 22, 65,
			65, 8, 22, 593, 593, 8, 8, 67, 67, 8, 202, 8, 1822, 206, 206, 206,
			205, 26, 23, 71, 71, 71, 71, 71, 211, 26, 638, 26, 215, 8, 8, 8,
			650, 26, 74, 74, 74, 26, 26, 668, 668, 76, 674, 76, 25, 76, 76,
			2051, 2051, 1, 26, 233, 233, 80, 80, 80, 2105, 242, 236, 242, 80,
			242, 242, 719, 719, 1, 728, 2186, 2186, 1, 1, 82, 82, 28, 83, 28,
			248, 28, 28, 251, 251, 10, 253, 29, 29, 29, 29, 29, 2308, 10, 86,
			10, 260, 263, 263, 263, 262, 10, 10, 89, 89, 10, 10, 269, 269, 91,
			2429, 91, 271, 91, 91, 820, 820, 4, 92, 31, 31, 94, 94, 94, 31, 11,
			281, 11, 283, 11, 11, 95, 95, 11, 32, 32, 32, 11, 11, 98, 98, 11,
			98, 11, 881, 890, 890, 890, 2666, 4, 299, 101, 101, 101, 101, 101,
			302, 4, 911, 4, 917, 103, 103, 103, 308, 107, 35, 35, 35, 107, 107,
			314, 314, 107, 35, 107, 317, 319, 319, 319, 8612, 13, 323, 971, 971,
			4, 4, 4, 325, 37, 109, 37, 110, 37, 37, 992, 992, 13, 37, 334, 334,
			13, 13, 337, 337, 38, 38, 38, 113, 38, 38, 3077, 3077, 40, 344, 13,
			13, 350, 350, 350, 1043, 40, 1048, 40, 3158, 118, 118, 118, 3179,
			121, 40, 119, 119, 121, 121, 1079, 1079, 364, 361, 364, 362, 1093,
			1093, 3280, 9841, 2, 368, 41, 41, 125, 125, 125, 371, 14, 124, 14,
			125, 14, 14, 377, 377, 44, 14, 380, 380, 44, 44, 128, 128, 44, 128,
			44, 1154, 43, 43, 43, 1162, 5, 130, 44, 44, 395, 395, 395, 131, 5,
			3554, 5, 44, 134, 134, 134, 3590, 5, 404, 404, 404, 5, 5, 3644,
			3644, 137, 407, 137, 136, 137, 137, 410, 410, 17, 46, 46, 46, 47,
			47, 47, 416, 47, 418, 47, 47, 47, 47, 422, 422, 17, 425, 425, 425,
			17, 17, 143, 143, 16, 431, 16, 1291, 16, 16, 433, 433, 17, 145, 49,
			49, 49, 49, 49, 1316, 17, 1322, 17, 442, 445, 445, 445, 1333, 152,
			17, 449, 449, 152, 152, 50, 50, 152, 452, 152, 151, 1367, 1367,
			1367, 12302, 2, 1376, 17, 17, 155, 155, 155, 154, 53, 1390, 53, 155,
			53, 53, 1403, 1403, 161, 157, 157, 157, 161, 161, 53, 53, 161, 476,
			161, 4283, 479, 479, 479, 4306, 2, 481, 485, 485, 1457, 1457, 1457,
			1453, 2, 13121, 2, 488, 164, 164, 164, 490, 56, 55, 55, 55, 56, 56,
			496, 496, 56, 166, 56, 499, 167, 167, 167, 4511, 20, 56, 506, 506,
			19, 19, 19, 169, 19, 170, 19, 512, 19, 19, 4616, 4616, 20, 172, 172,
			172, 20, 20, 173, 173, 175, 58, 175, 1565, 175, 175, 524, 524, 20,
			1579, 20, 20, 59, 59, 59, 4769, 20, 1595, 20, 533, 179, 179, 179,
			4819, 182, 20, 1619, 1619, 182, 182, 542, 542, 182, 181, 182, 1633,
			1640, 1640, 1640, 14762, 8, 61, 184, 184, 62, 62, 62, 553, 188,
			1664, 188, 557, 188, 188, 62, 62, 7, 188, 562, 562, 7, 7, 566, 566,
			7, 1700, 7, 15308, 190, 190, 190, 1709, 22, 191, 64, 64, 64, 64, 64,
			1727, 22, 577, 22, 193, 65, 65, 65, 581, 22, 65, 65, 65, 22, 22,
			587, 587, 593, 589, 593, 197, 593, 593, 1777, 1777, 8, 22, 199, 199,
			67, 67, 67, 1795, 202, 200, 202, 67, 202, 202, 604, 604, 8, 607,
			1822, 1822, 8, 8, 611, 611, 206, 68, 206, 1838, 206, 206, 205, 205,
			71, 1853, 23, 23, 23, 23, 23, 16766, 71, 623, 71, 208, 209, 209,
			209, 1880, 71, 71, 70, 70, 71, 71, 211, 211, 638, 1903, 638, 1907,
			638, 638, 5741, 5741, 26, 641, 215, 215, 647, 647, 647, 215, 8,
			1937, 8, 1943, 8, 8, 650, 650, 74, 218, 218, 218, 74, 74, 73, 73,
			74, 73, 74, 658, 661, 661, 661, 1982, 26, 221, 74, 74, 668, 668,
			668, 2000, 26, 6014, 26, 670, 674, 674, 674, 224, 76, 25, 25, 25,
			76, 76, 226, 226, 76, 227, 76, 227, 2051, 2051, 2051, 6151, 80, 229,
			688, 688, 26, 26, 26, 2069, 233, 692, 233, 77, 233, 233, 695, 695,
			80, 233, 2096, 2096, 80, 80, 2105, 2105, 236, 26, 236, 704, 236,
			236, 2119, 2119, 242, 2126, 80, 80, 238, 238, 238, 712, 242, 6425,
			242, 2146, 719, 719, 719, 2153, 728, 242, 722, 722, 728, 728, 724,
			724, 2186, 2177, 2186, 2180, 6560, 6560, 19682, 59048
		}
	};

	static const vector<vector<PeUint>> three_pow_c_pre = {
		{ // 2
			1, 3, 3, 9
		},
		{ // 3
			1, 9, 3, 9, 3, 3, 9, 27
		},
		{ // 4
			1, 9, 9, 9, 3, 3, 9, 27, 3, 27, 3, 27, 9, 9, 27, 81
		},
		{ // 5
			1, 27, 9, 9, 9, 9, 9, 81, 3, 81, 3, 27, 9, 9, 27, 81,
			3, 9, 27, 27, 3, 3, 27, 27, 9, 27, 9, 81, 27, 27, 81, 243
		},
		{ // 6
			1, 27, 27, 27, 9, 9, 9, 81, 9, 81, 9, 27, 9, 9, 81, 81, 3, 27, 81,
			81, 3, 3, 27, 27, 9, 27, 9, 243, 27, 27, 81, 243, 3, 81, 9, 9, 27,
			27, 27, 243, 3, 243, 3, 81, 27, 27, 27, 243, 9, 9, 27, 27, 9, 9,
			81, 81, 27, 81, 27, 81, 81, 81, 243, 729
		},
		{ // 7
			1, 81, 27, 27, 27, 27, 27, 81, 9, 243, 9, 81, 9, 9, 81, 81, 9, 27,
			81, 81, 9, 9, 27, 27, 9, 81, 9, 729, 81, 81, 81, 729, 3, 81, 27,
			27, 81, 81, 81, 243, 3, 729, 3, 81, 27, 27, 27, 243, 9, 27, 27, 27,
			9, 9, 243, 243, 27, 81, 27, 81, 81, 81, 243, 729, 3, 27, 81, 81, 9,
			9, 9, 243, 27, 81, 27, 27, 27, 27, 243, 243, 3, 81, 243, 243, 3, 3,
			81, 81, 27, 27, 27, 243, 27, 27, 243, 243, 9, 243, 9, 9, 27, 27,
			27, 729, 9, 243, 9, 243, 81, 81, 81, 729, 27, 9, 81, 81, 27, 27, 81,
			81, 81, 243, 81, 243, 243, 243, 729, 2187
		},
		{ // 8
			1, 81, 81, 81, 27, 27, 27, 243, 27, 243, 27, 81, 27, 27, 81, 81, 9,
			27, 243, 243, 9, 9, 81, 81, 9, 243, 9, 2187, 81, 81, 81, 729, 9, 81,
			27, 27, 81, 81, 81, 243, 9, 729, 9, 81, 27, 27, 27, 729, 9, 81, 81,
			81, 9, 9, 729, 729, 81, 243, 81, 81, 81, 81, 729, 729, 3, 27, 81,
			81, 27, 27, 27, 729, 81, 243, 81, 27, 81, 81, 243, 243, 3, 243, 729,
			729, 3, 3, 81, 81, 27, 81, 27, 729, 27, 27, 243, 243, 9, 243, 27,
			27, 27, 27, 27, 2187, 9, 243, 9, 243, 243, 243, 243, 729, 27, 9, 81,
			81, 27, 27, 81, 81, 81, 729, 81, 243, 243, 243, 729, 2187, 3, 243,
			27, 27, 81, 81, 81, 81, 9, 729, 9, 243, 9, 9, 243, 243, 27, 81, 81,
			81, 27, 27, 27, 27, 27, 81, 27, 729, 243, 243, 243, 2187, 3, 243,
			81, 81, 243, 243, 243, 729, 3, 2187, 3, 243, 81, 81, 81, 243, 27,
			27, 27, 27, 27, 27, 243, 243, 27, 81, 27, 243, 243, 243, 243, 2187,
			9, 81, 243, 243, 9, 9, 9, 243, 27, 81, 27, 81, 27, 27, 729, 729, 9,
			81, 243, 243, 9, 9, 243, 243, 81, 27, 81, 243, 81, 81, 729, 729, 27,
			729, 9, 9, 81, 81, 81, 729, 27, 729, 27, 729, 81, 81, 81, 2187, 81,
			27, 243, 243, 81, 81, 243, 243, 243, 243, 243, 729, 729, 729, 2187,
			6561
		},
		{ // 9
			1, 243, 81, 81, 81, 81, 81, 243, 27, 243, 27, 81, 27, 27, 243, 243,
			27, 27, 243, 243, 27, 27, 81, 81, 27, 243, 27, 2187, 81, 81, 81,
			2187, 9, 243, 27, 27, 243, 243, 243, 243, 9, 2187, 9, 81, 81, 81,
			81, 2187, 9, 243, 243, 243, 9, 9, 2187, 2187, 81, 243, 81, 243, 81,
			81, 729, 729, 9, 81, 81, 81, 27, 27, 27, 729, 81, 729, 81, 27, 81,
			81, 243, 243, 9, 243, 729, 729, 9, 9, 81, 81, 27, 243, 27, 2187, 27,
			27, 729, 729, 9, 243, 81, 81, 81, 81, 81, 2187, 9, 729, 9, 729, 729,
			729, 729, 2187, 81, 9, 243, 243, 81, 81, 81, 81, 81, 729, 81, 243,
			729, 729, 729, 6561, 3, 729, 27, 27, 81, 81, 81, 243, 27, 2187, 27,
			243, 27, 27, 729, 729, 81, 243, 243, 243, 81, 81, 27, 27, 81, 243,
			81, 2187, 243, 243, 243, 6561, 3, 729, 243, 243, 729, 729, 729,
			2187, 3, 6561, 3, 243, 81, 81, 81, 729, 27, 81, 81, 81, 27, 27, 729,
			729, 27, 243, 27, 729, 243, 243, 243, 2187, 9, 81, 243, 243, 27, 27,
			27, 243, 27, 81, 27, 243, 27, 27, 2187, 2187, 9, 243, 243, 243, 9,
			9, 243, 243, 243, 81, 243, 729, 243, 243, 729, 729, 27, 2187, 9, 9,
			81, 81, 81, 2187, 27, 729, 27, 729, 81, 81, 81, 6561, 81, 27, 729,
			729, 81, 81, 243, 243, 243, 243, 243, 2187, 729, 729, 2187, 6561, 3,
			81, 243, 243, 27, 27, 27, 729, 81, 729, 81, 243, 81, 81, 81, 81, 9,
			81, 729, 729, 9, 9, 243, 243, 9, 729, 9, 6561, 243, 243, 243, 729,
			27, 81, 81, 81, 81, 81, 81, 729, 27, 729, 27, 243, 27, 27, 27, 729,
			27, 81, 81, 81, 27, 27, 729, 729, 243, 729, 243, 81, 243, 243, 2187,
			2187, 3, 27, 243, 243, 81, 81, 81, 2187, 243, 243, 243, 81, 243,
			243, 729, 729, 3, 729, 2187, 2187, 3, 3, 243, 243, 81, 81, 81, 729,
			81, 81, 243, 243, 27, 729, 27, 27, 27, 27, 27, 6561, 27, 243, 27,
			243, 243, 243, 243, 729, 27, 27, 81, 81, 27, 27, 243, 243, 243,
			2187, 243, 729, 243, 243, 2187, 2187, 9, 243, 81, 81, 243, 243,
			243, 81, 9, 729, 9, 729, 9, 9, 243, 243, 27, 81, 81, 81, 27, 27, 81,
			81, 27, 81, 27, 729, 729, 729, 729, 2187, 9, 243, 81, 81, 243, 243,
			243, 729, 9, 2187, 9, 729, 243, 243, 243, 243, 81, 27, 27, 27, 81,
			81, 243, 243, 81, 81, 81, 243, 729, 729, 729, 6561, 27, 243, 729,
			729, 9, 9, 9, 729, 81, 243, 81, 81, 81, 81, 729, 729, 27, 81, 729,
			729, 27, 27, 729, 729, 81, 27, 81, 243, 81, 81, 2187, 2187, 81, 729,
			27, 27, 243, 243, 243, 729, 81, 2187, 81, 2187, 243, 243, 243, 2187,
			243, 81, 243, 243, 243, 243, 729, 729, 729, 729, 729, 729, 2187,
			2187, 6561, 19683
		},
		{ // 10
			1, 243, 243, 243, 81, 81, 81, 243, 81, 729, 81, 81, 81, 81, 243,
			243, 27, 81, 243, 243, 27, 27, 81, 81, 27, 729, 27, 6561, 243, 243,
			243, 6561, 27, 729, 27, 27, 243, 243, 243, 729, 27, 2187, 27, 243,
			81, 81, 81, 2187, 27, 243, 243, 243, 27, 27, 2187, 2187, 81, 243,
			81, 729, 81, 81, 2187, 2187, 9, 243, 243, 243, 27, 27, 27, 2187,
			243, 2187, 243, 27, 243, 243, 243, 243, 9, 243, 2187, 2187, 9, 9,
			81, 81, 81, 243, 81, 6561, 81, 81, 2187, 2187, 9, 729, 243, 243,
			243, 243, 243, 6561, 9, 729, 9, 2187, 2187, 2187, 2187, 6561, 81, 9,
			243, 243, 81, 81, 243, 243, 81, 2187, 81, 729, 729, 729, 729, 6561,
			9, 729, 81, 81, 81, 81, 81, 729, 27, 6561, 27, 729, 27, 27, 729,
			729, 81, 729, 729, 729, 81, 81, 27, 27, 81, 243, 81, 2187, 243, 243,
			243, 19683, 9, 2187, 243, 243, 729, 729, 729, 6561, 9, 6561, 9, 243,
			81, 81, 81, 2187, 27, 243, 243, 243, 27, 27, 2187, 2187, 27, 729,
			27, 2187, 729, 729, 729, 2187, 9, 243, 243, 243, 81, 81, 81, 729,
			81, 81, 81, 243, 81, 81, 2187, 2187, 9, 729, 729, 729, 9, 9, 729,
			729, 729, 243, 729, 729, 729, 729, 2187, 2187, 81, 6561, 9, 9, 243,
			243, 243, 2187, 81, 729, 81, 2187, 81, 81, 81, 19683, 81, 81, 729,
			729, 81, 81, 243, 243, 729, 729, 729, 6561, 729, 729, 6561, 6561, 3,
			243, 729, 729, 27, 27, 27, 2187, 81, 729, 81, 243, 81, 81, 243, 243,
			27, 81, 2187, 2187, 27, 27, 243, 243, 27, 729, 27, 6561, 729, 729,
			729, 729, 81, 81, 243, 243, 243, 243, 243, 729, 81, 2187, 81, 729,
			27, 27, 27, 2187, 81, 243, 243, 243, 81, 81, 2187, 2187, 243, 2187,
			243, 81, 243, 243, 6561, 6561, 3, 81, 729, 729, 243, 243, 243, 6561,
			729, 729, 729, 243, 729, 729, 2187, 2187, 3, 2187, 6561, 6561, 3, 3,
			243, 243, 81, 243, 81, 729, 81, 81, 729, 729, 27, 729, 81, 81, 81,
			81, 81, 6561, 27, 243, 27, 729, 729, 729, 729, 729, 27, 27, 243,
			243, 27, 27, 729, 729, 243, 6561, 243, 729, 243, 243, 2187, 2187, 9,
			243, 81, 81, 243, 243, 243, 81, 27, 729, 27, 729, 27, 27, 243, 243,
			27, 81, 81, 81, 27, 27, 243, 243, 27, 243, 27, 2187, 2187, 2187,
			2187, 6561, 9, 729, 243, 243, 243, 243, 243, 729, 9, 2187, 9, 2187,
			243, 243, 243, 729, 243, 81, 81, 81, 243, 243, 729, 729, 243, 81,
			243, 729, 729, 729, 729, 19683, 27, 729, 2187, 2187, 9, 9, 9, 729,
			81, 243, 81, 243, 81, 81, 2187, 2187, 27, 81, 729, 729, 27, 27, 729,
			729, 81, 81, 81, 243, 81, 81, 6561, 6561, 81, 729, 27, 27, 729, 729,
			729, 2187, 81, 2187, 81, 6561, 243, 243, 243, 6561, 243, 81, 243,
			243, 243, 243, 2187, 2187, 729, 729, 729, 729, 2187, 2187, 6561,
			19683, 3, 729, 81, 81, 243, 243, 243, 729, 27, 243, 27, 243, 27, 27,
			729, 729, 81, 27, 729, 729, 81, 81, 243, 243, 81, 243, 81, 2187, 81,
			81, 81, 2187, 9, 243, 81, 81, 729, 729, 729, 243, 9, 6561, 9, 81,
			243, 243, 243, 6561, 9, 729, 729, 729, 9, 9, 6561, 6561, 243, 729,
			243, 243, 243, 243, 729, 729, 27, 81, 81, 81, 81, 81, 81, 729, 81,
			729, 81, 81, 81, 81, 729, 729, 27, 729, 729, 729, 27, 27, 243, 243,
			27, 729, 27, 2187, 27, 27, 729, 729, 27, 243, 81, 81, 81, 81, 81,
			2187, 27, 2187, 27, 729, 729, 729, 729, 2187, 243, 27, 729, 729,
			243, 243, 81, 81, 243, 729, 243, 243, 2187, 2187, 2187, 19683, 3,
			2187, 27, 27, 243, 243, 243, 243, 81, 2187, 81, 243, 81, 81, 2187,
			2187, 243, 243, 243, 243, 243, 243, 81, 81, 243, 729, 243, 6561,
			729, 729, 729, 6561, 3, 729, 729, 729, 2187, 2187, 2187, 2187, 3,
			19683, 3, 729, 243, 243, 243, 729, 81, 81, 81, 81, 81, 81, 729, 729,
			81, 243, 81, 729, 243, 243, 243, 6561, 27, 81, 729, 729, 27, 27, 27,
			243, 27, 243, 27, 729, 27, 27, 6561, 6561, 27, 243, 243, 243, 27,
			27, 243, 243, 243, 81, 243, 2187, 243, 243, 729, 729, 27, 2187, 27,
			27, 81, 81, 81, 6561, 27, 2187, 27, 729, 243, 243, 243, 6561, 243,
			27, 2187, 2187, 243, 243, 729, 729, 243, 243, 243, 2187, 2187, 2187,
			2187, 19683, 9, 81, 243, 243, 81, 81, 81, 729, 243, 2187, 243, 729,
			243, 243, 81, 81, 9, 243, 729, 729, 9, 9, 729, 729, 9, 2187, 9,
			19683, 243, 243, 243, 2187, 27, 243, 81, 81, 81, 81, 81, 2187, 27,
			729, 27, 243, 81, 81, 81, 729, 27, 81, 81, 81, 27, 27, 729, 729,
			729, 729, 729, 243, 729, 729, 2187, 2187, 9, 27, 243, 243, 81, 81,
			81, 2187, 243, 243, 243, 81, 243, 243, 729, 729, 9, 729, 2187, 2187,
			9, 9, 729, 729, 243, 81, 243, 2187, 243, 243, 243, 243, 81, 2187,
			27, 27, 27, 27, 27, 19683, 81, 729, 81, 243, 243, 243, 243, 2187,
			81, 81, 81, 81, 81, 81, 243, 243, 729, 2187, 729, 2187, 729, 729,
			6561, 6561, 27, 729, 243, 243, 729, 729, 729, 243, 9, 2187, 9, 2187,
			9, 9, 729, 729, 81, 243, 243, 243, 81, 81, 81, 81, 81, 81, 81, 729,
			729, 729, 729, 2187, 27, 243, 81, 81, 729, 729, 729, 2187, 27, 6561,
			27, 729, 729, 729, 729, 243, 81, 27, 27, 27, 81, 81, 243, 243, 81,
			243, 81, 243, 2187, 2187, 2187, 6561, 81, 243, 729, 729, 27, 27, 27,
			2187, 243, 729, 243, 81, 243, 243, 729, 729, 81, 243, 2187, 2187,
			81, 81, 2187, 2187, 243, 27, 243, 729, 243, 243, 2187, 2187, 243,
			2187, 81, 81, 243, 243, 243, 729, 243, 6561, 243, 2187, 729, 729,
			729, 2187, 729, 243, 729, 729, 729, 729, 729, 729, 2187, 2187, 2187,
			2187, 6561, 6561, 19683, 59049
		}
	};

	// For k steps, we need 2^k precomputations and arrays of size 2^k

	vector<PeUint> c, d, three_pow_c; // Arrays for precomputed c, d, 3^c
	PeUint power_of_two = 1 << iterations_per_step;

	// Pick from available "pre-precomputed" arrays if we have them
	if (iterations_per_step < c_pre.size() + 2) {
		c = c_pre[iterations_per_step - 2];
		d = d_pre[iterations_per_step - 2];
		three_pow_c = three_pow_c_pre[iterations_per_step - 2];
	} else {
		// For higher numbers of steps, we'll start by computing c, d and 3^c
		// directly
		c.resize(power_of_two, 0);
		d.resize(power_of_two, 0);
		three_pow_c.resize(power_of_two, 1);

		// Compute k iterations for each integer from 0 to k
		for (PeUint i = 1; i < power_of_two; ++i) {
			d[i] = i;

			// Calculate k iterations of i, track
			// odd numbers encountered in c.
			for (int ik = 0; ik < iterations_per_step; ++ik) {
				if (math::IsOdd(d[i])) {
					d[i] = (3 * d[i] + 1) / 2;
					++c[i];
					three_pow_c[i] *= 3;
				} else {
					d[i] /= 2;
				}
			}
		}
	}

	// Precompute the "end values" within k iterations of 1.
	// Note that this function uses the n -> 3n+1 mapping for odd values,
	// however the optimisation we use in Method3 uses n -> (3n+1)/2,
	// so we need to account for the possibility of up to twice the
	// number of "short" iterations (in the case that every intermediate
	// value is odd).
	unordered_map<PeUint, PeUint> end_values =
		math::CollatzGraphIterations(2 * iterations_per_step);

	// Start at 1, with a sequence length of 1
	PeUint max_length_start_number = 1;
	PeUint max_sequence_length = 1;

	// Record the lengths of the sequences for any numbers used so far
	// up to twice the start limit (a rather arbitrary choice but some
	// of the numbers reached in these sequenences can become orders of
	// magnitude larger and will rapidly use up available RAM).
	vector<PeUint> sequence_lengths(2 * start_number_limit, 0);
	sequence_lengths[1] = 1;


	// Check every integer up to the limit
	for (PeUint i = 2; i < start_number_limit; ++i) {
		PeUint current_term = i, next_term = 0,
			sequence_length = 0;

		// Store the most and least significant bits of the number
		// in a (most significant) and b (least significant).
		PeUint a = 0, b = 0;

		// The Collatz iterations
		// Using an open loop that we break inside
		for (;;) {
			// Mask with 2^k - 1 to get the k least significant bits
			b = current_term & (power_of_two - 1);

			// Extract remaining bits and shift trailing zeros off
			a = (current_term - b) >> iterations_per_step;

			// Jump k steps ahead
			next_term = three_pow_c[b] * a + d[b];

			// Check for the "end loop".
			// Note this comes before the check for cached values to avoid
			// prematurely terminating (since a 1 or 2 is recorded with a sequence
			// length of 1 and 2 respectively, but this might not be the case
			// if k iterations have lead to 1 or 2).
			if ((next_term == 1) || (next_term == 2)) {
				// If we're in the end loop we may have overshot, but we're
				// within k steps of the end so we use our precomputed map.
				sequence_length += end_values[current_term];

				break;
			}

			// If we haven't broken the loop above, update the current term
			current_term = next_term;

			// Sequence length is incremented by the number of iterations per step,
			// plus extra for any odd values encountered in the iterations we've
			// taken this step. This is because we're counting iterations using
			// 3n+1, not (3n+1)/2, the latter effectively being two steps in one.
			//
			// This happens before checking cached values below to ensure we
			// record the correct number of steps
			sequence_length += c[b] + iterations_per_step;

			// If we reach a known sequence length, we can append it
			// to the current sequence length and break. Note we need
			// to check that tmp_number is within the range of sequence
			// lengths that we've recorded.
			if (((current_term < sequence_lengths.size())) &&
				(sequence_lengths[current_term] > 0)) {
				sequence_length += sequence_lengths[current_term];
				break;
			}
		}

		// Record the sequence length found for this number
		sequence_lengths[i] = sequence_length;

		// Check if the new sequence is longer than the previous maximum
		if (sequence_length > max_sequence_length) {
			max_sequence_length = sequence_length;
			max_length_start_number = i;
		}
	}

	// Record the maximum length sequence (just for interest's sake)
	sequence = math::CollatzSequence(max_length_start_number);

	return max_length_start_number;
}


ostream &PeProblem14::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
	"The following iterative sequence is defined for the set of positive integers:" << endl <<
	"    n -> n/2 (n is even)" << endl <<
	"    n -> 3n + 1 (n is odd)" << endl << endl <<
	"Using the rule above and starting with 13, we generate the following sequence:" << endl << endl <<
	"    13 -> 40 -> 20 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1" << endl << endl <<
	"It can be seen that this sequence (starting at 13 and finishing at 1)" << endl <<
	"contains 10 terms. Although it has not been proved yet (Collatz Problem)," << endl <<
	"it is thought that all starting numbers finish at 1." << endl << endl <<
	"Which starting number, under one million, produces the longest chain?" << endl << endl <<
	"NOTE: Once the chain starts the terms are allowed to go above one million." << endl << endl;

	return os;
}


ostream &PeProblem14::DisplaySolution(ostream &os)
{
	const PeUint kStartingNumberLimit = 1000000;
	vector<PeUint> maximum_sequence_1, maximum_sequence_2,
		maximum_sequence_3;
	auto maxmimum_start_1 = Method1(kStartingNumberLimit, maximum_sequence_1);
	auto maxmimum_start_2 = Method2(kStartingNumberLimit, maximum_sequence_2);
	auto maxmimum_start_3 = Method3(kStartingNumberLimit, maximum_sequence_3);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << maxmimum_start_1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"For Method 1, we simply perform the iteration on every integer up to" << endl <<
		"the maximum search value and record the number of steps it takes to" << endl <<
		"reach one." << endl << endl <<
		"Method 1 solution: " << maxmimum_start_1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"This time we keep a record of every number we've already tested and" << endl <<
		"how many steps it required for the iteration to reach one. This way we" << endl <<
		"avoid repeated calculations of the same sub-sequences. For example, we" << endl <<
		"can calculate the sequence 8 -> 4 -> 2 -> 1. If we then want to" << endl <<
		"calculate the sequence beginning at 10, we calculate 10 -> 5 -> 16 -> 8" << endl <<
		"and then, since we already know the sequence for 8, we can simply say" << endl <<
		"that the sequence for 10 is 3 steps longer than for 8 and stop." << endl << endl <<
		"Method 2 solution: " << maxmimum_start_2 << endl << endl <<

		formatting::MethodHeader(3) << endl << endl <<
		"This final method leverages an optimisation that allows several" << endl <<
		"iterations to be computed in one step. Iterating k times on" << endl <<
		"n = (2^k)*a + b can be calculated by (3^c)*a + d." << endl << endl <<

		"Here, we split the number n into b, its k least significant bits, and" << endl <<
		"(2^k)*a, the remaining bits. For example, 27 is 11011 in binary." << endl <<
		"For k=3, we would split 11011 into 11|011, giving b = 011 and a = 11." << endl << endl <<

		"C and d are precalculated for all values up to 2^k - 1. D is the" << endl <<
		"result of iterating k times on b, and c is the number of odd values" << endl <<
		"encountered on the way. For our example, b = 011 = 3. D is therefore:" << endl <<
		"    3 -> 5 -> 8 -> 4 (3 iterations)" << endl <<
		"C is therefore 2 (2 odd numbers, 3 and 5, were encountered)." << endl << endl <<

		"We can therefore calculate that after 3 iterations, 27 will become" << endl <<
		"(3^2)*3 + 4 = 31. Checking with the original iterations:" << endl <<
		"    27 -> 41 -> 62 -> 31" << endl << endl <<

		"We also use a \"reverse\" mapping to compute the numbers within k" << endl <<
		"iterations of 1 to catch the ends of the iterations e.g. if the next" << endl <<
		"term is 8 and we're using k=5, we'll miss the end point of 1 since" << endl <<
		"that's only 4 steps away. By precomputing the values within 5 steps of" << endl <<
		"1, we instead identify 8 as being in this set and can terminate the" << endl <<
		"iterations and find the correct sequence length." << endl << endl <<

		"These optimisations should make the search even faster than method 2," << endl <<
		"however they naturally need more storage space and lead to a classic" << endl <<
		"time-space tradeoff. The time taken to precompute the arrays c, d and" << endl <<
		"3^c scales with 2^k, so there will also be a point at which the time" << endl <<
		"taken by the precomputation steps erodes any speed gain." << endl << endl <<

		"However, by computing the values of c, d and 3^c for k steps offline," << endl <<
		"we can provide a further speedup, at the tradeoff of a bit of code" << endl <<
		"bloat since we're now storing the precomputed arrays as hard coded" << endl <<
		"arrays in the code itself. This does, however permit values like k=10" << endl <<
		"to provide a much more significant speed boost compared to method 2." << endl <<
		"A rough test of method 3 with k=10 and hard-coded arrays gave a speed" << endl <<
		"increase of roughly 40% compared to method 2." << endl << endl <<
		"Method 3 solution: " << maxmimum_start_3 << endl << endl;

	return os;
}

#define ProfilingFunc profiling::TimeProfileFunction<PeUint, PeUint, vector<PeUint> &>

ostream &PeProblem14::ProfileSolutions(int n_trials, ostream &os)
{
	// Display header
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	const PeUint kStartingNumberLimit = 1000000;
	vector<PeUint> sequence_1, sequence_2, sequence_3;

	// The highest pre-precomputed number of steps to highlight the biggest
	// advantage of Method 3 over Method 2
	const PeUint kMethod3StepsPerIteration = 10;


	// Profile each method
	ProfilingFunc(1, n_trials, os, Method1, kStartingNumberLimit, sequence_1);
	ProfilingFunc(2, n_trials, os, Method2, kStartingNumberLimit, sequence_2);

	profiling::TimeProfileFunction<PeUint, PeUint, vector<PeUint> &, PeUint>(3,
		n_trials, os, Method3, kStartingNumberLimit, sequence_3,
		kMethod3StepsPerIteration);

	return os;
}

#undef ProfilingFunc

}; // namespace pe