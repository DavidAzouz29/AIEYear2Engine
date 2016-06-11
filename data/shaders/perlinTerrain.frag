#version 410

in vec2 outCoord;
in vec4 vNormal; 
out vec4 out_color; 

uniform vec3 LightDir; 
uniform vec3 LightColour; 
uniform sampler2D perlin_texture; 
uniform sampler2D dirt_texture; 
uniform sampler2D grass_texture; 
uniform sampler2D snow_texture; 

void main() 
{
	float d = max(0, dot( normalize(vNormal).xyz, LightDir ) ); 
	float height = texture(perlin_texture, outCoord).r; 
	vec4 dirtColor = texture(dirt_texture, outCoord); 
	vec4 grassColor = texture(grass_texture, outCoord); 
	vec4 snowColor = texture(snow_texture, outCoord); 
	float blendValue = smoothstep(0.2, 0.5, height); 
	vec4 color = mix(dirtColor, grassColor, blendValue); 
	blendValue = smoothstep(0.7, 0.8, height); 
	out_color = d * mix(color, snowColor, blendValue); 
	out_color.a = 1; 
}

//TODO: LightColour; \
//void main() {	out_color = texture(perlin_texture, outCoord).rrrr; \
//void main() { out_color = vec4(outCoord.xy, 0, 1); \

/*out_color = d * texture(perlin_texture, outCoord).rrrr; \
out_color = d * mix(color, snowColor, blendValue); \
*/