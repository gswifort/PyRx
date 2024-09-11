import pytest

from pyrx.utils.doc_utils import (
    Parameter,
    Signature,
    get_docstring_sections,
    parse_docstring,
    parse_params,
)


class Test_parse_args:
    @pytest.mark.parametrize(
        "docstring, expected",
        (
            pytest.param("self, ", (Parameter("self"),), id="001"),
            pytest.param(
                "origin: PyGe.Point3d, xAxis: PyGe.Vector3d, yAxis : PyGe.Vector3d",
                (
                    Parameter("origin", "PyGe.Point3d"),
                    Parameter("xAxis", "PyGe.Vector3d"),
                    Parameter("yAxis", "PyGe.Vector3d"),
                ),
                id="002",
            ),
            pytest.param(
                "self, val: str|PyDb.ObjectId, dosubents : bool=True, allowHiddenLayer : bool=False",
                (
                    Parameter("self"),
                    Parameter("val", "str|PyDb.ObjectId"),
                    Parameter("dosubents", "bool", "True"),
                    Parameter("allowHiddenLayer", "bool", "False"),
                ),
                id="003",
            ),
        ),
    )
    def test_valid(self, docstring, expected):
        res = tuple(parse_params(docstring))
        assert res == expected


class Test_parse_docstring:
    @pytest.mark.parametrize(
        "docstring, expected",
        (
            pytest.param(  # 1 → → →
                """
setUcs( (AbstractViewTableRecord)arg1, (Point3d)arg2, (Vector3d)arg3, (Vector3d)arg4) -> None :

     C++ signature :
         void setUcs(class PyDbAbstractViewTableRecord {lvalue},class ZcGePoint3d,class ZcGeVector3d,class ZcGeVector3d)

 setUcs( (AbstractViewTableRecord)arg1, (OrthographicView)arg2) -> None :

     C++ signature :
         void setUcs(class PyDbAbstractViewTableRecord {lvalue},enum ZcDb::OrthographicView)

 setUcs( (AbstractViewTableRecord)arg1, (ObjectId)arg2) -> None :
     ![(self, )]!<[(Overloads:
     - origin: PyGe.Point3d, xAxis: PyGe.Vector3d, yAxis : PyGe.Vector3d
     - view: PyDb.OrthographicView
     - ucsId: PyDb.ObjectId
     )]><[{-1}]>

     C++ signature :
         void setUcs(class PyDbAbstractViewTableRecord {lvalue},class PyDbObjectId)
 Db.BlockTableRecord.getBinaryData""",  # docstring
                (  # expected →
                    (  # signatures →
                        Signature(
                            Parameter("self"),
                            Parameter("origin", "PyGe.Point3d"),
                            Parameter("xAxis", "PyGe.Vector3d"),
                            Parameter("yAxis", "PyGe.Vector3d"),
                        ),
                        Signature(Parameter("self"), Parameter("view", "PyDb.OrthographicView")),
                        Signature(Parameter("self"), Parameter("ucsId", "PyDb.ObjectId")),
                    ),  # ← signatures
                    "-1",  # id
                ),  # ← expected
                id="001",
            ),  # ← ← ← 1
            pytest.param(  # 2 → → →
                """
intersectWith( (Entity)arg1, (Entity)arg2, (Intersect)arg3) -> list :
    C++ signature :
        class boost::python::list intersectWith(class PyDbEntity {lvalue},class PyDbEntity,enum ZcDb::Intersect)
intersectWith( (Entity)arg1, (Entity)arg2, (Intersect)arg3, (int)arg4, (int)arg5) -> list :
    C++ signature :
        class boost::python::list intersectWith(class PyDbEntity {lvalue},class PyDbEntity,enum ZcDb::Intersect,__int64,__int64)
intersectWith( (Entity)arg1, (Entity)arg2, (Intersect)arg3, (Plane)arg4) -> list :
    C++ signature :
        class boost::python::list intersectWith(class PyDbEntity {lvalue},class PyDbEntity,enum ZcDb::Intersect,class PyGePlane)
intersectWith( (Entity)arg1, (Entity)arg2, (Intersect)arg3, (Plane)arg4, (int)arg5, (int)arg6) -> list :
    ![(self, )]!<[(Overloads:
    - entity: PyDb.Entity, intType : PyDb.Intersect
    - entity: PyDb.Entity, intType : PyDb.Intersect, thisGsMarker : int, otherGsMarker : int
    - entity: PyDb.Entity, intType : PyDb.Intersect, plane : PyGe.Plane
    - entity: PyDb.Entity, intType : PyDb.Intersect, plane : PyGe.Plane, thisGsMarker : int, otherGsMarker : int
    )]><[{4324}]>
    C++ signature :
        class boost::python::list intersectWith(class PyDbEntity {lvalue},class PyDbEntity,enum ZcDb::Intersect,class PyGePlane,__int64,__int64)
                """,  # docstring
                (  # expected →
                    (  # signatures →
                        Signature(
                            Parameter("self"),
                            Parameter("entity", "PyDb.Entity"),
                            Parameter("intType", "PyDb.Intersect"),
                        ),
                        Signature(
                            Parameter("self"),
                            Parameter("entity", "PyDb.Entity"),
                            Parameter("intType", "PyDb.Intersect"),
                            Parameter("thisGsMarker", "int"),
                            Parameter("otherGsMarker", "int"),
                        ),
                        Signature(
                            Parameter("self"),
                            Parameter("entity", "PyDb.Entity"),
                            Parameter("intType", "PyDb.Intersect"),
                            Parameter("plane", "PyGe.Plane"),
                        ),
                        Signature(
                            Parameter("self"),
                            Parameter("entity", "PyDb.Entity"),
                            Parameter("intType", "PyDb.Intersect"),
                            Parameter("plane", "PyGe.Plane"),
                            Parameter("thisGsMarker", "int"),
                            Parameter("otherGsMarker", "int"),
                        ),
                    ),  # ← signatures
                    "4324",  # id
                ),  # ← expected
                id="002",
            ),  # ← ← ← 2
            pytest.param(  # 3 → → →
                """
wblockClone( (DbObject)arg1, (RxObject)arg2, (IdMapping)arg3) -> DbObject :
    C++ signature :
        class PyDbObject wblockClone(class PyDbObject {lvalue},class PyRxObject {lvalue},class PyDbIdMapping {lvalue})
wblockClone( (DbObject)arg1, (RxObject)arg2, (IdMapping)arg3, (bool)arg4) -> DbObject :
    ![(self, owner: PyRx.RxObject, mapping: PyDb.IdMapping, isPrimary:bool=True)]!<[{7253}]>
    C++ signature :
        class PyDbObject wblockClone(class PyDbObject {lvalue},class PyRxObject {lvalue},class PyDbIdMapping {lvalue},bool)
                """,  # docstring
                (  # expected →
                    (  # signatures →
                        Signature(
                            Parameter("self"),
                            Parameter("owner", "PyRx.RxObject"),
                            Parameter("mapping", "PyDb.IdMapping"),
                            Parameter("isPrimary", "bool", "True"),
                        ),
                    ),  # ← signatures
                    "7253",  # id
                ),  # ← expected
                id="003",
            ),  # ← ← ← 3
        ),
    )
    def test_valid(self, docstring, expected):
        res = parse_docstring(docstring)
        assert res == expected


class Test_get_docstring_sections:
    @pytest.mark.parametrize(
        "docstring, expected",
        (
            pytest.param(
                """
setUcs( (AbstractViewTableRecord)arg1, (Point3d)arg2, (Vector3d)arg3, (Vector3d)arg4) -> None :
    C++ signature :
        void setUcs(class PyDbAbstractViewTableRecord {lvalue},class ZcGePoint3d,class ZcGeVector3d,class ZcGeVector3d)
setUcs( (AbstractViewTableRecord)arg1, (OrthographicView)arg2) -> None :
    C++ signature :
        void setUcs(class PyDbAbstractViewTableRecord {lvalue},enum ZcDb::OrthographicView)
setUcs( (AbstractViewTableRecord)arg1, (ObjectId)arg2) -> None :
    ![(self, )]!<[(Overloads:
    - origin: PyGe.Point3d, xAxis: PyGe.Vector3d, yAxis : PyGe.Vector3d
    - view: PyDb.OrthographicView
    - ucsId: PyDb.ObjectId
    )]><[{-1}]>
    C++ signature :
        void setUcs(class PyDbAbstractViewTableRecord {lvalue},class PyDbObjectId)
                """,
                (
                    "self, ",  # base_signature
                    """
                    - origin: PyGe.Point3d, xAxis: PyGe.Vector3d, yAxis : PyGe.Vector3d
                    - view: PyDb.OrthographicView
                    - ucsId: PyDb.ObjectId
                    """,  # overloads
                    "-1",  # docstring_id
                ),
                id="001",
            ),
            pytest.param(
                """
getBinaryData( (DbObject)arg1, (str)arg2) -> object :
    ![(self, key: str)]!<[{-1}]>
    C++ signature :
        class boost::python::api::object getBinaryData(class PyDbObject {lvalue},class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >)
            """,
                (
                    "self, key: str",  # base_signature
                    None,  # overloads
                    "-1",  # docstring_id
                ),
                id="002",
            ),
        ),
    )
    def test_valid(self, docstring: str, expected: tuple[str, str, str] | None):
        res = get_docstring_sections(docstring)
        expected = tuple(i.replace(" ", "").strip() if i is not None else None for i in expected)
        assert tuple(res) == expected


if __name__ == "__main__":
    pytest.main([f"{__file__}"])
