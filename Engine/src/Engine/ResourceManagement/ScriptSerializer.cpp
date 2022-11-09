#include <string>
#include "ScriptSerializer.h"
//#include "MoveRightScript.h"


namespace Engine 
{
    void ScriptSerializer::linkAndDeserializeScript(Entity& entity, std::string scriptName)
    {
        if (scriptName == "MoveRightScript")
        {
            //entity.addComponent<ScriptComponent>().linkBehavior<Game::MoveRightScript>();
        }
   
    }
}