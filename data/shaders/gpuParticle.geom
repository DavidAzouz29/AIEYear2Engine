#version 410

// Input is points but output a quad
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// Input data from vertex shader
in vec3 v3Position[];
in float vLifetime[];
in float vLifespan[];

// Output to fragment shader
out vec4 vColor;

uniform mat4 m4ProjectionView;
uniform mat4 m4CameraTransform;

uniform float fSizeStart;
uniform float fSizeEnd;

uniform vec4 v4ColorStart;
uniform vec4 v4ColorEnd;

void main() 
{
	// interpolate colour
	vColor = mix(v4ColorStart, v4ColorEnd, vLifetime[0] / vLifespan[0]);

	// calculate the size and create the corners of a quad
	float fHalfSize = mix(fSizeStart, fSizeEnd, vLifetime[0] / vLifespan[0]) * 0.5f;

	vec3 v3Corners[4];
	v3Corners[0] = gl_in[0].gl_Position.xyz + vec3(  fHalfSize, -fHalfSize, 0);
	v3Corners[1] = gl_in[0].gl_Position.xyz + vec3(  fHalfSize,  fHalfSize, 0);
	v3Corners[2] = gl_in[0].gl_Position.xyz + vec3( -fHalfSize, -fHalfSize, 0);
	v3Corners[3] = gl_in[0].gl_Position.xyz + vec3( -fHalfSize,  fHalfSize, 0);

	// billboard
/*	mat3 billboard;
	billboard[2] = normalize(m4CameraTransform[3].xyz - gl_in[0].gl_Position.xyz);
	billboard[0] = cross(m4CameraTransform[1].xyz, billboard[2]);
	billboard[1] = cross(billboard[2], billboard[0]); */

	vec3 zAxis = normalize(m4CameraTransform[3].xyz - v3Position[0]);
	vec3 xAxis = cross(m4CameraTransform[1].xyz, zAxis);
	vec3 yAxis = cross(zAxis, xAxis);
	mat3 billboard = mat3(xAxis, yAxis, zAxis); 

	// emit the 4 vertices for the quad
	for (int i = 0; i < 4; ++i)
	{
		//gl_Position = m4ProjectionView * vec4(billboard * v3Corners[i], 1);
		gl_Position = m4ProjectionView * vec4(billboard * v3Corners[i] + v3Position[0], 1);
		EmitVertex();
	}
}