#include "common.glh"

#if defined(VS_BUILD)

Layout(0) attribute vec3 position;

void main() {
	gl_Position = vec4(position, 1.0);
}

#elif defined(GS_BUILD)

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout (std140) uniform ShaderData {
	mat4 viewProjection;
	vec3 cameraPosition;
};

out vec2 texCoord0;

void main() {
	const vec3 up = vec3(0.0, 1.0, 0.0);

	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(cameraPosition - pos);
	vec3 right = cross(toCamera, up); // * size

	pos -= right;
	gl_Position = vec4(pos, 1.0) * viewProjection;
	texCoord0 = vec2(0.0, 0.0);
	EmitVertex();

	pos.y += 1.0;
	gl_Position = vec4(pos, 1.0) * viewProjection;
	texCoord0 = vec2(0.0, 1.0);
	EmitVertex();

	pos.y -= 1.0;
	pos += right;
	gl_Position = vec4(pos, 1.0) * viewProjection;
	texCoord0 = vec2(1.0, 0.0);
	EmitVertex();

	pos.y += 1.0;
	gl_Position = vec4(pos, 1.0) * viewProjection;
	texCoord0 = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}

#elif defined(FS_BUILD)

varying vec2 texCoord0;

uniform sampler2D billboard;

DeclareFragOutput(0, vec4);

void main() {
	SetFragOutput(0, texture2D(billboard, texCoord0));
}

#endif
