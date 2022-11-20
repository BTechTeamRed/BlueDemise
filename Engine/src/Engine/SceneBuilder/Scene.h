#pragma once

#include "entt.h"
#include "Engine/Core.h"
#include <Engine/Utilities/DeltaTime.h>
#include <string>

#include "Engine/Physics/OctTree.h"
namespace Engine
{
    class Entity;

    // Scene keeps a registry of Entities that are then rendered and processed as part of the game loop.
    class ENGINE_API Scene
    {
    public:
        bool m_closeScene = false;
        
        #pragma region Entity Management
        // Registry is a container to hold entities
        //Made public to allow for GLFW callbacks to access entities
        entt::registry m_registry;

        // Function to create an entity and add it to the scene
        // Takes in a tag as a string for input
        Entity createEntity(std::string tag);

        // Gets a view of entities with the defined components.
        template<typename... Components>
        auto getEntities()
        {
            return m_registry.view<Components...>();
        }
        #pragma endregion
        
        #pragma region Runtime Functions
        // Executes actions at the start of runtime
        void onRuntimeStart();

        // Executes actions when runtime stops
        void onRuntimeStop();

        // Executes actions every time runtime is updated (every frame).
        void onRuntimeUpdate(const DeltaTime& dt);
        #pragma endregion
	
    private:

        std::string m_name;
        DeltaTime m_deltaTime{ 0 };
        OctTree* m_physics;
        
        void runEntityScripts(const DeltaTime& dt);

        void renderScene();
        
        friend class Entity;
        friend class Serializer;
        
    };
}
