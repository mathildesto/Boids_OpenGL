#version 330 core

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

uniform sampler2D uTexture; // Texture

in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

vec3 blinnPhong() {
    vec3 wi = normalize(uLightDir_vs);
    vec3 wo = normalize(-vPosition_vs);
    vec3 halfVector = (wo + wi) / 2.0;

    return uLightIntensity * (uKd * dot(wi, vNormal_vs) + uKs * pow(dot(halfVector, vNormal_vs), uShininess));
}

void main() {
    fFragColor = vec4(blinnPhong(), 1.);
}

// void main() {
//     // Échantillonne la couleur de la texture à partir des coordonnées de texture
//     vec4 texColor = texture(uTexture, vTexCoords);

//     // Applique la couleur de la texture sur le terme de réflexion diffuse
//     vec3 diffuseColor = texColor.rgb * uKd;

//     // Calcule la contribution de la lumière selon la méthode de Blinn-Phong
//     vec3 lighting = blinnPhong();

//     // Combinaison de la couleur de la texture et de l'éclairage
//     vec3 finalColor = diffuseColor * lighting;

//     // Résultat final
//     fFragColor = vec4(finalColor, 1.0);
// }
