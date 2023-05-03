#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model; //converts local obj coordinates to camera coordinates
uniform mat4 view;//convert the normaliza coordinates to window coordinates
uniform mat4 projection; //convert camera coordinates to normalize coordinates

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    // TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
    TexCoord = texCoord;

}    