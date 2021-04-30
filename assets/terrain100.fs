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
    float cmp = fragTexCoord.y * resolution.y - offset;

    vec4 darkerDownColor = mix(downColor, vec4(0,0,0,1), 0.8);

    vec4 finalDownColor = mix(vec4(0,0.4,0,1), downColor, smoothstep(point, point + 10.0, cmp));
    finalDownColor = mix(finalDownColor, darkerDownColor, smoothstep(point+10.0, point + 200.0, cmp));
    finalDownColor = mix(finalDownColor, vec4(0.3,0.1,0,1), smoothstep(point+400.0, point + 1000.0, cmp));

    gl_FragColor = mix(finalDownColor, upColor, smoothstep(cmp-1.0, cmp+1.0, point));

    //    finalColor = vec4(pixelPos.x, pixelPos.y, 0.0, 1.0);
}