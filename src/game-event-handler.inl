inline void GameEventHandler::onKeyDown(uint32 keyCode, bool isRepeat) {
	updateInput(keyCode, 1.f, isRepeat);

	if (!isRepeat) {
		currentInputs.insert(keyCode);
	}
}

inline void GameEventHandler::onKeyUp(uint32 keyCode, bool isRepeat) {
	updateInput(keyCode, -1.f, isRepeat);

	if (!isRepeat) {
		currentInputs.erase(keyCode);
	}
}

inline void GameEventHandler::onMouseDown(uint32 mouseButton, uint8 numClicks) {
	updateInput(MOUSE_OFFSET + mouseButton, 1.f, false);

	currentInputs.insert(MOUSE_OFFSET + mouseButton);
}

inline void GameEventHandler::onMouseUp(uint32 mouseButton, uint8 numClicks) {
	updateInput(MOUSE_OFFSET + mouseButton, -1.f, false);

	currentInputs.erase(MOUSE_OFFSET + mouseButton);
}

inline void GameEventHandler::onMouseMove(int32 mousePosX, int32 mousePosY,
		int32 deltaX, int32 deltaY) {
	mouseX = mousePosX;
	mouseY = mousePosY;

	updateInput(MOUSE_MOVE_OFFSET, (float)deltaX, false);
	updateInput(MOUSE_MOVE_OFFSET + 1, (float)deltaY, false);
}

inline void GameEventHandler::onMouseWheelMove(int32 delta) {
	updateInput(MOUSE_SCROLL_OFFSET, (float)delta, false);
}

inline void GameEventHandler::addKeyControl(uint32 keyCode,
		InputControl& inputControl, float weight) {
	inputs[keyCode].emplace_back(weight, inputControl);
}

inline void GameEventHandler::addMouseControl(uint32 mouseButton,
		InputControl& inputControl, float weight) {
	inputs[MOUSE_OFFSET + mouseButton].emplace_back(weight, inputControl);
}

inline void GameEventHandler::addMouseMoveControl(uint32 axis,
		InputControl& inputControl, float weight) {
	inputs[MOUSE_MOVE_OFFSET + axis].emplace_back(weight, inputControl);
}

inline void GameEventHandler::addMouseWheelControl(InputControl& inputControl,
		float weight) {
	inputs[MOUSE_SCROLL_OFFSET].emplace_back(weight, inputControl);
}

inline void GameEventHandler::flush() {
	for (uint32 axis = 0; axis < Input::NUM_AXES + 1; ++axis) {
		for (auto it = std::begin(inputs[MOUSE_MOVE_OFFSET + axis]),
				end = std::end(inputs[MOUSE_MOVE_OFFSET + axis]); it != end; ++it) {
			it->second.setAmt(0.f);
		}
	}
}

inline bool GameEventHandler::isKeyDown(uint32 keyCode) {
	return currentInputs.find(keyCode) != currentInputs.end();
}

inline bool GameEventHandler::isMouseDown(uint32 mouseButton) {
	return currentInputs.find(MOUSE_OFFSET + mouseButton) != currentInputs.end();
}
