from __future__ import annotations

import typing as t

from pyrx import Db

from .point import point3d


@t.overload
def line(*points: t.Collection[t.SupportsFloat]) -> Db.Line: ...
@t.overload
def line(points: t.Collection[t.Collection[t.SupportsFloat]]) -> Db.Line: ...


def line(*points) -> Db.Line:
    """
    Return ``Db.Line`` for the given points.

    Raises:
        ValueError: incorrect number of points

    Examples:
        >>> l1 = line((0, 0), (100, 50))
        >>> l2 = line([(0, 0), (100, 50)])
        >>> l3 = Db.Line(Ge.Point3d(0.0, 0.0, 0.0), Ge.Point3d(100.0, 50.0, 0.0))
        >>> (
        ...    (l1.startPoint() == l2.startPoint() == l3.startPoint())
        ...    and (l1.endPoint() == l2.endPoint() == l3.endPoint())
        ... )
        True
    """
    if len(points) == 1 and isinstance(points[0], t.Sized) and len(points[0]) > 1 and isinstance(points[0][0], t.Sized):
        points = points[0]
    if not (count := len(points)) == 2:
        raise ValueError(f"two points must be given ({count} given)")
    points3d = (point3d(point) for point in points)
    line = Db.Line(*points3d)
    return line


@t.overload
def pline(*points: t.Collection[t.SupportsFloat]) -> Db.Polyline: ...
@t.overload
def pline(points: t.Collection[t.Collection[t.SupportsFloat]]) -> Db.Polyline: ...


def pline(*points) -> Db.Polyline:
    """
    Return ``Db.Polyline`` for the given points.

    Raises:
        ValueError: incorrect number of points

    Examples:
        >>> p1 = pline((0, 0), (100, 50), (100, 100))
        >>> p2 = pline([(0, 0), (100, 50), (100, 100)])
        >>> p3 = Db.Polyline([Ge.Point3d(0.0, 0.0, 0.0), Ge.Point3d(100.0, 50.0, 0.0), Ge.Point3d(100.0, 100.0, 0.0)])
        >>> p1.toPoint3dList() == p2.toPoint3dList() == p3.toPoint3dList()
        True
    """  # noqa: E501
    if len(points) == 1 and isinstance(points[0], t.Sized) and len(points[0]) > 1 and isinstance(points[0][0], t.Sized):
        points = points[0]
    if (count := len(points)) < 2:
        raise ValueError(f"at least two points must be given ({count} given)")
    points3d = (point3d(point) for point in points)
    pline = Db.Polyline(list(points3d))
    return pline
