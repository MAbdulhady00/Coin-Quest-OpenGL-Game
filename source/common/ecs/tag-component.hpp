#pragma once

#include <json/json.hpp>
#include "component.hpp"

namespace our
{
    // A tag component is a component that does not have any data associated with it.
    // As such it has no deserialize method.
    // It is used to mark an entity as having a certain property.
    // For example, an obstacle entity would have a ObstacleTagComponent to mark it as a coin.
    class TagComponent : public Component
    {
    public:
        virtual void deserialize(const nlohmann::json &data)
        {
            // No data to deserialize
        }
    };

}