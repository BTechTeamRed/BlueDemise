#pragma once

#include "entt.h"
#include "Components.h"
#include "DeltaTime.h"
#include "Engine/Core.h"

//Added for UI
#include <array>
#include "ExplorerPanel.h"
#include "EntitiesPanel.h"
#include "ComponentsPanel.h"
#include "MainMenu.h"

namespace Engine
{
    class ENGINE_API Entity;

    // Scene keeps a registry of Entities that are then rendered and processed as part of the game loop.
    // Also responsible for loading shaders, initializing OpenGl contexts and windows, and rendering.
    class Scene
    {
    public:

        #pragma region Entity Management
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

        #pragma region OpenGL Scene Management
        bool initializeGL();
        void renderScene(const DeltaTime& dt);
		
        //Update an MVP matrix, with the MVP generated in the function and returned.
        glm::mat4 updateMVP(TransformComponent transform, glm::mat4 view, glm::mat4 projection);

        void runEntityScripts(const DeltaTime& dt);

        bool initializeUI();
        void shutdownUI();

        void renderScene();

        void renderUI();

        //TODO: shader wrapper so switching out between different shaders is easier
        void loadShaders();
        #pragma endregion
		

        //TODO: Replace with serialized objects once that's added in
        void createEntities();

		//Create quad for sprites
        VerticesComponent createSprite();
		
		//Functions to create sprite specific OpenGL buffers. Returns the ID of said buffers.
		GLuint getSpriteVBO();
        GLuint getSpriteVAO();
        GLuint getSpriteIBO();

        //Set the color of the current drawable object. This would need to be run per entity/renderable. Requires the MVP and a vec4 color.
        void setColor(glm::mat4 mvp, glm::vec4 color);
			
        // Registry is a container to hold entities
        entt::registry m_registry;
        struct GLFWwindow* m_window;
        struct GLFWwindow* m_UIwindow;
        int m_windowWidth{1900};
        int m_windowHeight{1000};

        //main menu is the UI element that shows the game
        MainMenu m_mainMenu;

        //explorer panel is the UI element that shows the file explorer
        ExplorerPanel m_explorerPanel;

        //entities panel is the UI element that lists all the entities
        EntitiesPanel m_entitiesPanel;

        DeltaTime m_deltaTime{0};
        //creates an array of three components panels
        std::array<ComponentsPanel, 3> m_componentsPanels
        {
            "Active components",
            "Attributes",
            "Components"
        };

		//GL IDs for various objects. 
        GLuint m_programId;
        GLuint m_spriteVBO;
        GLuint m_spriteVAO;
        GLuint m_spriteIBO;

        bool createdVBO { false };
        bool createdVAO { false };
        bool createdIBO { false };

        friend class Entity;
    };
}
