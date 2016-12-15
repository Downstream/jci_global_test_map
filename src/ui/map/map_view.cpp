#include "map_view.h"

#include <Poco/LocalDateTime.h>

#include <ds/app/environment.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/debug/logger.h>
#include <ds/ui/interface_xml/interface_xml_importer.h>
#include <ds/query/query_client.h>

#include <ds/ui/sprite/image.h>

#include <ds/ui/sprite/circle.h>

#include "app/app_defs.h"
#include "app/globals.h"
#include "events/app_events.h"

namespace jci {

MapView::MapView(Globals& g)
	: inherited(g.mEngine)
	, mGlobals(g)
	, mLayout(nullptr)
{
	std::map<std::string, ds::ui::Sprite*>	spriteMap;
	ds::ui::XmlImporter::loadXMLto(this, ds::Environment::expand("%APP%/data/layouts/map_view.xml"), spriteMap);

	mLayout = dynamic_cast<ds::ui::LayoutSprite*>(spriteMap["layout"]);
	mMap = spriteMap["us_map"];

	// The following values can be edited in settings/app_settings.xml
	// I eyeballed the longitudes and latitudes, these values can be tweaked for better accuracy. 

	// Pixel rectangle map
	mMapPixelRect = mGlobals.getAppSettings().getRect("map:pixels", 0, ci::Rectf());
	// Longitude/latitude rectangle map (Note: 'longitude' is used loosely here - in other words, it is not a direct numerical translation, as a problem occurs at the anti-meridian)
	// For instance, if the map began at longitude 0.0, instead of 0 -> 180, -180 -> 0, it is adjusted to be 0 -> 360
	mMapGeoRect = mGlobals.getAppSettings().getRect("map:geo", 0, ci::Rectf());			
																						
	// these reference points are necessary to handle Iceland and Greenland (in reality, their longitudes overlap, however in the graphic they are on opposite ends of the image)
	mIcelandRef = mGlobals.getAppSettings().getFloat("map:west_iceland:longitude_ref", 0, -24.0f);
	mGreenlandRef = mGlobals.getAppSettings().getFloat("map:east_greenland:longitude_ref", 0, -10.0f);
	mGreenlandIcelandDivRef = mGlobals.getAppSettings().getFloat("map:iceland_greenland_divide:latitude_ref", 0, 67.0f);

	mPointColor = ci::ColorA(1.0f, 0.0f, 0.0f, 1.0f);

	layout();

	// LATITUDE AND LONGITUDE TEST POINTS
	// Left side of map (Europe, Africa, Asia, Australia) - boundary test
	mMapPoints.push_back(ci::Vec2f(65.710298f, -22.939453f));		// iceland				(L map: far left-top test)
	mMapPoints.push_back(ci::Vec2f(-38.543243f, 177.628109f));		// new zealand			(L map: far right-bottom test)
	mMapPoints.push_back(ci::Vec2f(36.204824f, 138.252924f));		// japan				(L map: random test)
	// Ride side of map (North America, South America) - boundary test
	mMapPoints.push_back(ci::Vec2f(68.637519f, -165.686403f));		// alaska				(R map: far left-top test)
	mMapPoints.push_back(ci::Vec2f(81.353858f, -11.513672f));		// greenland			(R map: far right-top test)
	mMapPoints.push_back(ci::Vec2f(-54.803851f, -67.060547f));		// argentina			(R map: bottom test)
	mMapPoints.push_back(ci::Vec2f(18.735693f, -70.162651f));		// dominican republic	(R map: random test)
	mMapPoints.push_back(ci::Vec2f(21.306944f, -157.858333f));		// honolulu				(R map: random test)

	for(auto point : mMapPoints){
		plotPoint(point.x, point.y, 5.0f);
	}

	animateOn();

}

void MapView::layout(){
	if(mLayout){
		mLayout->runLayout();
	}
}


void MapView::animateOn(){
	show();
	tweenOpacity(1.0f, mGlobals.getAppSettings().getFloat("animation:duration", 0, 0.35f));
}

void MapView::animateOff(){
	tweenOpacity(0.0f, mGlobals.getAppSettings().getFloat("animation:duration", 0, 0.35f), 0.0f, ci::EaseNone(), [this]{hide(); });
}

void MapView::plotPoint(float lat, float lon, float size)
{

	// If longitude east of anti-meridian, but west of Iceland - add 360 (see diagram for reference).
	// Also in reality, Greenland and Iceland have overlapping longitudes, but they are separated on opposite ends of the graphic provided.
	// To care of this, add an additional condition to "cut" any coordinates falling in Greenland and adjust its longitude as well.
	if (lon < mIcelandRef || (lon < mGreenlandRef && lat > mGreenlandIcelandDivRef)){
		lon += 360;
	}

	// Find corresponding pixel positions
	float pixelPosX = (lon - mMapGeoRect.getX1()) / mMapGeoRect.getWidth();
	pixelPosX *= mMapPixelRect.getWidth();
	pixelPosX += mMapPixelRect.getX1();

	float pixelPosY = (lat - mMapGeoRect.getY1()) / mMapGeoRect.getHeight();;
	pixelPosY *= mMapPixelRect.getHeight();
	pixelPosY += mMapPixelRect.getY1();

	// Draw point
	auto circ = new ds::ui::Circle(mEngine, true, size);
	mMap->addChildPtr(circ);
	circ->setPosition(pixelPosX, pixelPosY);
	circ->setColorA(mPointColor);
	circ->setCenter(0.5f, 0.5f);
}

} // namespace jci
