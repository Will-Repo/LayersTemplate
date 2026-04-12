#version 330 core
out vec4 fragColour;  
in vec3 outColour;
  
void main() {
    fragColour = vec4(outColour, 1.0);
}
