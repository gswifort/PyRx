import pytest

from pyrx import Db
from pyrx.db.text import get_text, set_text


class Test_get_text:
    def test_valid_text(self, db: Db.Database):
        obj = Db.Text()
        obj.setTextString("text")
        obj_id = db.addObject(obj)
        res_obj = get_text(obj)
        obj.close()
        res_id = get_text(obj_id)
        assert res_obj == res_id == "text"

    @pytest.mark.parametrize("content, expected", (pytest.param(r"{\Ktext}", "text", id="001"),))
    def test_valid_mtext(self, db: Db.Database, content: str, expected: str):
        obj = Db.MText()
        obj.setContents(content)
        obj_id = db.addObject(obj)
        res_obj = get_text(obj)
        obj.close()
        res_id = get_text(obj_id)
        assert res_obj == res_id == expected


class Test_set_text:
    def test_valid_text(self, db: Db.Database):
        obj = Db.Text()
        set_text(obj, "text")
        assert obj.textString() == "text"

        obj_id = db.addObject(obj)
        obj.close()
        set_text(obj_id, "text2")
        assert obj.textString() == "text2"

    def test_valid_mtext(self, db: Db.Database):
        obj = Db.MText()
        set_text(obj, "text")
        assert obj.text() == "text"

        obj_id = db.addObject(obj)
        obj.close()
        set_text(obj_id, "text2")
        assert obj.text() == "text2"
