from __future__ import annotations

from pyrx import Db
from pyrx.db.misc import get_db_object
from pyrx.exceptions import cast_to_arx_exception, eWasOpenForWrite


def get_text(text: Db.Text | Db.MText | Db.ObjectId, /) -> str:
    """Return the value of `Db.Text` or `Db.MText`"""
    if isinstance(text, Db.ObjectId):
        text = get_db_object(text)
    if isinstance(text, Db.Text):
        return text.textString()
    elif isinstance(text, Db.MText):
        return text.text()
    else:
        raise TypeError


def set_text(text: Db.Text | Db.MText | Db.ObjectId, value: str) -> None:
    """Set the value of `Db.Text` or `Db.MText`"""
    if isinstance(text, Db.ObjectId):
        text = get_db_object(text)
    try:
        with cast_to_arx_exception:
            text.upgradeOpen()
    except eWasOpenForWrite:
        pass
    if isinstance(text, Db.Text):
        return text.setTextString(value)
    elif isinstance(text, Db.MText):
        return text.setContents(value)
    else:
        raise TypeError
