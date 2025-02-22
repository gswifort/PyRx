#pragma once
#include "PyAcadDbObjectImpl.h"

//------------------------------------------------------------------------------------
//PyIAcadSubEntityImpl
#ifndef _GRXTARGET
class PyIAcadSubEntityImpl
{
public:
    explicit PyIAcadSubEntityImpl(IAcadSubEntity* ptr);
    virtual ~PyIAcadSubEntityImpl() = default;
    IAcadSubEntity* impObj(const std::source_location& src = std::source_location::current()) const;
protected:
    IAcadSubEntityPtr m_pimpl;
};
#endif

//------------------------------------------------------------------------------------
//PyIAcadMLeaderLeader
#ifndef _GRXTARGET
class PyIAcadMLeaderLeaderImpl : public PyIAcadSubEntityImpl
{
public:
    explicit PyIAcadMLeaderLeaderImpl(IAcadMLeaderLeader* ptr);
    virtual ~PyIAcadMLeaderLeaderImpl() = default;
    IAcadMLeaderLeader* impObj(const std::source_location& src = std::source_location::current()) const;
};
#endif

//------------------------------------------------------------------------------------
//PyIAcadSubEntSolidFaceImpl
#ifndef _GRXTARGET
class PyIAcadSubEntSolidFaceImpl : public PyIAcadSubEntityImpl
{
public:
    explicit PyIAcadSubEntSolidFaceImpl(IAcadSubEntSolidFace* ptr);
    virtual ~PyIAcadSubEntSolidFaceImpl() = default;
    IAcadSubEntSolidFace* impObj(const std::source_location& src = std::source_location::current()) const;
};
#endif

//------------------------------------------------------------------------------------
//PyIAcadSubDMeshFaceImpl
#ifndef _GRXTARGET
#ifndef _BRXTARGET
class PyIAcadSubDMeshFaceImpl : public PyIAcadSubEntityImpl
{
public:
    explicit PyIAcadSubDMeshFaceImpl(IAcadSubDMeshFace* ptr);
    virtual ~PyIAcadSubDMeshFaceImpl() = default;
    IAcadSubDMeshFace* impObj(const std::source_location& src = std::source_location::current()) const;
};
#endif
#endif

//------------------------------------------------------------------------------------
//PyIAcadSubDMeshEdgeImpl
#ifndef _GRXTARGET
#ifndef _BRXTARGET
class PyIAcadSubDMeshEdgeImpl : public PyIAcadSubEntityImpl
{
public:
    explicit PyIAcadSubDMeshEdgeImpl(IAcadSubDMeshEdge* ptr);
    virtual ~PyIAcadSubDMeshEdgeImpl() = default;
    IAcadSubDMeshEdge* impObj(const std::source_location& src = std::source_location::current()) const;
};
#endif
#endif

//------------------------------------------------------------------------------------
//PyIAcadSubDMeshVertexImpl
#ifndef _GRXTARGET
#ifndef _BRXTARGET
class PyIAcadSubDMeshVertexImpl : public PyIAcadSubEntityImpl
{
public:
    explicit PyIAcadSubDMeshVertexImpl(IAcadSubDMeshVertex* ptr);
    virtual ~PyIAcadSubDMeshVertexImpl() = default;
    IAcadSubDMeshVertex* impObj(const std::source_location& src = std::source_location::current()) const;
};
#endif
#endif

//------------------------------------------------------------------------------------
//PyIAcadEntityImpl1
class PyIAcadEntityImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadEntityImpl(IAcadEntity* ptr);
    virtual ~PyIAcadEntityImpl() override = default;

    PyIAcadAcCmColorPtr     GetTrueColor() const;
    void                    SetTrueColor(const PyIAcadAcCmColorImpl& val) const;
    CString                 GetLayer() const;
    void                    SetLayer(const CString& val);
    CString                 GetLinetype() const;
    void                    SetLinetype(const CString& val);
    double                  GetLinetypeScale() const;
    void                    SetLinetypeScale(double val);
    bool                    GetVisible() const;
    void                    SetVisible(bool bVisible);
    PyIAcadEntityPtrArray   ArrayPolar(int numberOfObjects, double angleToFill, const AcGePoint3d& centerPoint);
    PyIAcadEntityPtrArray   ArrayRectangular(int nRows, int nColumns, int nLevels, double rowDist, double colDist, double levelDist);
    void                    Highlight(bool highlight);
    PyIAcadEntityPtr        Copy();
    void                    Move(const AcGePoint3d& fromPoint, const AcGePoint3d& toPoint);
    void                    Rotate(const AcGePoint3d& basePoint, double rotationAngle);
    void                    Rotate3D(const AcGePoint3d& point1, const AcGePoint3d& point2, double rotationAngle);
    PyIAcadEntityPtr        Mirror(const AcGePoint3d& point1, const AcGePoint3d& point2);
    PyIAcadEntityPtr        Mirror3D(const AcGePoint3d& point1, const AcGePoint3d& point2, const AcGePoint3d& point3);
    void                    ScaleEntity(const AcGePoint3d& basePoint, double scaleFactor);
    void                    TransformBy(const AcGeMatrix3d& xform);
    void                    Update();
    void                    GetBoundingBox(AcGePoint3d& minPoint, AcGePoint3d& maxPoint);
    std::vector<AcGePoint3d> IntersectWith(const PyIAcadEntityImpl& intersectObject, PyAcExtendOption option);
    CString                 GetPlotStyleName() const;
    void                    SetPlotStyleName(const CString& val);
    PyAcLineWeight          GetLineweight() const;
    void                    SetLineweight(PyAcLineWeight lw);
    CString                 GetEntityTransparency() const;
    void                    SetEntityTransparency(const CString& val);
    PyIAcadHyperlinksPtr    GetHyperlinks() const;
    CString                 GetMaterial() const;
    void                    SetMaterial(const CString& val);
    CString                 GetEntityName() const;
    long                    GetEntityType() const;
    PyAcColor               GetColor() const;
    void                    SetColor(PyAcColor color);

    IAcadEntity* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadPViewportImpl
class PyIAcadPViewportImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadPViewportImpl(IAcadPViewport* ptr);
    virtual ~PyIAcadPViewportImpl() override = default;
    AcGePoint3d         GetCenter() const;
    void                SetCenter(const AcGePoint3d& val);
    AcGeVector3d        GetDirection() const;
    void                SetDirection(const AcGeVector3d& val);
    bool                GetGridOn() const;
    void                SetGridOn(bool bVisible);
    double              GetHeight() const;
    void                SetHeight(double val);
    double              GetWidth() const;
    void                SetWidth(double val);
    bool                GetViewportOn() const;
    void                SetViewportOn(bool bVisible);
    bool                GetClipped() const;
    bool                GetDisplayLocked() const;
    void                SetDisplayLocked(bool bVisible);
    PyAcViewportScale   GetStandardScale() const;
    void                SetStandardScale(PyAcViewportScale val);
    double              GetCustomScale() const;
    void                SetCustomScale(double val);
    CString             GetStyleSheet() const;
    void                SetStyleSheet(const CString& val);
    bool                GetUCSPerViewport() const;
    void                SetUCSPerViewport(bool UCSSaved);
    AcGePoint2d         GetSnapBasePoint() const;
    void                SetSnapBasePoint(const AcGePoint2d& val);
    bool                GetSnapOn() const;
    void                SetSnapOn(bool UCSSaved);
    double              GetSnapRotationAngle() const;
    void                SetSnapRotationAngle(double val);
    bool                GetUCSIconOn() const;
    void                SetUCSIconOn(bool bIconOn);
    bool                GetUCSIconAtOrigin() const;
    void                SetUCSIconAtOrigin(bool bIconAtOrigin);
    void                GetGridSpacing(double& XSpacing, double& YSpacing);
    void                SetGridSpacing(double XSpacing, double YSpacing);
    void                GetSnapSpacing(double& XSpacing, double& YSpacing);
    void                SetSnapSpacing(double XSpacing, double YSpacing);
    void                Display(bool bStatus);
    double              GetTwistAngle() const;
    void                SetTwistAngle(double val);
    double              GetLensLength() const;
    void                SetLensLength(double val);
    bool                GetRemoveHiddenLines() const;
    void                SetRemoveHiddenLines(bool bRemoval);
    AcGePoint3d         GetTarget() const;
    void                SetTarget(const AcGePoint3d& val);
    long                GetArcSmoothness() const;
    void                SetArcSmoothness(long val);
    int                 GetVisualStyle() const;
    void                SetVisualStyle(int val);
    int                 GetShadePlot() const;
    void                SetShadePlot(int val);
    PyIAcadViewPtr      GetModelView() const;
    void                SetModelView(const  PyIAcadViewImpl& val) const;
    PyIAcadViewPtr      GetSheetView() const;
    void                SetSheetView(const  PyIAcadViewImpl& val) const;
    AcDbObjectId        GetLabelBlockId() const;
    void                SetLabelBlockId(const AcDbObjectId& id);
    bool                GetHasSheetView() const;
    void                SyncModelView();
    int                 GetStandardScale2() const;
    void                SetStandardScale2(int val);
    bool                GetLayerPropertyOverrides() const;

    IAcadPViewport* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadPViewportPtr = std::unique_ptr<PyIAcadPViewportImpl>;

//------------------------------------------------------------------------------------
//PyIAcad3DFaceImpl
class PyIAcad3DFaceImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcad3DFaceImpl(IAcad3DFace* ptr);
    virtual ~PyIAcad3DFaceImpl() override = default;
    AcGePoint3dArray        GetCoordinates() const;
    void                    SetCoordinates(const AcGePoint3d& p1, const AcGePoint3d& p2, const AcGePoint3d& p3, const AcGePoint3d& p4);
    bool                    GetInvisibleEdge(int index) const;
    void                    SetInvisibleEdge(int index, bool flag);
    AcGePoint3d             GetCoordinate(int index) const;
    void                    SetCoordinate(int index, const AcGePoint3d& val);

    IAcad3DFace* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcad3DFacePtr = std::unique_ptr<PyIAcad3DFaceImpl>;

//------------------------------------------------------------------------------------
//PyIAcadPolygonMeshImpl
class PyIAcadPolygonMeshImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadPolygonMeshImpl(IAcadPolygonMesh* ptr);
    virtual ~PyIAcadPolygonMeshImpl() override = default;

    Point3dCoordinates      GetCoordinates() const;
    void                    SetCoordinates(const Point3dCoordinates& coords);
    bool                    GetMClose() const;
    void                    SetMClose(bool val);
    bool                    GetNClose() const;
    void                    SetNClose(bool val);
    long                    GetMDensity() const;
    void                    SetMDensity(long val);
    long                    GetNDensity() const;
    void                    SetNDensity(long val);
    long                    GetMVertexCount() const;
    long                    GetNVertexCount() const;
    PyAcPolymeshType        GetType() const;
    void                    SetType(PyAcPolymeshType val);
    void                    AppendVertex(const AcGePoint3d& val);
    PyIAcadEntityPtrArray   Explode() const;
    AcGePoint3d             GetCoordinate(int index) const;
    void                    SetCoordinate(int index, const AcGePoint3d& val);

    IAcadPolygonMesh* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadPolygonMeshPtr = std::unique_ptr<PyIAcadPolygonMeshImpl>;

//------------------------------------------------------------------------------------
//PyIAcad3DPolylineImpl
class PyIAcad3DPolylineImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcad3DPolylineImpl(IAcad3DPolyline* ptr);
    virtual ~PyIAcad3DPolylineImpl() override = default;

    Point3dCoordinates      GetCoordinates() const;
    void                    SetCoordinates(const Point3dCoordinates& coords);
    void                    AppendVertex(const AcGePoint3d& val);
    PyIAcadEntityPtrArray   Explode() const;
    AcGePoint3d             GetCoordinate(int index) const;
    void                    SetCoordinate(int index, const AcGePoint3d& val);
    PyAc3DPolylineType      GetType() const;
    void                    SetType(PyAc3DPolylineType val);
    bool                    GetClosed() const;
    void                    SetClosed(bool val);
    double                  GetLength() const;

    IAcad3DPolyline* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcad3DPolylinePtr = std::unique_ptr<PyIAcad3DPolylineImpl>;

//------------------------------------------------------------------------------------
//PyIAcadArcImpl
class PyIAcadArcImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadArcImpl(IAcadArc* ptr);
    virtual ~PyIAcadArcImpl() override = default;

    AcGePoint3d             GetStartPoint() const;
    AcGePoint3d             GetCenter() const;
    void                    SetCenter(const AcGePoint3d& val);
    AcGePoint3d             GetEndPoint() const;
    double                  GetRadius() const;
    void                    SetRadius(double val);
    double                  GetStartAngle() const;
    void                    SetStartAngle(double val);
    double                  GetEndAngle() const;
    void                    SetEndAngle(double val);
    double                  GetTotalAngle() const;
    double                  GetArcLength() const;
    double                  GetThickness() const;
    void                    SetThickness(double val);
    PyIAcadEntityPtrArray   Offset(double val) const;
    double                  GetArea() const;
    AcGeVector3d            GetNormal() const;
    void                    SetNormal(const AcGeVector3d& val);

    IAcadArc* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadArcPtr = std::unique_ptr<PyIAcadArcImpl>;

//------------------------------------------------------------------------------------
//PyIAcadAttributeImpl
class PyIAcadAttributeImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadAttributeImpl(IAcadAttribute* ptr);
    virtual ~PyIAcadAttributeImpl() override = default;

    long                    GetFieldLength() const;
    void                    SetFieldLength(long val);
    CString                 GetTagString() const;
    void                    SetTagString(const CString& val);
    CString                 GetPromptString() const;
    void                    SetPromptString(const CString& val);
    CString                 GetTextString() const;
    void                    SetTextString(const CString& val);
    CString                 GetStyleName() const;
    void                    SetStyleName(const CString& val);
    PyAcAlignment           GetAlignment() const;
    void                    SetAlignment(PyAcAlignment val);
    PyAcHorizontalAlignment GetHorizontalAlignment() const;
    void                    SetHorizontalAlignment(PyAcHorizontalAlignment val);
    PyAcVerticalAlignment   GetVerticalAlignment() const;
    void                    SetVerticalAlignment(PyAcVerticalAlignment val);
    double                  GetHeight() const;
    void                    SetHeight(double val);
    double                  GetRotation() const;
    void                    SetRotation(double val);
    double                  GetScaleFactor() const;
    void                    SetScaleFactor(double val);
    double                  GetObliqueAngle() const;
    void                    SetObliqueAngle(double val);
    AcGePoint3d             GetTextAlignmentPoint() const;
    void                    SetTextAlignmentPoint(const AcGePoint3d& val);
    AcGePoint3d             GetInsertionPoint() const;
    void                    SetInsertionPoint(const AcGePoint3d& val);
    AcGeVector3d            GetNormal() const;
    void                    SetNormal(const AcGeVector3d& val);
    long                    GetTextGenerationFlag() const;
    void                    SetTextGenerationFlag(long val);
    double                  GetThickness() const;
    void                    SetThickness(double val);
    long                    GetMode() const;
    void                    SetMode(long val);
    bool                    GetUpsideDown() const;
    void                    SetUpsideDown(bool val);
    bool                    GetBackward() const;
    void                    SetBackward(bool val);
    bool                    GetInvisible() const;
    void                    SetInvisible(bool val);
    bool                    GetConstant() const;
    void                    SetConstant(bool val);
    bool                    GetVerify() const;
    void                    SetVerify(bool val);
    bool                    GetPreset() const;
    void                    SetPreset(bool val);
    bool                    GetLockPosition() const;
    void                    SetLockPosition(bool val);
    bool                    GetMTextAttribute() const;
    void                    SetMTextAttribute(bool val);
    CString                 GetMTextAttributeContent() const;
    void                    SetMTextAttributeContent(const CString& val);
    void                    UpdateMTextAttribute();
    double                  GetMTextBoundaryWidth() const;
    void                    SetMTextBoundaryWidth(double val);
    PyAcDrawingDirection    GetMTextDrawingDirection() const;
    void                    SetMTextDrawingDirection(PyAcDrawingDirection val);


    IAcadAttribute* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadAttributePtr = std::unique_ptr<PyIAcadAttributeImpl>;

//------------------------------------------------------------------------------------
//PyIAcadRegionImpl
class PyIAcadRegionImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadRegionImpl(IAcadRegion* ptr);
    virtual ~PyIAcadRegionImpl() override = default;

    double                  GetArea() const;
    AcGePoint2d             GetCentroid() const;
    AcGePoint3d             GetMomentOfInertia() const;
    AcGeVector3d            GetNormal() const;
    double                  GetPerimeter() const;
    Point3dCoordinates      GetPrincipalDirections() const;
    AcGePoint3d             GetPrincipalMoments() const;
    AcGePoint3d             GetRadiiOfGyration() const;
    void                    Boolean(PyAcBooleanType val, const PyIAcadRegionImpl& region) const;
    PyIAcadEntityPtrArray   Explode() const;

    IAcadRegion* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadRegionPtr = std::unique_ptr<PyIAcadRegionImpl>;

//------------------------------------------------------------------------------------
//PyIAcad3DSolidImpl
class PyIAcad3DSolidImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcad3DSolidImpl(IAcad3DSolid* ptr);
    virtual ~PyIAcad3DSolidImpl() override = default;
    IAcad3DSolid* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcad3DSolidPtr = std::unique_ptr<PyIAcad3DSolidImpl>;

//------------------------------------------------------------------------------------
//PyIAcadCircleImpl
class PyIAcadCircleImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadCircleImpl(IAcadCircle* ptr);
    virtual ~PyIAcadCircleImpl() override = default;
    IAcadCircle* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadCirclePtr = std::unique_ptr<PyIAcadCircleImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimensionImpl
class PyIAcadDimensionImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadDimensionImpl(IAcadDimension* ptr);
    virtual ~PyIAcadDimensionImpl() override = default;
    IAcadDimension* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimensionPtr = std::unique_ptr<PyIAcadDimensionImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimAlignedImpl
class PyIAcadDimAlignedImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDimAlignedImpl(IAcadDimAligned* ptr);
    virtual ~PyIAcadDimAlignedImpl() override = default;
    IAcadDimAligned* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimAlignedPtr = std::unique_ptr<PyIAcadDimAlignedImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimAngularImpl
class PyIAcadDimAngularImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDimAngularImpl(IAcadDimAngular* ptr);
    virtual ~PyIAcadDimAngularImpl() override = default;
    IAcadDimAngular* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimAngularPtr = std::unique_ptr<PyIAcadDimAngularImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimDiametricImpl
class PyIAcadDimDiametricImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDimDiametricImpl(IAcadDimDiametric* ptr);
    virtual ~PyIAcadDimDiametricImpl() override = default;
    IAcadDimDiametric* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimDiametricPtr = std::unique_ptr<PyIAcadDimDiametricImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimRotatedImpl
class PyIAcadDimRotatedImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDimRotatedImpl(IAcadDimRotated* ptr);
    virtual ~PyIAcadDimRotatedImpl() override = default;
    IAcadDimRotated* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimRotatedPtr = std::unique_ptr<PyIAcadDimRotatedImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimOrdinateImpl
class PyIAcadDimOrdinateImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDimOrdinateImpl(IAcadDimOrdinate* ptr);
    virtual ~PyIAcadDimOrdinateImpl() override = default;
    IAcadDimOrdinate* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimOrdinatePtr = std::unique_ptr<PyIAcadDimOrdinateImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimRadialImpl
class PyIAcadDimRadialImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDimRadialImpl(IAcadDimRadial* ptr);
    virtual ~PyIAcadDimRadialImpl() override = default;
    IAcadDimRadial* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimRadialPtr = std::unique_ptr<PyIAcadDimRadialImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDim3PointAngularImpl
class PyIAcadDim3PointAngularImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDim3PointAngularImpl(IAcadDim3PointAngular* ptr);
    virtual ~PyIAcadDim3PointAngularImpl() override = default;
    IAcadDim3PointAngular* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDim3PointAngularPtr = std::unique_ptr<PyIAcadDim3PointAngularImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimArcLengthImpl
class PyIAcadDimArcLengthImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDimArcLengthImpl(IAcadDimArcLength* ptr);
    virtual ~PyIAcadDimArcLengthImpl() override = default;
    IAcadDimArcLength* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimArcLengthPtr = std::unique_ptr<PyIAcadDimArcLengthImpl>;

//------------------------------------------------------------------------------------
//PyIAcadDimRadialLargeImpl
class PyIAcadDimRadialLargeImpl : public PyIAcadDimensionImpl
{
public:
    explicit PyIAcadDimRadialLargeImpl(IAcadDimRadialLarge* ptr);
    virtual ~PyIAcadDimRadialLargeImpl() override = default;
    IAcadDimRadialLarge* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadDimRadialLargePtr = std::unique_ptr<PyIAcadDimRadialLargeImpl>;

//------------------------------------------------------------------------------------
//PyIAcadEllipseImpl
class PyIAcadEllipseImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadEllipseImpl(IAcadEllipse* ptr);
    virtual ~PyIAcadEllipseImpl() override = default;
    IAcadEllipse* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadEllipsePtr = std::unique_ptr<PyIAcadEllipseImpl>;

//------------------------------------------------------------------------------------
//PyIAcadLeaderImpl
class PyIAcadLeaderImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadLeaderImpl(IAcadLeader* ptr);
    virtual ~PyIAcadLeaderImpl() override = default;
    IAcadLeader* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadLeaderPtr = std::unique_ptr<PyIAcadLeaderImpl>;

//------------------------------------------------------------------------------------
//PyIAcadMTextImpl
class PyIAcadMTextImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadMTextImpl(IAcadMText* ptr);
    virtual ~PyIAcadMTextImpl() override = default;
    IAcadMText* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadMTextPtr = std::unique_ptr<PyIAcadMTextImpl>;

//------------------------------------------------------------------------------------
//PyIAcadPointImpl
class PyIAcadPointImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadPointImpl(IAcadPoint* ptr);
    virtual ~PyIAcadPointImpl() override = default;
    IAcadPoint* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadPointPtr = std::unique_ptr<PyIAcadPointImpl>;

//------------------------------------------------------------------------------------
//PyIAcadLWPolylineImpl
class PyIAcadLWPolylineImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadLWPolylineImpl(IAcadLWPolyline* ptr);
    virtual ~PyIAcadLWPolylineImpl() override = default;
    IAcadLWPolyline* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadLWPolylinePtr = std::unique_ptr<PyIAcadLWPolylineImpl>;

//------------------------------------------------------------------------------------
//PyIAcadPolylineImpl
class PyIAcadPolylineImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadPolylineImpl(IAcadPolyline* ptr);
    virtual ~PyIAcadPolylineImpl() override = default;
    IAcadPolyline* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadPolylinePtr = std::unique_ptr<PyIAcadPolylineImpl>;

//------------------------------------------------------------------------------------
//PyIAcadRayImpl
class PyIAcadRayImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadRayImpl(IAcadRay* ptr);
    virtual ~PyIAcadRayImpl() override = default;
    IAcadRay* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadRayPtr = std::unique_ptr<PyIAcadRayImpl>;

//------------------------------------------------------------------------------------
//PyIAcadShapeImpl
class PyIAcadShapeImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadShapeImpl(IAcadShape* ptr);
    virtual ~PyIAcadShapeImpl() override = default;
    IAcadShape* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadShapePtr = std::unique_ptr<PyIAcadShapeImpl>;

//------------------------------------------------------------------------------------
//PyIAcadSolidImpl
class PyIAcadSolidImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadSolidImpl(IAcadSolid* ptr);
    virtual ~PyIAcadSolidImpl() override = default;
    IAcadSolid* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadSolidPtr = std::unique_ptr<PyIAcadSolidImpl>;

//------------------------------------------------------------------------------------
//PyIAcadSplineImpl
class PyIAcadSplineImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadSplineImpl(IAcadSpline* ptr);
    virtual ~PyIAcadSplineImpl() override = default;
    IAcadSpline* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadSplinePtr = std::unique_ptr<PyIAcadSplineImpl>;

//------------------------------------------------------------------------------------
//PyIAcadTextImpl
class PyIAcadTextImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadTextImpl(IAcadText* ptr);
    virtual ~PyIAcadTextImpl() override = default;
    IAcadText* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadTextPtr = std::unique_ptr<PyIAcadTextImpl>;

//------------------------------------------------------------------------------------
//PyIAcadToleranceImpl
class PyIAcadToleranceImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadToleranceImpl(IAcadTolerance* ptr);
    virtual ~PyIAcadToleranceImpl() override = default;
    IAcadTolerance* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadTolerancePtr = std::unique_ptr<PyIAcadToleranceImpl>;

//------------------------------------------------------------------------------------
//PyIAcadTraceImpl
class PyIAcadTraceImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadTraceImpl(IAcadTrace* ptr);
    virtual ~PyIAcadTraceImpl() override = default;
    IAcadTrace* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadTracePtr = std::unique_ptr<PyIAcadTraceImpl>;

//------------------------------------------------------------------------------------
//PyIAcadXlineImpl
class PyIAcadXlineImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadXlineImpl(IAcadXline* ptr);
    virtual ~PyIAcadXlineImpl() override = default;
    IAcadXline* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadXlinePtr = std::unique_ptr<PyIAcadXlineImpl>;

//------------------------------------------------------------------------------------
//PyIAcadBlockReferenceImpl
class PyIAcadBlockReferenceImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadBlockReferenceImpl(IAcadBlockReference* ptr);
    virtual ~PyIAcadBlockReferenceImpl() override = default;
    IAcadBlockReference* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadBlockReferencePtr = std::unique_ptr<PyIAcadBlockReferenceImpl>;

//------------------------------------------------------------------------------------
//PyIAcadMInsertBlockImpl
class PyIAcadMInsertBlockImpl : public PyIAcadBlockReferenceImpl
{
public:
    explicit PyIAcadMInsertBlockImpl(IAcadMInsertBlock* ptr);
    virtual ~PyIAcadMInsertBlockImpl() override = default;
    IAcadMInsertBlock* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadMInsertBlockPtr = std::unique_ptr<PyIAcadMInsertBlockImpl>;

//------------------------------------------------------------------------------------
//PyIAcadExternalReferenceImpl
class PyIAcadExternalReferenceImpl : public PyIAcadBlockReferenceImpl
{
public:
    explicit PyIAcadExternalReferenceImpl(IAcadExternalReference* ptr);
    virtual ~PyIAcadExternalReferenceImpl() override = default;
    IAcadExternalReference* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadExternalReferencePtr = std::unique_ptr<PyIAcadExternalReferenceImpl>;


//------------------------------------------------------------------------------------
//PyIAcadHatchImpl
class PyIAcadHatchImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadHatchImpl(IAcadHatch* ptr);
    virtual ~PyIAcadHatchImpl() override = default;
    IAcadHatch* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadHatchPtr = std::unique_ptr<PyIAcadHatchImpl>;

//------------------------------------------------------------------------------------
//PyIAcadRasterImageImpl
class PyIAcadRasterImageImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadRasterImageImpl(IAcadRasterImage* ptr);
    virtual ~PyIAcadRasterImageImpl() override = default;
    IAcadRasterImage* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadRasterImagePtr = std::unique_ptr<PyIAcadRasterImageImpl>;

//------------------------------------------------------------------------------------
//PyIAcadLineImpl
class PyIAcadLineImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadLineImpl(IAcadLine* ptr);
    virtual ~PyIAcadLineImpl() override = default;
    IAcadLine* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadLinePtr = std::unique_ptr<PyIAcadLineImpl>;

//------------------------------------------------------------------------------------
//PyIAcadPolyfaceMeshImpl
class PyIAcadPolyfaceMeshImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadPolyfaceMeshImpl(IAcadPolyfaceMesh* ptr);
    virtual ~PyIAcadPolyfaceMeshImpl() override = default;
    IAcadPolyfaceMesh* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadPolyfaceMeshPtr = std::unique_ptr<PyIAcadPolyfaceMeshImpl>;

//------------------------------------------------------------------------------------
//PyIAcadMLineImpl
class PyIAcadMLineImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadMLineImpl(IAcadMLine* ptr);
    virtual ~PyIAcadMLineImpl() override = default;
    IAcadMLine* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadMLinePtr = std::unique_ptr<PyIAcadMLineImpl>;

//------------------------------------------------------------------------------------
//PyIAcadTableImpl
class PyIAcadTableImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadTableImpl(IAcadTable* ptr);
    virtual ~PyIAcadTableImpl() override = default;
    IAcadTable* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadTablePtr = std::unique_ptr<PyIAcadTableImpl>;

//------------------------------------------------------------------------------------
//PyIAcadSectionImpl
class PyIAcadSectionImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadSectionImpl(IAcadSection* ptr);
    virtual ~PyIAcadSectionImpl() override = default;
    IAcadSection* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadSectionPtr = std::unique_ptr<PyIAcadSectionImpl>;

//------------------------------------------------------------------------------------
//PyIAcadMLeaderImpl
class PyIAcadMLeaderImpl : public PyIAcadEntityImpl
{
public:
    explicit PyIAcadMLeaderImpl(IAcadMLeader* ptr);
    virtual ~PyIAcadMLeaderImpl() override = default;
    IAcadMLeader* impObj(const std::source_location& src = std::source_location::current()) const;
};
using PyIAcadMLeaderPtr = std::unique_ptr<PyIAcadMLeaderImpl>;
