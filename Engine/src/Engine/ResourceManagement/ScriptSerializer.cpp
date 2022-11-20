#include <string>
#include "ScriptSerializer.h"
#include "Engine/Scripts/ColorSwap.h"
#include "Engine/Scripts/MoveRightScript.h"
#include "Engine/Scripts/PlayerMovementScript.h"
#include "Engine/Scripts/Score.h"
#include "Engine/Scripts/Timer.h"


namespace Engine 
{
    void ScriptSerializer::linkAndDeserializeScript(Entity& entity, std::string scriptName)
    {
        if (scriptName == "ColorSwap")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<ColorSwap>();
        }
        if (scriptName == "MoveRightScript")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<MoveRightScript>();
        }
        if (scriptName == "PlayerMovementScript")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<PlayerMovementScript>();
        if (scriptName == "Score")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<Score>();
        }
        if (scriptName == "Timer")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<Timer>();
        }
   
    }
}