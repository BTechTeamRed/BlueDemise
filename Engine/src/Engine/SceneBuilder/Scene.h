#pragma once

#include "entt.h"
#include "Components.h"
#include <Engine/Utilities/DeltaTime.h>
#include "Engine/Core.h"
#include <Engine/ResourceManagement/ShaderNorms.h>
#include <string>

//Added for UI
#include <array>
#include "Engine/EngineUI/ExplorerPanel.h"
#include "Engine/EngineUI/EntitiesPanel.h"
#include "Engine/EngineUI/ComponentsPanel.h"
#include "Engine/EngineUI/MainMenu.h"

namespace Engine
{
    class Entity;
    class ShaderNorms;

    // Scene keeps a registry of Entities that are then rendered and processed as part of the game loop.
    // Also responsible for loading shaders, initializing OpenGl contexts and windows, and rendering.
    class ENGINE_API Scene
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

		//Create quad for sprites
        VerticesComponent createSprite();
        VerticesComponent createRectangle();
        void setupVertexAttribPtr(VerticesComponent& vc);
		
        int m_quadTexCoordinates = 5;
		int m_quadCoordinates = 3;
        int m_quadIndices = 6;

		//Functions to create specific OpenGL buffers. Returns the ID of said buffers.
        enum RenderableType
        {
            RT_Sprite,
            RT_Rectangle
        };
		GLuint getVBO(RenderableType = RT_Sprite);
        GLuint getVAO();
        GLuint getIBO(RenderableType = RT_Rectangle);

        //Set the color of the current drawable object. This would need to be run per entity/renderable. Requires the MVP and a vec4 color.
        void setColor(glm::mat4 mvp, glm::vec4 color);

        std::string m_name;
        
        struct GLFWwindow* m_window;
        struct GLFWwindow* m_UIwindow;
        int m_windowWidth{1920};
        int m_windowHeight{1080};

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

        #pragma region Shader Management
		//GL IDs for various objects. 
        GLuint m_programId;
        //Shader manager
        ShaderNorms* shaderNorms{ nullptr };
        #pragma endregion

        friend class Entity;
        friend class Serializer;

    public:
        // Registry is a container to hold entities
        //Made public to allow for GLFW callbacks to access entities
        entt::registry m_registry;
    };

    void windowResizeCallback(GLFWwindow* window, int width, int height);
}
