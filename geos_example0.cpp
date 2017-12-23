//0. ê}å`ÇÃçÏê¨

#include "GeosSiv3DHelper.hpp"

void Main()
{
	while (System::Update())
	{
		gg::Polygon* poly = ToGeosPoly(Rect(100, 50).setCenter(Window::Center()).asPolygon());
		
		ToSivPoly(*poly).draw();

		delete poly;
	}
}