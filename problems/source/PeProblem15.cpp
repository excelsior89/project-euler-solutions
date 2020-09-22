// Copyright 2020 Paul Robertson
//
// PeProblem15.cpp
//
// "Lattice paths"

#include "PeProblem15.h"

using namespace std;

namespace pe {

// This is an intentially naive method and isn't actually usable for N=20,
// but I've left it here as a curiosity, especially since it uses an interesting
// bitwise trick for finding the next k-combination given a previous one.
//
// We can find the possible paths by imagining each vertex on the grid as an
// intersection where we can choose to turn right or down. Naturally, once we
// reach the bottom or right edge of the grid, we no longer have any choice:
// we can only go right or down respectively.
//
// Notice that for an NxN grid, to get from the top left to the bottom right
// we will have to make N steps right and N steps down, for a total of 2N
// steps. The number of different ways to make this journey comes down to the
// order in which we take the right and down steps. One could take all the
// right steps first, followed by all the down steps, or alternate, or do all
// the down steps then all the right steps. For the 2x2 grid in the
// problem description, we could go RRDD, DDRR, RDRD, DRDR, DRRD, RDDR, for
// a total of 6 different paths, each containing 2 Rs and 2 Ds.
//
// If we replace the Rs and Ds with 1s and 0s, we get a set of 4 bit binary
// numbers, each of which contains 2 1s and 2 0s. We could therefore work out
// the number of possible paths by checking every 4 bit binary number and
// counting the number that contain 2 1s. Extending this means that for an NxN
// grid, we can find every number up to 2^(2N) that contains N 1s.
//
// This method is naturally very slow for larger N, as it requires us to iterate
// over a BIG range. It also will max out at a grid size of 32 (since we need
// to be able to go up to 2^(2N), so 2^(2*32) i.e. a 64 bit unsigned integer).
static PeUint Method_Silly(const PeUint grid_size)
{
	// Sanity check: if grid size is more than the bit size of
	// PeUint, we can't complete this and return 0
	if (grid_size >= 8 * sizeof(PeUint)) {
		return 0;
	}


	// The first path we choose is all rights, then all downs,
	// represented by 0...<N times>.. 1...<N times>..
	// For a 2x2 grid, this is  0011  = 2^2 - 1 = 3.
	// For a 3x3 grid, this is 000111 = 2^3 - 1 = 7.
	PeUint path_combo = (1 << grid_size) - 1;

	// The upper limit for any combination will be the opposite of
	// the above, that is, all downs then all rights, represented by
	// 1...<N times>.. 0...<N times>..
	// We can find this by left shifting the initial path N times
	PeUint upper_limit = path_combo << grid_size;



	// Record the count of numbers containing N 1s and N 0s
	PeUint number_of_paths = 1;

	// We can use a trick known as Gosper's Hack to keep finding the next
	// k-combination until we've run out of options. For more information, see
	// Knuth, D. E. (2009), "Bitwise tricks and techniques",
	// The Art of Computer Programming, 4, Fascicle 1, Addison-Wesley, p. 54,
	// ISBN 0-321-58050-8

	// Open loop, terminated by break condition
	for (;;) {
		// Extract rightmost bit 1
		PeUint u = path_combo & -path_combo;

		// Set last non-trailing bit 0, and clear to the right
		PeUint v = u + path_combo;

		// If we have overflow in v, or path_combo == 0,
		// then we've reached the end, the next k-combination cannot be represented.
		// We also need to break if we're past the upper limit i.e. the path combo
		// is greater than the maximum valid path of 1...<N bits>.. 0...<N bits>..
		if ((v==0) || (path_combo >= upper_limit)) {
			break;
		}

		// A bit of magic courtesy of Gosper...
		// v^x = 0'11^a10^b, (v^x)/u = 0'0^b1^{a+2}, and x ? x'100^b1^a
		path_combo = v + (((v ^ path_combo) / u) >> 2);


		++number_of_paths;
	}


	return number_of_paths;
}

// Given that the above "silly" method is unfeasibly slow, there's really only
// one way to go about calculating the number of possible paths: combinatorics.
// This problem is a classic combinatorial problem (and mathematically
// equivalent to another classic: "how many ways can you get N heads and N
// tails from 2N coin tosses").
//
// The total number of paths boils down to nCk (n choose k) with n = 2N and
// k = N, where N is the grid size.
//
// This method explores one way of calculting (2N)C(N). Just to keep things
// interesting we use Pascal's Triangle. If we rotate our imaginary grid by 45
// degrees and place the top left corner at the top of Pascal's triangle, we
// end up with a pattern telling us how many different paths lead up to a given
// node:
//
//         START
//           1
//         /   \
//       1       1
//     /   \   /   \
//   1       2       1
//     \   /   \   /
//       3       3
//         \   /
//           6
//          END
//
// We can use an array to represent a layer of the triangle and iteratively
// compute the values at each layer to come up with the final result.
static PeUint Method1(const PeUint grid_size)
{
	// The number of layers we need
	size_t layer_count = 2 * grid_size + 1;

	// The arrays to calculate each layer of the triangle
	vector<PeUint> triangle_layer(layer_count, 0),
		previous_triangle_layer(layer_count, 0);

	// Initialise layer 1 in both buffers otherwise when they swap we'll end up
	// with zero in the leading spot which we definitely don't want...
	previous_triangle_layer[0] = 1;
	previous_triangle_layer[1] = 1;
	triangle_layer[0] = 1;
	triangle_layer[1] = 1;

	// Iterate over each layer
	// At each layer we calculate the sum of each adjacent pair of values in
	// the previous layer in positions 1...layer-1, and then put a 1 in
	// triangle_layer[layer] i.e. at the end of the sums.
	// The 0th index is untouched by this calculate and remains 1.
	for (size_t layer = 2; layer < layer_count; ++layer) {
		for (size_t i = 1; i < layer; ++i) {
			triangle_layer[i] = previous_triangle_layer[i - 1] + previous_triangle_layer[i];
		}

		triangle_layer[layer] = 1;
		
		// Swap new layer into previous layer
		swap(triangle_layer, previous_triangle_layer);

		// Side note: we could implement the above with a single array twice
		// the size and keep two pointers as the start indices of the current
		// and previous layers. Swapping would then be a matter of resetting
		// each pointer to the appropriate index. This might be slightly more
		// efficient but would be less clear and more effect to implement.
	}

	// The number of paths (2N)C(N) is the middle element of the triangle layer
	return previous_triangle_layer[grid_size];
}


// Rather than using the iteratative summation method from above, we'll jump
// straight to using the formula for combinations ("n choose k"):
//     nCk = n! / (n-k)!k!
//
// An algorithm to calculate nCk will undoubtedly be useful for future problems,
// so I've added a general implementation to the utilities library. We simply
// call this function with n = 2k for grid size k to work out the number of
// possible paths.
//
// This still requires calculating factorials which means larger grid sizes will
// become unfeasibly large for finite precision arithmetic. The PeBigNum class
// developed for problem 13 could be used here to calculate arbitrarily large
// grids, but I'll leave that as an exercise for any curious readers.
static PeUint Method2(const PeUint grid_size)
{
	return math::NChooseK(2 * grid_size, grid_size);
}


ostream &PeProblem15::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"Starting in the top left corner of a 2x2 grid, and only being able to" << endl <<
		" move to the right and down, there are exactly 6 routes to the bottom" << endl <<
		"right corner." << endl << endl <<
		
		"    __ __         __            __    " << endl <<
		"         |          |__           |   " << endl <<
		"         |             |          |__\\" << endl <<
		"         V             V             /" << endl << endl <<
		
		"                                      " << endl <<
		"   |__ __        |__           |      " << endl <<
		"         |          |__\\       |__ __\\" << endl <<
		"         V             /             /" << endl << endl <<

		"How many such routes are there through a 20x20 grid?" << endl << endl;

	return os;
}


ostream &PeProblem15::DisplaySolution(ostream &os)
{
	const PeUint kGridSize = 20;
	auto n_possible_paths_1 = Method1(kGridSize);
	auto n_possible_paths_2 = Method2(kGridSize);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << n_possible_paths_1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"The answer to this problem boils down to combinations (nCk, n-choose-k" << endl <<
		"etc.). One way to think of this is to rotate the grid 45 degrees so we" << endl <<
		"start at the top corner and finish at the bottom. At each intersection" << endl <<
		"on the grid below the start, we can count the number of ways we can" << endl <<
		"get there by summing the number of ways to get to each intersection" << endl <<
		"that comes directly before. A definitely cool ASCII art illustration:" << endl << endl <<
		
		"       START" << endl <<
		"         1" << endl <<
		"       /   \\" << endl <<
		"     1       1      <-- There's only 1 way to get to each of these" << endl <<
		"   /   \\   /   \\" << endl <<
		" 1       2       1  <-- 1 way to get to each corner, 2 ways to get to" << endl <<
		"   \\   /   \\   /        the centre" << endl <<
		"     3       3      <-- 3 ways (1+2) to get to each of these" << endl <<
		"       \\   /" << endl <<
		"         6          <-- ...and therefore 6 ways to get to the end" << endl <<
		"        END" << endl << endl <<

		"You might notice that this forms Pascal's triangle (albeit missing" << endl <<
		"some of the outer bits). It also forms a nice iterative way to" << endl <<
		"calculate the total number of possible routes, which turns out to be" << endl <<
		"(2n)C(n), where n is the grid size." << endl << endl <<
		"Method 1 solution: " << n_possible_paths_1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"Just like in method 1, we're faced with calculating (2n)C(n) to work" << endl <<
		"out this problem. Rather than using iterative summation, this method" << endl <<
		"uses a slightly more direct calculation of the required combination." << endl <<
		"Starting with the formula nCk = n! / (n-k)!k!, we make use of the" << endl <<
		"symmetry nCk = nC(n-k) and pick the larger of k, n-k. This will cancel" << endl <<
		"with part of the numerator n! to leave:" << endl <<
		"    [(n)*(n-1)...(n-k+1)] / [(k)*(k-1)...1]" << endl << endl <<
		"We can now calculate this directly, however to reduce the chance of" << endl <<
		"integer overflow we also divide each pair of terms from the top and" << endl <<
		"bottom by their GCD first (e.g. n /= GCD(n,k), k /= GCD(n,k) etc.)" << endl << endl <<
		"Method 2 solution: " << n_possible_paths_2 << endl << endl;

	return os;
}

//#define ProfilingFunc profiling::TimeProfileFunction<PeUint, PeUint>
//
//ostream &PeProblem15::ProfileSolutions(int n_trials, ostream &os)
//{
//	// Display header
//	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;
//
//	const PeUint kGridSize = 50;
//
//	// Profile each method
//	ProfilingFunc(1, n_trials, os, Method1, kGridSize);
//	ProfilingFunc(2, n_trials, os, Method2, kGridSize);
//
//	return os;
//}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 50

PROFILE_SOLUTIONS(PeProblem15, Method1, Method2)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe