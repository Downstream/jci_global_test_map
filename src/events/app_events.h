#ifndef _JCIGLOBALTESTMAPAPP_APP_APPEVENTS_H_
#define _JCIGLOBALTESTMAPAPP_APP_APPEVENTS_H_

#include <ds/app/event.h>

namespace jci {

class IdleStartedEvent : public ds::RegisteredEvent < IdleStartedEvent > {
public:
	IdleStartedEvent(){};
};

class IdleEndedEvent : public ds::RegisteredEvent < IdleEndedEvent > {
public:
	IdleEndedEvent(){};

};

class RequestAppExitEvent : public ds::RegisteredEvent<RequestAppExitEvent>{
public:
	RequestAppExitEvent(){};
};

} // !namespace jci

#endif // !_JCIGLOBALTESTMAPAPP_APP_APPEVENTS_H_