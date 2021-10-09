#include "game-event-handler.hpp"
#include "core/input.hpp"

#define MOUSE_OFFSET Input::NUM_KEYS

void GameEventHandler::onKeyDown(uint32 keyCode, bool isRepeat) {
	updateInput(keyCode, 1.f, isRepeat);
}

void GameEventHandler::onKeyUp(uint32 keyCode, bool isRepeat) {
	updateInput(keyCode, -1.f, isRepeat);
}

void GameEventHandler::onMouseDown(uint32 mouseButton, uint8 numClicks) {
	updateInput(MOUSE_OFFSET + mouseButton, 1.f, false);
}

void GameEventHandler::onMouseUp(uint32 mouseButton, uint8 numClicks) {
	updateInput(MOUSE_OFFSET + mouseButton, -1.f, false);
}

void GameEventHandler::onMouseMove(int32 mousePosX, int32 mousePosY, int32 deltaX, int32 deltaY) {}

void GameEventHandler::addKeyControl(uint32 keyCode, InputControl& inputControl, float weight) {
	inputs[keyCode].push_back(std::pair<float, InputControl&>(weight, inputControl)); // TODO: emplace back
}

void GameEventHandler::addMouseControl(uint32 mouseButton, InputControl& inputControl, float weight) {
	inputs[MOUSE_OFFSET + mouseButton].push_back(std::pair<float, InputControl&>(weight, inputControl)); // TODO: emplace back
}

void GameEventHandler::updateInput(uint32 inputCode, float dir, bool isRepeat) {
	if (isRepeat) {
		return;
	}

	for (uint32 i = 0; i < inputs[inputCode].size(); ++i) {
		inputs[inputCode][i].second.addAmt(dir * inputs[inputCode][i].first);
	}
}

GameEventHandler::~GameEventHandler() {}
