from __future__ import annotations

import typing as t

from pyrx import Ge


@t.overload
def point3d(
    x: t.SupportsFloat, y: t.SupportsFloat, z: t.SupportsFloat = 0.0, /
) -> Ge.Point3d: ...
@t.overload
def point3d(*coords: t.SupportsFloat) -> Ge.Point3d: ...


def point3d(*args) -> Ge.Point3d:
    """
    Return ``Ge.Point3d`` for the given coordinates.

    Raises:
        ValueError: incorrect number of coordinates

    Examples:
        >>> p1 = point3d(100, 50)
        >>> p2 = point3d([100, 50])
        >>> p3 = Ge.Point3d(100.0, 50.0, 0.0)
        >>> p1 == p2 == p3
        True
    """
    length = len(args)
    if length == 1 and isinstance(args[0], t.Sized):
        args = args[0]
        length = len(args)
    if length < 2:
        raise ValueError(
            f"at least two coordinates (x, y) must be given ({length} given)"
        )
    if length == 2:
        x, y = (float(i) for i in args)
        z = 0.0
    elif length == 3:
        x, y, z = (float(i) for i in args)
    else:
        raise ValueError(
            f"a maximum of three coordinates must be given ({length} given)"
        )
    return Ge.Point3d(x, y, z)
