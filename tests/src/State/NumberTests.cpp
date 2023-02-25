#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/ReadableNumber.h>
#include <LuaCpp2/WritableNumber.h>

#include <LuaCpp2/ReadableInteger.h>
#include <LuaCpp2/WritableInteger.h>

BOOST_AUTO_TEST_SUITE(State__Numbers)

BOOST_AUTO_TEST_CASE(testCharMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	char val = std::numeric_limits<char>::min();
	state << Lua::WritableInteger<char>(val);

	char val2 = 0;
	Lua::ReadableInteger<char> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testShortMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	short val = std::numeric_limits<short>::min();
	state << Lua::WritableInteger<short>(val);

	short val2 = 0;
	Lua::ReadableInteger<short> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testIntMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	int val = std::numeric_limits<int>::min();
	state << Lua::WritableInteger<int>(val);

	int val2 = 0;
	Lua::ReadableInteger<int> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testLongMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	long val = std::numeric_limits<long>::min();
	state << Lua::WritableInteger<long>(val);

	long val2 = 0;
	Lua::ReadableInteger<long> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testLongLongMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	long long val = std::numeric_limits<long long>::min();
	state << Lua::WritableInteger<long long>(val);

	long long val2 = 0;
	Lua::ReadableInteger<long long> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testUCharMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned char val = std::numeric_limits<unsigned char>::min();
	state << Lua::UnsignedWritableInteger<unsigned char>(val);

	unsigned char val2 = 1;
	Lua::UnsignedReadableInteger<unsigned char> uri(val2);
	state >> uri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testUShortMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned short val = std::numeric_limits<unsigned short>::min();
	state << Lua::UnsignedWritableInteger<unsigned short>(val);

	unsigned short val2 = 1;
	Lua::UnsignedReadableInteger<unsigned short> uri(val2);
	state >> uri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testUIntMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned int val = std::numeric_limits<unsigned int>::min();
	state << Lua::UnsignedWritableInteger<unsigned int>(val);

	unsigned int val2 = 1;
	Lua::UnsignedReadableInteger<unsigned int> uri(val2);
	state >> uri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testULongMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned long val = std::numeric_limits<unsigned long>::min();
	state << Lua::WritableInteger<unsigned long>(val);

	unsigned long val2 = 1;
	Lua::UnsignedReadableInteger<unsigned long> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testULongLongMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned long long val = std::numeric_limits<unsigned long long>::min();
	state << Lua::UnsignedWritableInteger<unsigned long long>(val);

	unsigned long long val2 = 1;
	Lua::UnsignedReadableInteger<unsigned long long> uri(val2);
	state >> uri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testCharMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	char val = std::numeric_limits<char>::max();
	state << Lua::WritableInteger<char>(val);

	char val2 = 0;
	Lua::ReadableInteger<char> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testShortMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	short val = std::numeric_limits<short>::max();
	state << Lua::WritableInteger<short>(val);

	short val2 = 0;
	Lua::ReadableInteger<short> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testInttMax)
{
	Lua::State state;
	state.open();

	int val = std::numeric_limits<int>::max();
	state << Lua::WritableInteger<int>(val);

	int val2 = 0;
	Lua::ReadableInteger<int> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testLongtMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	long val = std::numeric_limits<long>::max();
	state << Lua::WritableInteger<long>(val);

	long val2 = 0;
	Lua::ReadableInteger<long> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testLongtLongMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	long long val = std::numeric_limits<long long>::max();
	state << Lua::WritableInteger<long long>(val);

	long long val2 = 0;
	Lua::ReadableInteger<long long> ri(val2);
	state >> ri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testUCharMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned char val = std::numeric_limits<unsigned char>::max();
	state << Lua::UnsignedWritableInteger<unsigned char>(val);

	unsigned char val2 = 0;
	Lua::UnsignedReadableInteger<unsigned char> uri(val2);
	state >> uri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testUShortMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned short val = std::numeric_limits<unsigned short>::max();
	state << Lua::UnsignedWritableInteger<unsigned short>(val);

	unsigned short val2 = 0;
	Lua::UnsignedReadableInteger<unsigned short> uri(val2);
	state >> uri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testUInttMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned int val = std::numeric_limits<unsigned int>::max();
	state << Lua::UnsignedWritableInteger<unsigned int>(val);

	unsigned int val2 = 0;
	Lua::UnsignedReadableInteger<unsigned int> uri(val2);
	state >> uri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testULongtMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	unsigned long val = std::numeric_limits<unsigned long>::max();
	state << Lua::UnsignedWritableInteger<unsigned long>(val);

	unsigned long val2 = 0;
	Lua::UnsignedReadableInteger<unsigned long> uri(val2);
	state >> uri;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testCharMinOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	constexpr short val = std::numeric_limits<char>::min() - 1;
	state << Lua::WritableInteger<short>(val);

	BOOST_CHECK_EXCEPTION(
		{
			char val2 = 0;
			Lua::ReadableInteger<char> ri(val2);
			state >> ri;
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testShortMinOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	constexpr int val = std::numeric_limits<short>::min() - 1;
	state << Lua::WritableInteger<int>(val);

	BOOST_CHECK_EXCEPTION(
		{
			short val2 = 0;
			Lua::ReadableInteger<short> ri(val2);
			state >> ri;
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testIntMinOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	constexpr long long val = std::numeric_limits<int>::min() - 1ll;
	state << Lua::WritableInteger<long long>(val);

	BOOST_CHECK_EXCEPTION(
		{
			int val2 = 0;
			Lua::ReadableInteger<int> ri(val2);
			state >> ri;
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testLongMinOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	constexpr long long val = std::numeric_limits<long>::min() - 1ll;
	state << Lua::WritableInteger<long long>(val);

	BOOST_CHECK_EXCEPTION(
		{
			long val2 = 0;
			Lua::ReadableInteger<long> ri(val2);
			state >> ri;
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testNegValueToUnsignedTypeExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	state << Lua::WritableInteger<int>(-1);

	BOOST_CHECK_EXCEPTION(
		{
			unsigned int val;
			Lua::UnsignedReadableInteger<unsigned int> uri(val);
			state >> uri;
		},
		std::invalid_argument,
		[] (const std::invalid_argument& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testCharMaxOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	constexpr short val = std::numeric_limits<char>::max() + 1;
	state << Lua::WritableInteger<short>(val);

	BOOST_CHECK_EXCEPTION(
		{
			char val2 = 0;
			Lua::ReadableInteger<char> ri(val2);
			state >> ri;
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testShortMaxOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	constexpr int val = std::numeric_limits<short>::max() + 1;
	state << Lua::WritableInteger<int>(val);

	BOOST_CHECK_EXCEPTION(
		{
			short val2 = 0;
			Lua::ReadableInteger<short> ri(val2);
			state >> ri;
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testIntMaxOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	constexpr long long val = std::numeric_limits<int>::max() + 1ll;
	state << Lua::WritableInteger<long long>(val);

	BOOST_CHECK_EXCEPTION(
		{
			int val2 = 0;
			Lua::ReadableInteger<int> ri(val2);
			state >> ri;
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testLongMaxOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	constexpr long long val = std::numeric_limits<long>::max() + 1ll;
	state << Lua::WritableInteger<long long>(val);

	BOOST_CHECK_EXCEPTION(
		{
			long val2 = 0;
			Lua::ReadableInteger<long> ri(val2);
			state >> ri;
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testEngineIntegerOverflowExcep)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	BOOST_CHECK_EXCEPTION(
		{
			constexpr unsigned long long val = (unsigned long long) (
				std::numeric_limits<long long>::max()
			) + 1ll;

			state << Lua::UnsignedWritableInteger<unsigned long long>(val);
		},
		std::overflow_error,
		[] (const std::overflow_error& e) { return true; }
	);
}

BOOST_AUTO_TEST_CASE(testFloatMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	float val = std::numeric_limits<float>::min();
	state << Lua::WritableNumber<float>(val);

	float val2 = 0;
	Lua::ReadableNumber<float> rn(val2);
	state >> rn;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testDoubleMin)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	double val = std::numeric_limits<double>::min();
	state << Lua::WritableNumber<double>(val);

	double val2 = 0;
	Lua::ReadableNumber<double> rn(val2);
	state >> rn;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testFloatMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	float val = std::numeric_limits<float>::max();
	state << Lua::WritableNumber<float>(val);

	float val2 = 0;
	Lua::ReadableNumber<float> rn(val2);
	state >> rn;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_CASE(testDoubleMax)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	double val = std::numeric_limits<double>::max();
	state << Lua::WritableNumber<double>(val);

	double val2 = 0;
	Lua::ReadableNumber<double> rn(val2);
	state >> rn;

	BOOST_TEST(val == val2);
}

BOOST_AUTO_TEST_SUITE_END()