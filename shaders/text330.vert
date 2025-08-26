#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec4 vColor;
out vec2 vTexCoords;

uniform mat4 viewProjectionMatrix;

void main()
{
    gl_Position = viewProjectionMatrix * vec4(aPos, 1.0f);
    vColor = vec4(aColor.rgb / 255, aColor.a);
    vTexCoords = aTexCoords;
}
