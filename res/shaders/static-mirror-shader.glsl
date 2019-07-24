#include "common.glh"

#define DENSITY 0.005
#define GRADIENT 1.5

varying vec2 texCoord0;
varying vec3 localPos;
varying vec3 lightDir;
varying float visibility;

#if defined(VS_BUILD)

#include "noise.glh"

Layout(0) attribute vec3 position;
Layout(1) attribute vec2 texCoord;
Layout(2) attribute vec3 normal;
Layout(3) attribute vec3 tangent;
Layout(4) attribute mat4 transforms[2];

layout (std140) uniform ShaderData {
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
	float time;
};

void main() {
	vec4 vertPos = vec4(position, 1.0) * transforms[0];

	float dist = length(vertPos.xyz - cameraPosition);
	visibility = clamp( exp(-pow(dist * DENSITY, GRADIENT)), 0.0, 1.0 );

	vec3 N = normalize( vec3(vec4(normal, 0.0) * transforms[1]) );
	vec3 T = normalize( vec3(vec4(tangent, 0.0) * transforms[1]) );
	T = normalize(T - dot(T, N) * N);
	mat3 TBN = transpose(mat3(T, cross(N, T), N));

	localPos = TBN * vec3(vec4(position, 1.0) * transforms[1]);
	lightDir = TBN * normalize(lightPosition - localPos);

    gl_Position = vertPos;
    texCoord0 = texCoord;
	gl_ClipDistance[0] = dot(vec4(0, 1, 0, transforms[1][3][1]), vertPos);
}

#elif defined(FS_BUILD)

uniform sampler2D diffuse;
uniform sampler2D normalMap;

layout (std140) uniform ShaderData {
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
	float time;
};

DeclareFragOutput(0, vec4);

void main() {
	vec3 normal = texture2D(normalMap, texCoord0).rgb;
	normal = normalize(2.0 * normal - 1.0);

	float diffuseVal = max(dot(normal, lightDir), 0.0);
	float specular = SPECULAR_STRENGTH * pow(max(dot(normalize(cameraPosition - localPos),
			reflect(-lightDir, normal)), 0.0), 32);
	
	vec3 color = lightColor * ((1 - ambientLight) * diffuseVal + ambientLight + specular);
	
	SetFragOutput(0, texture2D(diffuse, texCoord0)
			* vec4(mix(vec3(0.0, 0.15, 0.3), color, visibility), 1.0));
}
#endif
