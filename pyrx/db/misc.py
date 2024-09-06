import inspect
import typing as t

from pyrx import Db
from pyrx.exceptions import UnknownDbClass, cast_to_arx_exception


def get_db_object(obj_id: Db.ObjectId, /, *, open_mode: Db.OpenMode = Db.OpenMode.kForRead) -> Db.DbObject:
    """
    Retrieves a database object given its ID.

    This function retrieves an instance of a database object corresponding to the provided object ID (`Db.ObjectId`).
    The function dynamically determines the class of the object using the `objectClass` method of the ID and attempts to
    instantiate it with the specified `open_mode`. If the class cannot be found or is invalid, an `UnknownDbClass`
    exception is raised.

    Args:
        obj_id: The unique identifier of the database object. open_mode: The mode in which to open the object.

    Returns:
        Db.DbObject: The instance of the database object corresponding to the provided ID.

    Raises:
        UnknownDbClass: If the class corresponding to the ID's object class name cannot be found or is not a valid
        class.
    """
    if not isinstance(obj_id, Db.ObjectId):
        raise TypeError(f"object id must be of class Db.ObjectId, not {obj_id.__class__.__name__}")
    obj_class_name = obj_id.objectClass().name()
    _cls = getattr(Db, obj_class_name.removeprefix("AcDb"), None)
    if _cls is None or not inspect.isclass(_cls):
        raise UnknownDbClass(obj_class_name)
    with cast_to_arx_exception:
        obj = _cls(obj_id, open_mode)
    return obj


def iter_db_objects(
    obj_ids: t.Iterable[Db.ObjectId],
    /,
    *,
    open_mode: Db.OpenMode = Db.OpenMode.kForRead,
) -> t.Generator[Db.DbObject, None, None]:
    """Retrieves a database object given its ID.

    This function retrieves an instance of a database object corresponding to the provided object ID (`Db.ObjectId`).
    The function dynamically determines the class of the object using the `objectClass` method of the ID and attempts to
    instantiate it with the specified `open_mode`. If the class cannot be found or is invalid, an `UnknownDbClass`
    exception is raised.

    Args:
        obj_ids: The unique identifiers of the database objects. open_mode: The mode in which to open the objects.

    Returns:
        Db.DbObject: The instances of the database objects corresponding to the provided IDs.

    Raises:
        UnknownDbClass: If the class corresponding to the ID's object class name cannot be found or is not a valid
        class.
    """
    for obj_id in obj_ids:
        yield get_db_object(obj_id, open_mode=open_mode)


def get_db() -> Db.Database:
    """Return default (working) database."""
    return Db.workingDb()
