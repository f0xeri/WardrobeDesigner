#version 420

layout (location = 0) out vec4 idColor;

uniform vec4 color;

void main() {
    idColor = color;
}
