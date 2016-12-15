#ifndef _JCIGLOBALTESTMAPAPP_APP_H_
#define _JCIGLOBALTESTMAPAPP_APP_H_

#include <cinder/app/AppBasic.h>
#include <ds/app/app.h>
#include <ds/app/event_client.h>
#include <ds/touch/touch_debug.h>

#include "app/globals.h"

namespace jci {
class AllData;

class JCIGlobalTestMapApp : public ds::App {
public:
	JCIGlobalTestMapApp();

	virtual void		mouseDown(ci::app::MouseEvent e);
	virtual void		mouseDrag(ci::app::MouseEvent e);
	virtual void		mouseUp(ci::app::MouseEvent e);
	virtual void		keyDown(ci::app::KeyEvent event);
	void				setupServer();
	void				update();

	virtual void		fileDrop(ci::app::FileDropEvent event);

private:

	void				forceStartIdleMode();
	void				onAppEvent(const ds::Event&);

	// Data acquisition
	Globals				mGlobals;

	//Idle state of the app to detect state change
	bool				mIdling;

	// Handle mouse events and simulate touch events
	ds::TouchDebug		mTouchDebug;

	// App events can be handled here
	ds::EventClient		mEventClient;
};

} // !namespace jci

#endif // !_JCIGLOBALTESTMAPAPP_APP_H_