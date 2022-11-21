[vertex]
[layout]
(location=0) in vec3 pos;

[uniform]
float value;

[out]
vec2 uv;

[main]
uv = vec2(1.0, 1.0);

[fragment]
[uniform]
float value2;

[in]
vec2 uv;

[out]
vec4 color;

[main]
color = vec4(0.8, 0, 0, 1.0);