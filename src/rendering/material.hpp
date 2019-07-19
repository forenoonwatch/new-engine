#pragma once

#include "containers/hash-map.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"

#include "texture.hpp"

struct MaterialSpec {
	HashMap<String, String> textureNames;
	HashMap<String, float> floats;
	HashMap<String, Vector3f> vectors;
	HashMap<String, Matrix> matrices;
};

class Material {
	public:
		inline Material(Texture& diffuse, Texture& normal)
			: diffuse(diffuse)
			, normal(normal) {}

		inline Texture& getDiffuse() { return diffuse; }
		inline Texture& getNormal() { return normal; }
	private:
		Texture& diffuse;
		Texture& normal;
};
