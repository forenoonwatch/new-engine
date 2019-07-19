#include "common.glh"

varying vec2 texCoord0;
varying vec4 light;

#if defined(VS_BUILD)

Layout(0) attribute vec3 position;
Layout(1) attribute vec2 texCoord;
Layout(2) attribute vec3 normal;
Layout(4) attribute vec3 jointIndices;
Layout(5) attribute vec3 jointWeights;
Layout(6) attribute mat4 transforms[2];

layout (std140) uniform ShaderData {
	mat4 jointTransforms[MAX_JOINTS];
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
};

void main() {
	mat4 jointTransform = jointTransforms[int(jointIndices[0])] * jointWeights[0];
	jointTransform += jointTransforms[int(jointIndices[1])] * jointWeights[1];
	jointTransform += jointTransforms[int(jointIndices[2])] * jointWeights[2];

	vec4 globalNormal = normalize(vec4(normal, 0.0) * (jointTransform * transforms[1]));
	vec3 localPos = vec3(vec4(position, 1.0) * (jointTransform * transforms[1]));
	vec3 lightDir = normalize(lightPosition - localPos);

	float diffuse = max(dot(globalNormal.xyz, lightDir), 0.0);
	float specular = SPECULAR_STRENGTH * pow(max(dot(normalize(cameraPosition - localPos),
			reflect(-lightDir, globalNormal.xyz)), 0.0), 32);

	gl_Position = vec4(position, 1.0) * (jointTransform * transforms[0]);
	texCoord0 = texCoord;
	light = vec4(lightColor * ((1 - ambientLight) * diffuse + ambientLight + specular), 1.0);
}

#elif defined(FS_BUILD)
uniform sampler2D diffuse;

DeclareFragOutput(0, vec4);

void main() {
	SetFragOutput(0, texture2D(diffuse, texCoord0) * light);
}
#endif
