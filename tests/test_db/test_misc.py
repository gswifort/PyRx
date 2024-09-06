import re

import pytest

from pyrx import Db
from pyrx.db.misc import get_db_object, iter_db_objects


class Test_get_db_object:
    def test_valid(self, db: Db.Database):
        mtext = Db.MText()
        text = Db.Text()
        line = Db.Line()
        pline = Db.Polyline()

        mtext_id = db.addObject(mtext)
        text_id = db.addObject(text)
        line_id = db.addObject(line)
        pline_id = db.addObject(pline)

        mtext.close()
        text.close()
        line.close()
        pline.close()

        mtext_res = get_db_object(mtext_id)
        text_res = get_db_object(text_id)
        line_res = get_db_object(line_id)
        pline_res = get_db_object(pline_id)

        assert type(mtext_res) is Db.MText
        assert type(text_res) is Db.Text
        assert type(line_res) is Db.Line
        assert type(pline_res) is Db.Polyline

    def test_open_mode(self, db: Db.Database):
        obj = Db.Line()
        obj_id = db.addObject(obj)
        obj.close()

        res = get_db_object(obj_id, open_mode=Db.OpenMode.kForWrite)
        assert type(res) is Db.Line
        assert res.isWriteEnabled()
        res.close()

        res2 = get_db_object(obj_id, open_mode=Db.OpenMode.kForRead)
        assert type(res2) is Db.Line
        assert res2.isReadEnabled()
        assert not res2.isWriteEnabled()

    def test_not_a_ObjectId_raise_TypeError(self):
        with pytest.raises(
            TypeError,
            match=re.escape("object id must be of class Db.ObjectId, not MText"),
        ):
            get_db_object(Db.MText())


class Test_iter_db_objects:
    def test_valid(self, db: Db.Database):
        mtext = Db.MText()
        text = Db.Text()
        line = Db.Line()
        pline = Db.Polyline()

        mtext_id = db.addObject(mtext)
        text_id = db.addObject(text)
        line_id = db.addObject(line)
        pline_id = db.addObject(pline)

        mtext.close()
        text.close()
        line.close()
        pline.close()

        res = iter_db_objects((mtext_id, text_id, line_id, pline_id))
        assert type(next(res)) is Db.MText
        assert type(next(res)) is Db.Text
        assert type(next(res)) is Db.Line
        assert type(next(res)) is Db.Polyline

    def test_open_mode(self, db: Db.Database):
        obj_1, obj_2 = Db.Line(), Db.Line()
        obj_id_1, obj_id_2 = db.addObject(obj_1), db.addObject(obj_2)
        obj_1.close()
        obj_2.close()

        res = iter_db_objects((obj_id_1, obj_id_2), open_mode=Db.OpenMode.kForWrite)
        for item in res:
            assert type(item) is Db.Line
            assert item.isWriteEnabled()
            item.close()

        res2 = iter_db_objects((obj_id_1, obj_id_2), open_mode=Db.OpenMode.kForRead)
        for item in res2:
            assert type(item) is Db.Line
            assert item.isReadEnabled()
            assert not item.isWriteEnabled()
            item.close()
