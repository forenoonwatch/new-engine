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

void Transform::lookAt(const Vector3f& point) {
	Vector3f fwd = (point - translation).normalized();
	Vector3f rht = Vector3f(0.f, 1.f, 0.f).cross(fwd).normalized();
	Vector3f up = fwd.cross(rht);

	rotation = Quaternion::fromAxes(rht, up, fwd);
	/*float dt = fwd[2]; // fwd.dot(Vector3f.FORWARD) aka (0, 0, 1)
	fwd = Vector3f(0.f, 0.f, 1.f).cross(fwd);

	rotation = Quaternion(fwd[0], fwd[1], fwd[2], dt).normalized();*/
}
