//6. “Ê•ïAƒ{ƒƒmƒC•ªŠ„

#include "GeosSiv3DHelper.hpp"

void Main()
{
	double angle = 0.0;

	while (System::Update())
	{
		angle += 0.02;
		gg::Polygon* star = ToGeosPoly(Geometry2D::CreatePlus(100.0, 20.0).rotated(angle).movedBy(Window::Size()*0.5));

		gt::VoronoiDiagramBuilder builder;
		builder.setSites(*star->getCoordinates());

		gg::GeometryFactory::unique_ptr factory = gg::GeometryFactory::create();
		std::auto_ptr<gg::Geometry> edges = builder.getDiagramEdges(*factory);

		gg::CoordinateSequence* cs = edges->getCoordinates();

		ToSivPoly(*star).draw();

		for (size_t i = 0; i + 1 < cs->size(); ++i)
		{
			const double x0 = cs->getX(i);
			const double y0 = cs->getY(i);
			const double x1 = cs->getX(i + 1);
			const double y1 = cs->getY(i + 1);
			Line(x0, y0, x1, y1).draw(1.0, Palette::Cyan);
		}

		ToSivPoly(*edges->getEnvelope()).drawFrame(1.0, Palette::Cyan);

		delete star;
	}
}