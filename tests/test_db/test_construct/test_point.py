import re

import pytest

from pyrx import Ge
from pyrx.db.construct.point import point3d


class Test_point3d:
    @pytest.mark.parametrize(
        "args, expected",
        (
            ((0, 0), Ge.Point3d(0.0, 0.0, 0.0)),
            ((0, 0.1), Ge.Point3d(0.0, 0.1, 0.0)),
            ((1, 0), Ge.Point3d(1.0, 0.0, 0.0)),
            ((1, 0, -5.34), Ge.Point3d(1.0, 0.0, -5.34)),
        ),
    )
    def test_valid_xyz(self, args, expected):
        res = point3d(*args)
        assert res == expected

    @pytest.mark.parametrize(
        "args, expected",
        (
            ((0, 0), Ge.Point3d(0.0, 0.0, 0.0)),
            ((0, 0.1), Ge.Point3d(0.0, 0.1, 0.0)),
            ((1, 0), Ge.Point3d(1.0, 0.0, 0.0)),
            ((1, 0, -5.34), Ge.Point3d(1.0, 0.0, -5.34)),
        ),
    )
    def test_valid_coords(self, args, expected):
        res = point3d(args)
        assert res == expected

    @pytest.mark.parametrize(
        "args, given",
        (
            ((0.0,), 1),
            ((), 0),
        ),
    )
    def test_xyz_too_few_coords_raise_ValueError(self, args, given):
        with pytest.raises(
            ValueError,
            match=re.escape(f"at least two coordinates (x, y) must be given ({given} given)"),
        ):
            point3d(*args)

    @pytest.mark.parametrize(
        "args, given",
        (
            ((0.0,), 1),
            ((), 0),
        ),
    )
    def test_coords_too_few_coords_raise_ValueError(self, args, given):
        with pytest.raises(
            ValueError,
            match=re.escape(f"at least two coordinates (x, y) must be given ({given} given)"),
        ):
            point3d(args)

    @pytest.mark.parametrize(
        "args, given",
        (((0.0, 1.0, 2.0, 3.0), 4),),
    )
    def test_xyz_too_many_coords_raise_ValueError(self, args, given):
        with pytest.raises(
            ValueError,
            match=re.escape(f"a maximum of three coordinates must be given ({given} given)"),
        ):
            point3d(*args)

    @pytest.mark.parametrize(
        "args, given",
        (((0.0, 1.0, 2.0, 3.0), 4),),
    )
    def test_coords_too_many_coords_raise_ValueError(self, args, given):
        with pytest.raises(
            ValueError,
            match=re.escape(f"a maximum of three coordinates must be given ({given} given)"),
        ):
            point3d(args)
