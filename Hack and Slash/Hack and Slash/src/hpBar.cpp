#include "HpBar.h"

void HpBar::Draw() {
	if (entity != NULL) {
		// draw outside of hp bar
		SDL_SetRenderDrawColor(globals::renderer, 240, 51, 159, SDL_ALPHA_OPAQUE);
		SDL_Rect barContainer = { x, y, barWidth, barHeight };
		SDL_RenderDrawRect(globals::renderer, &barContainer);

		//percentage of entities health remaining
		float hpPercent = entity->hp / (entity->hpMax*1.0f);//*1.0f upgrade math into float division so we dont lose decimals
		//example 50/100 = 0.5, but in integer division, it removes decimal, so we get 0

		SDL_Rect hpRect = { x + 2, y + 2, (barWidth - 4)*hpPercent, barHeight - 4 };

		SDL_RenderFillRect(globals::renderer, &hpRect);
	}
}
