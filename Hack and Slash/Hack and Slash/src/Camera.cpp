#include "Camera.h"
#include "Renderer.h"

void Camera::Update() {
	if (target != nullptr) {
		// great, we're following someone
		float targetX = target->x - RENDERER.camera.w / 2;
		float targetY = target->y - RENDERER.camera.h / 2;

		if (RENDERER.smoothCamera) {
			// MOVE to target position
			RENDERER.camera.x += ((targetX)-RENDERER.camera.x)*lerp*TM.GetDt();
			RENDERER.camera.y += ((targetY)-RENDERER.camera.y)*lerp*TM.GetDt();
		}
		else {
			//if just lock on hero, no movement
			RENDERER.camera.x = targetX;
			RENDERER.camera.y = targetY;
		}
	}
	// else, dont follow

}

void Camera::SetTarget(Entity *entity) {
	target = entity;
}
