/**
 * \file stringtest.cpp
 *
 * \brief Tests a ChunkyString for correctness.
 */

#include "gtest/gtest.h"
#ifndef LOAD_GENERIC_STRING
/* This test code can be used two ways.  It can either test a local
 * ChunkyString implementation, or it can dynamically load an implementation
 * at runtime and test that.  In the first homework, you'll load an
 * implementation that someone else used, but in later assignments, you'll
 * set the value to zero to use your own.
 */
#define LOAD_GENERIC_STRING 0       // 0 = Normal, 1 = Load Code Dynamically
#endif

 #define INSERT_ERASE 0             // 0 = Do not test, 1 = do test

#if LOAD_GENERIC_STRING
#else
#include "chunkystring.hpp"         // Just include and link as normal.
using TestingString = ChunkyString;
#endif

#include <string>
#include <sstream>
#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <cassert>

#include "signal.h"
#include "unistd.h"

using namespace std;

static const size_t CHUNKSIZE = 12;     // Assuming a chunksize of 12

//--------------------------------------------------
//           HELPER FUNCTIONS
//--------------------------------------------------

enum randomness_t  { MIN_VALUE, MAX_VALUE, MID_VALUE, RANDOM_VALUE };

/**
 * Returns a value n, where 0 < n < maximum.  Depending on the choice
 * parameter, n will either be at the midpoint, one of the extreme ends
 * or at a random position.
 */
static int maybeRandomInt(int maximum, randomness_t choice)
{
    switch (choice) {
    case MIN_VALUE:
        return 0;

    case MAX_VALUE:
        return maximum;

    case MID_VALUE:
        return (maximum + 1) / 2;

    case RANDOM_VALUE:
    default:
        // We take rather unnecessary care here to avoid skewing the
        // distribution.  Only really necessary if maximum is very
        // large.
        ++maximum;
        long divisor = RAND_MAX / maximum;
        int candidate;

        do {
            candidate = random() / divisor;
        } while (candidate >= maximum);

        return candidate;
    }
}

/// Returns a random character, using the maybeRandomInt function.
static char randomChar()
{
    return maybeRandomInt(127, RANDOM_VALUE);
}

/**
 * \brief Converts any type that operator << can write out into a string.
 *
 * \param thing         thing to convert
 * \returns             string representation of thing
 * \throws std::invalid_argument  if conversion fails
 */
template <typename T>
std::string stringFrom (const T& thing)
{
    std::stringstream stream;
    stream << thing;

    if (!stream.fail())
        return stream.str();
    else
        throw std::invalid_argument("Conversion to string failed");
}


/**
 * \brief Make sure that equivalent TestingStrings are not actually using
 *        the same data (i.e. not a shallow copy)
 *
 * \param test          TestingString that will be modified in this test
 * \param copy          TestingString that will remain constant in this test
 * \param origin        Description of the calling function, to assist the
 *                      user in tracing back an error.
 *
 * \remark  The first input will be modified during the test, but is returned
 *          to its original value before the test returns.
 */
void checkDeepCopy(TestingString& test, const TestingString& copy,
                   string origin)
{
    // Don't deal with empty strings
    if (test.size() == 0) return;

    string backtrace = "Backtrace: " + origin;
    // Select random character to change
    size_t pos = maybeRandomInt(test.size() - 1, RANDOM_VALUE);
    TestingString::iterator tIter = test.begin();
    std::advance(tIter, pos);
    TestingString::iterator cIter = copy.begin();
    std::advance(cIter, pos);

    // Select a random character that is not the same as the existing one
    char c;

    do {
        c = randomChar();
    } while (c == *tIter);

    // Modify test string and see that copy is unchanged
    EXPECT_TRUE(test == copy) << backtrace;
    EXPECT_EQ(*cIter, *tIter) << backtrace;
    *tIter = c;
    EXPECT_NE(c, *cIter) << backtrace;
    EXPECT_NE(*cIter, *tIter) << backtrace;
    EXPECT_TRUE(test != copy) << backtrace;

    // Return them to equality
    *tIter = *cIter;
    EXPECT_TRUE(test == copy) << backtrace;
}


/**
 * \brief Compare the size of TestingString to the size of an expected value.
 *
 * \param test          TestingString to check
 * \param copy          Expected value of the TestingString
 * \param backtrace     Assists the user in tracing back an error.
 */
void checkSizeWithControl(const TestingString& test, const string& control,
                          string backtrace)
{
    EXPECT_EQ(control.size(), test.size()) << backtrace;
    ASSERT_EQ(control.size(), stringFrom(control).size()) << backtrace;
}

/**
 * \brief Compare a TestingString to an expected value.
 *
 * \param test          TestingString to check
 * \param copy          Expected value of the TestingString
 * \param backtrace     Assists the user in tracing back an error.
 */
void checkEqWithControl(const TestingString& test, const string& control,
                          string backtrace)
{
    ASSERT_EQ(stringFrom(control), stringFrom(test)) << backtrace;
    ASSERT_EQ(control, stringFrom(test)) << backtrace;
}

/**
 * \brief Compare the TestingString to an expected value.
 *
 * \details Tests size, operator<<, begin, end, forward iteration, and
 *          edge case comparisons.
 *
 * \param test          TestingString to check
 * \param copy          Expected value of the TestingString
 * \param origin        Description of the calling function, to assist the
 *                      user in tracing back an error.
 */
void checkWithControl(const TestingString& test, const string& control,
                      string origin)
{
    string backtrace = "Backtrace: " + origin;
    // Make sure controls and tests match
    checkSizeWithControl(test, control, backtrace);
    checkEqWithControl(test, control, backtrace);

    TestingString::const_iterator tIter = test.begin();
    string::const_iterator cIter = control.begin();

    // Walk forward
    for ( ; cIter != control.end(); ++cIter, ++tIter) {
        ASSERT_EQ(*cIter, *tIter) << backtrace;
    }

    EXPECT_TRUE(tIter == test.end()) << backtrace;

    // Walk backward
    for ( ; cIter != control.begin(); ) {
        --cIter;
        --tIter;
        ASSERT_EQ(*cIter, *tIter) << backtrace;
    }

    EXPECT_TRUE(tIter == test.begin()) << backtrace;

    // Test edge case characters
    TestingString chunkyChr0;
    string      checkChr0;
    chunkyChr0.push_back(0); // Null character
    checkChr0.push_back(0);

    EXPECT_EQ((control     == checkChr0), (test     == chunkyChr0)  )
            << backtrace;
    EXPECT_EQ((checkChr0 == control), (chunkyChr0 == test)      )
            << backtrace;
    EXPECT_EQ((control     <  checkChr0), (test     <  chunkyChr0)  )
            << backtrace;
    EXPECT_EQ((checkChr0 <  control), (chunkyChr0 <  test)      )
            << backtrace;

    TestingString chunkyChr127;
    string      checkChr127;
    chunkyChr127.push_back(127); // Delete character
    checkChr127.push_back(127);

    EXPECT_EQ((control       == checkChr127), (test       == chunkyChr127)  )
            << backtrace;
    EXPECT_EQ((checkChr127 == control), (chunkyChr127 == test)        )
            << backtrace;
    EXPECT_EQ((control       <  checkChr127), (test       <  chunkyChr127)  )
            << backtrace;
    EXPECT_EQ((checkChr127 <  control), (chunkyChr127 <  test)        )
            << backtrace;
}


/**
 * \brief Compare two TestingStrings to an expected values and each other.
 *
 * \details Tests operator==, operator!=, operator<. and all functions
 *          tested by checkWithControl(test, control).
 *
 * \param first         TestingString to check
 * \param second        TestingString to check
 * \param fControl      Expected value of 'first'
 * \param sControl      Expected value of 'second'
 * \param origin        Description of the calling function, to assist the
 *                      user in tracing back an error.
 */
void checkTwoWithControl(
            const TestingString& first, const TestingString& second,
            const string& fControl,     const string& sControl,
            string origin)
{
    string backtrace = "Backtrace: " + origin;

    // Check validity of each TestingString
    checkWithControl(first, fControl, "first : " + origin);
    checkWithControl(second, sControl, "second : " + origin);

    // Test equality and inequality
    EXPECT_EQ(fControl == sControl, first == second) << backtrace;
    EXPECT_EQ(sControl == fControl, second == first) << backtrace;
    EXPECT_EQ(fControl != sControl, first != second) << backtrace;
    EXPECT_EQ(sControl != fControl, second != first) << backtrace;
    EXPECT_EQ(fControl < sControl, first < second) << backtrace;
    EXPECT_EQ(sControl < fControl, second < first) << backtrace;
}

/// Compares two TestingStrings by redirecting to
/// checkWithControl(const TestingString&, const TestingString&,
///             const string&, const string&, string).
void checkBothIdentical(const TestingString& first, const TestingString& second,
                        string origin)
{
    checkTwoWithControl(first, second,
                        stringFrom(first), stringFrom(second), origin);
}

/**
 * \brief Compare a TestingString and a TestingString iterator with expected
 *        values.
 *
 * \details Tests iterator correctness and all functions tested by
 *          checkWithControl(test, control).
 *
 * \param test          TestingString to check
 * \param control       Expected value of test
 * \param tIter         TestingString iterator to check
 * \param cIter         Expected value of tIter
 * \param origin        Description of the calling function, to assist the
 *                      user in tracing back an error.
 */
void checkIterWithControl(
            const TestingString& test,            const string& control,
            const TestingString::iterator& tIter, const string::iterator& cIter,
            string origin)
{
    string backtrace = "Backtrace: " + origin;
    EXPECT_EQ(cIter == control.begin(), tIter == test.begin()) << backtrace;
    EXPECT_EQ(cIter == control.end(), tIter == test.end()) << backtrace;

    if (tIter != test.end())
        EXPECT_EQ(*cIter, *tIter) << backtrace;

    // Check this AFTER iterator checks, so iterators are not invalidated
    checkWithControl(test, control, origin);
}
/**
 * \brief Compare a TestingString and a TestingString iterator with expected
 *        values.
 *
 * \details Tests iterator inequality 
 *
 * \param test          TestingString to check
 * \param control       Expected value of test
 * \param tIter         TestingString iterator to check
 * \param cIter         Expected value of tIter
 * \param origin        Description of the calling function, to assist the
 *                      user in tracing back an error.
 */
void checkIterInequalityWithControl(
            const TestingString& test,            const string& control,
            const TestingString::iterator& tIter, const string::iterator& cIter,
            string origin)
{
    string backtrace = "Backtrace: " + origin;
    EXPECT_NE(cIter == control.begin(), tIter == test.begin()) << backtrace;
    EXPECT_NE(cIter == control.end(), tIter == test.end()) << backtrace;

    if (tIter != test.end())
        EXPECT_NE(*cIter, *tIter) << backtrace;

}

/**
 * \brief Assuming chunks are supposed to be at least 1/divisor full, checks
 *        for the lowest allowable utilization for the input string
 *
 * \remarks For insertion only, we assume chunks are at least 1/2 full.
 *          For erase, chunks can go down to 1/4 full.
 *
 * \param test          TestingString to check
 * \param divisor       Fullness of chunk = 1/divisor
 * \param origin        String to describe the caller of this function to
 *                      aid in human debugging.
 */
void checkUtilization(const TestingString& test, size_t divisor, string origin)
{
    string backtrace = "Backtrace: " + origin;

    if (test.size() == 0)
        return;

    size_t chunks = 1;
    size_t size = test.size() - 1;
    divisor = CHUNKSIZE / divisor;
    chunks += (size + divisor - 1) / divisor;
    EXPECT_GT(test.utilization(), double(size) / double(chunks * CHUNKSIZE))
            << origin;
}

//--------------------------------------------------
//           TEST FUNCTIONS
//--------------------------------------------------

/// Test some basic functions
TEST(initial, basics)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    checkEqWithControl(test, control, "pushing back one character");
}

TEST(size, one_element)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    checkSizeWithControl(test, control, "size after pushing back one char");
}

TEST(size, empty)
{
    TestingString test;
    string control;

    checkSizeWithControl(test, control, "size of empty string");
}

TEST(equality, one_element)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    checkWithControl(test, control, "check to see two strings are equal");
}

TEST(equality, empty)
{
    TestingString test;
    string control;

    checkWithControl(test, control, "check that 2 empty strings are equal");
}

TEST(equality, negate)
{
    TestingString test;
    string control;

    test.push_back('A');

    EXPECT_FALSE(stringFrom(test) == stringFrom(control)) 
    << "testing for inequality using ==" << endl;
}

TEST(inequality, one_element)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');
    control.push_back('B');

    EXPECT_NE(stringFrom(test), stringFrom(control)) 
    << "check to see two strings are unequal" << endl;
}

TEST(inequality, empty)
{
    TestingString test;
    string control;

    control.push_back('A');

    EXPECT_NE(stringFrom(test), stringFrom(control)) 
    << "check that the empty string is different from one_element" << endl;
}

TEST(inequality, negate)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    EXPECT_FALSE(stringFrom(test) != stringFrom(control)) 
    << "testing for inequality using ==" << endl;
}

TEST(less_than, one_element)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('B');

    EXPECT_LT(stringFrom(test), stringFrom(control)) 
    << "check that A < B" << endl;
}

TEST(less_than, empty)
{
    TestingString test;
    string control;

    control.push_back('A');

    EXPECT_LT(stringFrom(test), stringFrom(control)) 
    << "check that empty < A " << endl;
}

TEST(less_than, same)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    EXPECT_FALSE(stringFrom(test) < stringFrom(control)) 
    << "check that A !< A" << endl;
}

TEST(utilization, hello)
{
    TestingString test;

    test.push_back('H');
    test.push_back('e');
    test.push_back('l');
    test.push_back('l');
    test.push_back('o');    

    checkUtilization(test, 2, "testing utilization using push_back");
}

#if INSERT_ERASE
TEST(insert, one_char_at_start)
{
    TestingString test;
    string control;

    test.push_back('1');
    test.push_back('2');
    control.push_back('0');
    control.push_back('1');
    control.push_back('2');

    test.insert(test.begin(), '0');

    checkWithControl(test, control, "check insert one char before begin");

}

TEST(insert, one_char_at_end)
{
    TestingString test;
    string control;

    test.push_back('0');
    test.push_back('1');
    control.push_back('0');
    control.push_back('1');
    control.push_back('2');

    test.insert(test.end(), '2');

    checkWithControl(test, control, "check insert one char at end");

}

TEST(insert, empty)
{
    TestingString test;
    string control;

    control.push_back('0');

    test.insert(test.end(), '0');

    checkWithControl(test, control, "check insert to empty");

}

TEST(erase, push_one_erase_one)
{
    TestingString test;
    string control;

    test.push_back('0');

    test.erase(test.begin());

    checkWithControl(test, control, "check erase from one element is empty");

}

TEST(erase, push_two_erase_one)
{
    TestingString test;
    string control;

    test.push_back('0');
    test.push_back('1');
    control.push_back('1');

    test.erase(test.begin());

    checkWithControl(test, control, "check erase begin");

}

TEST(erase, push_two_erase_two)
{
    TestingString test;
    string control;

    test.push_back('0');
    test.push_back('1');

    test.erase(test.erase(test.begin())); //recursive to use returned iterator

    checkWithControl(test, control, "check erase all");

}
#endif

TEST(operator_plus, empty)
{
    TestingString test1;
    TestingString test2;
    string control;

    test1 += test2;

    checkWithControl(test1, control, "check that sum of empties is empty");
}

TEST(operator_plus, one_char_to_empty)
{
    TestingString test1;
    TestingString test2;
    string control;

    test1.push_back('A');
    control.push_back('A');
    test1 += test2;

    checkWithControl(test1, control, 
        "check that adding empty does not change string");
}

TEST(operator_plus, empty_to_one_char)
{
    TestingString test1;
    TestingString test2;
    string control;

    test2.push_back('A');
    control.push_back('A');
    test1 += test2;

    checkWithControl(test1, control, "check that adding to empty works");
}

TEST(operator_plus, doubling)
{
    TestingString test1;
    string control;

    test1.push_back('A');
    control.push_back('A');
    control.push_back('A');
    test1 += test1;

    checkWithControl(test1, control, 
        "check that operator_plus doubling works");
}

TEST(operator_plus, big_doubling)
{
    TestingString test1;
    string control;

    for (size_t i = 0; i < 200; ++i)
    {
        test1.push_back('A');
    }

    for (size_t i = 0; i < 400; ++i)
    {
        control.push_back('A');
    }

    test1 += test1;

    checkWithControl(test1, control, 
        "check that operator_plus big_doubling works");
}

TEST(iterator, one_element_equality)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    TestingString::iterator a = test.begin();
    string::iterator b = control.begin();

    checkIterWithControl(test, control, a, b, 
        "checking one_element_equality iterator");
}

TEST(iterator, end_minus_one_equality)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    TestingString::iterator a = test.end();
    string::iterator b = control.begin();
    --a;

    checkIterWithControl(test, control, a, b, 
        "checking end_minus_one_equality iterator");
}

TEST(iterator, begin_plus_one_equality)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    TestingString::iterator a = test.begin();
    string::iterator b = control.end();
    ++a;

    checkIterWithControl(test, control, a, b, 
        "checking begin_plus_one_equality iterator");
}


TEST(iterator, begin_plus_one_inequality)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('B');

    TestingString::iterator a = test.begin();
    string::iterator b = control.end();
    
    checkIterInequalityWithControl(test, control, a, b, 
        "checking begin_plus_one_inequality iterator");
}

TEST(iterator, dereferencing)
{
    TestingString test;
    string control;

    test.push_back('A');
    control.push_back('A');

    TestingString::iterator a = test.begin();
    string::iterator b = control.begin();
    
    EXPECT_EQ(*a, *b);
}

#if INSERT_ERASE
TEST(utilization, only_insert)
{
    TestingString test;

    // randomly insert 1000 random characters into test
    for (size_t i = 0; i < 1000; ++i)
    {
        int index = maybeRandomInt(test.size(), RANDOM_VALUE);
        TestingString::iterator a = test.begin();

        for (int i = 1; i < index; ++i)
        {
            ++a;
        }

        test.insert(a, randomChar());
    }

    checkUtilization(test, 2, "checking utilization only_insert");
}

TEST(utilization, insert_erase_half_random)
{
    TestingString test;

    // randomly inserts 1000 random characters into test
    for (size_t i = 0; i < 1000; ++i)
    {
        int index = maybeRandomInt(test.size(), RANDOM_VALUE);
        TestingString::iterator a = test.begin();

        for (int i = 1; i < index; ++i)
        {
            ++a;
        }

        test.insert(a, randomChar());
    }

    // randomly delete 500 characters from test
    for (size_t i = 0; i < 500; ++i)
    {
        int index = maybeRandomInt(test.size(), RANDOM_VALUE);
        TestingString::iterator a = test.begin();

        for (int i = 1; i < index; ++i)
        {
            ++a;
        }

        a = test.erase(a);
    }

    checkUtilization(test, 4, 
        "checking utilization insert_erase_half_random");
}

TEST(utilization, insert_half_erase_more_than_quarter)
{
    TestingString test;

    // Insert 6 half-chunks worth of elements
    for (size_t i = 0; i < CHUNKSIZE*3; ++i) 
    {
        int index = maybeRandomInt(test.size(), RANDOM_VALUE);
        TestingString::iterator a = test.begin();

        for (int i = 1; i < index; ++i)
        {
            ++a;
        }

        test.insert(a, randomChar());
    }

    for (int i = 5; i >= 0; --i) // Iterates across 6 half-chunks, ideally
    {
        TestingString::iterator a = test.begin();

        for (size_t j = 1; j < i*CHUNKSIZE/2; ++j) 
        {
            // Puts iterator into starting position of nth chunk, 1 <= n <= 6
            ++a; 
        }
        
        for (size_t k = 0; k < CHUNKSIZE/6; ++k) // Keep 1/6 of elements
        {
            ++a;
        }
        // Erase other stuff
        for (size_t l = 0; l < CHUNKSIZE/2-CHUNKSIZE/6; ++l) 
        {
            a = test.erase(a); 
        }
    }

    checkUtilization(test, 4, 
        "checking utilization insert_half_erase_more_than_quarter");
}

TEST(utilization, insert_250_erase_200)
{
    TestingString test;
    for(size_t i = 0; i < 50; i++)
    {
        // push_back 3 characters
        for(size_t k = 0; k < 3; k++)
        {
            test.push_back('A');
        }
        // erase 1 element from end
        TestingString::iterator a = test.end();
        --a;
        test.erase(a);

        // erase 1 element from beginning
        TestingString::iterator b = test.begin();
        test.erase(b);

        // push_back 1 element
        test.push_back('A');

        // erase 1 element from beginning
        b = test.begin();
        test.erase(b);

        // push_back 1 element
        test.push_back('A');

        // erase 1 element from beginning
        b = test.begin();
        test.erase(b);

    }
    checkUtilization(test, 4, "checking utilization insert_250_erase_200");
}
#endif


//--------------------------------------------------
//           RUNNING THE TESTS
//--------------------------------------------------

// Called if the test runs too long.
static void timeout_handler(int)
{
    // We go super-low-level here, because we can't trust anything in
    // the C/C++ library to really be working right.
    write(STDERR_FILENO, "Timeout occurred!\n", 18);
    abort();
}

/// Run tests
int main(int argc, char** argv)
{
    // Initalize testing environment
    ::testing::InitGoogleTest(&argc, argv);

#if LOAD_GENERIC_STRING
    // Load the desired string implementation, if that's what we're doing.

    if (argc != 2) {
        cerr << "Usage ./stringtest [Google Test options] plugin" << endl;
        exit(0);
    }

    GenericString::loadImplementation(argv[1]);
#else
    // No setup to do if we're using ChunkyString directly
#endif
    signal(SIGALRM, timeout_handler);   // Call this when the timer expires
    alarm(10);                          // set the timer at 10 seconds
    return RUN_ALL_TESTS();
}

/* (Junk to make emacs use Stroustrup-style indentation)
 * Local Variables:
 * c-file-style: "stroustrup"
 * End:
 */
