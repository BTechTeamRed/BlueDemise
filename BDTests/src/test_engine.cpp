#include <iostream>
#include "gtest/gtest.h"
#include <thread>
#include <chrono>
#include <Engine/SceneBuilder/Scene.h>
#include "Engine/Utilities/DeltaTime.h"
#include "Engine/SceneBuilder/Entity.h"
#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/SceneBuilder/Components.h"
#include "Engine/ResourceManagement/Serializer.h"

namespace BDTests
{
	class SerializerTest : public ::testing::Test {
	protected:
		void SetUp() override
		{
			// GTEST_SKIP() << "Attempting to deserialize gives me this odd \"unknown file SEH exception\" error that I have no clue how to fix";
		}

		// void TearDown() override {}
		Engine::Scene scene;
	};

	TEST_F(SerializerTest, emptyScene)
	{
		std::string serializedJson = Engine::Serializer::serializeScene(&scene, "emptyScene");
		std::string output = R"({"scene":{"entities":null,"name":""}})";
		EXPECT_EQ(serializedJson, output);
	}

	TEST_F(SerializerTest, oneEntityScene)
	{
		Engine::Entity myEntity = scene.createEntity("myEntity");
		std::string serializedJson = Engine::Serializer::serializeScene(&scene, "sceneOneComponent");
		std::string output = "{\"scene\":{\"entities\":[{\"components\":[],\"tag\":\"myEntity\"}],\"name\":\"\"}}";
		EXPECT_EQ(serializedJson, output);
	}

	TEST_F(SerializerTest, oneEntityWithComponentScene)
	{
		Engine::Entity myEntity = scene.createEntity("myEntity");
		myEntity.addComponent<Engine::TransformComponent>();
		std::string serializedJson = Engine::Serializer::serializeScene(&scene, "sceneOneComponent");
		std::string output = "{\"scene\":{\"entities\":[{\"components\":[{\"name\":\"TransformComponent\",\"position\":{\"x\":0.0,\"y\":0.0,\"z\":0.0},\"rotation\":{\"x\":0.0,\"y\":0.0,\"z\":0.0},\"scale\":{\"x\":0.0,\"y\":0.0,\"z\":0.0}}],\"tag\":\"myEntity\"}],\"name\":\"\"}}";
		EXPECT_EQ(serializedJson, output);
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

	TEST(DeltaTimeTest, deltaTimeCtor)
	{
		Engine::DeltaTime dt;
		EXPECT_EQ(dt.getMilliseconds(), 0);
	}

	int main(int argc, char** argv) {
		::testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
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