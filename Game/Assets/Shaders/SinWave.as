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
if(timeCounter < time)
{
	vec2 cPos = 2.0 * uv - 1.0;
	float cLength = length(cPos);
	vec2 tex_uv = uv + (cPos/cLength) * mix(sin(cLength * 20.0 + (time-timeCounter)/(timeCounter+2) * 20.0) * 0.03, 0, cLength / 0.25);
	color = texture(myTextureSampler, tex_uv).rgba * col;
}
else
{
	color = texture( myTextureSampler, uv).rgba * col;
}