#include "camera.hpp"

void CameraSystem::updateComponents(float delta, BaseECSComponent** components) {
	Transform& transform = ((TransformComponent*)components[0])->transform;
	Camera& camera = ((CameraComponent*)components[1])->camera;

	Matrix m = transform.toMatrix().transpose();
	Transform reflectTransform;
	reflectTransform.setTranslation(transform.getTranslation() * Vector3f(1.f, -1.f, 1.f));

	Vector3f f = Vector3f(1.f, -1.f, 1.f) * m[2];
	Vector3f u = f.cross(m[0]);

	reflectTransform.setRotation(Quaternion::fromAxes(m[0], u, f));

	//Quaternion rot(Vector3f(1.f, 0.f, 0.f), -2.f * a);
	//Quaternion rot(m[0], -2.f * a);
	//reflectTransform.setRotation(transform.getRotation().rotateBy(rot));
	//reflectTransform.setRotation(rot.rotateBy(transform.getRotation()));

	camera.viewProjection = camera.projection * transform.inverse();
	camera.reflectMVP = camera.projection * reflectTransform.inverse();
	camera.position = transform.getTranslation();
	//camera.viewProjection = camera.reflectMVP;
	//camera.position = transform.getTranslation() * Vector3f(1.f, -1.f, 1.f);
}
