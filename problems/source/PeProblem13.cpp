// Copyright 2020 Paul Robertson
//
// PeProblem13.cpp
//
// "Large sum"

#include "PeProblem13.h"

using namespace std;

namespace pe {

#define MNUMBERS 100

// Data for the problem
const array<string, MNUMBERS> kLargeNumbers {
	"37107287533902102798797998220837590246510135740250",
	"46376937677490009712648124896970078050417018260538",
	"74324986199524741059474233309513058123726617309629",
	"91942213363574161572522430563301811072406154908250",
	"23067588207539346171171980310421047513778063246676",
	"89261670696623633820136378418383684178734361726757",
	"28112879812849979408065481931592621691275889832738",
	"44274228917432520321923589422876796487670272189318",
	"47451445736001306439091167216856844588711603153276",
	"70386486105843025439939619828917593665686757934951",
	"62176457141856560629502157223196586755079324193331",
	"64906352462741904929101432445813822663347944758178",
	"92575867718337217661963751590579239728245598838407",
	"58203565325359399008402633568948830189458628227828",
	"80181199384826282014278194139940567587151170094390",
	"35398664372827112653829987240784473053190104293586",
	"86515506006295864861532075273371959191420517255829",
	"71693888707715466499115593487603532921714970056938",
	"54370070576826684624621495650076471787294438377604",
	"53282654108756828443191190634694037855217779295145",
	"36123272525000296071075082563815656710885258350721",
	"45876576172410976447339110607218265236877223636045",
	"17423706905851860660448207621209813287860733969412",
	"81142660418086830619328460811191061556940512689692",
	"51934325451728388641918047049293215058642563049483",
	"62467221648435076201727918039944693004732956340691",
	"15732444386908125794514089057706229429197107928209",
	"55037687525678773091862540744969844508330393682126",
	"18336384825330154686196124348767681297534375946515",
	"80386287592878490201521685554828717201219257766954",
	"78182833757993103614740356856449095527097864797581",
	"16726320100436897842553539920931837441497806860984",
	"48403098129077791799088218795327364475675590848030",
	"87086987551392711854517078544161852424320693150332",
	"59959406895756536782107074926966537676326235447210",
	"69793950679652694742597709739166693763042633987085",
	"41052684708299085211399427365734116182760315001271",
	"65378607361501080857009149939512557028198746004375",
	"35829035317434717326932123578154982629742552737307",
	"94953759765105305946966067683156574377167401875275",
	"88902802571733229619176668713819931811048770190271",
	"25267680276078003013678680992525463401061632866526",
	"36270218540497705585629946580636237993140746255962",
	"24074486908231174977792365466257246923322810917141",
	"91430288197103288597806669760892938638285025333403",
	"34413065578016127815921815005561868836468420090470",
	"23053081172816430487623791969842487255036638784583",
	"11487696932154902810424020138335124462181441773470",
	"63783299490636259666498587618221225225512486764533",
	"67720186971698544312419572409913959008952310058822",
	"95548255300263520781532296796249481641953868218774",
	"76085327132285723110424803456124867697064507995236",
	"37774242535411291684276865538926205024910326572967",
	"23701913275725675285653248258265463092207058596522",
	"29798860272258331913126375147341994889534765745501",
	"18495701454879288984856827726077713721403798879715",
	"38298203783031473527721580348144513491373226651381",
	"34829543829199918180278916522431027392251122869539",
	"40957953066405232632538044100059654939159879593635",
	"29746152185502371307642255121183693803580388584903",
	"41698116222072977186158236678424689157993532961922",
	"62467957194401269043877107275048102390895523597457",
	"23189706772547915061505504953922979530901129967519",
	"86188088225875314529584099251203829009407770775672",
	"11306739708304724483816533873502340845647058077308",
	"82959174767140363198008187129011875491310547126581",
	"97623331044818386269515456334926366572897563400500",
	"42846280183517070527831839425882145521227251250327",
	"55121603546981200581762165212827652751691296897789",
	"32238195734329339946437501907836945765883352399886",
	"75506164965184775180738168837861091527357929701337",
	"62177842752192623401942399639168044983993173312731",
	"32924185707147349566916674687634660915035914677504",
	"99518671430235219628894890102423325116913619626622",
	"73267460800591547471830798392868535206946944540724",
	"76841822524674417161514036427982273348055556214818",
	"97142617910342598647204516893989422179826088076852",
	"87783646182799346313767754307809363333018982642090",
	"10848802521674670883215120185883543223812876952786",
	"71329612474782464538636993009049310363619763878039",
	"62184073572399794223406235393808339651327408011116",
	"66627891981488087797941876876144230030984490851411",
	"60661826293682836764744779239180335110989069790714",
	"85786944089552990653640447425576083659976645795096",
	"66024396409905389607120198219976047599490197230297",
	"64913982680032973156037120041377903785566085089252",
	"16730939319872750275468906903707539413042652315011",
	"94809377245048795150954100921645863754710598436791",
	"78639167021187492431995700641917969777599028300699",
	"15368713711936614952811305876380278410754449733078",
	"40789923115535562561142322423255033685442488917353",
	"44889911501440648020369068063960672322193204149535",
	"41503128880339536053299340368006977710650566631954",
	"81234880673210146739058568557934581403627822703280",
	"82616570773948327592232845941706525094512325230608",
	"22918802058777319719839450180888072429661980811197",
	"77158542502016545090413245809786882778948721859617",
	"72107838435069186155435662884062257473692284509516",
	"20849603980134001723930671666823555245252804609722",
	"53503534226472524250874054075591789781264330331690"
};

// Number of leading digits to return.
// If this is longer than the length of the sum then things will naturally
// get a bit silly...
// Asking for more than ~14 digits will start to break things down
// in methods 1 and 2 as they rely on finite precision arithmetic
const PeUint kNDigits = 10;

// A "hopeful" approach: convert each number to a double and sum them.
// The accuracy of a double precision floating point numbers is roughly
// 16 decimal digits, so the first ten digits should remain accurate
// enough even though the calculated sum might differ from the true sum.
// Perhaps unsurprisingly, this method is the fastest method as it uses the
// most "built in" arithmetic.
static PeUint Method1(const array<string, MNUMBERS> &number_array)
{
	// Collect the sum of the numbers as doubles
	double sum = 0.0;

	for (const auto &i: number_array) {
		sum += stod(i);
	}

	// Get the leading N digits
	double sum_exp10 = floor(log10(sum)) - static_cast<double>(kNDigits - 1);
	sum /= pow(10.0, sum_exp10);

	// Round the sum and cast as an integer value
	// Using long long (64 bit) precision should let us support up to 19 digits
	PeUint first_n_digits = static_cast<PeUint>(round(sum));

	return first_n_digits;
}

// A different "hopeful" approach: take the leading digits of each number as an
// integer and sum them using integer arithmetic. By the same reasoning as for
// floating point precision, the first digits of the sum should stay the same
// as the true value provided we use enough significant digits from each number.
// The method is the second fastest since it involves a bit more string
// manipulation to read in the digits compared to method 1, but is still using
// built in arithmetic to do its calculations
static PeUint Method2(const array<string, MNUMBERS> &number_array)
{
	// Collect the sum of the numbers as long long (64 bit) unsigned
	PeUint sum = 0;

	// Careful not to make this too large for the integer data type
	const PeUint kSummationDigits = 16;
	for (const auto &i: number_array) {
		// Take leading digits of string to convert to integer
		sum += stoull(i.substr(0, kSummationDigits));
	}

	// Get the leading N digits
	double sum_exp10 = floor(log10(sum)) - static_cast<double>(kNDigits - 1);
	sum /= static_cast<PeUint>(pow(10.0, sum_exp10)); // Integer division
	
	// Sum is already a long long so return it directly
	return sum;
}

// This method uses arbitrary precision integer arithmetic to calculate the
// final sum. This is implemented in a class PeBigInt, which started out as an
// attempt to answer this problem but grew into a slightly more full-featured
// class which supports addition, subtraction, multiplication and integer
// division for signed integers.
// Compared to the first two methods, this method is very slow. This shouldn't
// be a surprise, as it's implementing its own arithmetic processes at a much
// higher level than conventional computer arithmetic, and using generally
// naive algorithms to do so. It is, however, the only method that will return
// the correct result, no matter how long the numbers get.
static PeUint Method3(const array<string, MNUMBERS> &number_array)
{
	// Collect the sum of the numbers as PeBigInts
	PeBigInt sum(0);

	for (const auto &i: number_array) {
		sum += PeBigInt(i);
	}

	// Convert the sum to a string
	string cStr = string(sum);

	// Take the first 10 digits and return as a regular unsigned integer
	return stoull(cStr.substr(0, kNDigits));
}


ostream &PeProblem13::DisplayProblem(ostream &os)
{
	os << formatting::ProblemHeader(kProblemNumber) << endl << endl <<
	"    Work out the first ten digits of the sum of the following one-hundred 50-digit numbers." << endl << endl <<
	
	"    37107287533902102798797998220837590246510135740250" << endl <<
	"    46376937677490009712648124896970078050417018260538" << endl <<
	"    74324986199524741059474233309513058123726617309629" << endl <<
	"    91942213363574161572522430563301811072406154908250" << endl <<
	"    23067588207539346171171980310421047513778063246676" << endl <<
	"    89261670696623633820136378418383684178734361726757" << endl <<
	"    28112879812849979408065481931592621691275889832738" << endl <<
	"    44274228917432520321923589422876796487670272189318" << endl <<
	"    47451445736001306439091167216856844588711603153276" << endl <<
	"    70386486105843025439939619828917593665686757934951" << endl <<
	"    62176457141856560629502157223196586755079324193331" << endl <<
	"    64906352462741904929101432445813822663347944758178" << endl <<
	"    92575867718337217661963751590579239728245598838407" << endl <<
	"    58203565325359399008402633568948830189458628227828" << endl <<
	"    80181199384826282014278194139940567587151170094390" << endl <<
	"    35398664372827112653829987240784473053190104293586" << endl <<
	"    86515506006295864861532075273371959191420517255829" << endl <<
	"    71693888707715466499115593487603532921714970056938" << endl <<
	"    54370070576826684624621495650076471787294438377604" << endl <<
	"    53282654108756828443191190634694037855217779295145" << endl <<
	"    36123272525000296071075082563815656710885258350721" << endl <<
	"    45876576172410976447339110607218265236877223636045" << endl <<
	"    17423706905851860660448207621209813287860733969412" << endl <<
	"    81142660418086830619328460811191061556940512689692" << endl <<
	"    51934325451728388641918047049293215058642563049483" << endl <<
	"    62467221648435076201727918039944693004732956340691" << endl <<
	"    15732444386908125794514089057706229429197107928209" << endl <<
	"    55037687525678773091862540744969844508330393682126" << endl <<
	"    18336384825330154686196124348767681297534375946515" << endl <<
	"    80386287592878490201521685554828717201219257766954" << endl <<
	"    78182833757993103614740356856449095527097864797581" << endl <<
	"    16726320100436897842553539920931837441497806860984" << endl <<
	"    48403098129077791799088218795327364475675590848030" << endl <<
	"    87086987551392711854517078544161852424320693150332" << endl <<
	"    59959406895756536782107074926966537676326235447210" << endl <<
	"    69793950679652694742597709739166693763042633987085" << endl <<
	"    41052684708299085211399427365734116182760315001271" << endl <<
	"    65378607361501080857009149939512557028198746004375" << endl <<
	"    35829035317434717326932123578154982629742552737307" << endl <<
	"    94953759765105305946966067683156574377167401875275" << endl <<
	"    88902802571733229619176668713819931811048770190271" << endl <<
	"    25267680276078003013678680992525463401061632866526" << endl <<
	"    36270218540497705585629946580636237993140746255962" << endl <<
	"    24074486908231174977792365466257246923322810917141" << endl <<
	"    91430288197103288597806669760892938638285025333403" << endl <<
	"    34413065578016127815921815005561868836468420090470" << endl <<
	"    23053081172816430487623791969842487255036638784583" << endl <<
	"    11487696932154902810424020138335124462181441773470" << endl <<
	"    63783299490636259666498587618221225225512486764533" << endl <<
	"    67720186971698544312419572409913959008952310058822" << endl <<
	"    95548255300263520781532296796249481641953868218774" << endl <<
	"    76085327132285723110424803456124867697064507995236" << endl <<
	"    37774242535411291684276865538926205024910326572967" << endl <<
	"    23701913275725675285653248258265463092207058596522" << endl <<
	"    29798860272258331913126375147341994889534765745501" << endl <<
	"    18495701454879288984856827726077713721403798879715" << endl <<
	"    38298203783031473527721580348144513491373226651381" << endl <<
	"    34829543829199918180278916522431027392251122869539" << endl <<
	"    40957953066405232632538044100059654939159879593635" << endl <<
	"    29746152185502371307642255121183693803580388584903" << endl <<
	"    41698116222072977186158236678424689157993532961922" << endl <<
	"    62467957194401269043877107275048102390895523597457" << endl <<
	"    23189706772547915061505504953922979530901129967519" << endl <<
	"    86188088225875314529584099251203829009407770775672" << endl <<
	"    11306739708304724483816533873502340845647058077308" << endl <<
	"    82959174767140363198008187129011875491310547126581" << endl <<
	"    97623331044818386269515456334926366572897563400500" << endl <<
	"    42846280183517070527831839425882145521227251250327" << endl <<
	"    55121603546981200581762165212827652751691296897789" << endl <<
	"    32238195734329339946437501907836945765883352399886" << endl <<
	"    75506164965184775180738168837861091527357929701337" << endl <<
	"    62177842752192623401942399639168044983993173312731" << endl <<
	"    32924185707147349566916674687634660915035914677504" << endl <<
	"    99518671430235219628894890102423325116913619626622" << endl <<
	"    73267460800591547471830798392868535206946944540724" << endl <<
	"    76841822524674417161514036427982273348055556214818" << endl <<
	"    97142617910342598647204516893989422179826088076852" << endl <<
	"    87783646182799346313767754307809363333018982642090" << endl <<
	"    10848802521674670883215120185883543223812876952786" << endl <<
	"    71329612474782464538636993009049310363619763878039" << endl <<
	"    62184073572399794223406235393808339651327408011116" << endl <<
	"    66627891981488087797941876876144230030984490851411" << endl <<
	"    60661826293682836764744779239180335110989069790714" << endl <<
	"    85786944089552990653640447425576083659976645795096" << endl <<
	"    66024396409905389607120198219976047599490197230297" << endl <<
	"    64913982680032973156037120041377903785566085089252" << endl <<
	"    16730939319872750275468906903707539413042652315011" << endl <<
	"    94809377245048795150954100921645863754710598436791" << endl <<
	"    78639167021187492431995700641917969777599028300699" << endl <<
	"    15368713711936614952811305876380278410754449733078" << endl <<
	"    40789923115535562561142322423255033685442488917353" << endl <<
	"    44889911501440648020369068063960672322193204149535" << endl <<
	"    41503128880339536053299340368006977710650566631954" << endl <<
	"    81234880673210146739058568557934581403627822703280" << endl <<
	"    82616570773948327592232845941706525094512325230608" << endl <<
	"    22918802058777319719839450180888072429661980811197" << endl <<
	"    77158542502016545090413245809786882778948721859617" << endl <<
	"    72107838435069186155435662884062257473692284509516" << endl <<
	"    20849603980134001723930671666823555245252804609722" << endl <<
	"    53503534226472524250874054075591789781264330331690" << endl << endl;

	return os;
}


ostream &PeProblem13::DisplaySolution(ostream &os)
{
	auto first_ten1 = Method1(kLargeNumbers);
	auto first_ten2 = Method2(kLargeNumbers);
	auto first_ten3 = Method3(kLargeNumbers);

	os << formatting::SolutionHeader(kProblemNumber) << endl << endl <<
		"Answer: " << first_ten3 << endl << endl <<

		formatting::MethodHeader(1) << endl << endl <<
		"No standard number types in C++ can support 50 digits of precision" << endl <<
		"reliably, however since we're not being asked for the complete sum," << endl <<
		"we can get away with using standard data types to get an approximate" << endl <<
		"result that should still have the same leading digits as the true answer." << endl <<
		"Double precision floating point numbers can support roughly 15-16" << endl <<
		"digits accurately. By calculating the sum using doubles, we can be sure" << endl <<
		"that the leading 10 digits will still be the same as in the true sum," << endl <<
		"even if the remaining digits aren't." << endl << endl <<
		"Method 1 solution: " << first_ten1 << endl << endl <<

		formatting::MethodHeader(2) << endl << endl <<
		"This method is similar to the first in that it relies on finite" << endl <<
		"precision being \"good enough\" to get the leading 10 digits. This" << endl <<
		"time, we convert the numbers to long integers (using the leading 16" << endl <<
		"digits) and calcualte the sum using integer addition. The summation" << endl <<
		"process here is accurate, but the initial rounding of the numbers" << endl <<
		"introduces inaccuracies."<< endl << endl <<
		"Method 2 solution: " << first_ten2 << endl << endl <<

		formatting::MethodHeader(3) << endl << endl <<
		"This is a bit of overkill, but we can directly compute the answer using" << endl <<
		"arbitrary precision arithmetic. The big tradeoff here is speed; this" << endl <<
		"way is orders of magnitude slower than the first two, but it is the" << endl <<
		"method guaranteed to be accurate right down to the last digit for any" << endl <<
		"size of input digits. It uses a custom PeBigInt class implemented" << endl <<
		"especially for this problem, so all we have to do is sum up all" << endl <<
		"the numbers and return the first ten digits of the result." << endl << endl <<
		"The trick, of course, is in implementing addition as an algorithm" << endl <<
		"that operates on an array of digits instead of relying on the \"+\"" << endl <<
		"operator." << endl << endl <<
		"One way this can be implemented is as follows:" << endl <<
		"    - Treat each number as an array of digits" << endl <<
		"    - Make the shorter array the same size as the longer array" << endl <<
		"        - Do this by padding the shorter array with zeros at the front" << endl <<
		"          (most significant end)" << endl <<
		"    - Create a new array one longer than the summand arrays to hold" << endl <<
		"      the sum" << endl <<
		"    - Create a flag to record if carry occurs and initialise it as" << endl <<
		"      false" << endl <<
		"    - Starting at the least significant end and going digit-by-digit:" << endl <<
		"        - Add the digits from the two arrays" << endl <<
		"        - If the carry flag is true, add one" << endl <<
		"        - If the sum is greater than the base (in this case, 10)," << endl <<
		"          subtract the base from the sum (e.g. 6+7=13, 13>10 so" << endl <<
		"          13-10=3)" << endl <<
		"        - If the sum was greater than the base, set the carry flag to" << endl <<
		"          true, otherwise set the carry flag to false" << endl <<
		"        - Record the sum, after subtracting the base if necessary, in" << endl <<
		"          the same position in the sum array" << endl <<
		"    - Following the above method from least to most significant digit," << endl <<
		"      we end up calculating the sum" << endl <<
		"    - When we reach the final, most significant, digits, if the carry" << endl <<
		"      flag is true then we set the most significan digit of the" << endl <<
		"      answer to 1 (remember we made this array one bigger than the" << endl <<
		"      summand arrays)" << endl << endl <<
		"Method 3 solution: " << first_ten3 << endl << endl;

	return os;
}

ostream &PeProblem13::ProfileSolutions(int n_trials, ostream &os)
{
	os << formatting::ProfileHeader(kProblemNumber) << endl << endl;

	PeUint first_ten1 = 0,
		first_ten2 = 0,
		first_ten3 = 0;

	clock_t start_time(clock());
	for (int i = 0; i < n_trials; ++i) {
		first_ten1 = Method1(kLargeNumbers);
	}
	clock_t method_1_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		first_ten2 = Method2(kLargeNumbers);
	}
	clock_t method_2_time = clock() - start_time;

	start_time = clock();
	for (int i = 0; i < n_trials; ++i) {
		first_ten3 = Method3(kLargeNumbers);
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