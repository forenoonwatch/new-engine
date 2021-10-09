#pragma once

#include "core/common.hpp"
#include "core/application-event-handler.hpp"

class SDLApplication
{
public:
	static SDLApplication* create();
	
	virtual ~SDLApplication();
	virtual void processMessages(double delta, IApplicationEventHandler&);
	virtual bool isRunning();
private:
	bool isAppRunning;
	static uint32 numInstances;

	SDLApplication();
};
