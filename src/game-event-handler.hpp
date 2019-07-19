#pragma once

#include "core/application-event-handler.hpp"
#include "input/input-control.hpp"
#include "containers/hash-map.hpp"
#include "containers/array.hpp"

class GameEventHandler: public IApplicationEventHandler {
	public:
		GameEventHandler() {}

		virtual void onKeyDown(uint32, bool) override;
		virtual void onKeyUp(uint32, bool) override;
		virtual void onMouseDown(uint32, uint8) override;
		virtual void onMouseUp(uint32, uint8) override;
		virtual void onMouseMove(int32, int32, int32, int32) override;

		void addKeyControl(uint32 keyCode, InputControl& inputControl, float weight = 1.f);
		void addMouseControl(uint32 mouseButton, InputControl& inputControl, float weight = 1.f);

		virtual ~GameEventHandler();
	private:
		HashMap<uint32, Array<std::pair<float, InputControl&>>> inputs;

		void updateInput(uint32 inputCode, float dir, bool isRepeat);

		NULL_COPY_AND_ASSIGN(GameEventHandler);
};
