#version 330 core

// Attributs de sommet
layout(location = 10) in vec3 aVertexPosition; // Position du sommet
layout(location = 11) in vec3 aVertexNormal; // Normale du sommet
layout(location = 12) in vec2 aVertexTexCoords; // Coordonnées de texture du sommet

// Matrices de transformation reçues en uniform
uniform mat4 uMVPMatrix; // ModelViewProjection
uniform mat4 uMVMatrix; // ModelView
uniform mat4 uNormalMatrix;

// Sorties du shader
out vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
out vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
out vec2 vTexCoords; // Coordonnées de texture du sommet

void main() {
    // Coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1.);
    vec4 vertexNormal = vec4(aVertexNormal, 0.); // attention, c'est un 0 ici car vecteur

    vPosition_vs = vec3(uMVMatrix * vertexPosition); // on multiplie la position par la matrice camera
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;

    gl_Position = uMVPMatrix * vertexPosition;
}