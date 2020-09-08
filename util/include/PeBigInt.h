#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace pe {
	
// A simple arbitrary precision unsigned integer class
// supporting addition, subtraction, multiplication and integer division
class PeBigInt
{
public:
	// Default constructor (gives a zero value)
	PeBigInt();
	
	// Copy and move constructors
	PeBigInt(const PeBigInt &that);
	PeBigInt(PeBigInt &&that) noexcept;
	
	// Constructors from integer types
	PeBigInt(int val);
	PeBigInt(long int val);
	PeBigInt(long long int val);
	PeBigInt(unsigned val);
	PeBigInt(long unsigned val);
	PeBigInt(long long unsigned val);
	
	// Constructors from floating point types
	PeBigInt(float val);
	PeBigInt(double val);
	PeBigInt(long double val);
	
	// String constructors
	// These take a positive, numeric integer string.
	// Decimals aren't supported.
	// Exponent notation supported if strictly of the form:
	//	x...x[e|E]p...p
	// e.g.
	//  1234e121
	//  1e1000
	//  653E001
	//  75357E010
	//
	// Note: no sign symbol, no decimals, no negative exponents, no whitespace
	PeBigInt(const char *valstr);
	PeBigInt(const std::string &valstr);
	
	virtual ~PeBigInt();
	
	// Copy and move assignment operator
	PeBigInt &operator=(const PeBigInt &rhs);
	PeBigInt &operator=(PeBigInt &&rhs) noexcept;

	// Unary operators
	PeBigInt operator-() const;

	// Relational operators
	bool operator<(const PeBigInt &rhs) const;
	bool operator>(const PeBigInt &rhs) const;
	bool operator<=(const PeBigInt &rhs) const;
	bool operator>=(const PeBigInt &rhs) const;
	bool operator==(const PeBigInt &rhs) const;
	bool operator!=(const PeBigInt &rhs) const;

	// Arithmetic operators
	PeBigInt &operator+=(const PeBigInt &rhs);
	PeBigInt &operator-=(const PeBigInt &rhs);
	PeBigInt &operator*=(const PeBigInt &rhs);
	PeBigInt &operator/=(const PeBigInt &rhs);



	// Friends defined inside class body are inline and are hidden from non-ADL lookup
	// Passing lhs by value helps optimise chains like a+b+c
	friend PeBigInt operator+(PeBigInt lhs, const PeBigInt& rhs)
	{
		lhs += rhs; // Reuse compound assignment
		return lhs; // Return the result by value (uses move constructor)
	}

	friend PeBigInt operator-(PeBigInt lhs, const PeBigInt& rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	friend PeBigInt operator*(PeBigInt lhs, const PeBigInt& rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	friend PeBigInt operator/(PeBigInt lhs, const PeBigInt& rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	
	// Conversion operators
	// Values that are too large for the data type
	// will be converted to the maximum value for
	// said data type.
	// Only the largest of each data type is converted to here,
	// odds are if bignum arithmetic is required you're dealing
	// with numbers beyond typical shorter data types...
	
	// Conversion to integer types
	operator long long int() const;
	operator long long unsigned() const;
	
	// Conversion to floating point types
	operator long double() const;
	
	// Conversion to string
	operator std::string() const;

	// Convenience zero test
	bool isZero() const;
	
	// Radix shift, analogous to << and >> for binary
	void radixShift(int n);


	// Private helper functions
private:
	// Initialiser functions
	void fromSigned(long long int val);
	void fromUnsigned(long long unsigned val);
	void fromFloating(long double val);
	void fromString(const std::string &valstr);

	// Helper functions for operator overloading.
	// These typically work with the absolute value
	// of the number.
	
	// Comparison
	bool absEq(const PeBigInt &rhs) const;
	bool absLt(const PeBigInt &rhs) const;

	// Arithmetic
	PeBigInt &absPlusEq(const PeBigInt &rhs);
	PeBigInt &absMinusEq(const PeBigInt &rhs);
	PeBigInt &absMultEq(const PeBigInt &rhs);

	// Warning: Horrifically slow. Only really here for a sense of completeness.
	PeBigInt &absDivEq(const PeBigInt &rhs);

	// Divide by a denominator less than kBase.
	// Note that denominator is not checked against kBase;
	// dividing by a denominator greater than kBase is undefined.
	PeBigInt &absShortDivEq(int denominator);

	// Utility

	// Remove any leading zeros
	void popLeadingZeros();


	void shortDivision(int divisor, int &quotient);

	// Members
private:
	int sign_;
	std::vector<int> digits_;

	// The base used in our representation
	// and its power of ten, useful for determining
	// string lengths and radix shifts
	static const int kBase = 10;
	static const int kBasePower = 1;

	// Static regex for matching initialiser string
	static const std::regex kValueStringRe;
}; // class PeBigInt
	
} // namespace pe