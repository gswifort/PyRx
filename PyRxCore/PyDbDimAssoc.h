#pragma once
#include "PyDbObject.h"
#include "dbdimassoc.h"
#include "dbdimptref.h"

#pragma pack (push, 8)
class PyDbObjectId;
class PyDbIdMapping;
class PyDbPointRef;
class PyDbOsnapPointRef;

//-------------------------------------------------------------------------------------------------------------
//PyDbDimAssoc
void makePyDbDimAssocWrapper();


class PyDbDimAssoc : public PyDbObject
{
public:

    PyDbDimAssoc();
    PyDbDimAssoc(const PyDbObjectId& id);
    PyDbDimAssoc(const PyDbObjectId& id, AcDb::OpenMode mode);
    PyDbDimAssoc(const PyDbObjectId& id, AcDb::OpenMode mode, bool erased);
    PyDbDimAssoc(AcDbDimAssoc* ptr, bool autoDelete);
    virtual ~PyDbDimAssoc() override = default;
    PyDbObjectId            dimObjId() const;
    void                    setDimObjId(const PyDbObjectId& dimId);
    void                    setAssocFlag1(int ptType, bool value);
    void                    setAssocFlag2(int assocFlg);
    bool                    assocFlag2(int ptType) const;
    int                     assocFlag1(void);
    void                    setPointRef(int ptType, PyDbPointRef& ptRef);
    PyDbPointRef            pointRef(int ptType) const;
    PyDbOsnapPointRef       osnapPointRef(int ptType) const;
    void                    setRotatedDimType(AcDbDimAssoc::RotatedDimType dimType);
    AcDbDimAssoc::RotatedDimType rotatedDimType() const;
    void                    addToPointRefReactor();
    void                    addToDimensionReactor1();
    void                    addToDimensionReactor2(bool isAdd);
    void                    removePointRef(int ptType);
    void                    updateDimension1();
    void                    updateDimension2(bool update);
    void                    updateDimension3(bool update, bool skipReactors);
    void                    removeAssociativity1();
    void                    removeAssociativity2(bool force);
    bool                    isTransSpatial() const;
    void                    setTransSpatial(bool value);
    void                    startCmdWatcher();
    void                    startOopsWatcher1();
    void                    startOopsWatcher2(bool bAddAll);
    void                    removeOopsWatcher(void);
    void                    restoreAssocFromOopsWatcher(void);
    bool                    hasOopsWatcher(void) const;
    PyDbObjectId            post1(const PyDbObjectId& dimId);
    PyDbObjectId            post2(const PyDbObjectId& dimId, bool isActive);
    boost::python::list     getDimAssocGeomIds() const;
    bool                    isAllGeomErased() const;
    void                    swapReferences(const PyDbIdMapping& idMap);
    void                    updateFillet(const boost::python::list& ids);
    void                    updateAssociativity(const boost::python::list& ids);
    void                    updateXrefSubentPath();
    void                    updateSubentPath(PyDbIdMapping& idMap);
    void                    updateDueToMirror(bool wasInMirror);

    static PyRxClass      desc();
    static std::string    className();
    static PyDbDimAssoc   cloneFrom(const PyRxObject& src);
    static PyDbDimAssoc   cast(const PyRxObject& src);
public:
    AcDbDimAssoc* impObj(const std::source_location& src = std::source_location::current()) const;
};
#pragma pack (pop)