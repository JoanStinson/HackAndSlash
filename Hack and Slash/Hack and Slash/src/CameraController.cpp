#include "CameraController.h"

void CameraController::update() {
	if (target != NULL) {
		// great, we're following someone
		float targetX = target->x - globals::camera.w / 2;
		float targetY = target->y - globals::camera.h / 2;

		if (globals::smoothCamera) {
			// MOVE to target position
			globals::camera.x += ((targetX)-globals::camera.x)*lerp*TimeController::timeController.dT;
			globals::camera.y += ((targetY)-globals::camera.y)*lerp*TimeController::timeController.dT;
		}
		else {
			//if just lock on hero, no movement
			globals::camera.x = targetX;
			globals::camera.y = targetY;
		}
	}
	// else, dont follow

}
