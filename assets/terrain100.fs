#version 100

precision mediump float;

#define ARRAY_SIZE 128

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform vec2 pointList[ARRAY_SIZE];
uniform float offset;
uniform vec2 resolution;
uniform vec4 upColor;
uniform vec4 downColor;

vec2 getData(int index)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (i == index) return pointList[i];
    }
    return pointList[ARRAY_SIZE - 1];
}


void main()
{
    float screen_step = gl_FragCoord.x / resolution.x;
    int index = int(floor(screen_step * float(ARRAY_SIZE)));

    float point = mix(getData(index).y, getData(index+1).y, screen_step * float(ARRAY_SIZE) - float(index));

    gl_FragColor = mix(downColor, upColor, step(fragTexCoord.y * resolution.y - offset, point));

    //    finalColor = vec4(pixelPos.x, pixelPos.y, 0.0, 1.0);
}