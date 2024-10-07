#version 330 core
out vec4 FragColor;

in vec2 TexCoords;  // Recibimos las coordenadas de textura desde el vertex shader
uniform sampler2D texture_diffuse;  // La textura que aplicaremos a las l�mparas

void main()
{
    // Usamos la textura en las l�mparas
    FragColor = texture(texture_diffuse, TexCoords);
}
