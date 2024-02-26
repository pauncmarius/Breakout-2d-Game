//effect_processing.vs
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform bool confuse;
uniform float time;

void main()
{
    gl_Position =vec4(vertex.xy, 0.0f, 1.0f); // Sets the position of the vertex
    vec2 texture = vertex.zw; // Extracts texture coordinates from input
    if (confuse)
    {
        TexCoords = vec2(texture.x, texture.y); // Modifies texture coordinates if 'confuse' is true
    }
    else
    {
        TexCoords = texture; // Keeps texture coordinates as-is if 'confuse' is false
    }
}
