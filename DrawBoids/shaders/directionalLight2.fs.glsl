#version 330

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs; //wi (need normalization)
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity; //Li

uniform sampler2D uTexture;

in vec3 vPosition_vs; //w0 normalize(-vPos)
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;


vec3 blinnPhong() {
    vec3 Li = uLightIntensity;
    vec3 N = normalize(vNormal_vs);
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightDir_vs);
    vec3 halfVector = (w0 + wi)/2.f;

    return Li*(uKd*max(dot(wi, N), 0.) + uKs*max(pow(dot(halfVector, N), 0.), uShininess));
}

void main() {

    vec4 texColor = texture(uTexture, vTexCoords);
    vec3 lighting = blinnPhong();
    fFragColor = vec4(texColor.rgb * lighting, texColor.a);

}