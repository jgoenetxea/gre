uniform vec4 Color;

varying vec2 gEndpoints[2];
varying vec2 gPosition;

uniform float Radius;
uniform mat4 Projection;

// Return distance from point 'p' to line segment 'a b':
float line_distance(vec2 p, vec2 a, vec2 b)
{
    float dist = distance(a,b);
    vec2 v = normalize(b-a);
    float t = dot(v,p-a);
    vec2 spinePoint;
    if (t > dist) spinePoint = b;
    else if (t > 0.0) spinePoint = a + t*v;
    else spinePoint = a;
    return distance(p,spinePoint);
}

void main()
{
    float d = line_distance(gPosition, gEndpoints[0], gEndpoints[1]);
    gl_FragColor = vec4(vec3(1.0 - 12.0 * d), 1.0);
}

