// Copyright 2020-2021 Paul Robertson
//
// PeProblem17.h
//
// "Number letter counts"

#include "PeProblem17.h"

#include "PeBigInt.h"


using namespace std;

namespace pe {

// This is a standalone constant, we do special checks for zero
const string kZeroName = "zero";

// String array of number names from 1-19 (the "teens" are special so they have to go here)
// First element empty to make the index match the value
const string kNumberNamesOnes[] = {
	"",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
	"ten",
	"eleven",
	"twelve",
	"thirteen",
	"fourteen",
	"fifteen",
	"sixteen",
	"seventeen",
	"eighteen",
	"nineteen"
};

// String array of "tens" number names from 0-90
// First two elements are empty and shouldn't be used
// but are left there so that array[2] corresponds to twenty
const string kNumberNamesTens[] ={
	"",
	"",
	"twenty",
	"thirty",
	"forty",
	"fifty",
	"sixty",
	"seventy",
	"eighty",
	"ninety"
};

// This is a standalone constant, we don't really need
// to check "powers of 100"
const string kHundredsName = "hundred";

// String array of powers of thousand names. Similar to above,
// the first entry is left blank so that the array index
// corresponds to the power e.g. array[2] = 1000^2 = million
// I've stopped the array at sextillion, one more than should be
// needed for a 64 bit unsigned integer.
const string kNumberNamesPowersOfThousand[] = {
	"",
	"thousand",
	"million",
	"billion",
	"trillion",
	"quadrillion",
	"quintillion",
	"sextillion"
};

// Special constant for "and"
const string kAnd = "and";


///////// PRE-COMPUTED STRING LENGTHS ////////

// String length of "zero"
const PeUint kZeroNameLength = 4;


// String length array of number names from 0-19
const PeUint kNumberNameLengthsOnes[] = {
	0,
	3, // len("one")
	3, // len("two")
	5, // len("three")
	4, // len("four")
	4, // len("five")
	3, // len("six")
	5, // len("seven")
	5, // len("eight")
	4, // len("nine")
	3, // len("ten")
	6, // len("eleven")
	6, // len("twelve")
	8, // len("thirteen")
	8, // len("fourteen")
	7, // len("fifteen")
	7, // len("sixteen")
	9, // len("seventeen")
	8, // len("eighteen")
	8, // len("nineteen")
};

// String length array of tens
const PeUint kNumberNameLengthsTens[] ={
	0,
	0,
	6,  // len("twenty")
	6,  // len("thirty")
	5,  // len("forty")
	5,  // len("fifty")
	5,  // len("sixty")
	7,  // len("seventy")
	6,  // len("eighty")
	6,  // len("ninety")
};

// Length of "hundred"
const PeUint kHundredsNameLength = 7; // len("hundred")

// String length array of powers of thousand names.
const PeUint kNumberNameLengthsPowersOfThousand[] = {
	0,
	8,  // len("thousand")
	7,  // len("million")
	7,  // len("billion")
	8,  // len("trillion")
	11, // len("quadrillion")
	11, // len("quintillion")
	10, // len("sextillion")
};

const PeUint kAndLength = 3; // len("and")

// Convert a number from 1-999 to a word representation
// Use the flag <use_spaces> to include spaces (normal, human readability)
// or not (used for the actual Project Euler problem)
static string NumberToWordsUnder1000(PeUint n, bool use_spaces = true)
{
	// Only deal with numbers under 1000
	if (n > 999) {
		n %= 1000;
	}

	// Special checks for 0-20 (return name directly)
	// This function shouldn't be called with 0 directly
	// n == 0 at this point will return an empty string
	if (n < 20) {
		return kNumberNamesOnes[n];
	}


	const PeUint kTensRadix = 10;
	ostringstream number_as_words;

	// Optional space and hyphen characters
	const string kSpace = use_spaces ? " " : "";
	const string kHyphen = use_spaces ? " " : "";

	// Split into buckets
	vector <PeUint> tens_buckets = math::NumberToRadixBuckets(n, kTensRadix);

	// Ones and tens must exist (n < 20 check above will eliminate ones only cases)
	const auto &ones = tens_buckets[0];
	const auto &tens = tens_buckets[1];

	// Check for hundreds bucket
	if (tens_buckets.size() == 3) {
		// If the bucket exists it must be non-zero, so we don't need to check for zero
		const auto &hundreds = tens_buckets[2];
		number_as_words << kNumberNamesOnes[hundreds] << kSpace << kHundredsName;

		// Do we need the "and" ?
		if (ones > 0 || tens > 0) {
			number_as_words << kSpace << kAnd << kSpace;
		}
	}

	// Special check for tens and ones being under twenty
	if (tens == 1 || (tens == 0 && ones > 0)) {
		PeUint number_under_twenty = kTensRadix*tens + ones;
		number_as_words << kNumberNamesOnes[number_under_twenty];
	} else {
		// Tens
		if (tens > 0) {
			number_as_words << kNumberNamesTens[tens];

			// Do we need a hyphen?
			if (ones > 0) {
				number_as_words << kHyphen;
			}
		}

		// Ones
		if (ones > 0) {
			number_as_words << kNumberNamesOnes[ones];
		}
	}

	return number_as_words.str();
}


// Convert a number to a word representation
// Use the flag <use_spaces> to include spaces (normal, human readability)
// or not (used for the actual Project Euler problem)
static string NumberAsWords(PeUint n, bool use_spaces = true)
{
	// Special check for zero
	if (n == 0) {
		return kZeroName;
	}

	// Quick return if we have a number less than 1000
	if (n < 1000) {
		return NumberToWordsUnder1000(n, use_spaces);
	}

	const PeUint kThousandsRadix = 1000;
	const string kSpace = use_spaces ? " " : "";

	// Stream to build the word representation of the number
	ostringstream number_as_words;

	// Split into buckets of thousands
	vector<PeUint> thousands_buckets = math::NumberToRadixBuckets(n, kThousandsRadix);

	// Loop over thousands buckets, starting at end (i.e. largest)
	size_t idx = thousands_buckets.size() - 1;

	for (auto i_thousands = thousands_buckets.rbegin();
		i_thousands != thousands_buckets.rend(); ++i_thousands, --idx) {

		// Only add words if we have a non-zero entry
		if (*i_thousands > 0) {
			// Convert the bucket value to words and append relevant "power of thousands" word,
			// but only if we're not at the end
			if (idx > 0) {
				number_as_words << NumberToWordsUnder1000(*i_thousands, use_spaces) << kSpace << 
					kNumberNamesPowersOfThousand[idx] << kSpace;
			} else {
				if (*i_thousands < 100) {
					number_as_words << kAnd << kSpace;
				}

				number_as_words << NumberToWordsUnder1000(*i_thousands);
			}
		}
	}

	// Check for a trailing space and pop if needed
	string number_as_words_string = number_as_words.str();

	if (number_as_words_string.back() == ' ') {
		number_as_words_string.pop_back();
	}

	return number_as_words_string;
}


// Get the length of a number's word representation, for numbers 1-999
// This method ignores spaces and hyphens
static PeUint NumberWordLengthUnder1000(PeUint n)
{
	// Only deal with numbers under 1000
	if (n > 999) {
		n %= 1000;
	}

	// Special checks for 0-20 (return name directly)
	// This function shouldn't be called with 0 directly
	// n == 0 at this point will return an empty string
	if (n < 20) {
		return kNumberNameLengthsOnes[n];
	}


	const PeUint kTensRadix = 10;
	PeUint number_length = 0;

	// Split into buckets
	vector <PeUint> tens_buckets = math::NumberToRadixBuckets(n, kTensRadix);

	// Ones and tens must exist (n < 20 check above will eliminate ones only cases)
	const auto &ones = tens_buckets[0];
	const auto &tens = tens_buckets[1];

	// Check for hundreds bucket
	if (tens_buckets.size() == 3) {
		// If the bucket exists it must be non-zero, so we don't need to check for zero
		const auto &hundreds = tens_buckets[2];
		number_length += kNumberNameLengthsOnes[hundreds] + kHundredsNameLength;

		// Do we need the "and" ?
		if (ones > 0 || tens > 0) {
			number_length += kAndLength;
		}
	}

	// Special check for tens and ones being under twenty
	if (tens == 1 || (tens == 0 && ones > 0)) {
		PeUint number_under_twenty = kTensRadix*tens + ones;
		number_length += kNumberNameLengthsOnes[number_under_twenty];
	} else {
		// Tens
		if (tens > 0) {
			number_length += kNumberNameLengthsTens[tens];

			// We're ignoring hyphens and spaces
		}

		// Ones
		if (ones > 0) {
			number_length += kNumberNameLengthsOnes[ones];
		}
	}

	return number_length;
}


// Get the string length of a number's word representation
// This method, unlike the NumberAsWords method, never includes
// spaces or hyphens
static PeUint NumberWordLength(PeUint n)
{
	// Special check for zero
	if (n == 0) {
		return kZeroNameLength;
	}

	// Quick return if we have a number less than 1000
	if (n < 1000) {
		return NumberWordLengthUnder1000(n);
	}

	const PeUint kThousandsRadix = 1000;

	// The final number word length
	PeUint number_length = 0;

	// Split into buckets of thousands
	vector<PeUint> thousands_buckets = math::NumberToRadixBuckets(n, kThousandsRadix);

	// Loop over thousands buckets, starting at end (i.e. largest)
	size_t idx = thousands_buckets.size() - 1;

	for (auto i_thousands = thousands_buckets.rbegin();
		i_thousands != thousands_buckets.rend(); ++i_thousands, --idx) {

		// Only add words if we have a non-zero entry
		if (*i_thousands > 0) {
			// Convert the bucket value to words and append relevant "power of thousands" word,
			// but only if we're not at the end
			if (idx > 0) {
				number_length += NumberWordLengthUnder1000(*i_thousands) + 
					kNumberNameLengthsPowersOfThousand[idx];
			} else {
				if (*i_thousands < 100) {
					number_length += kAndLength;
				}

				number_length += NumberWordLengthUnder1000(*i_thousands);
			}
		}
	}

	return number_length;
}


// A somewhat artificially bad solution: construct a valid string representation
// of the number (spaces, hyphens and all), then count the number of characters,
// excluding spaces and hyphens, and return the length.
static PeUint Method1(PeUint n)
{
	PeUint number_word_length_count = 0;

	// Loop from 1 to n
	for (PeUint i_n = 1; i_n <= n; ++i_n) {
		// Construct a word representation of the number, without spaces or hyphens,
		string number_string = NumberAsWords(i_n, false);

		number_word_length_count += (PeUint)number_string.size();
	}
	
	return number_word_length_count;
}


// Unlike Method 1, we accumulate string lengths in Method 2, rather than
// building the entire number string itself. We also make use of a flag
// in the NumberToWords helper function which stops it putting spaces or hyphens
// in the output string.
static PeUint Method2(PeUint n)
{
	PeUint number_word_length_count = 0;

	// Loop from 1 to n
	for (PeUint i_n = 1; i_n <= n; ++i_n) {
		// Get the "word length" of the number
		number_word_length_count += NumberWordLength(i_n);
	}

	return number_word_length_count;
}


// Helper functions for Method3

// Count the number of times the word "and" occurs below 1000
static PeUint AndsCountBelow1000(PeUint n)
{
	// Quick exit for 0
	if (n == 0) {
		return 0;
	}

	// Limit n to <1000
	if (n >= 1000) {
		n = 999;
	}

	// "and" has a reasonably simple formula:
	// It doesn't occur for numbers below 100 (inclusive) and occurs in 
	// every number above 100, except for multiples of 100
	return n > 100 ? n - 100 - (n / 100) + 1 : 0; // integer division
}


// Count the number of times the word "hundred" occurs below 1000
static PeUint HundredsCountBelow1000(PeUint n)
{
	// Quick exit for 0
	if (n == 0) {
		return 0;
	}

	// Limit n to <1000
	if (n >= 1000) {
		n = 999;
	}

	// "hundred" has a simple formula:
	// It occurs in every number above 100 (inclusive), so we simply count every number past 100
	return n >= 100 ? n - 99 : 0;
}


// Number of times a "ones" number (i.e. 1-9) occurs below 1000
// Each number occurs, as a stand alone word, 9 times each 100,
// plus 100 times for its "corresponding hundred".
// E.g. "four" occurs 9 times each 100, plus 100 times for 400-499
//
// Parameter <n> is the number to count up to (1-1000)
// Parameter <m> is the "word number" in question (1-9)
//
// E.g. OnesNumbersCountBelow1000(737, 5) = 7*9 + 100 + 3 = 166, because "five" occurs:
//   9 times each hundred for [1-99, 100-199, 200-299, 300-399, 400-499, 500-599, 600-699]
//   100 (additional) times for 500-599
//   3 times for 700-737
static PeUint OnesNumbersCountBelow1000(PeUint n, PeUint m)
{
	// Quick exit for 0
	if (n == 0) {
		return 0;
	}

	// Limit n to <1000
	if (n >= 1000) {
		n = 999;
	}

	// Record the hundreds number
	PeUint hundreds_number = n / 100; // integer division
	PeUint tens_ones_number = n % 100;

	// Nine times each 100
	// Note integer division
	PeUint word_count = 9 * hundreds_number;
	
	
	if (hundreds_number > m) {
		// Extra hundred if we're completely past the mth hundred
		word_count += 100;
	} else if (hundreds_number == m) {
		// More specific count needed if we're in the mth hundred
		word_count += tens_ones_number + 1;
	}

	// Now for the remainder after the hundreds:
	// one each ten, but skip the teens and add 1 if the ones digit
	// is greater than or equal to the "word number"
	// Note integer division
	word_count += tens_ones_number / 10 - static_cast<PeUint>(tens_ones_number >= 10 + m) + 
		static_cast<PeUint>((tens_ones_number % 10) >= m);

	return word_count;
}


// Number of times a "teens" number (i.e. 10-19) occurs below 1000
// Each number occurs, as a stand alone word, once each 100,
// so our main challenge is checking the mod(n, 100) case.
// E.g. Fifty occurs 20 times each in 1-200 (50-59, 150-159)
//
// Parameter <n> is the number to count up to (1-1000)
// Parameter <m> is the "word number" in question (10-19)
//
// E.g. TeensNumbersCountBelow1000(752, 12) = 7 + 1 = 8, because "twelve" occurs:
//   Once each hundred for [1-99, 100-199, 200-299, 300-399, 400-499, 500-599, 600-699]
//   Once in for 700-752 (since 52 >= 12)
static PeUint TeensNumbersCountBelow1000(PeUint n, PeUint m)
{
	// Quick exit for 0
	if (n == 0) {
		return 0;
	}

	// Limit n to <1000
	if (n >= 1000) {
		n = 999;
	}

	// Once per hundred for each complete hundred, plus one extra is mod(n,100) is above m
	return n / 100 + static_cast<PeUint>((n % 100) >= m);
}


// Number of times a "tens" number (i.e. 20-90) occurs below 1000
// Each number occurs, as a stand alone word, 10 times each 100,
// so our main challenge is checking the mod(n, 100) case.
// E.g. Fifty occurs 10 times each 100
//
// Parameter <n> is the number to count up to (1-1000)
// Parameter <m> is the "word number" in question (20-90)
//
// E.g. TensNumbersCountBelow1000(752, 50) = 7*10 + 3 = 73, because "fifty" occurs:
//   10 times each hundred for [1-99, 100-199, 200-299, 300-399, 400-499, 500-599, 600-699]
//   3 times for 700-752 (750, 751, 752 i.e. three occurrences)
static PeUint TensNumbersCountBelow1000(PeUint n, PeUint m)
{
	// Quick exit for 0
	if (n == 0) {
		return 0;
	}

	// Limit n to <1000
	if (n >= 1000) {
		n = 999;
	}

	// Record the number mod 100
	PeUint tens_ones_number = n % 100;

	// Ten times each 100
	// We need the integer division by 100 since we're counting
	// only "complete" hundreds
	PeUint word_count = 10 * (n / 100); // integer division

	// The final "hundred":
	// If the tens number (n % 100) is above m, we might need to count
	// more words, but no more than 10 more
	if (tens_ones_number >= m) {
		if (tens_ones_number < m + 9) {
			// Extra hundred if we're completely past the mth hundred
			word_count += tens_ones_number - m + 1;
		} else {
			word_count += 10;
		}
	}

	return word_count;
}


// A different approach that I must confess I couldn't quite be bothered finishing
// for the general case, so I've only solved it up to n = 1000.
// We know how to form the English language names for a given number, so we must
// know how to predict the number of times each word appears in a given range of
// numbers.
static PeUint Method3(PeUint n)
{
	// Ands, and hundreds
	PeUint number_word_length_count = kAndLength * AndsCountBelow1000(n) + 
		kHundredsNameLength * HundredsCountBelow1000(n);

	// Ones
	for (size_t i = 1; i < 10; ++i) {
		number_word_length_count += kNumberNameLengthsOnes[i] * OnesNumbersCountBelow1000(n, i);
	}

	// Teens
	for (size_t i = 10; i < 20; ++i) {
		number_word_length_count += kNumberNameLengthsOnes[i] * TeensNumbersCountBelow1000(n, i);
	}

	// Tens
	for (size_t i = 2, i_10 = 20; i < 10; ++i, i_10 += 10) {
		number_word_length_count += kNumberNameLengthsTens[i] * TensNumbersCountBelow1000(n, i_10);
	}

	// Thousands
	if (n >= 1000) {
		number_word_length_count += 11; // Length of "one" and "thousand"
	}

	return number_word_length_count;
}


ostream &PeProblem17::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
	"If the numbers 1 to 5 are written out in words: one, two, three, four, five," << endl <<
	"then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total." << endl << endl <<

	"If all the numbers from 1 to 1000 (one thousand) inclusive were" << endl <<
	"written out in words, how many letters would be used?" << endl << endl <<

	"NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and forty-two)" << endl <<
	"contains 23 letters and 115 (one hundred and fifteen) contains 20 letters." << endl <<
	"The use of \"and\" when writing out numbers is in compliance with British usage." << endl << endl;
	
	return os;
}


ostream &PeProblem17::DisplaySolution(ostream &os)
{
	const PeUint kTarget = 1000;
	auto answer1 = Method1(kTarget);
	auto answer2 = Method2(kTarget);
	auto answer3 = Method3(kTarget);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << answer1 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"A \"straightforward\" solution: construct a conventional English-language string" << endl <<
		"representation of the number, excluding spaces and hyphens, then count the" << endl <<
		"number of characters. Iteratively sum these string lengths to give the" << endl << 
		"overall character count from 1 to the target." << endl << endl <<
		"Method 1 solution: " << answer1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"Improve a bit on Method 1 by pre-computing the lengths of the word" << endl <<
		"components rather than bothering to construct the string itself." << endl << endl <<
		"Method 2 solution: " << answer2 << endl << endl <<

		formatting::MethodHeader(3) << endl << endl <<
		"A slightly different approach: we know that numbers words (ones, teens," << endl <<
		"tens, hundreds) occur regularly in number names, so we should be able to" << endl <<
		"calculate the total characters used without iteratively converting numbers to" << endl <<
		"word representations." << endl <<
		"For example, the word \"hundred\" will occur once in every number past 100" << endl <<
		"(inclusive), so we can calculate this with the expression:" << endl <<
		"    n >= 100 ? n - 99 : 0" << endl <<
		"Similarly, the word \"and\" will occur for every number above 100 (exclusive)," << endl <<
		"except for whole multiples of 100, leading to the expression:" << endl <<
		"    n > 100 ? n - 100 - (n / 100) + 1 : 0" << endl <<
		"By creating expressions for counting each number word and multiplying by the" << endl <<
		"string length of each number word, we can obtain the total character count." << endl <<
		"Note the expressions created for this method are only accurate for n in [1, 1000]" << endl << endl <<
		"Method 3 solution: " << answer3 << endl << endl;

	return os;
}

// Solution profiling

#define PROFILE_RETURN_TYPE_ PeUint
#define PROFILE_INPUT_TYPES_ PeUint
#define PROFILE_ARGS_ 1000

PROFILE_SOLUTIONS(PeProblem17, Method1, Method2, Method3)

#undef PROFILE_RETURN_TYPE_
#undef PROFILE_INPUT_TYPES_
#undef PROFILE_ARGS_

}; // namespace pe