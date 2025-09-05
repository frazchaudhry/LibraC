#version 330 core

layout (location = 0) in vec2 aPos;

out vec4 vColor;

uniform vec4 aColor;
uniform mat4 model;
uniform mat4 viewProjectionMatrix;

void main()
{
    gl_Position = viewProjectionMatrix * model * vec4(aPos, 0.0, 1.0f);
    vColor = vec4(aColor.rgb / 255, aColor.a);
}
