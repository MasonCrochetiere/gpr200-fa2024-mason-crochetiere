#include "../DeanLib/include/EventListener.h"
#include "../DeanLib/include/EventSystem.h"

EventListener::EventListener()
{
}

EventListener::~EventListener()
{
	EventSystem::getInstance()->removeListenerFromAllEvents( this );
}
