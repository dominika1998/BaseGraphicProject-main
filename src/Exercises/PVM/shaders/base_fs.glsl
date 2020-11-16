#version 410

layout(location=0) out vec4 vFragColor;

layout(std140) uniform Modifiers {
    float strength;
    vec3 light;
};

in vec4 vertex_color;


void main() {
    vFragColor.rgb = strength * light * vertex_color.rgb;
    vFragColor.a = vertex_color.a;
}