#version 330 core

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

in vec3 vPosition_vs;// Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs;// Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords;// Coordonnées de texture du sommet

out vec4 fFragColor;

vec3 blinnPhong() {
    float d = distance(vPosition_vs, uLightPos_vs);
    vec3 li = uLightIntensity / (d * d);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 wo = normalize(-vPosition_vs);
    vec3 halfVector = (wo + wi)/2.f;

    return li * (
        uKd * dot(wi, vNormal_vs) +
        uKs * pow(dot(halfVector, vNormal_vs), uShininess)
    );
}

void main() {
    fFragColor = vec4(blinnPhong(), 1.);
}