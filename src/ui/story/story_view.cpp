#include "story_view.h"

#include <Poco/LocalDateTime.h>

#include <ds/app/environment.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/debug/logger.h>

#include "app/app_defs.h"
#include "app/globals.h"
#include "events/app_events.h"
#include "ds/ui/interface_xml/interface_xml_importer.h"
#include "model/generated/mappoint_model.h"
#include "ui/map/map_view.h"

namespace jci {

StoryView::StoryView(Globals& g)
	: ds::ui::Sprite(g.mEngine)
	, mGlobals(g)
	, mEventClient(g.mEngine.getNotifier(), [this](const ds::Event *m){ if(m) this->onAppEvent(*m); })
	, mPrimaryLayout(nullptr)
{
	hide();
	setOpacity(0.0f);


	std::map<std::string, ds::ui::Sprite*>	spriteMap;
	ds::ui::XmlImporter::loadXMLto(this, ds::Environment::expand("%APP%/data/layouts/story_view.xml"), spriteMap);
	mPrimaryLayout = dynamic_cast<ds::ui::LayoutSprite*>(spriteMap["root_layout"]);

	// calls layout
	setData();
	animateOn();

}

void StoryView::onAppEvent(const ds::Event& in_e){
	if(in_e.mWhat == IdleEndedEvent::WHAT()){
		const IdleEndedEvent& e((const IdleEndedEvent&)in_e);
		animateOn();
	} else if(in_e.mWhat == IdleStartedEvent::WHAT()){
		animateOff();
	}

	// If you have an event that is dispatched when new content is queryied, you could map that here.
	if(in_e.mWhat == StoryDataUpdatedEvent::WHAT()){
		setData();
	}
}

void StoryView::setData() {
	std::vector<ds::model::MapPointRef> mapPoints;
	//iceland (L map: far left-top test)
	ds::model::MapPointRef point;
	point.setLatitude(65.710298f); point.setLongitude(-22.939453f);
	mapPoints.push_back(point);
	// new zealand (L map: far right-bottom test)
	ds::model::MapPointRef point2;
	point2.setLatitude(-38.543243f); point2.setLongitude(177.628109f);
	mapPoints.push_back(point2);


	// alaska (R map: far left-top test)
	ds::model::MapPointRef point3;
	point3.setLatitude(68.637519f); point3.setLongitude(-165.686403f);
	mapPoints.push_back(point3);
	// greenland (R map: far right-top test)
	ds::model::MapPointRef point4;
	point4.setLatitude(81.353858f); point4.setLongitude(-11.513672f);
	mapPoints.push_back(point4);
	// argentina (R map: bottom test)
	ds::model::MapPointRef point5;
	point5.setLatitude(-54.803851f); point5.setLongitude(-67.060547f);
	mapPoints.push_back(point5);

	// japan
	ds::model::MapPointRef point6;
	point6.setLatitude(36.204824f); point6.setLongitude(138.252924f);
	mapPoints.push_back(point6);
	// dominican republic
	ds::model::MapPointRef point7;
	point7.setLatitude(18.735693f); point7.setLongitude(-70.162651f);
	mapPoints.push_back(point7);
	// honolulu
	ds::model::MapPointRef point8;
	point8.setLatitude(21.306944f); point8.setLongitude(-157.858333f);
	mapPoints.push_back(point8);

	//auto mappy = new MapView(mGlobals, mapPoints);
	//mPrimaryLayout->addChildPtr(mappy);

	layout();
}

void StoryView::layout(){
	if(mPrimaryLayout){
		mPrimaryLayout->runLayout();
	}
}

void StoryView::animateOn(){
	show();
	tweenOpacity(1.0f, mGlobals.getAnimDur());

	// Recursively animate on any children, including the primary layout
	tweenAnimateOn(true, 0.0f, 0.05f);
}

void StoryView::animateOff(){
	tweenOpacity(0.0f, mGlobals.getAnimDur(), 0.0f, ci::EaseNone(), [this]{hide(); });
}

void StoryView::updateServer(const ds::UpdateParams& p){
	ds::ui::Sprite::updateServer(p);

	// any changes for this frame happen here
}



} // namespace jci
