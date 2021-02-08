// Copyright 2020-2021 Paul Robertson
//
// PeBigInt.cpp
//
// A custom arbitrary precision signed integer arithmetic class

#include "PeBigInt.h"

using namespace std;

namespace pe {

// Static constant regex for value string matching
// Using optimize flag to give matching slightly more speed
const regex PeBigInt::kValueStringRe = regex("([-+])?([0-9]+)([eE][0-9]+)?", regex::optimize);

PeBigInt::PeBigInt() :
	sign_(1)
{
}
	
// Copy and move constructors
PeBigInt::PeBigInt(const PeBigInt &that) :
	sign_(that.sign_),
	digits_(that.digits_)
{
}
	
PeBigInt::PeBigInt(PeBigInt &&that) noexcept :
	sign_(that.sign_),
	digits_(move(that.digits_))
{
}
	
// Constructors from integer types
PeBigInt::PeBigInt(int val) :
	sign_(1)
{
	fromSigned(val);
}
	
PeBigInt::PeBigInt(long int val) :
	sign_(1)
{
	fromSigned(val);
}

PeBigInt::PeBigInt(PeInt val) :
	sign_(1)
{
	fromSigned(val);
}

PeBigInt::PeBigInt(unsigned val) :
	sign_(1)
{
	fromUnsigned(val);
}

PeBigInt::PeBigInt(long unsigned val) :
	sign_(1)
{
	fromUnsigned(val);
}
	
PeBigInt::PeBigInt(PeUint val) :
	sign_(1)
{
	fromUnsigned(val);
}
	
// Constructors from floating point types
PeBigInt::PeBigInt(float val) :
	sign_(1)
{
	fromFloating(val);
}

PeBigInt::PeBigInt(double val) :
	sign_(1)
{
	fromFloating(val);
}

PeBigInt::PeBigInt(long double val) :
	sign_(1)
{
	fromFloating(val);
}
	
// String constructors
// These take a numeric string and create an
// appropriate integer representation
PeBigInt::PeBigInt(const char *valstr) :
	sign_(1)
{
	fromString(valstr);
}

PeBigInt::PeBigInt(const std::string &valstr) :
	sign_(1)
{
	fromString(valstr);
}
	
PeBigInt::~PeBigInt()
{
}
	
// Copy and move assignment operator
PeBigInt &PeBigInt::operator=(const PeBigInt &rhs)
{
	// Check for self assignment
	if (this == &rhs) {
		return *this;
	}
	
	sign_ = rhs.sign_;
	digits_ = rhs.digits_;

	return *this;
}

PeBigInt &PeBigInt::operator=(PeBigInt &&rhs) noexcept
{
	// Check for self assignment
	if (this == &rhs) {
		return *this;
	}

	sign_ = rhs.sign_;
	digits_ = move(rhs.digits_);

	return *this;
}

// Unary operators
PeBigInt PeBigInt::operator-() const
{
	PeBigInt negative_this(*this);
	negative_this.sign_ = -sign_;

	return negative_this;
}

// Relational operators
bool PeBigInt::operator<(const PeBigInt &rhs) const
{
	// Sign checks: all negative numbers are greater than positive numbers
	if (sign_ < rhs.sign_) {
		return true;
	} else if (sign_ > rhs.sign_) {
		return false;
	}

	// Signs are equal if we get here, do absolute value comparison
	bool abs_val_lt = absLt(rhs);

	// If we have two negative numbers, we need to invert the comparison outcome
	if (sign_ < 0) {
		return !abs_val_lt;
	} else {
		return abs_val_lt;
	}
}

bool PeBigInt::operator>(const PeBigInt &rhs) const
{
	return rhs < *this;
}

bool PeBigInt::operator<=(const PeBigInt &rhs) const
{
	return !(*this > rhs);
}

bool PeBigInt::operator>=(const PeBigInt &rhs) const
{
	return !(*this < rhs);
}

bool PeBigInt::operator==(const PeBigInt &rhs) const
{
	// Check for same sign
	if (sign_ != rhs.sign_) {
		// If the signs differ, have a special check for possible zeros
			
		// Even if the signs somehow differ, treat zeros as equal
		if (isZero() && rhs.isZero()) {
			return true;
		} else { // Otherwise, values are not equal because signs differ
			return false;
		}
	} else { // Same signs, do absolute equality check
		return absEq(rhs);
	}
}

bool PeBigInt::operator!=(const PeBigInt &rhs) const
{
	return !(*this == rhs);
}

// Arithmetic operators

PeBigInt &PeBigInt::operator+=(const PeBigInt &rhs)
{
	// Use signs to determine which operations are needed
	if (sign_ == rhs.sign_) {
		// Same sign, simple addition is fine
		absPlusEq(rhs);
	} else {
		// Different signs, this becomes like subtraction.
		// We do the equivalent of abs(abs(greater) - abs(smaller))
		// then use the sign of greater to set the sign of the answer.
		if (absLt(rhs)) {
			// If the rhs absolute value is larger,
			// we make a temporary copy of rhs to
			// do absMinusEq on and move the resulting
			// digits to this.
			PeBigInt rhs_copy(rhs);
			rhs_copy.absMinusEq(*this);
			digits_ = move(rhs_copy.digits_);
			sign_ = rhs_copy.sign_;
		} else {
			// If this absolute value is larger,
			// we can do straightforward subtraction
			absMinusEq(rhs);
		}
	}

	return *this;
}

PeBigInt &PeBigInt::operator-=(const PeBigInt &rhs)
{
	// Turn subtraction into addition
	operator+=(-rhs);

	return *this;
}

PeBigInt &PeBigInt::operator*=(const PeBigInt &rhs)
{
	// Do multiplication of absolute values
	absMultEq(rhs);

	// Multiply signs
	sign_ *= rhs.sign_;

	return *this;
}

PeBigInt &PeBigInt::operator/=(const PeBigInt &rhs)
{
	// Do division of absolute values
	absDivEq(rhs);

	// Multiply signs
	sign_ *= rhs.sign_;

	return *this;
}
	
// Conversion operators
// Values that are too large for the data type
// will be converted to the maximum value for
// said data type
	
// Conversion to integer types
PeBigInt::operator PeInt() const
{
	// Work out longest decimal size supported, subtracting one
	// from the power of 2 since we only support the positive "half" of PeInt range
	size_t digitsize =
		(size_t)floor((double)(8 * sizeof(PeInt) - 1) * log10(2.0));

	// Too large, return maximum possible value
	if (digits_.size() * kBasePower > digitsize) {
		return sign_ > 0 ? INTMAX_MAX : INTMAX_MIN; // Standard requires at least 64 bit
	}

	// Definitely small enough, we can do a direct conversion
	if (digits_.size() * kBasePower < digitsize) {
		PeInt sum = 0, base_mul = 1;

		for (const auto &ai : digits_) {
			sum += base_mul * (PeInt)ai;

			// This can overflow on the last loop but
			// won't be used after that so don't worry
			base_mul *= (PeInt)kBase;
		}

		// Update the sign
		if (sign_ < 0) {
			sum = -sum;
		}

		return sum;
	}

	// digits_.size() * kBasePower == bitsize, we might be ok but we need to
	// do a more exact check. Putting this last to avoid the extra cost of
	// checking against PeBigInt conversions of INTMAX_MAX and INTMAX_MIN if
	// we don't have to.

	if (*this > PeBigInt(INTMAX_MAX)) {
		return INTMAX_MAX;
	} else if (*this < PeBigInt(INTMAX_MIN)) {
		return INTMAX_MIN;
	} else {
		PeInt sum = 0, base_mul = 1;

		for (const auto &ai : digits_) {
			sum += base_mul * (PeInt)ai;

			// This can overflow on the last loop but
			// won't be used after that so don't worry
			base_mul *= (PeInt)kBase;
		}

		// Update the sign
		if (sign_ < 0) {
			sum = -sum;
		}

		return sum;
	}
}

PeBigInt::operator PeUint() const
{
	// Quick check: return any negative numbers as 0
	if (sign_ < 0) {
		return 0ull;
	}

	// Work out longest decimal size supported
	size_t digitsize =
		(size_t)floor((double)(8 * sizeof(PeUint)) * log10(2.0));

	// Too large, return maximum possible value
	if (digits_.size() * kBasePower > digitsize) {
		return UINTMAX_MAX; // Standard requires at least 64 bit
	}

	// Definitely small enough, we can do a direct conversion
	if (digits_.size() * kBasePower < digitsize) {
		PeUint sum = 0, base_mul = 1;

		for (const auto &ai : digits_) {
			sum += base_mul * (PeUint)ai;

			// This can overflow on the last loop but
			// won't be used after that so don't worry
			base_mul *= (PeUint)kBase;
		}

		return sum;
	}

	// digits_.size() * kBasePower == bitsize, we might be ok but we need to
	// do a more exact check. Putting this last to avoid the extra cost of
	// checking against PeBigInt conversions of UINTMAX_MAX we don't have to.

	if (*this > PeBigInt(UINTMAX_MAX)) {
		return UINTMAX_MAX;
	} else {
		PeUint sum = 0, base_mul = 1;

		for (const auto &ai : digits_) {
			sum += base_mul * (PeUint)ai;

			// This can overflow on the last loop but
			// won't be used after that so don't worry
			base_mul *= (PeUint)kBase;
		}

		return sum;
	}
}
	
// Conversion to floating point
PeBigInt::operator long double() const
{
	// Quick check for size overflow:
	// Is this number's potential exponent bigger than
	// long double allows?
	if (digits_.size() * kBasePower > LDBL_MAX_10_EXP) {
		return LDBL_MAX;
	}

	// If this didn't return above, the number might still be
	// too big, do a more fine-grained check
	if (*this > PeBigInt(LDBL_MAX)) {
		return LDBL_MAX;
	}

	// This number should be small enough to be
	// represented by a long double
	long double sum = 0.0, base_mul = 1.0;

	for (const auto &ai : digits_) {
		sum += base_mul * (long double)ai;

		// This can overflow on the last loop but
		// won't be used after that so don't worry
		base_mul *= (long double)kBase;
	}

	// Finally update the sign
	if (sign_ < 0) {
		sum = -sum;
	}

	return sum;
}
	
// Conversion to string
PeBigInt::operator string() const
{
	// Zero check
	if (isZero()) {
		return string("0");
	}

	ostringstream ss;

	// Write sign if negative
	if (sign_ < 0) {
		ss << "-";
	}

	// Write most significant digit "as normal"
	ss << digits_.back();

	// Write remaining digits with zero padding
	auto i_digit = digits_.rbegin() + 1; 
	for (; i_digit != digits_.rend(); ++i_digit) {
		ss << setfill('0') << setw(kBasePower) << *i_digit;
	}

	return ss.str();
}

// Helper functions for operator overloading.
// These typically work with the absolute value
// of the number.
	
// Comparison
bool PeBigInt::absEq(const PeBigInt &rhs) const
{
	// If number of digits doesn't match then they aren't equal
	if (digits_.size() != rhs.digits_.size()) {
		return false;
	}

	// Same number of digits, check digit-by-digit from
	// most to least significant and return false if any
	// inequality is found
	for (auto ithis = digits_.rbegin(), irhs = rhs.digits_.rbegin();
		ithis != digits_.rend(); ++ithis, ++irhs) {
		if (*ithis != *irhs) return false;
	}

	// If we've made it here then the two digits should be equal
	return true;
}

bool PeBigInt::absLt(const PeBigInt &rhs) const
{
	// Rhs has more digits, therefore rhs is bigger
	if (digits_.size() < rhs.digits_.size()) {
		return true;
	}

	// This has more digits, therefore this is bigger
	if (digits_.size() > rhs.digits_.size()) {
		return false;
	}

	// Same number of digits, check digit-by-digit from
	// most to least significant and return as soon as an
	// inequality is found
	for (auto ithis = digits_.rbegin(), irhs = rhs.digits_.rbegin();
		ithis != digits_.rend(); ++ithis, ++irhs) {
		if (*ithis < *irhs) return true;
		if (*ithis > *irhs) return false;
	}

	// If we reach here, the values are equal
	return false;
}

// Arithmetic
PeBigInt &PeBigInt::absPlusEq(const PeBigInt &rhs)
{
	PeUint carry = 0;

	// Loop over digits up to size of large number, with an extra loop if carry occurs
	for (size_t i = 0; i < std::max(digits_.size(), rhs.digits_.size()) || carry; ++i) {
		
		// Extend this number if we're past its size
		if (i == digits_.size()) {
			digits_.push_back(0);
		}

		digits_[i] += carry + (i < rhs.digits_.size() ? rhs.digits_[i] : 0);
		
		// Carry check
		carry = digits_[i] >= kBase;
		if (carry) {
			digits_[i] -= kBase;
		}
	}

	return *this;
}

// Note rhs must be smaller than this otherwise undefined
// behaviour occurs
PeBigInt &PeBigInt::absMinusEq(const PeBigInt &rhs)
{
	PeUint carry = 0;

	// Loop over rhs digits, or until carry occurs
	for (size_t i = 0; i < rhs.digits_.size() || carry; ++i) {
		digits_[i] -= carry + (i < rhs.digits_.size() ? rhs.digits_[i] : 0);
		
		// Carry check
		carry = digits_[i] < 0 ? 1 : 0;
		if (carry) {
			digits_[i] += kBase;
		}
	}

	// Clear any leading zeros
	while (digits_.size() > 1 && digits_.back() == 0) {
		digits_.pop_back();
	}

	return *this;
}

PeBigInt &PeBigInt::absMultEq(const PeBigInt &rhs)
{
	// Result digits
	vector<PeUint> res(digits_.size() + rhs.digits_.size(), 0);

	PeUint carry = 0;
	// Outer loop over this numbers digits
	for (size_t i = 0; i < digits_.size(); ++i) {
		// Inner loop over rhs digits with extra pass if carry occurs
		for (size_t j = 0; (j < rhs.digits_.size()) || carry; ++j) {
			PeInt cur = res[i + j] + digits_[i] *
				(j < rhs.digits_.size() ? rhs.digits_[j] : 0) + carry;

			res[i + j] = cur % kBase;
			carry = cur / kBase; // Integer division
		}
	}

	// Clear any leading zeros
	while ((res.size() > 1) && (res.back() == 0)) {
		res.pop_back();
	}

	// Move result to this
	digits_ = move(res);

	return *this;
}

// This function is slower than the other arithmetic operators.
// Throws runtime_error if rhs is zero.
PeBigInt &PeBigInt::absDivEq(const PeBigInt &rhs)
{
	// Divison by zero check
	if (rhs.isZero()) {
		throw runtime_error("PeBigInt: Division by zero.");
	}
		
	// Quick check for same abs value:
	// just set this value to 1.
	if (absEq(rhs)) {
		digits_ = vector<PeUint>({1});
	} else if (absLt(rhs)) {
		digits_ = vector<PeUint>({0});
	} else {
		// Copy this and rhs and set signs to positive
		PeBigInt numerator(*this);
		PeBigInt denominator(rhs);
		numerator.sign_ = 1;
		denominator.sign_ = 1;

		// Record the lengths of the numerator and denominator as integers
		int denominator_length = (int)(denominator.digits_.size());

		// The "quick denominator" is the leading digit of the denominator
		PeUint quick_d = denominator.digits_.back();

		// Initialise the quotient as numerator / a
		PeBigInt quotient(numerator);
		quotient.absShortDivEq(quick_d);

		// We actually want this to be "a000..." where
		// a000... is the same length as the denominator,
		// so we radix shift the quotient to account for
		// quick_d being < kBase
		quotient.radixShift(1 - denominator_length);

		PeBigInt remainder = denominator + PeBigInt(1);
		PeBigInt quotient_new;
		while (!remainder.absLt(denominator)) {
			remainder = numerator - quotient * denominator;
			quotient_new = remainder;
			quotient_new.absShortDivEq(quick_d);
			quotient_new.radixShift(1 - denominator_length);
			quotient_new += quotient;
			quotient += quotient_new;
			quotient.absShortDivEq(2);
		}

		remainder = numerator - quotient * denominator; // Calculate a final value for R
		if (remainder.sign_ < 0) {
			quotient -= PeBigInt(1);
			remainder += denominator;
		}

		digits_ = quotient.digits_;
	}

	return *this;
}

// Divide by a denominator less than kBase
// Note this doesn't check
PeBigInt &PeBigInt::absShortDivEq(PeInt denominator)
{
	PeUint carry = 0;

	// Loop over this number's digits in reverse order
	for (auto i = digits_.rbegin(); i != digits_.rend(); ++i) {
		PeUint cur = *i + carry * kBase;
		*i = cur / denominator;
		carry = cur % denominator;
	}

	popLeadingZeros();

	return *this;
}


// Helper functions for conversion from numeric types

// Non-integer floats are rounded
void PeBigInt::fromFloating(long double val)
{
	// Sign check
	if (val < 0.0) {
		sign_ = -1;
		val = -val;
	}

	val = roundl(val);
	long double base_cast = (long double)kBase;
	while (val > base_cast) {
		digits_.push_back((unsigned)(roundl(fmodl(val, base_cast))));
		val = floorl(val / base_cast);
	}
	digits_.push_back((unsigned)roundl(val));
}

void PeBigInt::fromSigned(PeInt val)
{
	// Check for negative
	if (val < 0) {
		sign_ = -1;
		val = -val; // Flip sign to allow unsigned conversion
	}

	// Convert the unsigned value
	fromUnsigned((PeUint)val);
}

void PeBigInt::fromUnsigned(PeUint val)
{
	while (val >= kBase) {
		digits_.push_back(val % kBase);
		val /= kBase;
	}
	digits_.push_back(val);
}

// String must match the regex [-+]?[0-9]+([eE][0-9]+)?
// i.e. leading sign optional, whole number, optional positive exponent.
// Invalid strings will be ignored and the value will be initialised to zero.
//
// Examples of valid strings include:
//	"123e45"
//	"-872"
//	"65443"
//	"+10e10"
//	"-10e10"
//
// Note that the exponent is not arbitrary precision!
// The exponent is a PeUint (long long unsigned) and must be within the range
// that your compiler supports. If this is 64 bits, the largest valid exponent
// in in the string is 18,446,744,073,709,551,615 i.e. a number that is over
// 18 quintillion decimal digits long... Good luck allocating that much memory.
//
// Even if it's only 32 bits, that's still 4,294,967,295 digits.
// Can you allocate a vector of 4 billion digits? Possibly. Will you get any
// reasonable performance from this class for a number that large? Unlikely.
// You probably need to look elsewhere for that kind of speedy math.
void PeBigInt::fromString(const string &valstr)
{
	// Check that initialiser string matches
	smatch value_match;

	if (regex_match(valstr, value_match, kValueStringRe)) {
		string strbase;
		PeUint exp_value = 0;

		// Sign match
		// Positive is the default so we only need to take action for a negative
		if (value_match[1].matched) {
			if (value_match[1].str() == "-") {
				sign_ = -1;
			}
		}

		// Value match
		if (value_match[2].matched) {
			strbase = value_match[2].str();
		}

		// Exponent match
		if (value_match[3].matched) {
			// Note +1 to chop off "e" at start of match
			exp_value = stoull(string(value_match[3].first + 1, value_match[3].second));
		}


		// Initialise base value
		PeUint i = (PeUint)strbase.length();
		for (; i > kBasePower; i -= kBasePower) {
			digits_.push_back(stoull(strbase.substr(i - kBasePower, kBasePower)));
		}
		digits_.push_back(stoull(strbase.substr(0, i)));

		// Apply exponent as radix shift
		radixShift(exp_value);

	} else { // Invalid initialiser string, set to zero and return
		digits_.push_back(0);
	}
}

// Convenience function to test if value is zero
// This is true if the digits field is empty, or if
// it contains a single zero, regardless of sign
bool PeBigInt::isZero() const
{
	return digits_.empty() || ((digits_.size() == 1) && (digits_.front() == 0));
}

// Remove any leading zeros
void PeBigInt::popLeadingZeros()
{
	// Clear any leading zeros
	while ((digits_.size() > 0) && (digits_.back() == 0)) {
		digits_.pop_back();
	}
}

PeBigInt &PeBigInt::power(PeUint exponent)
{
	// Only act on positive exponents
	if (exponent > 0) {
		PeBigInt tmp_val(1);

		// Square this number and halve the exponent until exponent is zero
		while (exponent > 1) {
			if (math::IsOdd(exponent)) {
				// If exponent is odd, accumuluate a multiplication in
				// tmp_val before squaring
				tmp_val *= *this;
			}

			// Square current number and halve exponent
			square();
			exponent /= 2; // Integer division
		}

		// Accumulate any "odd division" steps from above
		operator*=(tmp_val);
	}

	return *this;
}

// Radix shift function, equivalent to multiplying/dividing by kBase^n
// Analogous to << and >> operators in binary
// Positive n gives left shift (multiplies), negative n gives right shift (divides)
PeBigInt &PeBigInt::radixShift(PeInt n)
{
	// Right shift of more than digit size sets number to zero
	if ((PeInt)digits_.size() + n <= 0) {
		digits_.clear();
		digits_.push_back(0);
		return *this;
	}

	
	if (n > 0) { // Left shift - insert elements at front
		digits_.insert(digits_.begin(), n, 0);

		// Clear any leading zeros
		while (digits_.size() > 1 && digits_.back() == 0) {
			digits_.pop_back();
		}
	} else if (n < 0) { // Right shift - remove elements at front
		digits_.erase(digits_.begin(), digits_.begin() - n);
	}

	return *this;
}

// Reverse this number's digits
PeBigInt &PeBigInt::reverseDigits()
{
	// First, reverse the digits array
	reverse(digits_.begin(), digits_.end());

	// Not using accumulate here because we want PeBigInt in case we have overflow
	for (auto &i : digits_) {
		i = math::ReverseDigits(i);
	}

	return *this;
}

// Square this number i.e. n = n * n
// Used as a helper function for power()
PeBigInt &PeBigInt::square()
{
	// Result digits
	vector<PeUint> res(2 * digits_.size(), 0);

	// Outer loop over this numbers digits
	for (size_t i = 0; i < digits_.size(); ++i) {
		// Inner loop over rhs digits with extra pass if carry occurs
		for (size_t j = 0, carry = 0; (j < digits_.size()) || carry; ++j) {
			PeInt cur = res[i + j] + digits_[i] *
				(j < digits_.size() ? digits_[j] : 0) + carry;

			res[i + j] = (int)(cur % kBase);
			carry = (size_t)(cur / kBase);
		}
	}

	// Clear any leading zeros
	while ((res.size() > 1) && (res.back() == 0)) {
		res.pop_back();
	}

	// Move result to this
	digits_ = move(res);

	return *this;
}

// Return the sum of this number's digits (ignores sign)
PeBigInt PeBigInt::sumDigits()
{
	PeBigInt digit_sum(0);

	// Not using accumulate here because we want PeBigInt in case we have overflow
	for (const auto &i : digits_) {
		digit_sum += PeBigInt(math::SumDigits(i));
	}

	return digit_sum;
}
	
} // namespace pe