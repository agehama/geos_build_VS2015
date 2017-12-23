//3. バッファ演算

#include "GeosSiv3DHelper.hpp"

void Main()
{
	const double pos = 150.0;

	gg::Polygon* star = ToGeosPoly(Geometry2D::CreateNStar(5, 100, 50).movedBy(pos, Window::Height()*0.5));
	gg::Polygon* plus = ToGeosPoly(Geometry2D::CreatePlus(100.0, 50.0).movedBy(Window::Width() - pos, Window::Height()*0.5));

	while (System::Update())
	{
		gg::Geometry* resultGStar = star->buffer(15.0, 10);
		gg::Geometry* resultGPlus = plus->buffer(-15.0, 10);

		const MultiPolygon resultSStar = ToSivPoly(*resultGStar);
		const MultiPolygon resultSPlus = ToSivPoly(*resultGPlus);

		resultSStar.draw();
		resultSStar.drawWireframe(1.0, Palette::Gray);

		resultSPlus.draw();
		resultSPlus.drawWireframe(1.0, Palette::Gray);

		ToSivPoly(*star).drawFrame(1.0, Palette::Cyan);
		ToSivPoly(*plus).drawFrame(1.0, Palette::Cyan);

		delete resultGStar;
		delete resultGPlus;
	}

	delete star;
	delete plus;
}