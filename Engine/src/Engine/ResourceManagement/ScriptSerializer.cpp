#include <string>
#include "ScriptSerializer.h"
#include "Engine/Scripts/MoveRightScript.h"


namespace Engine 
{
    void ScriptSerializer::linkAndDeserializeScript(Entity& entity, std::string scriptName)
    {
        if (scriptName == "MoveRightScript")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<MoveRightScript>();
        }
   
    }
}