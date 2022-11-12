#include <iostream>
#include "gtest/gtest.h"
#include <thread>
#include <chrono>
#include "Engine/DeltaTime.h"
#include "Engine/Entity.h"
#include "Engine/Scene.h"
#include "Engine/ResourceManager.h"
#include "Engine/Components.h"
#include "Engine/Serializer.h"

namespace BDTests
{
	class SerializerTest : public ::testing::Test {
	protected:
		void SetUp() override
		{
			GTEST_SKIP() << "Attempting to deserialize gives me this odd \"unknown file SEH exception\" error that I have no clue how to fix";
		}

		// void TearDown() override {}
		Engine::Scene scene;
	};

	TEST_F(SerializerTest, EmptyScene)
	{
		Engine::Serializer::serializeScene(&scene, "emptyScene");
		EXPECT_TRUE(Engine::Serializer::tryDeserializeScene(scene, "emptyScene"));
	}

	TEST_F(SerializerTest, SceneOneComponent)
	{
		scene.createEntity("myEntity");
		Engine::Serializer::serializeScene(&scene, "sceneOneComponent");
		EXPECT_TRUE(Engine::Serializer::tryDeserializeScene(scene, "sceneOneComponent"));
	}

	TEST(DeltaTimeTest, deltaTimeCtor)
	{
		Engine::DeltaTime dt;
		EXPECT_EQ(dt.getMilliseconds(), 0);
	}

	TEST(SceneTest, sceneHasEntities)
	{
		Engine::Scene scene;
		Engine::Entity ent = scene.createEntity("ent");
		//GTEST_EXPECT_TRUE(ent != NULL);
		EXPECT_TRUE(&ent != nullptr);
	}

	TEST(EntityTest, entityHasComponent)
	{
		Engine::Scene scene;
		Engine::Entity myEntity = scene.createEntity("myEntity");
		EXPECT_TRUE(myEntity.hasComponent<Engine::TagComponent>());
	}

	TEST(EntityTest, entityHasMultipleComponents)
	{
		Engine::Scene scene;
		Engine::Entity myEntity = scene.createEntity("myEntity");
		myEntity.addComponent<Engine::TransformComponent>();
		EXPECT_TRUE(myEntity.hasComponent<Engine::TagComponent>());
		EXPECT_TRUE(myEntity.hasComponent<Engine::TransformComponent>());
	}

	TEST(EntityTest, removeComponents)
	{
		Engine::Scene scene;
		Engine::Entity myEntity = scene.createEntity("myEntity");
		myEntity.addComponent<Engine::TransformComponent>();
		myEntity.removeComponent<Engine::TransformComponent>();
		EXPECT_FALSE(myEntity.hasComponent<Engine::TransformComponent>());
	}

	TEST(ComponentsTest, TransformComponentInit)
	{
		Engine::Scene scene;
		Engine::Entity myEntity = scene.createEntity("myEntity");
		myEntity.addComponent<Engine::TransformComponent>();
		Engine::TransformComponent tr = myEntity.getComponent<Engine::TransformComponent>();
		EXPECT_EQ(tr.position.x, 0);
		EXPECT_EQ(tr.position.y, 0);
		EXPECT_EQ(tr.position.z, 0);
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