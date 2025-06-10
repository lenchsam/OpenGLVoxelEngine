#version 460 core
out vec4 FragColor;

in vec3 Color;

void main()
{
    // alphs = 1 which is opaque
	FragColor = vec4(Color, 1.0);
}
