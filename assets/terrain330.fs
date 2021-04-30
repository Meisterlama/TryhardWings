#version 330

#define ARRAY_SIZE 512

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform vec2 pointList[ARRAY_SIZE];
uniform float offset;
uniform vec2 resolution;
uniform vec4 upColor;
uniform vec4 downColor;

// Output fragment color
out vec4 finalColor;


void main()
{
    float screen_step = fragTexCoord.x;
    int index = int(floor(screen_step * ARRAY_SIZE));

    float point = mix(pointList[index].y, pointList[index+1].y, screen_step * ARRAY_SIZE - index);

    float cmp = fragTexCoord.y * resolution.y - offset;

    vec4 darkerDownColor = mix(downColor, vec4(0,0,0,1), 0.8);

    vec4 finalDownColor = mix(vec4(0,0.4,0,1), downColor, smoothstep(point, point + 10, cmp));
    finalDownColor = mix(finalDownColor, darkerDownColor, smoothstep(point+10, point + 200, cmp));
    finalDownColor = mix(finalDownColor, vec4(0.3,0.1,0,1), smoothstep(point+400, point + 1000, cmp));

    finalColor = mix(finalDownColor, upColor, smoothstep(cmp-1, cmp+1, point));

    //    finalColor = vec4(pixelPos.x, pixelPos.y, 0.0, 1.0);
}