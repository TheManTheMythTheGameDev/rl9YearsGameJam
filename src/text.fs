#version 330
// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform sampler2D bgTex;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // NOTE: Implement here your fragment shader code
    vec4 color = colDiffuse * texture(texture0, fragTexCoord);
    vec4 bgCol = texture(bgTex, fragTexCoord);
    if (bgCol.r == 1 && bgCol.g == 1 && bgCol.b == 1)
    {
        color = vec4(1.0, 1.0, 1.0, color.a);
    }

    finalColor = color;
}

