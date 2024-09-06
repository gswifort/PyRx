from __future__ import annotations

import typing as t
from functools import partial

from pyrx import Db, Ed, Rx
from pyrx.db.misc import get_db_object, iter_db_objects
from pyrx.exceptions import PromptStatus, cast_to_arx_exception

if t.TYPE_CHECKING:
    from pyrx import Ge


@t.overload
def entsel(*types: type[Rx.RxClass] | type[Db.DbObject], prompt: str, point: t.Literal[False]) -> Db.DbObject: ...


@t.overload
def entsel(
    *types: type[Rx.RxClass] | type[Db.DbObject], prompt: str, point: t.Literal[True]
) -> tuple[Db.DbObject, Ge.Point3d]: ...


def entsel(*types: type[Rx.RxClass] | type[Db.DbObject], prompt="Select object: ", point=False):  # TODO: tests
    """
    Select an entity on the screen.

    Filters the object type if ``types`` is given. Raises subclasses of ``PromptStatus``
    if the command was canceled or an object of the inappropriate type was selected.

    Examples:
        >>> entsel(Db.Line, Db.Polyline)
        <PyDb.Polyline object at 0x00000250DAE7D670>

        >>> entsel(Db.Line, Db.Polyline, point=True)
        (
            <PyDb.Line object at 0x00000250DB0F1030>,
            <PyGe.Point3d(497.58106138402866,335.24394203853723,0.00000000000000)>
        )

        >>> entsel(Db.Line, Db.Polyline)
        # user terminates command
        Traceback (most recent call last):
        (...)
        pyrx.exceptions.eRejected
    """
    args = [prompt]
    if types:
        args.append([i.desc() if not isinstance(i, Rx.RxClass) else i for i in types])

    with cast_to_arx_exception:
        status, ent_id, pick_point = Ed.Editor.entSel(*args)
    PromptStatus.raise_for_status(status)
    obj = get_db_object(ent_id)
    if point:
        return obj, pick_point
    else:
        return obj


def select(
    *types: type[Rx.RxClass] | type[Db.DbObject], prompt: t.Optional[str] = None
) -> t.Generator[Db.DbObject, None, None]:  # TODO: tests
    """
    Select an entities on the screen.

    Filters the object type if ``types`` is given. Raises subclasses of ``PromptStatus``
    if the command was canceled or an object of the inappropriate type was selected.

    Examples:
        >>> tuple(select(Db.Line, Db.Polyline))
        (
            <PyDb.Line object at 0x00000250DAEF18A0>,
            <PyDb.Polyline object at 0x00000250DAEF1800>,
            <PyDb.Line object at 0x00000250DB0F0450>
        )
    """
    func = Ed.Editor.select if prompt is None else partial(Ed.Editor.selectPrompt, prompt, "")
    types = [i.desc() if not isinstance(i, Rx.RxClass) else i for i in types]
    filter = [(Db.DxfCode.kDxfStart, ",".join(_type.dxfName() for _type in types))]

    with cast_to_arx_exception:
        status, sset = func(filter)
    PromptStatus.raise_for_status(status)
    return iter_db_objects(sset.objectIds())
