[vertex]
[layout]
[uniform]
[out]
[main]
[fragment]
[uniform]
float time;
float timeCounter;
[in]
[out]
[main]
if(timeCounter < 20)
{
	vec2 cPos = 2.0* uv - 1.0;
	float cLength = length(cPos);
	vec2 tex_uv = uv + (cPos/cLength) * mix(cos(cLength * 12.0 - time * 4.0) * 0.03, 0.0, cLength / 0.25);
	color = texture(myTextureSampler, tex_uv).rgba * col;
}
else
{
	color = texture( myTextureSampler, uv).rgba * col;
}