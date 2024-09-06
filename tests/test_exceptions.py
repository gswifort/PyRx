from __future__ import annotations

import re
from unittest.mock import patch

import pytest

from pyrx import Db, Ed
from pyrx.exceptions import (
    ARXException,
    ARXExceptionTranslator,
    PromptStatus,
    cast_to_arx_exception,
    eRejected,
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


class TestPromptStatus:
    def test_subclass_with_existing_err_num_raise_TypeError(self):
        class _Test(PromptStatus):
            err_num = 1

        with pytest.raises(TypeError, match=re.escape("PromptStatus subclass with err_num=1 already exists")):

            class _Test2(PromptStatus):
                err_num = 1

    def test_raise_for_status_with_eOk_return_None(self):
        status = Ed.PromptStatus.eOk
        assert PromptStatus.raise_for_status(status) is None

    def test_raise_for_status_with_not_eOk_raise_PromptStatus(self):
        status = Ed.PromptStatus.eRejected
        with pytest.raises(eRejected):
            PromptStatus.raise_for_status(status)

    def test_raise_for_status_with_not_known_err_num_raise_PromptStatus(self):
        subclasses = dict(PromptStatus._subclasses)
        status = Ed.PromptStatus.eRejected
        subclasses.pop(int(status))
        with patch.object(PromptStatus, "_subclasses", subclasses):
            with pytest.raises(PromptStatus, match="eRejected") as exc_info:
                PromptStatus.raise_for_status(status)
        assert exc_info.type is PromptStatus

    def test_not_a_PromptStatus_raise_TypeError(self):
        with pytest.raises(TypeError, match="status must be of type Ed.PromptStatus, not str"):
            PromptStatus.raise_for_status("eRejected")
