#include "game-event-handler.hpp"

void GameEventHandler::updateInput(uint32 inputCode, float dir, bool isRepeat) {
	if (isRepeat) {
		return;
	}

	for (uint32 i = 0; i < inputs[inputCode].size(); ++i) {
		inputs[inputCode][i].second.addAmt(dir * inputs[inputCode][i].first);
	}
}

GameEventHandler::~GameEventHandler() {}
