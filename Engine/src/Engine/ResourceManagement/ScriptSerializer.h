#include "Engine/Core.h"
#include <string>

#include "Engine/SceneBuilder/Entity.h"

class Entity;

namespace Engine
{
    class ENGINE_API ScriptSerializer
    {
    public:
        static void linkAndDeserializeScript(Entity& entity, std::string scriptName);
    };
}