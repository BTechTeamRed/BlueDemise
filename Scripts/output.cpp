#include <string>
#include kevinScript.h
#include sampleScript.h
#include testScript.h


namespace Engine 
{
    void ScriptSerializer::linkAndDeserializeScript(Entity& entity, std::string scriptName)
    {
        if (scriptName == "kevinScript")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<kevinScript>();
        }
        if (scriptName == "sampleScript")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<sampleScript>();
        }
        if (scriptName == "testScript")
        {
            entity.addComponent<ScriptComponent>().linkBehavior<testScript>();
        }
   
    }
}