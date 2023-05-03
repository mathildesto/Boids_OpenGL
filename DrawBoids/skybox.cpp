// #include "p6/p6.h"
// #include <vcruntime.h>
// #include <cstddef> // For offsetof()
// #include <glm/gtc/random.hpp>
// #include "Cam/Camera.h"
// // #include "Cam/Freefly.h"
// // #include "Cam/Trackball.h"
// #include "glimac/common.hpp"
// #include "glimac/sphere_vertices.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
// #include "glm/ext/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"
// #include "boids3D/boid3D.hpp"
// #include "stb/stb_image.h"


// const unsigned int width = 800;
// const unsigned int height = 800;

// float skyboxVertices[] =
// {
// 	//   Coordinates
// 	-1.0f, -1.0f,  1.0f,//        7--------6
// 	 1.0f, -1.0f,  1.0f,//       /|       /|
// 	 1.0f, -1.0f, -1.0f,//      4--------5 |
// 	-1.0f, -1.0f, -1.0f,//      | |      | |
// 	-1.0f,  1.0f,  1.0f,//      | 3------|-2
// 	 1.0f,  1.0f,  1.0f,//      |/       |/
// 	 1.0f,  1.0f, -1.0f,//      0--------1
// 	-1.0f,  1.0f, -1.0f
// };

// unsigned int skyboxIndices[] =
// {
// 	// Right
// 	1, 2, 6,
// 	6, 5, 1,
// 	// Left
// 	0, 4, 7,
// 	7, 3, 0,
// 	// Top
// 	4, 5, 6,
// 	6, 7, 4,
// 	// Bottom
// 	0, 3, 2,
// 	2, 1, 0,
// 	// Back
// 	0, 1, 5,
// 	5, 4, 0,
// 	// Front
// 	3, 7, 6,
// 	6, 2, 3
// };


// int main()
// {

//     auto ctx       = p6::Context{{width, height, "TP6"}};
//     ctx.maximize_window();

//     // auto trackball = Camera::Trackball{};
//     // ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
//     //     trackball.moveFront(scroll.dy);
//     // };
//     // ctx.mouse_dragged = [&](p6::MouseDrag drag) {
//     //     trackball.rotateUp(drag.delta.y * 100);
//     //     trackball.rotateLeft(drag.delta.x * 100);
//     // };


//    /** Shader **/
//    const p6::Shader shader = p6::load_shader(
//        "shaders/skybox.vs.glsl",
//        "shaders/skybox.fs.glsl"
//    );

//    const GLint uView            = glGetUniformLocation(shader.id(), "view");
//    const GLint uProjection = glGetUniformLocation(shader.id(), "projection");

// 	// Enables the Depth Buffer
// 	glEnable(GL_DEPTH_TEST);
// 	// Enables Cull Facing
// 	glEnable(GL_CULL_FACE);
// 	// Keeps front faces
// 	glCullFace(GL_FRONT);
// 	// Uses counter clock-wise standard
// 	glFrontFace(GL_CCW);


// 	// Creates camera object
// 	Cam camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

//    GLuint cubeVBO, cubeVAO;

//     Window3D window;

// // // Cube vertices
// // std::vector<glm::vec3> cubeVertices {
// //     glm::vec3(window.WINDOW_MIN_X, window.WINDOW_MIN_Y, window.WINDOW_MIN_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MIN_Y, window.WINDOW_MIN_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MAX_Y, window.WINDOW_MIN_Z), glm::vec3(window.WINDOW_MIN_X,  window.WINDOW_MAX_Y, window.WINDOW_MIN_Z),
// //     glm::vec3(window.WINDOW_MIN_X, window.WINDOW_MIN_Y,  window.WINDOW_MAX_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MIN_Y,  window.WINDOW_MAX_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MAX_Y,  window.WINDOW_MAX_Z), glm::vec3(window.WINDOW_MIN_X,  window.WINDOW_MAX_Y, window.WINDOW_MAX_Z)
// // };

// // // Cube indices
// // std::vector<GLuint> cubeIndices {
// //     0, 1, 2, 2, 3, 0,
// //     1, 5, 6, 6, 2, 1,
// //     5, 4, 7, 7, 6, 5,
// //     4, 0, 3, 3, 7, 4,
// //     3, 2, 6, 6, 7, 3,
// //     4, 5, 1, 1, 0, 4
// // };



// // VBO
// glGenBuffers(1, &cubeVBO);
// glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
// glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(glm::vec3), cubeVertices.data(), GL_STATIC_DRAW);

// //EBO
// GLuint cubeEBO;
// glGenBuffers(1, &cubeEBO);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(GLuint), cubeIndices.data(), GL_STATIC_DRAW);

// // VAO
// glGenVertexArrays(1, &cubeVAO);
// glBindVertexArray(cubeVAO);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

// const GLuint VERTEX_ATTR_POSITION = 0;
// glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
// glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

// // Unbind VAO and VBO
// glBindVertexArray(0);
// glBindBuffer(GL_ARRAY_BUFFER, 0);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

// std::string facesCubemap[6] = {
//     "assets/textures/skybox/back.jpg",
//     "assets/textures/skybox/bottom.jpg",
//     "assets/textures/skybox/front.jpg",
//     "assets/textures/skybox/left.jpg",
//     "assets/textures/skybox/right.jpg",
//     "assets/textures/skybox/top.jpg",

// };

// 	// Creates the cubemap texture object
// 	unsigned int cubemapTexture;
// 	glGenTextures(1, &cubemapTexture);
// 	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	// These are very important to prevent seams
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
// 	// This might help with seams on some systems
// 	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

//     // Cycles through all the textures and attaches them to the cubemap object
// 	for (unsigned int i = 0; i < 6; i++)
// 	{
// 		int width, height, nrChannels;
// 		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
// 		if (data)
// 		{
// 			stbi_set_flip_vertically_on_load(false);
// 			glTexImage2D
// 			(
// 				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
// 				0,
// 				GL_RGB,
// 				width,
// 				height,
// 				0,
// 				GL_RGB,
// 				GL_UNSIGNED_BYTE,
// 				data
// 			);
// 			stbi_image_free(data);
// 		}
// 		else
// 		{
// 			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
// 			stbi_image_free(data);
// 		}
// 	}


//    ctx.update = [&]() {
//     		// Specify the color of the background
// 		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
// 		// Clean the back buffer and depth buffer
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//         shader.use();

//         glm::mat4 view = glm::mat4(1.0f);
// 		glm::mat4 projection = glm::mat4(1.0f);
// 		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
// 		// The last row and column affect the translation of the skybox (which we don't want to affect)
// 		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
// 		projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
// 		glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
// 		glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

// 		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
// 		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
// 		glBindVertexArray(cubeVAO);
// 		glActiveTexture(GL_TEXTURE0);
// 		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
// 		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
// 		glBindVertexArray(0);

// 		// Switch back to the normal depth function
// 		glDepthFunc(GL_LESS);
//    };

//    // Should be done last. It starts the infinite loop.
//    ctx.start();

//    glDeleteBuffers(1, &cubeVBO); // 1 car 1 seul vbo
//    glDeleteVertexArrays(1, &cubeVAO);
// }
