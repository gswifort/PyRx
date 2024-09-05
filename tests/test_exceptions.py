from __future__ import annotations

import re

import pytest

from pyrx import Db
from pyrx.exceptions import (
    ARXException,
    ARXExceptionTranslator,
    cast_to_arx_exception,
    eWasOpenForWrite,
)


class TestARXExceptionTranslator:
    def test_singleton(self):
        assert ARXExceptionTranslator() is ARXExceptionTranslator()

    def test_context_manager(self, db: Db.Database):
        text = Db.Text()
        text_id = db.addToModelspace(text)
        with pytest.raises(eWasOpenForWrite):
            with cast_to_arx_exception:
                Db.Text(text_id, Db.OpenMode.kForRead)


class TestARXException:
    def test_subclass_with_existing_err_name_raise_TypeError(self):
        class _Test(ARXException):
            err_name = "TEST"

        with pytest.raises(TypeError, match=re.escape("ARXException subclass with err_name='TEST' already exists")):

            class _Test2(ARXException):
                err_name = "TEST"
