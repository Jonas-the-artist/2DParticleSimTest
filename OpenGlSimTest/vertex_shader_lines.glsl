#version 330 core
layout (location = 0) in vec2 position;

// Vertex Shader. window: 640, 480
void main() {
    vec2 pos = position;
    gl_Position = vec4(pos.x/(640/2), pos.y/(480/2), 0.0, 1.0);
}
