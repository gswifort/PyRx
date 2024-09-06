from __future__ import annotations

import pytest

from pyrx import Db
from pyrx.db.viewport import get_viewports
from pyrx.exceptions import LayoutDoesNotExist

from . import RESOURCES


class Test_get_viewports:
    dwg_file = RESOURCES / "test_viewports.dwg"

    @pytest.fixture(scope="class")
    @classmethod
    def _data(cls):
        db = Db.Database(False, True)
        db.readDwgFile(str(cls.dwg_file), Db.DatabaseOpenMode.kForReadAndAllShare, False, "")
        ld = Db.Dictionary(db.layoutDictionaryId())
        layout = Db.Layout(ld.getAt("Layout1"))
        yield db, layout

    def test_without_paperspace(self, _data: tuple[Db.Database, Db.Layout]):
        db, layout = _data
        res = tuple(get_viewports(layout, db, paperspace=False))
        assert len(res) == 3

    def test_with_paperspace(self, _data: tuple[Db.Database, Db.Layout]):
        db, layout = _data
        res = tuple(get_viewports(layout, db, paperspace=True))
        assert len(res) == 4

    def test_default_db_and_layout(self, _data: tuple[Db.Database, Db.Layout]):
        db, layout = _data
        cur_db = Db.curDb()
        try:
            Db.setWorkingDb(db)
            lm = Db.LayoutManager()
            assert lm.getActiveLayoutName(True) == "additional_layout"
            res = tuple(get_viewports(paperspace=False))
        finally:
            Db.setWorkingDb(cur_db)
        assert len(res) == 0

    def test_str_layout(self, _data: tuple[Db.Database, Db.Layout]):
        LAYOUT_NAME = "Layout1"
        db, layout = _data
        assert layout.getLayoutName() == LAYOUT_NAME
        res = tuple(get_viewports(LAYOUT_NAME, db, paperspace=False))
        assert len(res) == 3

    def test_id_layout(self, _data: tuple[Db.Database, Db.Layout]):
        db, layout = _data
        layout_id = layout.objectId()
        res = tuple(get_viewports(layout_id, db, paperspace=False))
        assert len(res) == 3

    def test_not_existing_str_layout_raise_LayoutDoesNotExist(self, _data: tuple[Db.Database, Db.Layout]):
        LAYOUT_NAME = "non_existent"
        db, layout = _data
        with pytest.raises(LayoutDoesNotExist, match=LAYOUT_NAME):
            tuple(get_viewports(LAYOUT_NAME, db, paperspace=False))
