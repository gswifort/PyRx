from __future__ import annotations

import typing as t

from pyrx import Db
from pyrx.db import get_db
from pyrx.exceptions import LayoutDoesNotExist, cast_to_arx_exception, eKeyNotFound


def get_viewports(
    layout: Db.ObjectId | Db.Layout | str | None = None, db: t.Optional[Db.Database] = None, paperspace=False
) -> t.Generator[Db.Viewport, None, None]:
    """
    Generate viewports for a given layout in a database.

    If a layout is provided, retrieve its viewports from the database. If no layout is specified, retrieve the viewports
    for the active layout. The function supports various layout inputs, including `Db.ObjectId`, `Db.Layout`, and layout
    names as strings. It defaults to the active layout if no layout is specified. By default, model space viewports
    are returned unless `paperspace` is set to True, in which case all viewports are returned.

    Args:
        layout: Layout identifier, which can be a ``Db.ObjectId``, ``Db.Layout``, layout name as a string, or ``None`` to use the active layout.
        db: Database instance to retrieve layout information from. Defaults to working database if not provided.
        paperspace: Whether to return all viewports, including paperspace viewports. Defaults to False.

    Yields:
        Db.Viewport: Viewport objects from the specified layout.

    Raises:
        LayoutDoesNotExist: If the layout name provided as a string does not exist in the database.
    """  # noqa: E501
    if db is None:
        db = get_db()
    lm = Db.LayoutManager()
    ld = Db.Dictionary(db.layoutDictionaryId())
    if isinstance(layout, Db.Layout):
        _layout = layout
    elif layout is None:
        name = lm.getActiveLayoutName(True, db)
        _layout = Db.Layout(ld.getAt(name))
    elif isinstance(layout, str):
        try:
            with cast_to_arx_exception:
                _layout = Db.Layout(ld.getAt(layout))
        except eKeyNotFound:
            raise LayoutDoesNotExist(layout) from None
    elif isinstance(layout, Db.ObjectId):
        with cast_to_arx_exception:
            _layout = Db.Layout(layout)

    viewports_ids = _layout.getViewportArray()
    return (Db.Viewport(id) for id in (viewports_ids if paperspace else viewports_ids[1:]))
