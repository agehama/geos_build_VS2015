//5. ê}å`ìØémÇÃãóó£ÅAç≈ãﬂê⁄ì_

#include "GeosSiv3DHelper.hpp"

void Main()
{
	gg::CoordinateArraySequence points;
	std::vector<LineString> lines;

	gg::Polygon* star = ToGeosPoly(Geometry2D::CreateNStar(5, 100, 50).movedBy(Window::Size()*0.5));

	while (System::Update())
	{
		std::vector<Vec2> points;

		gg::Polygon* plus = ToGeosPoly(Geometry2D::CreatePlus(40.0, 20.0).movedBy(Mouse::Pos()));
		god::DistanceOp distanceOp(*star, *plus);

		gg::CoordinateSequence* ps = distanceOp.closestPoints();

		for (size_t i = 0; i < ps->size(); ++i)
		{
			const double x0 = ps->getX(i);
			const double y0 = ps->getY(i);
			points.emplace_back(x0, y0);
		}

		LineString(points).draw(1.0, Palette::Red);

		ToSivPoly(*star).drawFrame();
		ToSivPoly(*plus).drawFrame();

		delete plus;
	}

	delete star;
}