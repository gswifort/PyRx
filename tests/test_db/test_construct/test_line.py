import re

import pytest

from pyrx import Db, Ge
from pyrx.db.construct import line, pline


class Test_line:
    @pytest.mark.parametrize(
        "args, expected",
        (
            pytest.param(
                ((0, 0), (100, 50)),
                Db.Line(Ge.Point3d(0.0, 0.0, 0.0), Ge.Point3d(100.0, 50.0, 0.0)),
                id="001",
            ),
            pytest.param(
                ((100.2, 500.852, 0.663), (1000523, 5052.963, 0.585)),
                Db.Line(
                    Ge.Point3d(100.2, 500.852, 0.663),
                    Ge.Point3d(1000523.0, 5052.963, 0.585),
                ),
                id="002",
            ),
        ),
    )
    def test_valid(self, args, expected: Db.Line):
        res_unpack = line(*args)
        res_pack = line(args)
        assert res_pack.startPoint() == res_unpack.startPoint() == expected.startPoint()
        assert res_pack.endPoint() == res_unpack.endPoint() == expected.endPoint()

    @pytest.mark.parametrize(
        "args, given",
        (
            pytest.param(((0, 0),), 1, id="001"),
            pytest.param(((0, 0), (100, 50), (200, 0)), 3, id="002"),
        ),
    )
    def test_invalid_points_count_raise_ValueError(self, args, given):
        with pytest.raises(ValueError, match=re.escape(f"two points must be given ({given} given)")):
            line(*args)


class Test_pline:
    @pytest.mark.parametrize(
        "args, expected",
        (
            pytest.param(
                ((0, 0), (100, 50), (50.3, 45.8, 12.334)),
                Db.Polyline(
                    [
                        Ge.Point3d(0.0, 0.0, 0.0),
                        Ge.Point3d(100.0, 50.0, 0.0),
                        Ge.Point3d(50.3, 45.8, 12.334),
                    ]
                ),
                id="001",
            ),
            pytest.param(
                (
                    (100.2, 500.852, 0.663),
                    (1000523, 5052.963, 0.585),
                    (120.0, 250.0),
                ),
                Db.Polyline(
                    [
                        Ge.Point3d(100.2, 500.852, 0.663),
                        Ge.Point3d(1000523.0, 5052.963, 0.585),
                        Ge.Point3d(120.0, 250.0, 0.0),
                    ]
                ),
                id="002",
            ),
        ),
    )
    def test_valid(self, args, expected: Db.Polyline):
        res_unpack = pline(*args)
        res_pack = pline(args)
        assert res_pack.toPoint3dList() == res_unpack.toPoint3dList() == expected.toPoint3dList()

    @pytest.mark.parametrize(
        "args, given",
        (
            pytest.param(((0, 0),), 1, id="001"),
            pytest.param((), 0, id="002"),
        ),
    )
    def test_invalid_points_count_raise_ValueError(self, args, given):
        with pytest.raises(
            ValueError,
            match=re.escape(f"at least two points must be given ({given} given)"),
        ):
            pline(*args)
