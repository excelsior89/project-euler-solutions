// Copyright 2020-2022 Paul Robertson
//
// PeProblem18.cpp
//
// "Maximum path sum I"

#include "PeProblem18.h"


using namespace std;

namespace pe {
// Helper functions for pyramid data structure (in this case, simply an array)
static PeUint GetLeftChildIndex(PeUint parent_index)
{
	return parent_index + static_cast<PeUint>(floor((-1.0f + sqrt(static_cast<float>(1 + 8 * parent_index))) / 2.0f)) + 1;
}

static PeUint GetRightChildIndex(PeUint parent_index)
{
	return parent_index + static_cast<PeUint>(floor((-1.0f + sqrt(static_cast<float>(1 + 8 * parent_index))) / 2.0f)) + 2;
}


// Perform a recursive summation of a "path" down through a pyramid array.
// The array is supplied as a vector. The starting index must be given
// (defaults to 0, i.e. the start of the array), as well as the "path", here as an integer
// representing the series of left/right choices to take down the pyramid. For example:
//   6 = 110 in binary, which would be interpreted (from LSB to MSB) as left, right, right.
//   Given the pyramid:
//           03
//         12  10
//       07  02  08
//     15  19  20  05
//  This will give the path 03 + 12 + 02 + 20 = 37
static PeUint SumPath(const vector<PeUint> &pyramid_array, PeUint path_as_uint, PeUint ind = 0)
{
	// Odd = right, even = left
	PeUint right_child = path_as_uint & ((PeUint)1u);

	// Get the new child index
	PeUint child_ind = right_child ? GetRightChildIndex(ind) : GetLeftChildIndex(ind);

	// Recursion time!
	if (child_ind < pyramid_array.size()) {
		return pyramid_array[ind] + SumPath(pyramid_array, path_as_uint >> 1, child_ind);
	} else { // Base case: we've gone past end of the array
		return pyramid_array[ind];
	}
}

// The brute-force solution explicitly alluded to in the problem statement:
// simply test every possible path and record the maximum sum.
// The total number of possible paths is 2^(n-1), where n is the number of 
// layers in the pyramid.
static PeUint Method1(const vector<PeUint> &pyramid_array)
{
	// Calculate the number of possible paths: 2 ^ (n - 1) where n is the number of layers
	PeUint array_size = pyramid_array.size();
	
	// Depth of the array
	PeUint array_depth = static_cast<PeUint>(floor((-1.0f + sqrt(static_cast<float>(1 + 8 * array_size))) / 2.0f));

	// Limit this to 2^31 - keep within 32 bit range, but that'll be unfeasibly long anyway
	if (array_depth < 31) {
		PeUint number_of_paths = 1 << (array_depth - 1);

		// Assumption: pyramid entries are all non-negative.
		PeUint path_sum = 0, max_path_sum = 0;

		// Calculate every possible path (sloooowwwww...)
		for (PeUint i = 0; i < number_of_paths; ++i) {
			path_sum = SumPath(pyramid_array, i);

			if (path_sum > max_path_sum) {
				max_path_sum = path_sum;
			}
		}

		return max_path_sum;
	}

	return 0;
}


// An improved solution which takes advantage of the recursive nature of the problem:
static PeUint Method2(const vector<PeUint> &pyramid_array)
{
	// Copy of the array
	vector<PeUint> path_sums_array(pyramid_array);
	PeUint array_size = path_sums_array.size();

	// Depth of the array
	PeUint array_depth = static_cast<PeUint>(floor((-1.0f + sqrt(static_cast<float>(1 + 8 * array_size))) / 2.0f));

	// Compute maximum path length for sub-pyramids in the array by iteratively adding the maximum child
	// of each entry in each layer (except for the lowest layer).
	for (PeUint i = array_size - array_depth; i > 0; --i) {
		PeUint max_child = max(path_sums_array[GetLeftChildIndex(i - 1)], path_sums_array[GetRightChildIndex(i - 1)]);
		path_sums_array[i - 1] += max_child;
	}

	return path_sums_array[0];
}


ostream &PeProblem18::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"By starting at the top of the triangle below and moving to adjacent numbers on" << endl <<
		"the row below, the maximum total from top to bottom is 23." << endl << endl <<

		"      3" << endl <<
		"    7   4" << endl <<
		"  2   4   6" << endl <<
		"8   5   9   3" << endl << endl <<

		"That is, 3 + 7 + 4 + 9 = 23." << endl << endl <<

		"Find the maximum total from top to bottom of the triangle below:" << endl << endl <<

		"                                          75" << endl <<
		"                                       95    64" << endl <<
		"                                    17    47    82" << endl <<
		"                                 18    35    87    10" << endl <<
		"                              20    04    82    47    65" << endl <<
		"                           19    01    23    75    03    34" << endl <<
		"                        88    02    77    73    07    63    67" << endl <<
		"                     99    65    04    28    06    16    70    92" << endl <<
		"                  41    41    26    56    83    40    80    70    33" << endl <<
		"               41    48    72    33    47    32    37    16    94    29" << endl <<
		"            53    71    44    65    25    43    91    52    97    51    14" << endl <<
		"         70    11    33    28    77    73    17    78    39    68    17    57" << endl <<
		"      91    71    52    38    17    14    91    43    58    50    27    29    48" << endl <<
		"   63    66    04    68    89    53    67    30    73    16    69    87    40    31" << endl <<
		"04    62    98    27    23    09    70    98    73    93    38    53    60    04    23" << endl << endl <<

		"NOTE: As there are only 16384 routes, it is possible to solve this problem by" << endl <<
		"trying every route. However, Problem 67, is the same challenge with a triangle" << endl <<
		"containing one-hundred rows; it cannot be solved by brute force, and requires" << endl <<
		"a clever method!" << endl << endl;

	return os;
}

ostream &PeProblem18::DisplaySolution(ostream &os)
{
	const vector<PeUint> kArray = {
		75,
		95, 64,
		17, 47, 82,
		18, 35, 87, 10,
		20,  4, 82, 47, 65,
		19,  1, 23, 75,  3, 34,
		88,  2, 77, 73,  7, 63, 67,
		99, 65,  4, 28,  6, 16, 70, 92,
		41, 41, 26, 56, 83, 40, 80, 70, 33,
		41, 48, 72, 33, 47, 32, 37, 16, 94, 29,
		53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14,
		70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57,
		91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48,
		63, 66,  4, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31,
		 4, 62, 98, 27, 23,  9, 70, 98, 73, 93, 38, 53, 60,  4, 23
	};
		
	auto answer1 = Method1(kArray);
	auto answer2 = Method2(kArray);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << answer1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"The brute-force solution explicitly alluded to in the problem" << endl <<
		"statement: simply test every possible path and record the maximum sum." << endl <<
		"The total number of possible paths is 2^(n-1), where n is the number" << endl <<
		"of layers in the pyramid." << endl << endl <<
		"Method 1 solution: " << answer1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"This problem can be approached recursively. Consider the trivial case" << endl <<
		"of the highest sum path for a single value: the value itself. The case" << endl <<
		"for a two layer pyramid is nearly as simple; we pick the maximum child" << endl <<
		"of the top value. For example:" << endl <<
		"   8" << endl <<
		" 4   6" << endl <<
		"The maximum path sum is 8 + 6 = 14." << endl <<
		"Now consider if the 4 and 6 had layers below them. The maximum sum path" << endl <<
		"from the 8 would go through whichever value had the higher maximum sum" << endl <<
		"path. To illustrate this, we'll look at a 3 layer pyramid:" << endl <<
		"      8" << endl <<
		"    4   6" << endl <<
		"  9   1   2" << endl <<
		"The maximum path here is 8 + 4 + 9 = 21. We can work this out by" << endl <<
		"computing the maximum sum path across each layer starting from the" << endl <<
		"bottom. The trivial solution for the bottom layer is simply the values" << endl <<
		"already present. For the next layer up, we add the maximum child to" << endl <<
		"each value to get:" << endl <<
		"        08" << endl <<
		">    13    08" << endl <<
		"  09    01    02" << endl <<
		"4 + max(9,1) = 13, 6 + max(1,2) = 8" << endl <<
		"We now move to the final, top layer and add the maximum path length from" << endl <<
		"the second layer: 8 + 13 = 21." << endl <<
		"This method traverses the pyramid once so should run in O(n) time, a big" << endl <<
		"improvement over the O(2^n) solution from Method 1." << endl << endl <<
		"Method 2 solution: " << answer2 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ const vector<PeUint> &
#define PROFILE_ARGS_ vector<PeUint>({\
75,\
95, 64,\
17, 47, 82,\
18, 35, 87, 10,\
20,  4, 82, 47, 65,\
19,  1, 23, 75,  3, 34,\
88,  2, 77, 73,  7, 63, 67,\
99, 65,  4, 28,  6, 16, 70, 92,\
41, 41, 26, 56, 83, 40, 80, 70, 33,\
41, 48, 72, 33, 47, 32, 37, 16, 94, 29,\
53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14,\
70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57,\
91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48,\
63, 66,  4, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31,\
 4, 62, 98, 27, 23,  9, 70, 98, 73, 93, 38, 53, 60,  4, 23})

PROFILE_SOLUTIONS(PeProblem18, Method1, Method2)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe