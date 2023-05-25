#version 330

layout(location = 10) in vec3 aVertexPosition;
layout(location = 11) in vec3 aVertexNormal;
layout(location = 12) in vec2 aVertexTexCoords;

//Matrices de transformations
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

//Variables de sorties
out vec3 vPosition_vs;
out vec3 vNormal_vs;
out vec2 vTexCoords;

void main() {
    //Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    //Calcul des variables de sorties
    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;

    gl_Position = uMVPMatrix * vertexPosition;
}