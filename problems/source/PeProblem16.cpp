// Copyright 2020-2021 Paul Robertson
//
// PeProblem16.cpp
//
// "Power digit sum"

#include "PeProblem16.h"

#include "PeBigInt.h"

using namespace std;

namespace pe {

// This method is basically explicitly stating the multiplication algorithm
// used in the PeBigInt class and using it to iteratively multiply the base
// number <power> times, then computing the digit sum.
static PeUint Method1(PeUint base, PeUint power)
{
	PeUint val = base;

	// Choosing 10000 as the number system base here. Larger values would
	// also be fine as long as kBase^2 < UINT64_MAX.
	const PeUint kBase = 10000;

	// Array to store the digits of the product
	vector<PeUint> digit_array;

	// First construct the base value as a digit array
	while (val >= kBase) {
		digit_array.push_back(val % kBase);
		val /= kBase;
	}
	digit_array.push_back(val);

	// Keep a copy of the base value
	vector<PeUint> base_value(digit_array);

	// Simple exponentiation algorithm:
	// Keep multiplying the current value by the base value <power> times
	for (PeUint i_power = 0; i_power < power - 1; ++i_power) {
		// Result digits
		vector<PeUint> res(digit_array.size() + base_value.size(), 0);

		PeUint carry = 0;
		// Outer loop over this numbers digits
		for (size_t i = 0; i < digit_array.size(); ++i) {
			// Inner loop over rhs digits with extra pass if carry occurs
			for (size_t j = 0; (j < base_value.size()) || carry; ++j) {
				PeInt cur = res[i + j] + digit_array[i] *
					(j < base_value.size() ? base_value[j] : 0) + carry;

				res[i + j] = cur % kBase;
				carry = cur / kBase;
			}
		}

		// Clear any leading zeros
		while ((res.size() > 1) && (res.back() == 0)) {
			res.pop_back();
		}

		// Move the result to the temporary array
		digit_array = move(res);
	}

	// Compute the digit sum
	PeUint digit_sum = 0;

	for (const auto &i_digit : digit_array) {
		digit_sum += math::SumDigits(i_digit);
	}


	return digit_sum;
}

// I wasn't able to find a more elegant approach than using bignum arithmetic
// to compute the power and taking the digit sum. This method uses the PeBigInt
// class, extended with power() and sumDigits() methods. The power() method
// used here should be somewhat more efficient than the naive method above. It
// involves repeatedly squaring a value and dividing the initial exponent by 2
// until the exponent reaches 1. Any odd exponents encountered on the way are
// recorded by accumulating multiplications of the present value. The
// accumulated values are multiplied with the result of the repeated squaring
// to obtain the final result.
//
// An example:
//   3^9
//   b = 3, p = 9, tmp = 1
//  
//   Since 9 is odd, tmp *= b ---> tmp = 3
//   
//   b *= b,  p /= 2
//   b = 9,   p = 4
//
//   Since 4 is even, no accumulation is needed in tmp
//
//   b *= b,  p /= 2
//   b = 81,  p = 2
//
//   Since 2 is even, no accumulation is needed in tmp
//
//   b *= b,  p /= 2
//   b = 6561,  p = 1
//
//   Since the exponent is now 1, we terminate and multiply by tmp
//
//   b *= tmp
//   b = 19683
static PeUint Method2(PeUint base, PeUint power)
{
	// Set up PeBigInt with the base
	PeBigInt big_power(base);

	// Raise to power
	big_power.power(power);
	
	// Collect digit sum and cast to PeUint (not checking for overflow here)
	return PeUint(big_power.sumDigits());
}


ostream &PeProblem16::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
	"2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26." << endl <<
	"What is the sum of the digits of the number 2^1000?" << endl << endl;

	return os;
}


ostream &PeProblem16::DisplaySolution(ostream &os)
{
	const PeUint kBase = 2, kPower = 1000;
	auto answer1 = Method1(kBase, kPower);
	auto answer2 = Method2(kBase, kPower);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << answer1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"This problem requires the use of arbitrary precision arithmetic to" << endl <<
		"compute the power befor summing its digits. The multiplication" << endl <<
		"algorithm is effectively coding up the \"by hand\" algorithm commonly" << endl <<
		"taught it schools; digits are multiplied by column and their last" << endl <<
		"digit is recorded in the same column of the result. Any value from the" << endl <<
		"next column up is carried over and added to the result of this next" << endl <<
		"column's multiplication. The result of this multiplication then has" << endl <<
		"its digits summed by computing the digit sum of each entry in the" << endl <<
		"digits array then summing these digit sums." << endl << endl <<
		"Method 1 solution: " << answer1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"This method is essentially the same as method 1, but we extend the" << endl <<
		"PeBigInt class with power() and sumDigits() methods. The power()" << endl <<
		"method evaluates a power by squaring the number and halving the" << endl <<
		"exponent until the exponent is 1. Any odd exponents encountered on the" << endl <<
		"way are accumulated with an extra multiplication of the present value." << endl <<
		"These accumulated values are multiplied with the result of the" << endl <<
		"repeated squaring to obtain the final result. The sumDigits() method" << endl <<
		"is straightforward as it simply involves computing the digit sum for" << endl <<
		"each entry of the digits array, then summing the result." << endl << endl <<
		"Method 2 solution: " << answer2 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint, PeUint
#define PROFILE_ARGS_ 2, 1000

PROFILE_SOLUTIONS(PeProblem16, Method1, Method2)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe