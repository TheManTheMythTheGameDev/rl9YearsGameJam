#version 100
precision mediump float;
// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;
varying vec4 fragPos;

uniform sampler2D texture0;
uniform sampler2D bgTex;
uniform mat4 mvp;

// NOTE: Add here your custom variables

void main()
{
    // NOTE: Implement here your fragment shader code
    vec4 color = fragColor * texture2D(texture0, fragTexCoord);

    vec4 fragPosMvp = mvp * fragPos;
    fragPosMvp.x = fragPosMvp.x / fragPosMvp.w;
    fragPosMvp.y = fragPosMvp.y / fragPosMvp.w;
    fragPosMvp.z = fragPosMvp.z / fragPosMvp.w;
    fragPosMvp = fragPosMvp * 0.5;
    fragPosMvp = fragPosMvp + 0.5;
    vec2 sampleCoords = fragPosMvp.xy;

    vec4 bgCol = texture2D(bgTex, sampleCoords);
    if (bgCol.r < 0.2 && bgCol.g < 0.2 && bgCol.b < 0.2)
    {
        color = vec4(0.5, 0.5, 0.5, color.a);
    }

    gl_FragColor = color;
}

