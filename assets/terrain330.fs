#version 330

#define ARRAY_SIZE 512

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform vec2 pointList[ARRAY_SIZE];
uniform vec2 offset;
uniform vec2 resolution;
uniform vec4 upColor;
uniform vec4 downColor;

// Output fragment color
out vec4 finalColor;


void main()
{
    float screen_step = gl_FragCoord.x / resolution.x;
    int index = int(floor(screen_step * ARRAY_SIZE));

    float point = mix(pointList[index].y, pointList[index+1].y, screen_step * ARRAY_SIZE - index);

    finalColor = mix(downColor, upColor, step(fragTexCoord.y * resolution.y - offset.y, point));

    //    finalColor = vec4(pixelPos.x, pixelPos.y, 0.0, 1.0);
}