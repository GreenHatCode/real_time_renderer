#pragma once

namespace scene
{
    class ISceneObject {
    public:
        virtual std::string get_name() const = 0;
        virtual ~ISceneObject() = default;
    };
    
} // namespace scene
