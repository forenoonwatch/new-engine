#include "common.glh"

varying vec2 texCoord0;
varying vec3 localPos;
varying vec3 lightDir;

#if defined(VS_BUILD)

Layout(0) attribute vec3 position;
Layout(1) attribute vec2 texCoord;
Layout(2) attribute vec3 normal;
Layout(3) attribute vec3 tangent;
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

	vec3 N = normalize( vec3(vec4(normal, 0.0) * (jointTransform * transforms[1])) );
	vec3 T = normalize( vec3(vec4(tangent, 0.0) * (jointTransform * transforms[1])) );
	T = normalize(T - dot(T, N) * N);
	mat3 TBN = transpose(mat3(T, cross(N, T), N));

	localPos = TBN * vec3(vec4(position, 1.0) * (jointTransform * transforms[1]));
	lightDir = TBN * normalize(lightPosition - localPos);

	gl_Position = vec4(position, 1.0) * (jointTransform * transforms[0]);
	texCoord0 = texCoord;
}

#elif defined(FS_BUILD)
uniform sampler2D diffuse;
uniform sampler2D normalMap;

layout (std140) uniform ShaderData {
	mat4 jointTransforms[MAX_JOINTS];
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
};

DeclareFragOutput(0, vec4);

void main() {
	vec3 normal = texture2D(normalMap, texCoord0).rgb;
	normal = normalize(2.0 * normal - 1.0);

	float diffuseVal = max(dot(normal, lightDir), 0.0);
	float specular = SPECULAR_STRENGTH * pow(max(dot(normalize(cameraPosition - localPos),
			reflect(-lightDir, normal)), 0.0), 32);

	SetFragOutput(0, texture2D(diffuse, texCoord0)
			* vec4(lightColor * ((1 - ambientLight) * diffuseVal + ambientLight + specular), 1.0));
}
#endif
