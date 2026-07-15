#pragma once
#include <shader/shader.h>

namespace assimp_wrapper {
    class Material final {



    public:
        Material();
        void bind(Shader &shader);
    };
}

