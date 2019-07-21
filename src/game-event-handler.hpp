#pragma once

#include "core/input.hpp"
#include "core/application-event-handler.hpp"

#include "containers/hash-map.hpp"
#include "containers/array.hpp"
#include "containers/hash-set.hpp"

#include "input/input-control.hpp"

class GameEventHandler: public IApplicationEventHandler {
	public:
		GameEventHandler() {}

		virtual void onKeyDown(uint32, bool) override;
		virtual void onKeyUp(uint32, bool) override;
		virtual void onMouseDown(uint32, uint8) override;
		virtual void onMouseUp(uint32, uint8) override;
		virtual void onMouseMove(int32, int32, int32, int32) override;
		virtual void onMouseWheelMove(int32 delta) override;

		void addKeyControl(uint32 keyCode, InputControl& inputControl, float weight = 1.f);
		void addMouseControl(uint32 mouseButton, InputControl& inputControl, float weight = 1.f);
		void addMouseMoveControl(uint32 axis, InputControl& inputControl, float weight = 1.f);
		void addMouseWheelControl(InputControl& inputControl, float weight = 1.f);

		bool isKeyDown(uint32 keyCode);
		bool isMouseDown(uint32 mouseButton);

		void flush();

		inline int32 getMouseX() const { return mouseX; }
		inline int32 getMouseY() const { return mouseY; }

		virtual ~GameEventHandler();
	private:
		static constexpr size_t MOUSE_OFFSET = Input::NUM_KEYS;
		static constexpr size_t MOUSE_MOVE_OFFSET = MOUSE_OFFSET + Input::NUM_MOUSEBUTTONS;
		static constexpr size_t MOUSE_SCROLL_OFFSET = MOUSE_MOVE_OFFSET + Input::NUM_AXES;

		HashMap<uint32, Array<std::pair<float, InputControl&>>> inputs;
		HashSet<uint32> currentInputs;

		int32 mouseX;
		int32 mouseY;

		void updateInput(uint32 inputCode, float dir, bool isRepeat);

		NULL_COPY_AND_ASSIGN(GameEventHandler);
};

#include "game-event-handler.inl"
