#include <iostream>
#include "gtest/gtest.h"
#include <thread>
#include <chrono>
#include "Engine/DeltaTime.h"
#include "Engine/Entity.h"
#include "Engine/Scene.h"
#include "Engine/ResourceManager.h"

namespace BDTests
{
	// IndependentMethod is a test case - here, we have 3 tests for this 1 test case
	TEST(DeltaTimeTest, timeIsWorking)
	{
		Engine::DeltaTime dt;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_GT(dt.getMilliseconds(), 1);
	}

	TEST(SceneTest, sceneHasEntities)
	{
		Engine::Scene scene;
		Engine::Entity ent = scene.createEntity("ent");
		//GTEST_EXPECT_TRUE(ent != NULL);
		EXPECT_TRUE(&ent != nullptr);
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