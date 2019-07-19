#pragma once

#include "animation/rig.hpp"
#include "animation/animation.hpp"

#include "material.hpp"

namespace ModelLoader {
	bool loadModels(const String& fileName,
			Array<IndexedModel>& models, Array<uint32>& modelMaterialIndices,
			Array<MaterialSpec>& materials, Array<Rig>& rigs, Array<Animation>& animations);

	bool loadOnlyModels(const String& fileName, Array<IndexedModel>& models);
}
