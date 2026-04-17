#version 330 core
out vec4 fragColour;  
in vec2 outTex;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float opacity;
};

uniform Material material;

void main() {
    fragColour = texture(material.texture_diffuse1, outTex);
    fragColour.a = material.opacity;
}
