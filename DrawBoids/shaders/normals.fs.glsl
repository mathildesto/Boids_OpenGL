#version 330 core

uniform sampler2D uWaterTexture; // Texture

in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

void main() {
    // fFragColor = vec4(normalize(vNormal_vs), 0.1);
    fFragColor = vec4(texture(uWaterTexture, vTexCoords).xyz, 0.1);

}