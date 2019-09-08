#include "Camera.h"
#include "Renderer.h"

void Camera::Update() {
	if (target != nullptr) {
		// Following something
		float targetX = target->x - RENDERER.camera.w / 2;
		float targetY = target->y - RENDERER.camera.h / 2;

		if (RENDERER.smoothCamera) {
			// Move to target position
			RENDERER.camera.x += ((targetX)-RENDERER.camera.x)*lerp*TM.GetDt();
			RENDERER.camera.y += ((targetY)-RENDERER.camera.y)*lerp*TM.GetDt();
		}
		else {
			// If just lock on target, no movement
			RENDERER.camera.x = targetX;
			RENDERER.camera.y = targetY;
		}
	}
}

void Camera::SetTarget(Entity *entity) {
	target = entity;
}
