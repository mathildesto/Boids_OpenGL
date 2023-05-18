#include <iostream>
#include <vector>
#include <unordered_set>
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include <cam/Freefly.h>

#include "tinyobjloader/tiny_obj_loader.h"
#include "glimac/common.hpp"

#include "Aquarium/fish.hpp"


int main()
{
    auto freefly = Camera::Freefly{};
    auto ctx     = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

    glEnable(GL_DEPTH_TEST);

    FishProgram fish{};
    fish.setVAO();


    /* Loop until the user closes the window */
    ctx.update = [&]() {

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        fish.draw(freefly, ctx);
        freefly.handleEvent(ctx);

    };

    ctx.start();

    return 0;
}