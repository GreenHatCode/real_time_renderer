#pragma once
#include <shader/shader.h>
#include <cstdint>

namespace assimp_wrapper {

    struct texture 
    {
        uint32_t id;
        std::string type;
    };

    class Material final {



    public:
        Material();
        void bind(Shader &shader);
    };
}

