from __future__ import annotations

import typing as t

from pyrx import Db
from pyrx.db.misc import get_db


def get_layouts(db: t.Optional[Db.Database] = None, model=False) -> t.Generator[Db.Layout, None, None]:
    """
    Retrieve layouts from the specified database, optionally excluding model space layouts.

    Args:
        db: The database to retrieve layouts from. Defaults to ``None``, in which case the default (working) database is used.
        model: If ``False``, exclude the layout associated with model space. Defaults to ``False``.
    """  # noqa: E501

    if db is None:
        db = get_db()
    ld = Db.Dictionary(db.layoutDictionaryId())
    layouts = (Db.Layout(id) for id in ld.asDict().values())
    if not model:
        layouts = (layout for layout in layouts if not layout.getBlockTableRecordId() == db.modelSpaceId())
    return layouts
