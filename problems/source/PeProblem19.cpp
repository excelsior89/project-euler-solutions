// Copyright 2020-2022 Paul Robertson
//
// PeProblem19.cpp
//
// "Counting Sundays"

#include "PeProblem19.h"


using namespace std;

namespace pe {
// Helper data structure for dealing with dates.
//
// There are C++ utilities for time calculations e.g. <ctime> and <chrono>
// If you're using C++20 or later, you should also have access to specific classes
// for representing days, months etc.
// See https://en.cppreference.com/w/cpp/chrono
// However, since this problem is _about_ calendar mathematics, it seems to defeat the
// purpose to draw too much on existing time utilities, so we're rolling our own here.


// Note this struct does not validate the weekday against the date, nor does it
// validate the date against the month. It will permit impossible dates!
struct PeDate {
	PeDate(PeInt year = 0, PeInt month = 1, PeInt date = 1, PeInt day = 0) :
		year(year),
		month(month),
		date(date),
		day(day)
	{
	}

	// Copy constructor
	PeDate(const PeDate &other) :
		year(other.year),
		month(other.month),
		date(other.date),
		day(other.day)
	{
	}

	// Copy assignment
	PeDate &operator=(const PeDate &other)
	{
		// Guard self assignment
		if (this == &other) {
			return *this;
		}

		// Simple copies
		year = other.year;
		month = other.month;
		date = other.date;
		day = other.day;

		return *this;
	}

	// Comparison operators
	inline friend bool operator< (const PeDate &lhs, const PeDate &rhs) {
		return std::tie(lhs.year, lhs.month, lhs.date)
		< std::tie(rhs.year, rhs.month, rhs.date); // Keep the same order
	}
	inline friend bool operator> (const PeDate &lhs, const PeDate &rhs) { return rhs < lhs; }
	inline friend bool operator<=(const PeDate &lhs, const PeDate &rhs) { return !(lhs > rhs); }
	inline friend bool operator>=(const PeDate &lhs, const PeDate &rhs) { return !(lhs < rhs); }

	inline friend bool operator==(const PeDate &lhs, const PeDate &rhs) {
		return std::tie(lhs.year, lhs.month, lhs.date)
			== std::tie(rhs.year, rhs.month, rhs.date); // Keep the same order
	}
	inline friend bool operator!=(const PeDate &lhs, const PeDate &rhs) { return !(lhs == rhs); }

	// Determine if the current year is a leap year:
	// A year is a leap year if:
	// It's divisble by 4, but not by 100, unless it's also divisible by 400
	// E.g. 1904 is a leap year (divisible by 4, not by 100)
	// 1900 is NOT a leap year (divisble by 4, but is divisible by 100 and not by 400)
	// 2000 IS a leap year (divisible by 4, 100, 400)
	bool isLeapYear()
	{
		if (year % 400 == 0) {
			// Divisible by 400 is always a leap year since it's also divisible by 100 and 4
			return true;
		} else if (year % 100 == 0) {
			// Only divisible by 100, not a leap year
			return false;
		} else if (year % 4 == 0) {
			// Only divisible by 4 (and not 100 or 400), is a leap year
			return true;
		} else {
			// Not divisible by 4, not a leap year
			return false;
		}
	}

	void advanceByDays(PeInt days)
	{
		for (PeInt i = 0; i < days; ++i) {
			// Increment weekday - simple mod-7 check
			day = ++day % 7;


			// Increment date
			++date;

			// Date rollover handling
			switch (month) {
				// 31 day months: Jan, Mar, May, Jul, Aug, Oct, Dec
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
				{
					if (date > 31) {
						date = 1;
						++month;
					}
					break;
				}
				// 30 day months: Apr, Jun, Sep, Nov
				case 4:
				case 6:
				case 9:
				case 11:
				{
					if (date > 30) {
						date = 1;
						++month;
					}
					break;
				}
				// February!
				case 2:
				{
					// 29 days in a leap year, otherwise 28
					if (date > (isLeapYear() ? 29 : 28)) {
						date = 1;
						++month;
					}
					break;
				}
			}

			// Advance year if we're out of months
			if (month > 12) {
				month = 1;
				++year;
			}
		}
	}

	PeInt year;
	PeInt month;
	PeInt date;
	PeInt day; // Zero-indexed: 0 = Mon, 6 = Sun. This lets us use a simple modulo to advance the weekday.
};

// Compute the day of the week from a supplied date (in the Gregorian calendar)
static PeInt ComputeWeekdayFromDate(PeInt year, PeInt month, PeInt day)
{
	// Months and days need to be re-mapped: March is now the first month
	month -= 2;

	// Jan and Feb count as part of the previous year, so we may need to adust the year
	if (month < 1) {
		--year;
		month += 12;
	}
	
	// Century as a 2 digit number (e.g. 1976 --> 19)
	PeInt century = floor(year / 100);

	// Year as a 2 digit number (e.g. 1976 --> 76)
	PeInt year_two = year % 100;

	// The unadjusted day from this formula gives us 0 = Sunday, 6 = Saturday,
	// so we'll need to re-map it.
	PeInt unadjusted_day = (day + static_cast<PeInt>(floor(2.6 * month - 0.2)) - 2 * century + 
		year_two + static_cast<PeInt>(floor(year_two / 4.0f)) + static_cast<PeInt>(floor(century / 4.0f))) % 7;

	--unadjusted_day;
	if (unadjusted_day < 0) {
		unadjusted_day += 7;
	}

	return unadjusted_day;
}

// Simplistic: create a calendar-like system and iterate forwards,
// counting days that match the target date and weekday
static PeUint Method1(const PeDate &start_date, const PeDate &end_date, PeInt target_date, PeInt target_day)
{
	PeDate test_date(start_date);
	PeUint day_on_date_count = 0;
	PeUint year = start_date.year;

	// Incrementally advance days and perform Sunday check each time
	while (test_date < end_date) {
		if (test_date.date == target_date && test_date.day == target_day) {
			++day_on_date_count;
		}

		test_date.advanceByDays(1);
	}

	return day_on_date_count;
}


// We're dealing with several periodic patterns:
// Leap years, months, days per month and weekdays.
// It's possible to derive a formula to calculate the weekday from a supplied date:
//
//   mod(D + floor(2.6*M - 0.2) - 2*C + Y + floor(Y/4) + floor(C/4), 7)
//
// where:
//   D is the date of the month (1-31)
//   M is the month (1-12), but the months start from March (1) and end with February (12)
//   This means that January and February "belong" to the previous year
//   Y is the last two digits of the year (e.g. 1984 --> 84)
//   C is the first two digits of the year (e.g. 1984 --> 19)
static PeUint Method2(const PeDate &start_date, const PeDate &end_date, PeInt target_date, PeInt target_day)
{
	PeUint day_on_date_count = 0;

	// Iterate over years and months, checking the target date vs weekday
	for (PeInt i_year = start_date.year; i_year <= end_date.year; ++i_year) {
		for (PeInt i_month = 1; i_month <= 12; ++i_month) {
			if (ComputeWeekdayFromDate(i_year, i_month, target_date) == target_day) {
				++day_on_date_count;
			}
		}
	}

	return day_on_date_count;
}


ostream &PeProblem19::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
		"You are given the following information, but you may prefer to do some"
		"research for yourself." << endl << endl <<

		"    1 Jan 1900 was a Monday." << endl << endl <<
		"    Thirty days has September," << endl <<
		"    April, June and November." << endl <<
		"    All the rest have thirty-one," << endl <<
		"    Saving February alone," << endl <<
		"    Which has twenty-eight, rain or shine." << endl <<
		"    And on leap years, twenty-nine." << endl << endl <<
		"    A leap year occurs on any year evenly divisible by 4, but not on a century" << endl <<
		"    unless it is divisible by 400." << endl << endl <<

		"How many Sundays fell on the first of the month during the twentieth century" << endl <<
		"(1 Jan 1901 to 31 Dec 2000)?" << endl << endl;

	return os;
}

ostream &PeProblem19::DisplaySolution(ostream &os)
{
	PeDate start_date(1901, 1 /* Jan */, 1, 1 /* Tuesday */);
	PeDate end_date(2000, 12 /* Dec */, 31, 6 /* Sunday */);
	PeInt target_date = 1, target_day = 6;

	auto answer1 = Method1(start_date, end_date, target_date, target_day);
	auto answer2 = Method2(start_date, end_date, target_date, target_day);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << answer1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"The \"brute force\" method here involves creating our own calendar system" << endl <<
		"and advancing through the days one-by-one, checking for if the first of the" << endl <<
		"month is a Sunday at each step." << endl << endl <<
		"Method 1 solution: " << answer1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"Despite the complexity, weekdays and dates ARE predictable." << endl <<
		"A formula can be used to calculate the day of the week given the" << endl <<
		"year, month and day:" << endl << endl <<
		"    mod(D + floor(2.6*M - 0.2) - 2*C + Y + floor(Y/4) + floor(C/4), 7)" << endl << endl <<
		"where:" << endl <<
		"    D is the date (1-31)" << endl <<
		"    M is the month (1-12)*" << endl <<
		"    C is the first two digits of year (1984 --> 19)" << endl <<
		"    Y is the last two digits of year (1984 --> 84)" << endl << endl <<
		"*For this formula, the months start at March (1) and end at February (12)." << endl <<
		"January and February belong to the previous year e.g. Jan 1991 \"counts\"" << endl <<
		"as the 11th month of 1990. The weekday returned begins at Sunday (0)" << endl <<
		"and ends at Saturday (6)." << endl << endl <<
		"Method 2 solution: " << answer2 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ const PeDate&, const PeDate&, PeInt, PeInt
#define PROFILE_ARGS_ PeDate(1900, 1 /* Jan */, 1, 0 /* Monday */), PeDate(2000, 12 /* Dec */, 31, 6 /* Sunday */), 1, 0

PROFILE_SOLUTIONS(PeProblem19, Method1, Method2)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe