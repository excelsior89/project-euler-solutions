#include "PeProblem11.h"

using namespace std;

namespace pe {

// The search grid, stored in row-major form
const unsigned kNumberGrid[] = {
	 8,  2, 22, 97, 38, 15,  0, 40,  0, 75,  4,  5,  7, 78, 52, 12, 50, 77, 91,  8,
	49, 49, 99, 40, 17, 81, 18, 57, 60, 87, 17, 40, 98, 43, 69, 48,  4, 56, 62,  0,
	81, 49, 31, 73, 55, 79, 14, 29, 93, 71, 40, 67, 53, 88, 30,  3, 49, 13, 36, 65,
	52, 70, 95, 23,  4, 60, 11, 42, 69, 24, 68, 56,  1, 32, 56, 71, 37,  2, 36, 91,
	22, 31, 16, 71, 51, 67, 63, 89, 41, 92, 36, 54, 22, 40, 40, 28, 66, 33, 13, 80,
	24, 47, 32, 60, 99,  3, 45,  2, 44, 75, 33, 53, 78, 36, 84, 20, 35, 17, 12, 50,
	32, 98, 81, 28, 64, 23, 67, 10, 26, 38, 40, 67, 59, 54, 70, 66, 18, 38, 64, 70,
	67, 26, 20, 68,  2, 62, 12, 20, 95, 63, 94, 39, 63,  8, 40, 91, 66, 49, 94, 21,
	24, 55, 58,  5, 66, 73, 99, 26, 97, 17, 78, 78, 96, 83, 14, 88, 34, 89, 63, 72,
	21, 36, 23,  9, 75,  0, 76, 44, 20, 45, 35, 14,  0, 61, 33, 97, 34, 31, 33, 95,
	78, 17, 53, 28, 22, 75, 31, 67, 15, 94,  3, 80,  4, 62, 16, 14,  9, 53, 56, 92,
	16, 39,  5, 42, 96, 35, 31, 47, 55, 58, 88, 24,  0, 17, 54, 24, 36, 29, 85, 57,
	86, 56,  0, 48, 35, 71, 89,  7,  5, 44, 44, 37, 44, 60, 21, 58, 51, 54, 17, 58,
	19, 80, 81, 68,  5, 94, 47, 69, 28, 73, 92, 13, 86, 52, 17, 77,  4, 89, 55, 40,
	 4, 52,  8, 83, 97, 35, 99, 16,  7, 97, 57, 32, 16, 26, 26, 79, 33, 27, 98, 66,
	88, 36, 68, 87, 57, 62, 20, 72,  3, 46, 33, 67, 46, 55, 12, 32, 63, 93, 53, 69,
	 4, 42, 16, 73, 38, 25, 39, 11, 24, 94, 72, 18,  8, 46, 29, 32, 40, 62, 76, 36,
	20, 69, 36, 41, 72, 30, 23, 88, 34, 62, 99, 69, 82, 67, 59, 85, 74,  4, 36, 16,
	20, 73, 35, 29, 78, 31, 90,  1, 74, 31, 49, 71, 48, 86, 81, 16, 23, 57,  5, 54,
	 1, 70, 54, 71, 83, 51, 54, 69, 16, 92, 33, 48, 61, 43, 52,  1, 89, 19, 67, 48
};

const unsigned kGridWidth = (unsigned)sqrt(sizeof(kNumberGrid) / sizeof(unsigned));
const unsigned kGridHeight = kGridWidth;
const unsigned kSetSize = 4;

// Brute force solution
// Test all possible adjacent sets of digits vertically, horizontally and diagonally
// and record the maximum
static unsigned Method1(const unsigned number_grid[], const unsigned height, 
	const unsigned width, const unsigned set_size)
{
	unsigned max_product = 0, ibase = 0;
	// 4 temporary values for the row, column and two diagonal values to check
	vector<unsigned> tmp_products({1,1,1,1});


	for (unsigned irow = 0; irow < height - set_size; ++irow) {
		for (unsigned icol = 0; icol < width - set_size; ++icol) {
			// This is the reference cell for the vertical, horizontal and
			// diagonal calculations
			ibase = icol + width * irow;

			// Reset temporary products
			tmp_products[0] = 1;
			tmp_products[1] = 1;
			tmp_products[2] = 1;
			tmp_products[3] = 1;

			for (unsigned iset = 0; iset < set_size; ++iset) {
				// Row forward
				tmp_products[0] *= number_grid[ibase + iset];

				// Column down
				tmp_products[1] *= number_grid[ibase + iset * width];

				// Top left-bottom right diagonal (trace)
				tmp_products[2] *= number_grid[ibase + iset * (width + 1)];

				// Top right-bottom left diagonal (skew)
				tmp_products[3] *= number_grid[ibase + (set_size - 1) + iset * (width - 1)];
			}

			const auto &tmp_max = max_element(tmp_products.begin(), tmp_products.end());

			// Update maximum product
			max_product = max_product > *tmp_max ? max_product : *tmp_max;			
		}
	}

	// "Edge case" 1: Columns at the right side (no room for rows or diagonals)
	for (unsigned irow = 0; irow < height - set_size; ++irow) {
		for (unsigned icol = width - set_size; icol < width; ++icol) {
			// This is the reference cell for the vertical, horizontal and
			// diagonal calculations
			ibase = icol + width * irow;

			// Reset temporary products
			tmp_products[0] = 1;

			for (unsigned iset = 0; iset < set_size; ++iset) {
				// Column down
				tmp_products[0] *= number_grid[ibase + width * iset];
			}

			// Update maximum product
			max_product = max_product > tmp_products[0] ? max_product : tmp_products[0];			
		}
	}

	// "Edge case" 2: Rows at the bottom (no room for columns or diagonals)
	for (unsigned icol = 0; icol < width - set_size; ++icol) {
		for (unsigned irow = height - set_size; irow < height; ++irow) {
			// This is the reference cell for the vertical, horizontal and
			// diagonal calculations
			ibase = icol + width * irow;

			// Reset temporary products
			tmp_products[0] = 1;

			for (unsigned iset = 0; iset < set_size; ++iset) {
				// Row forward
				tmp_products[0] *= number_grid[ibase + iset];
			}

			// Update maximum product
			max_product = max_product > tmp_products[0] ? max_product : tmp_products[0];			
		}
	}

	return max_product;
}

// Still a rather brute force method, but just to be different, in this method
// we search the row, column and diagonal spaces separately and attempt to skip
// over any zeroes found
static unsigned Method2(const unsigned number_grid[], const unsigned height, 
	const unsigned width, const unsigned set_size)
{
	unsigned max_product = 0, tmp_product1 = 1, tmp_product2 = 1;

	// Column products
	for (unsigned icol = 0; icol < width; ++icol) {
		for (unsigned irow = 0; irow < height - set_size; ++irow) {
			// Reset temporary product
			tmp_product1 = 1;

			for (unsigned iset = 0; iset < set_size; ++iset) {
				// Column down
				tmp_product1 *= number_grid[width * irow + icol + iset * width];

				// Zero check - no point in continuing to calculate a product
				// if we've found a zero, skip forward over the rows to move
				// irow to the row containing the zero and break this loop.
				// Remember, irow will still be incremented by the row loop
				// so by pointing irow at the zero, we'll move past it for the
				// next iteration.
				//
				// We don't need to check here for irow going past the boundary
				// as the for loop is already checking that and all calculations
				// using irow occur before this statement.
				if (tmp_product1 == 0) {
					irow += iset;
					break;
				}
			}

			// Update maximum product
			max_product = max_product > tmp_product1 ? max_product : tmp_product1;			
		}
	}


	// Row products
	// N.B. swapping outer and inner loops compared to column search above
	for (unsigned irow = 0; irow < height; ++irow) {
		for (unsigned icol = 0; icol < width - set_size; ++icol) {
			// Reset temporary product
			tmp_product1 = 1;

			for (unsigned iset = 0; iset < set_size; ++iset) {
				// Column down
				tmp_product1 *= number_grid[width * irow + icol + iset];

				// Zero check - same idea as for columns, but we advance the
				// column iterator instead of the row iterator.
				//
				// Again, we can get away with not checking icol is still
				// in range since the for loop checks that already and nothing
				// references the array after this statement executes.
				if (tmp_product1 == 0) {
					icol += iset;
					break;
				}
			}

			// Update maximum product
			max_product = max_product > tmp_product1 ? max_product : tmp_product1;			
		}
	}

	// Diagonal products
	for (unsigned icol = 0; icol < width - set_size; ++icol) {
		for (unsigned irow = 0; irow < height - set_size; ++irow) {
			// Reset temporary products
			tmp_product1 = 1;
			tmp_product2 = 1;

			for (unsigned iset = 0; iset < set_size; ++iset) {
				// Top left bottom right
				tmp_product1 *= number_grid[width * irow + icol + iset * (width + 1)];
				// Top right bottom left
				tmp_product2 *= number_grid[width * irow + icol + (set_size - 1) + iset * (width - 1)];

				// No zero check here: skipping for diagonals is more complicated and
				// we're doing both diagonals in one loop so we don't attempt to skip
				// over any rows or columns. We can still stop calulcating if _both_
				// temporary products are zero though.
				if ((tmp_product1 == 0) && (tmp_product2 == 0)) {
					break;
				}
			}

			// Update maximum product
			max_product = max_product > tmp_product1 ? max_product : tmp_product1;
			max_product = max_product > tmp_product2 ? max_product : tmp_product2;	
		}
	}

	return max_product;
}


PeProblem11::PeProblem11()
{
}


ostream &PeProblem11::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
	"In the 20x20 grid below, four numbers along a diagonal line have been highlighted." << endl << endl <<

	"08  02  22  97  38  15  00  40  00  75  04  05  07  78  52  12  50  77  91  08" << endl <<
	"49  49  99  40  17  81  18  57  60  87  17  40  98  43  69  48  04  56  62  00" << endl <<
	"81  49  31  73  55  79  14  29  93  71  40  67  53  88  30  03  49  13  36  65" << endl <<
	"52  70  95  23  04  60  11  42  69  24  68  56  01  32  56  71  37  02  36  91" << endl <<
	"22  31  16  71  51  67  63  89  41  92  36  54  22  40  40  28  66  33  13  80" << endl <<
	"24  47  32  60  99  03  45  02  44  75  33  53  78  36  84  20  35  17  12  50" << endl <<
	"32  98  81  28  64  23  67  10 _26_ 38  40  67  59  54  70  66  18  38  64  70" << endl <<
	"67  26  20  68  02  62  12  20  95 _63_ 94  39  63  08  40  91  66  49  94  21" << endl <<
	"24  55  58  05  66  73  99  26  97  17 _78_ 78  96  83  14  88  34  89  63  72" << endl <<
	"21  36  23  09  75  00  76  44  20  45  35 _14_ 00  61  33  97  34  31  33  95" << endl <<
	"78  17  53  28  22  75  31  67  15  94  03  80  04  62  16  14  09  53  56  92" << endl <<
	"16  39  05  42  96  35  31  47  55  58  88  24  00  17  54  24  36  29  85  57" << endl <<
	"86  56  00  48  35  71  89  07  05  44  44  37  44  60  21  58  51  54  17  58" << endl <<
	"19  80  81  68  05  94  47  69  28  73  92  13  86  52  17  77  04  89  55  40" << endl <<
	"04  52  08  83  97  35  99  16  07  97  57  32  16  26  26  79  33  27  98  66" << endl <<
	"88  36  68  87  57  62  20  72  03  46  33  67  46  55  12  32  63  93  53  69" << endl <<
	"04  42  16  73  38  25  39  11  24  94  72  18  08  46  29  32  40  62  76  36" << endl <<
	"20  69  36  41  72  30  23  88  34  62  99  69  82  67  59  85  74  04  36  16" << endl <<
	"20  73  35  29  78  31  90  01  74  31  49  71  48  86  81  16  23  57  05  54" << endl <<
	"01  70  54  71  83  51  54  69  16  92  33  48  61  43  52  01  89  19  67  48" << endl << endl <<
	
	"The product of these numbers is 26 * 63 * 78 * 14 = 1788696." << endl << endl <<
	
	"What is the greatest product of four adjacent numbers in the same" << endl <<
	"direction (up, down, left, right, or diagonally) in the 20x20 grid?" << endl << endl;

	return os;
}


ostream &PeProblem11::DisplaySolution(ostream &os)
{
	auto max_prod1 = Method1(kNumberGrid, kGridHeight, kGridWidth, kSetSize);
	auto max_prod2 = Method2(kNumberGrid, kGridHeight, kGridWidth, kSetSize);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << max_prod1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"Brute force: we search every possible row, column and diagonal product" << endl <<
		"in the grid and record the maximum. Special care has to be taken to" << endl <<
		"include the rows at the bottom and columns at the right of the grid" << endl <<
		"which are too close to a boundary for a diagonal. These are checked" << endl <<
		"in two separate loops following the main loop." << endl << endl <<
		"Method 1 solution: " << max_prod1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"A small improvement to the brute force method:" << endl <<
		"We can skip forward on rows or columns if we find a zero at any point." << endl <<
		"We check that the running product we're calculating is nonzero as we" << endl <<
		"multiply in each new value. If the product becomes zero, we know that" << endl <<
		"the last value we used must be zero. We can therefore skip forward so" << endl <<
		"that we resume our search past this window. While it would be possible," << endl <<
		"it's more complex to do this for diagonals so we don't bother here." << endl << endl <<
		"Method 2 solution: " << max_prod2 << endl << endl;

	return os;
}

ostream &PeProblem11::ProfileSolutions(int n_trials, ostream &os)
{
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	const unsigned kGrid = 1000000;
	unsigned max_prod1 = 0, max_prod2 = 0;

	clock_t start_time(clock());
	for (int i = 0; i < n_trials; ++i) {
		max_prod1 = Method1(kNumberGrid, kGridHeight, kGridWidth, kSetSize);
	}
	clock_t method_1_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		max_prod2 = Method2(kNumberGrid, kGridHeight, kGridWidth, kSetSize);
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