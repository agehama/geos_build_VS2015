//1. 図形の接触判定

#include "GeosSiv3DHelper.hpp"

void Main()
{
	Font font(30);

	gg::Polygon* star = ToGeosPoly(Geometry2D::CreateNStar(5, 100, 50).movedBy(Window::Size()*0.5));

	while (System::Update())
	{
		gg::Polygon* plus = ToGeosPoly(Geometry2D::CreatePlus(30.0, 10.0).movedBy(Mouse::Pos()));

		gg::IntersectionMatrix* relation = star->relate(*plus);

		font(CharacterSet::Widen(relation->toString())).draw();

		ToSivPoly(*star).draw(Color(255, 0, 0));
		ToSivPoly(*plus).draw(Color(0, 255, 0));

		delete plus;
		delete relation;
	}

	delete star;
}