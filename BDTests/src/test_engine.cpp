#include <iostream>
#include "gtest/gtest.h"

#include "Engine/Components.h"

namespace BDTests
{
	// IndependentMethod is a test case - here, we have 2 tests for this 1 test case
	TEST(IndependentMethod, is1equalTo0)
	{
		EXPECT_EQ(1, 0);
		EXPECT_EQ(1, 1);
	}

	TEST(IndependentMethod, is0equalTo0) 
	{
		EXPECT_EQ(0, 0);
	}

	// The fixture for testing class EngineTest. From google test primer.
	class EngineTest : public ::testing::Test
	{
	protected:
		// You can remove any or all of the following functions if its body
		// is empty.

		EngineTest()
		{
			// You can do set-up work for each test here.
		}

		virtual ~EngineTest()
		{
			// You can do clean-up work that doesn't throw exceptions here.
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:
		virtual void SetUp()
		{
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		virtual void TearDown()
		{
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Objects declared here can be used by all tests in the test case for EngineTest.
		//Component c;  //We don't have a component class so this is just an example
	};


	// Test case must be called the class above
	//Also note: use TEST_F instead of TEST to access the test fixture (from google test primer)
	TEST_F(EngineTest, MethodBarDoesAbc)
	{
		//method stuff idk
		EXPECT_EQ(1, 1);
	}
}