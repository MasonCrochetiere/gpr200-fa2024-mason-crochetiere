#pragma once

#include "../DeanLib/include/Trackable.h"

class Event;
class EventSystem;

class EventListener:public Trackable
{
public:
	EventListener();
	virtual ~EventListener();

	virtual void handleEvent( const Event& theEvent ) = 0;

private:
};