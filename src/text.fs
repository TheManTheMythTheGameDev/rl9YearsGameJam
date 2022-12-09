#version 330
// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec4 fragPos;

uniform sampler2D texture0;
uniform sampler2D bgTex;
uniform mat4 mvp;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // NOTE: Implement here your fragment shader code
    vec4 color = fragColor * texture(texture0, fragTexCoord);

    vec4 fragPosMvp = mvp * fragPos;
    fragPosMvp.xyz /= fragPosMvp.w;
    fragPosMvp = fragPosMvp * 0.5 + 0.5;
    vec2 sampleCoords = fragPosMvp.xy;

    vec4 bgCol = texture(bgTex, sampleCoords);
    if (bgCol.r < 0.2 && bgCol.g < 0.2 && bgCol.b < 0.2)
    {
        color = vec4(0.5, 0.5, 0.5, color.a);
    }

    finalColor = color;
}

