#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoords;  // Nuevas coordenadas de textura

out vec2 TexCoords;  // Se pasarán al fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;  // Pasamos las coordenadas de textura
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
