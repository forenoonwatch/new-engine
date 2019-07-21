#include "common.glh"

#define AMPLITUDE 3
#define TIME_SCALE 0.2
#define SPACE_SCALE 0.1

#define DENSITY 0.01
#define GRADIENT 2

varying vec2 texCoord0;
varying vec4 clipSpace;
varying vec3 localPos;
varying vec3 lightDir;
varying float visibility;
varying float fresnel;

#if defined(VS_BUILD)

#include "noise.glh"

Layout(0) attribute vec2 xyPos;
Layout(1) attribute vec4 adjacent;
Layout(2) attribute vec2 texCoord;
Layout(3) attribute mat4 transforms[2];

layout (std140) uniform ShaderData {
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
	float time;
	vec3 fogColor;
};

float height(vec2 xy) {
	return AMPLITUDE * cnoise(SPACE_SCALE * (xy + cameraPosition.xz) + TIME_SCALE * vec2(time))
		* cnoise(-SPACE_SCALE * 0.75 * (xy + cameraPosition.xz) + 0.5 * TIME_SCALE * vec2(time));
}

void main() {
	vec2 vertXY = xyPos;
	vec2 v1XY = adjacent.xy;
	vec2 v2XY = adjacent.zw;

	float h0 = height(vertXY);

	vec3 position = vec3(vertXY.x, h0, vertXY.y);
	vec4 vertPos = vec4(position, 1.0) * transforms[0];

	float dist = length(vertPos.xyz);
	visibility = clamp( exp(-pow(dist * DENSITY, GRADIENT)), 0.0, 1.0 );

	vec3 T = normalize(vec3(v1XY.x, height(v1XY + vertXY) - h0, v1XY.y));
	vec3 B = normalize(vec3(v2XY.x, height(vertXY + v2XY) - h0, v2XY.y));
	vec3 N = normalize(cross(T, B));
	mat3 TBN = transpose(mat3(T, cross(N, T), N));

	localPos = TBN * (position + vec3(cameraPosition.x, 0.0, cameraPosition.z));
	lightDir = TBN * normalize(lightPosition - localPos);

	gl_Position = vertPos;
	texCoord0 = texCoord * 25.0;
	clipSpace = vertPos;
	fresnel = dot(N, normalize(cameraPosition - vertPos.xyz));
}

#elif defined(FS_BUILD)

#define WAVE_STRENGTH 0.2

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D normalMap;
//uniform sampler2D dudvMap;

layout (std140) uniform ShaderData {
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
	float time;
	vec3 fogColor;
};

DeclareFragOutput(0, vec4);

void main() {
	vec2 ndc = (clipSpace.xy / clipSpace.w) * 0.5 + 0.5;

	vec3 normal = texture2D(normalMap, texCoord0).rgb;
	//normal = 1.25 * normalize(2.0 * normal - 1.0);
	normal = 2 * normalize(normal);

	//vec2 distort = mad(texture2D(dudvMap, texCoord0).rg, 2.0, -1.0) * WAVE_STRENGTH * sin(time);

	float diffuseVal = max(dot(normal, lightDir), 0.0);
	float specular = SPECULAR_STRENGTH * pow(max(dot(normalize(cameraPosition - localPos),
			reflect(-lightDir, normal)), 0.0), 3);
	
	vec3 color = lightColor * ((1 - ambientLight) * diffuseVal + ambientLight + specular);

	vec3 flect = texture2D(reflection, vec2(ndc.x, 1 - ndc.y)).rgb;
	vec3 fract = texture2D(refraction, ndc).rgb;

	color *= mix(fract, flect, fresnel);
	//color = mix(color, vec3(0.372549, 0.61176, 0.60784), 0.7);
	color = mix(color, vec3(0, 0.15, 0.3), 0.7);

	SetFragOutput(0, vec4(mix(fogColor, color, visibility), 1.0));
}

#endif
