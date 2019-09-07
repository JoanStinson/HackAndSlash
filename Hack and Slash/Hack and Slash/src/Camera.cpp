#include "Camera.h"
#include "Globals.h"

void Camera::Update() {
	if (target != nullptr) {
		// great, we're following someone
		float targetX = target->x - globals::camera.w / 2;
		float targetY = target->y - globals::camera.h / 2;

		if (globals::smoothCamera) {
			// MOVE to target position
			globals::camera.x += ((targetX)-globals::camera.x)*lerp*TM.GetDt();
			globals::camera.y += ((targetY)-globals::camera.y)*lerp*TM.GetDt();
		}
		else {
			//if just lock on hero, no movement
			globals::camera.x = targetX;
			globals::camera.y = targetY;
		}
	}
	// else, dont follow

}
