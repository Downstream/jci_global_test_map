jci_global_test_map
=================
**Overview**

This is a sample application to demonstrate mapping longitude/latitude coordinates to the global projection display at JCI Shanghai.

**Compiling**

* Grab [ds_cinder develop branch](https://github.com/Downstream/ds_cinder)
* You'll need [cinder 0.8.6 vc2013](http://libcinder.org/download/)

***

## People Involved

**Original Developers** - Antoinette Bumatay - using a big chunk of Luke Purcell's code from [Community Voices](https://github.com/Downstream/northwest_mutual_tapestry/tree/community_voices) 

## Mapping Overview

<img src="https://github.com/Downstream/jci_global_test_map/blob/master/data/images/temp/1920x917_map_and_projection_area.png"/>

Mapping longitude/latitude coordinates is relatively straightforward on a equirectangular map; however, there are two items that need to be taken into consideration with this specific projection display.
* The Anti-Meridian
* Iceland/Greenland

**The Anti-Meridian**

The Anti-Meridian lies at 180 degrees longitude. On a Pacific Centered World Map, this causes a small issue computationally, as there is a jump from -180 to 180 degrees longitude.

This can be handled, by adjusting the longitude to be continuous.

<img src="https://raw.githubusercontent.com/Downstream/jci_global_test_map/master/docs/media/diagram.png"/>

**Iceland/Greenland**

In this specific projection, Iceland and Greenland are displayed on opposite ends of the image. In reality, Iceland and Greenland have some overlap in longitudes.

This can be handled by adding an additional statement to "adjust" (as above) any coordinates falling in Greenland's area.

<img src="https://raw.githubusercontent.com/Downstream/jci_global_test_map/master/docs/media/caseGreenlandIceland.png" width="500"/>

**Computation**

From there, the calculation is pretty straight forward.

In `settings\app_settings.xml`, define the longitude/latitude corner points of the geo map, as well as the reference longitudes/latitudes to handle Iceland and Greenland:
```
	<!-- map rectangles for pixel and geo (longitude, latitude) positions (see diagram)-->
	<rect name="map:pixels" t="0.0" l="0.0" w="1920.0" h="917.0" />
	<rect name="map:geo" t="83.0" l="-24.0" b="-60.0" r="348.0"  />
	
	<!-- map reference points (see diagram)-->
	<float  name="map:west_iceland:longitude_ref" value="-24.0" />
	<float  name="map:east_greenland:longitude_ref" value="-10.0" />
	<float  name="map:iceland_greenland_divide:latitude_ref" value="67.0" />
```

In `src\ui\map\map_view.cpp`:
```
	void MapView::plotPoint(float lat, float lon, float size)
	{
		// Make any longitude adjustments
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
```

## Additional Information

All the longitudes and latitudes (i.e. corner point coordinates and reference points for the geo map) for this specific map image were approximated. These values can be tweaked in `settings\app_settings.xml` for better accuracy.

Coordinates of various locations were hardcoded in `src\ui\map\map_view.cpp` for additional testing purposes.
