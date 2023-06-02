#include <iostream>
#include <vector>
#include <unordered_set>
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include <camera/Freefly.h>

#include "tinyobjloader/tiny_obj_loader.h"
#include "glimac/common.hpp"

#include "scene/fish.hpp"
#include "scene/shark.hpp"
#include "scene/plant.hpp"

#include "boids3D/FishBoidProgram.hpp"



int main()
{
    auto freefly = Camera::Freefly{};
    auto ctx     = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

    glEnable(GL_DEPTH_TEST);

    FishProgram fish{}; 
    fish.setVAO();

    SharkProgram shark{}; 
    shark.setVAO();

    PlantProgram plant{};
    plant.setVAO();


    /* Loop until the user closes the window */
    ctx.update = [&]() {

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        freefly.handleEvent(ctx);
        fish.draw(freefly, ctx);
        // shark.draw(freefly, ctx);
        // plant.draw(freefly, ctx);

    };

    ctx.start();

    return 0;
}