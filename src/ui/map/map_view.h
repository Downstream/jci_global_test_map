#pragma once
#ifndef _NWM_COMMUNITY_VOICES_APP_UI_MAP_VIEW
#define _NWM_COMMUNITY_VOICES_APP_UI_MAP_VIEW


#include <ds/ui/sprite/sprite.h>
#include <ds/app/event_client.h>
#include <ds/ui/layout/layout_sprite.h>

#include "cinder/Vector.h"

namespace jci {

class Globals;

/**
* \class nwm::StoryView
*			A sample view
*/
class MapView final : public ds::ui::Sprite  {
public:
	MapView(Globals& g);

private:
	typedef ds::ui::Sprite				inherited;

	void								onAppEvent(const ds::Event&);

	void								animateOn();
	void								animateOff();
	void								layout();

	void								plotPoint(float lat, float lon, float size);

	Globals&							mGlobals;

	ds::ui::LayoutSprite*				mLayout;
	ds::ui::Sprite*						mMap;

	ci::Rectf							mMapPixelRect;
	ci::Rectf							mMapGeoRect;

	std::vector<ci::Vec2f>				mMapPoints;

	float								mGreenlandRef;
	float								mIcelandRef;
	float								mGreenlandIcelandDivRef;


	ci::ColorA							mPointColor;
};

} // namespace jci

#endif
