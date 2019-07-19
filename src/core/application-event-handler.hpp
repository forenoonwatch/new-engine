#pragma once

#include "common.hpp"

class IApplicationEventHandler {
	public:
		IApplicationEventHandler() {}

		virtual void onKeyDown(uint32 keyCode, bool isRepeat) {}
		virtual void onKeyUp(uint32 keyCode, bool isRepeat) {}
		
		virtual void onMouseDown(uint32 keyCode, uint8 numClicks) {}
		virtual void onMouseUp(uint32 keyCode, uint8 numClicks) {}
		virtual void onMouseMove(int32 mousePosX, int32 mousePosY, int32 deltaX, int32 deltaY) {}

		virtual ~IApplicationEventHandler() {}
	private:
		NULL_COPY_AND_ASSIGN(IApplicationEventHandler);
};
