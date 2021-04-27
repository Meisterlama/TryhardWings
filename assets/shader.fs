#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 pointList[512];
uniform vec2 offset;
uniform vec2 resolution;
uniform vec4 upColor;
uniform vec4 downColor;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    vec2 pixelPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y) - offset;

        int index = int(pixelPos.x);
        finalColor = mix(downColor, upColor, step(pixelPos.y, pointList[index].y));

//    finalColor = vec4(pixelPos.x, pixelPos.y, 0.0, 1.0);
}