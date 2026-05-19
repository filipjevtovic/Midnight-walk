//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;

uniform float flicker_a;
uniform float flicker_b;
uniform float flicker_c;
uniform float time;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main() {
    float flicker;
    if (flicker_b < 0.5) {
        flicker = 1.0;
    }
    else {
        float t = floor(time * 20.0);
        float noise = fract(cos(flicker_b * t + 2.123)) * fract(cos(flicker_c * t));
        noise = pow(noise, 4.0);
        flicker = flicker_a * (0.1 + 0.9 * noise);
        flicker = clamp(flicker, 0.0, 1.0);
    }
    FragColor = vec4(lightColor * 10.0, 1.0) * flicker;

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0) {
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
    else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
