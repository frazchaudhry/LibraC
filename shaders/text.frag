#version 460 core

in vec4 vColor;
in vec2 vTexCoords;

out vec4 FragColor;

uniform sampler2D fontAtlasTexture;

void main()
{
    FragColor = vec4(texture(fontAtlasTexture, vec2(vTexCoords.x, vTexCoords.y)).r) * vColor;
}
