#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aInstancePos;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 worldPos = aPos + aInstancePos;

    gl_Position = projection * view * model * vec4(worldPos, 1.0);
    TexCoord = aTexCoord;
}