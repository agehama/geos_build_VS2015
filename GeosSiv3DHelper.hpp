#pragma once

#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineSegment.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateFilter.h>
#include <geos/index/quadtree/Quadtree.h>
#include <geos/index/ItemVisitor.h>
#include <geos/geom/IntersectionMatrix.h>
#include <geos/geomgraph/PlanarGraph.h>
#include <geos/operation/linemerge/LineMergeGraph.h>
#include <geos/planargraph.h>
#include <geos/planargraph/Edge.h>
#include <geos/planargraph/Node.h>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/operation/polygonize/Polygonizer.h>
#include <geos/opBuffer.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/operation/linemerge/LineMerger.h>
#include <geos/opDistance.h>
#include <geos/operation/predicate/RectangleContains.h>
#include <geos/triangulate/VoronoiDiagramBuilder.h>

#include <Siv3D.hpp>

#ifdef _DEBUG
#pragma comment(lib, "geos_d.lib")
#else
#pragma comment(lib, "geos.lib")
#endif

namespace gg = geos::geom;
namespace gob = geos::operation::buffer;
namespace god = geos::operation::distance;
namespace gt = geos::triangulate;

inline gg::Polygon* ToGeosPoly(const Polygon& polygon)
{
	gg::CoordinateArraySequence pts;

	for (int i = static_cast<int>(polygon.num_vertices) - 1; 0 <= i; --i)
	{
		const Vec2 p = polygon.vertex(i);
		pts.add(gg::Coordinate(p.x, p.y));
	}

	if (!pts.empty())
	{
		pts.add(pts.front());
	}

	gg::GeometryFactory::unique_ptr factory = gg::GeometryFactory::create();
	return factory->createPolygon(factory->createLinearRing(pts), {});
}

inline bool IsClockWise(const std::vector<Vec2>& closedPath)
{
	double sum = 0;

	for (int i = 0; i < closedPath.size(); ++i)
	{
		const auto& p1 = closedPath[i];
		const auto& p2 = closedPath[(i + 1) % closedPath.size()];

		sum += (p2.x - p1.x)*(p2.y + p1.y);
	}

	return sum < 0.0;
}

inline Polygon ToSivPolyFromPolygon(const gg::Polygon& poly)
{
	std::vector<Vec2> points;

	const gg::LineString* outer = poly.getExteriorRing();
	for (int i = static_cast<int>(outer->getNumPoints()) - 1; 0 < i; --i)
	{
		const gg::Coordinate& p = outer->getCoordinateN(i);
		points.emplace_back(p.x, p.y);
	}

	Polygon result(points);

	for (size_t i = 0; i < poly.getNumInteriorRing(); ++i)
	{
		const gg::LineString* hole = poly.getInteriorRingN(i);

		std::vector<Vec2> holePoints;
		for (int n = static_cast<int>(hole->getNumPoints()) - 1; 0< n; --n)
		//for (int n = 0; n < hole->getNumPoints(); ++n)
		{
			gg::Point* pp = hole->getPointN(n);
			holePoints.emplace_back(pp->getX(), pp->getY());
			//const gg::Coordinate& p = hole->getCoordinateN(n);
			//holePoints.emplace_back(p.x, p.y);
		}

		result.addHole(holePoints);
	}

	return result;
}

inline MultiPolygon ToSivPoly(const gg::Geometry& poly)
{
	if (poly.getGeometryTypeId() == gg::GEOS_POLYGON)
	{
		return MultiPolygon({ ToSivPolyFromPolygon(dynamic_cast<const gg::Polygon&>(poly)) });
	}

	return{};
}

/*
inline MultiPolygon ToSivPoly(const gg::Geometry& poly)
{
	const bool isPolygon = poly.getGeometryTypeId() == gg::GEOS_POLYGON ||
		poly.getGeometryTypeId() == gg::GEOS_MULTIPOLYGON ||
		poly.getGeometryTypeId() == gg::GEOS_LINEARRING;

	gg::CoordinateSequence* ps = poly.getCoordinates();

	if (ps->size() == 0 || !isPolygon)
	{
		LOG_ERROR(L"Not Polygon");
		return{};
	}

	std::vector<Polygon> result;
	
	double beginX = 0, beginY = 0;
	std::vector<Vec2> points;

	for (int i = 0; i < ps->size(); ++i)
	{
		const double x = ps->getX(i);
		const double y = ps->getY(i);

		if (points.empty())
		{
			beginX = x;
			beginY = y;
			points.emplace_back(x, y);
		}
		else if (beginX == x && beginY == y)
		{
			std::reverse(points.begin(), points.end());

			const bool isClockWise = IsClockWise(points);
			if (isClockWise)
			{
				result.emplace_back(points);
			}
			else
			{
				if (result.empty())
				{
					std::reverse(points.begin(), points.end());
					result.emplace_back(points);
				}
				else
				{
					result.back().addHole(points);
				}
			}

			points.clear();
		}
		else
		{
			points.emplace_back(x, y);
		}
	}

	return MultiPolygon(result);
}
*/