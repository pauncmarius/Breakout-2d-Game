//effect_processing.frag
#version 330 core
in vec2 TexCoords; // Texture coordinates from vertex shader
out vec4 color; // Output color of the pixel

uniform sampler2D scene; // The texture to be processed
uniform bool confuse; // Flag to apply confusion effect
uniform float blurAmount; // Amount of blur to apply

const float offset = 1.0 / 300.0; // Pixel offset for blurring, depends on texture size

void main()
{
    if (confuse)
    {
        vec4 blurColor = vec4(0.0);
        for (int i = -3; i <= 3; i++)
        {
            blurColor += texture(scene, TexCoords + vec2(offset * i * blurAmount, 0.0)); // Applies blur by averaging neighboring pixels
        }
        color = blurColor / 17.0; // Normalizes the color after blurring
    }
    else
    {
        color = texture(scene, TexCoords); // Applies the original texture color if no blur is needed
    }
}
