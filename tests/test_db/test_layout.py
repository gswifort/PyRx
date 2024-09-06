import pytest

from pyrx import Db
from pyrx.db.layout import get_layouts

from . import RESOURCES


class Test_get_layouts:
    dwg_file = RESOURCES / "test_layout.dwg"

    @pytest.fixture(scope="class")
    @classmethod
    def _db(cls):
        db = Db.Database(False, True)
        db.readDwgFile(str(cls.dwg_file))
        yield db

    def test_without_model(self, _db):

        res = tuple(get_layouts(_db, model=False))
        assert len(res) == 2
        assert all(isinstance(i, Db.Layout) for i in res)
        assert res[0].getLayoutName() == "unique_layout_name_1"
        assert res[1].getLayoutName() == "unique_layout_name_2"

    def test_with_model(self, _db):
        res = tuple(get_layouts(_db, model=True))
        assert len(res) == 3
        assert all(isinstance(i, Db.Layout) for i in res)
        assert res[0].getLayoutName() == "Model"
        assert res[1].getLayoutName() == "unique_layout_name_1"
        assert res[2].getLayoutName() == "unique_layout_name_2"

    def test_default_db(self, _db):
        cur_db = Db.curDb()
        try:
            Db.setWorkingDb(_db)
            res = tuple(get_layouts(model=False))
            assert len(res) == 2
            assert all(isinstance(i, Db.Layout) for i in res)
            assert res[0].getLayoutName() == "unique_layout_name_1"
            assert res[1].getLayoutName() == "unique_layout_name_2"
        finally:
            Db.setWorkingDb(cur_db)
