//4. Ü‚êü‚©‚çƒ|ƒŠƒSƒ“‚Ì¶¬

#include "GeosSiv3DHelper.hpp"

void Main()
{
	gg::CoordinateArraySequence points;
	std::vector<LineString> lines;

	while (System::Update())
	{
		if (Input::MouseL.clicked)
		{
			points.add(gg::Coordinate(Mouse::Pos().x, Mouse::Pos().y));
		}

		if (Input::MouseL.pressed && !points.empty() && 30.0 < points.back().distance(gg::Coordinate(Mouse::Pos().x, Mouse::Pos().y)))
		{
			points.add(gg::Coordinate(Mouse::Pos().x, Mouse::Pos().y));
		}

		if (Input::MouseL.released)
		{
			gg::GeometryFactory::unique_ptr factory = gg::GeometryFactory::create();

			gg::PrecisionModel model(gg::PrecisionModel::Type::FLOATING);

			gob::BufferParameters param(10, gob::BufferParameters::CAP_ROUND, gob::BufferParameters::JOIN_BEVEL, 10.0);
			//gob::BufferParameters param(10, gob::BufferParameters::CAP_FLAT, gob::BufferParameters::JOIN_BEVEL, 10.0);

			gob::OffsetCurveBuilder builder(&model, param);

			std::vector<gg::CoordinateSequence*> result;

			builder.getLineCurve(&points, 15.0, result);
			//builder.getSingleSidedLineCurve(&points, 10.0, result, true, false);

			for (gg::CoordinateSequence* p : result)
			{
				std::vector<Vec2> ps;
				for (size_t i = 0; i < p->size(); ++i)
				{
					ps.emplace_back(p->getX(i), p->getY(i));
				}

				lines.emplace_back(ps);
			}

			points.clear();
		}

		if (!points.empty())
		{
			for (size_t i = 0; i+1 < points.size(); ++i)
			{
				const double x0 = points.getX(i);
				const double y0 = points.getY(i);
				const double x1 = points.getX(i + 1);
				const double y1 = points.getY(i + 1);
				Line(x0, y0, x1, y1).draw();
			}
		}

		for (const auto& l : lines)
		{
			l.draw();
		}
	}
}