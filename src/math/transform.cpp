#include "transform.hpp"

Matrix Transform::inverse() const {
	// NOTE: If scales are entirely uniform, then this can always be represented
	// by another transform, calculated like this. However, in the case of
	// non-uniform scaling, the inverse matrix is not guaranteed to fit the
	// format of the transform class. Hence, the transform's inverse must be
	// returned in matrix form, NOT as another transform.
//	Vector3f invScale = scale.reciprocal();
//	Quaternion invRotation = rotation.inverse();
//	Vector3f invTranslation = invRotation.rotate(invScale*-translation);
//	return Transform(invTranslation, invRotation, invScale);

	// TODO: Use the special fast matrix inverse for transform matrices. There's an article
	// about it in my bookmarks
	Matrix inverse(toMatrix().inverse());
	return inverse;
}

Transform& Transform::lerp(const Transform& dest, float amt, Transform& result) const {
	result.setTranslation(translation.lerp(dest.getTranslation(), amt));
	result.setRotation(rotation.slerp(dest.getRotation(), amt));
	result.setScale(scale.lerp(dest.getScale(), amt));
}
