#include "common.glh"

#if defined(VS_BUILD)

Layout(0) attribute float type;
Layout(1) attribute vec3 position;
Layout(3) attribute float age;
Layout(4) attribute vec4 transScale; // <transMin, transMax, scaleMin, scaleMax>

varying vec2 transScale0;

void main() {
	gl_Position = vec4(position, 1.0);
	transScale0 = mad(transScale.yw - transScale.xz, age * type, transScale.xz);
}

#elif defined(GS_BUILD)

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout (std140) uniform ShaderData {
	mat4 viewProjection;
	vec3 cameraPosition;
};

in vec2 transScale0[];

out vec2 texCoord1;
out float transparency1;

void main() {
	transparency1 = transScale0[0].x;

	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(cameraPosition - pos);
	vec3 right = normalize(cross(toCamera, vec3(0.0, 1.0, 0.0)));
	vec3 up = cross(right, toCamera) * transScale0[0].y;
	right *= transScale0[0].y;

	pos -= right;
	pos -= up;
	gl_Position = vec4(pos, 1.0) * viewProjection;
	texCoord1 = vec2(0.0, 0.0);
	EmitVertex();

	pos = mad(up, 2.0, pos);
	gl_Position = vec4(pos, 1.0) * viewProjection;
	texCoord1 = vec2(0.0, 1.0);
	EmitVertex();

	pos = mad(up, -2.0, pos);
	pos = mad(right, 2.0, pos);
	gl_Position = vec4(pos, 1.0) * viewProjection;
	texCoord1 = vec2(1.0, 0.0);
	EmitVertex();

	pos = mad(up, 2.0, pos);
	gl_Position = vec4(pos, 1.0) * viewProjection;
	texCoord1 = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}

#elif defined(FS_BUILD)

varying vec2 texCoord1;
varying float transparency1;

uniform sampler2D billboard;

DeclareFragOutput(0, vec4);

void main() {
	SetFragOutput(0, texture2D(billboard, texCoord1) * transparency1);
}

#endif
