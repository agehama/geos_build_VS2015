//2. ブーリアン演算

#include "GeosSiv3DHelper.hpp"

void Main()
{
	gg::Polygon* star = ToGeosPoly(Geometry2D::CreateNStar(5, 100, 50).movedBy(Window::Size()*0.5));

	double angle = 0.0;
	while (System::Update())
	{
		angle += 0.01;
		gg::Polygon* plus = ToGeosPoly(Geometry2D::CreatePlus(30.0, 10.0).rotated(angle).movedBy(Window::Size()*0.5));

		gg::Geometry* resultGPoly = plus->symDifference(star);
		const MultiPolygon resultSPoly = ToSivPoly(*resultGPoly);

		resultSPoly.draw();
		resultSPoly.drawWireframe(1.0, Palette::Black);

		ToSivPoly(*plus).drawFrame();
		ToSivPoly(*star).drawFrame();

		delete plus;
		delete resultGPoly;
	}

	delete star;
}