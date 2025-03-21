#include "stdafx.h"
#include "PyAcadDbObjectImpl.h"
#include "PyAcadEntityImpl.h"
#include "PyAcadApplicationImpl.h"

//------------------------------------------------------------------------------------
//PyIAcad helpers
// 
//this is duplicate code for sheetsets, though not for Zw or Gs
#if defined(_ZRXTARGET)
extern HRESULT ZcAxGetIUnknownOfObject(LPUNKNOWN*, ZcDbObjectId&, LPDISPATCH);
extern HRESULT ZcAxGetIUnknownOfObject(LPUNKNOWN*, ZcDbObject*, LPDISPATCH);
extern HRESULT ZcAxGetDatabase(ZcDbDatabase* pDb, LPDISPATCH pAppDisp, LPDISPATCH* pDisp);
#define IID_IAcadObject IID_IZcadObject
#define IID_IAcadDatabase IID_IZcadDatabase
#define AcAxGetIUnknownOfObject ZcAxGetIUnknownOfObject
#define AcAxGetDatabase ZcAxGetDatabase
#elif defined(_GRXTARGET)
extern HRESULT GcAxGetIUnknownOfObject(LPUNKNOWN*, GcDbObjectId&, LPDISPATCH);
extern HRESULT GcAxGetIUnknownOfObject(LPUNKNOWN*, GcDbObject*, LPDISPATCH);
extern HRESULT GcAxGetDatabase(GcDbDatabase* pDb, LPDISPATCH pAppDisp, LPDISPATCH* pDisp);
#define IID_IAcadObject IID_IGcadObject
#define IID_IAcadDatabase IID_IGcadDatabase
#define AcAxGetIUnknownOfObject GcAxGetIUnknownOfObject
#define AcAxGetDatabase GcAxGetDatabase
#else
extern HRESULT AcAxGetIUnknownOfObject(LPUNKNOWN*, AcDbObjectId&, LPDISPATCH);
extern HRESULT AcAxGetIUnknownOfObject(LPUNKNOWN*, AcDbObject*, LPDISPATCH);
extern HRESULT AcAxGetDatabase(AcDbDatabase* pDb, LPDISPATCH pAppDisp, LPDISPATCH* pDisp);
#endif

IAcadObject* GetIAcadObjectFromAcDbObjectId(const AcDbObjectId& id)
{
    AcDbObjectId _id(id);
    IUnknown* pUnk = nullptr;
    LPDISPATCH pAppDisp = acedGetIDispatch(false);
    if (AcAxGetIUnknownOfObject(&pUnk, _id, pAppDisp) == S_OK && pUnk)
    {
        IAcadObject* pObj = nullptr;
        if (pUnk->QueryInterface(IID_IAcadObject, (void**)&pObj) == S_OK && pObj)
            return pObj;
    }
    return nullptr;
}

IAcadObject* GetIAcadObjectFromAcDbObject(AcDbObject* pSrcObject)
{
    IUnknown* pUnk = nullptr;
    LPDISPATCH pAppDisp = acedGetIDispatch(false);
    if (AcAxGetIUnknownOfObject(&pUnk, pSrcObject, pAppDisp) == S_OK && pUnk)
    {
        IAcadObject* pObj = nullptr;
        if (pUnk->QueryInterface(IID_IAcadObject, (void**)&pObj) == S_OK && pObj)
            return pObj;
    }
    return nullptr;
}

IAcadDatabase* GetIAcadDatabaseFromAcDbDatabse(AcDbDatabase* pSrcObject)
{
    LPDISPATCH pUnk = nullptr;
    LPDISPATCH pAppDisp = acedGetIDispatch(false);
    if (AcAxGetDatabase(pSrcObject, pAppDisp, &pUnk) == S_OK && pUnk)
    {
        IAcadDatabase* pObj = nullptr;
        if (pUnk->QueryInterface(IID_IAcadDatabase, (void**)&pObj) == S_OK && pObj)
            return pObj;
    }
    return nullptr;
}

//------------------------------------------------------------------------------------
//PyIAcadObjectImpl
PyIAcadObjectImpl::PyIAcadObjectImpl(IAcadObject* ptr)
    : m_pimpl(ptr)
{
}

CString PyIAcadObjectImpl::GetHandle() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_Handle(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

CString PyIAcadObjectImpl::GetObjectName() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_ObjectName(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

TypedVariants PyIAcadObjectImpl::GetXData(const CString& appName) const
{
    TypedVariants typedVariants;
    try
    {
        _variant_t xdataTypes = {};
        _variant_t xdataValues = {};
        _bstr_t bstrAppName{ appName };
        CHECKHR(impObj()->GetXData(bstrAppName, &xdataTypes.GetVARIANT(), &xdataValues.GetVARIANT()));

        auto xdataTypeLen = VariantGetElementCount(xdataTypes);
        auto xdataDalueLen = VariantGetElementCount(xdataValues);
        if (xdataTypeLen != xdataDalueLen)
            return typedVariants;

        CComSafeArray<VARIANT> safeVariantArray;
        if (safeVariantArray.Attach(xdataValues.parray) == S_OK)
            xdataValues.Detach();

        for (auto idx = 0; idx < xdataTypeLen; idx++)
        {
            int16_t xdcode = 0;
            CHECKHR(VariantGetInt16Elem(xdataTypes, idx, &xdcode));
            const auto& variantItem = safeVariantArray.GetAt(idx);
            if (IsVariantString(variantItem))
            {
                std::wstring val(wcslen(variantItem.bstrVal) + 1, '\0');
                if (CHECKHR(VariantToString(variantItem, val.data(), val.size())))
                    typedVariants.emplace_back(TypedVariant{ xdcode, val });
            }
            else if (variantItem.vt == VT_I2 || variantItem.vt == VT_UI2)
            {
                int16_t val = 0;
                if (CHECKHR(VariantToInt16(variantItem, &val)))
                    typedVariants.emplace_back(TypedVariant{ xdcode, val });
            }
            else if (variantItem.vt == VT_I4 || variantItem.vt == VT_UI4)
            {
                int32_t val = 0;
                if (CHECKHR(VariantToInt32(variantItem, &val)))
                    typedVariants.emplace_back(TypedVariant{ xdcode, val });
            }
            else if (variantItem.vt == VT_R4 || variantItem.vt == VT_R8)
            {
                double val = .0;
                if (CHECKHR(VariantToDouble(variantItem, &val)))
                    typedVariants.emplace_back(TypedVariant{ xdcode, val });
            }
            else if (IsVariantArray(variantItem))
            {
                AcGePoint3d val;
                ULONG pcElem = 0;
                constexpr ULONG szof = sizeof(AcGePoint3d) / sizeof(double);
                if (CHECKHR(VariantToDoubleArray(variantItem, asDblArray(val), szof, &pcElem)))
                    typedVariants.emplace_back(TypedVariant{ xdcode, val });
            }
            else
            {
                //TODO: Binary Data?
                acutPrintf(_T("\nUnrecognised variant %ls, %ld"), __FUNCTIONW__, __LINE__);
            }
        }
    }
    catch (...)
    {
        acutPrintf(_T("\nError Fail@ = %ls"), __FUNCTIONW__);
    }
    return typedVariants;
}

void PyIAcadObjectImpl::SetXData(const TypedVariants& typedVariants)
{
    try
    {
        CComSafeArray<int16_t> safeCodesArray(typedVariants.size());
        CComSafeArray<VARIANT> safeVariantArray(typedVariants.size());
        for (size_t idx = 0; idx < typedVariants.size(); idx++)
        {
            const auto& typedVariant = typedVariants.at(idx);
            safeCodesArray[int(idx)] = typedVariant.code;
            TypedVariant::ETypeCode eType = static_cast<TypedVariant::ETypeCode>(typedVariant.variant.index());
            switch (eType)
            {
                case TypedVariant::kInt16:
                {
                    auto& v = safeVariantArray[int(idx)];
                    const auto& tv = std::get<TypedVariant::kInt16>(typedVariant.variant);
                    CHECKHR(InitVariantFromInt16(tv, &v));
                    break;
                }
                case TypedVariant::kInt32:
                {
                    auto& v = safeVariantArray[int(idx)];
                    const auto& tv = std::get<TypedVariant::kInt32>(typedVariant.variant);
                    CHECKHR(InitVariantFromInt32(tv, &v));
                    break;
                }
                case TypedVariant::kFloat:
                {
                    auto& v = safeVariantArray[int(idx)];
                    const auto& tv = std::get<TypedVariant::kFloat>(typedVariant.variant);
                    CHECKHR(InitVariantFromDouble(tv, &v));
                    break;
                }
                case TypedVariant::kPoint3d:
                {
                    auto& v = safeVariantArray[int(idx)];
                    constexpr ULONG szof = sizeof(AcGePoint3d) / sizeof(double);
                    const auto& tv = std::get<TypedVariant::kPoint3d>(typedVariant.variant);
                    CHECKHR(InitVariantFromDoubleArray(asDblArray(tv), szof, &v));
                    break;
                }
                case TypedVariant::kString:
                {
                    auto& v = safeVariantArray[int(idx)];
                    const auto& tv = std::get<TypedVariant::ETypeCode::kString>(typedVariant.variant);
                    CHECKHR(InitVariantFromString(tv.data(), &v));
                    break;
                }
            }
        }

        VARIANT xdataTypes;
        VariantInit(&xdataTypes);
        xdataTypes.vt = VT_ARRAY | VT_I2;
        xdataTypes.parray = safeCodesArray;

        VARIANT xdataValues;
        VariantInit(&xdataValues);
        xdataValues.vt = VT_ARRAY | VT_VARIANT;
        xdataValues.parray = safeVariantArray;

        if (auto hr = impObj()->SetXData(xdataTypes, xdataValues); hr != S_OK)
            acutPrintf(_T("\nError Fail HR-0x%X  = %ls, %ld"), hr, __FUNCTIONW__, __LINE__);
    }
    catch (...)
    {
        acutPrintf(_T("\nError Fail@ = %ls"), __FUNCTIONW__);
    }
}

void PyIAcadObjectImpl::Delete() const
{
    PyThrowBadHr(impObj()->Delete());
}

LONG_PTR PyIAcadObjectImpl::GetObjectId() const
{
    LONG_PTR id = 0;
    PyThrowBadHr(impObj()->get_ObjectID(&id));
    return id;
}

LONG_PTR PyIAcadObjectImpl::GetOwnerId() const
{
    LONG_PTR id = 0;
    PyThrowBadHr(impObj()->get_OwnerID(&id));
    return id;
}

PyIAcadDatabasePtr PyIAcadObjectImpl::GetDatabase() const
{
    IAcadDatabase* pDb = nullptr;
    PyThrowBadHr(impObj()->get_Database(&pDb));
    return std::make_unique<PyIAcadDatabaseImpl>(pDb);
}

bool PyIAcadObjectImpl::GetHasExtensionDictionary() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_HasExtensionDictionary(&rtVal));
    return rtVal != VARIANT_FALSE;
}

PyIAcadDictionaryPtr PyIAcadObjectImpl::GetExtensionDictionary() const
{
    IAcadDictionary* pval = nullptr;
    PyThrowBadHr(impObj()->GetExtensionDictionary(&pval));
    return std::make_unique<PyIAcadDictionaryImpl>(pval);
}

PyIAcadDocumentPtr PyIAcadObjectImpl::GetDocument() const
{
    LPDISPATCH lpdsp = nullptr;
    PyThrowBadHr(impObj()->get_Document(&lpdsp));
    return std::make_unique<PyIAcadDocumentImpl>(static_cast<IAcadDocument*>(lpdsp));
}

void PyIAcadObjectImpl::Erase() const
{
    PyThrowBadHr(impObj()->Erase());
}

bool PyIAcadObjectImpl::IsEqualTo(const PyIAcadObjectImpl& other) const
{
    return other.m_pimpl == m_pimpl;
}

bool PyIAcadObjectImpl::IsNull() const
{
    return m_pimpl == nullptr;
}

std::size_t PyIAcadObjectImpl::hash() const
{
    return std::hash<IAcadObject*>{}((IAcadObject*)m_pimpl);
}

AcDbObjectId PyIAcadObjectImpl::id() const
{
    LONG_PTR id = 0;
    AcDbObjectId oid;
    PyThrowBadHr(impObj()->get_ObjectID(&id));
    oid.setFromOldId(id);
    return oid;
}

IAcadObject* PyIAcadObjectImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadObject*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadPlotConfigurationImpl
PyIAcadPlotConfigurationImpl::PyIAcadPlotConfigurationImpl(IAcadPlotConfiguration* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

CString PyIAcadPlotConfigurationImpl::GetName() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_Name(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadPlotConfigurationImpl::SetName(const CString& val)
{
    _bstr_t bstrval{ val };
    PyThrowBadHr(impObj()->put_Name(bstrval));
}

CString PyIAcadPlotConfigurationImpl::GetConfigName() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_ConfigName(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadPlotConfigurationImpl::SetConfigName(const CString& val)
{
    _bstr_t bstrval{ val };
    PyThrowBadHr(impObj()->put_ConfigName(bstrval));
}

CString PyIAcadPlotConfigurationImpl::GetCanonicalMediaName() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_CanonicalMediaName(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadPlotConfigurationImpl::SetCanonicalMediaName(const CString& val)
{
    _bstr_t bstrval{ val };
    PyThrowBadHr(impObj()->put_CanonicalMediaName(bstrval));
}

PyAcPlotPaperUnits PyIAcadPlotConfigurationImpl::GetPaperUnits() const
{
    AcPlotPaperUnits rtVal = (AcPlotPaperUnits)PyAcPlotPaperUnits::pyacInches;
    PyThrowBadHr(impObj()->get_PaperUnits(&rtVal));
    return (PyAcPlotPaperUnits)rtVal;
}

void PyIAcadPlotConfigurationImpl::SetPaperUnits(PyAcPlotPaperUnits val)
{
    PyThrowBadHr(impObj()->put_PaperUnits((AcPlotPaperUnits)val));
}

bool PyIAcadPlotConfigurationImpl::GetPlotViewportBorders() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_PlotViewportBorders(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetPlotViewportBorders(bool val)
{
    PyThrowBadHr(impObj()->put_PlotViewportBorders(val ? VARIANT_TRUE : VARIANT_FALSE));
}

bool PyIAcadPlotConfigurationImpl::GetShowPlotStyles() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_ShowPlotStyles(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetShowPlotStyles(bool val)
{
    PyThrowBadHr(impObj()->put_ShowPlotStyles(val ? VARIANT_TRUE : VARIANT_FALSE));
}

PyAcPlotRotation PyIAcadPlotConfigurationImpl::GetPlotRotation() const
{
    AcPlotRotation rtVal = (AcPlotRotation)PyAcPlotRotation::pyac0degrees;
    PyThrowBadHr(impObj()->get_PlotRotation(&rtVal));
    return (PyAcPlotRotation)rtVal;
}

void PyIAcadPlotConfigurationImpl::SetPlotRotation(PyAcPlotRotation val)
{
    PyThrowBadHr(impObj()->put_PlotRotation((AcPlotRotation)val));
}

bool PyIAcadPlotConfigurationImpl::GetCenterPlot() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_CenterPlot(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetCenterPlot(bool val)
{
    PyThrowBadHr(impObj()->put_CenterPlot(val ? VARIANT_TRUE : VARIANT_FALSE));
}

bool PyIAcadPlotConfigurationImpl::GetPlotHidden() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_PlotHidden(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetPlotHidden(bool val)
{
    PyThrowBadHr(impObj()->put_PlotHidden(val ? VARIANT_TRUE : VARIANT_FALSE));
}

PyAcPlotType PyIAcadPlotConfigurationImpl::GetPlotType() const
{
    AcPlotType rtVal = (AcPlotType)PyAcPlotType::pyacDisplay;
    PyThrowBadHr(impObj()->get_PlotType(&rtVal));
    return (PyAcPlotType)rtVal;
}

void PyIAcadPlotConfigurationImpl::SetPlotType(PyAcPlotType val)
{
    PyThrowBadHr(impObj()->put_PlotType((AcPlotType)val));
}

CString PyIAcadPlotConfigurationImpl::GetViewToPlot() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_ViewToPlot(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadPlotConfigurationImpl::SetViewToPlot(const CString& val)
{
    _bstr_t bstrval{ val };
    PyThrowBadHr(impObj()->put_ViewToPlot(bstrval));
}

bool PyIAcadPlotConfigurationImpl::GetUseStandardScale() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_UseStandardScale(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetUseStandardScale(bool val)
{
    PyThrowBadHr(impObj()->put_UseStandardScale(val ? VARIANT_TRUE : VARIANT_FALSE));
}

PyAcPlotScale PyIAcadPlotConfigurationImpl::GetStandardScale() const
{
    AcPlotScale rtVal = (AcPlotScale)PyAcPlotScale::pyacScaleToFit;
    PyThrowBadHr(impObj()->get_StandardScale(&rtVal));
    return (PyAcPlotScale)rtVal;
}

void PyIAcadPlotConfigurationImpl::SetStandardScale(PyAcPlotScale val)
{
    PyThrowBadHr(impObj()->put_StandardScale((AcPlotScale)val));
}

void PyIAcadPlotConfigurationImpl::GetCustomScale(double& numerator, double& denominator)
{
    PyThrowBadHr(impObj()->GetCustomScale(&numerator, &denominator));
}

void PyIAcadPlotConfigurationImpl::SetCustomScale(double numerator, double denominator)
{
    PyThrowBadHr(impObj()->SetCustomScale(numerator, denominator));
}

bool PyIAcadPlotConfigurationImpl::GetScaleLineweights() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_ScaleLineweights(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetScaleLineweights(bool val)
{
    PyThrowBadHr(impObj()->put_ScaleLineweights(val ? VARIANT_TRUE : VARIANT_FALSE));
}

bool PyIAcadPlotConfigurationImpl::GetPlotWithLineweights() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_PlotWithLineweights(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetPlotWithLineweights(bool val)
{
    PyThrowBadHr(impObj()->put_PlotWithLineweights(val ? VARIANT_TRUE : VARIANT_FALSE));
}

bool PyIAcadPlotConfigurationImpl::GetPlotViewportsFirst() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_PlotViewportsFirst(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetPlotViewportsFirst(bool val)
{
    PyThrowBadHr(impObj()->put_PlotViewportsFirst(val ? VARIANT_TRUE : VARIANT_FALSE));
}

CString PyIAcadPlotConfigurationImpl::GetStyleSheet() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_StyleSheet(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadPlotConfigurationImpl::SetStyleSheet(const CString& val)
{
    _bstr_t bstrval{ val };
    PyThrowBadHr(impObj()->put_StyleSheet(bstrval));
}

void PyIAcadPlotConfigurationImpl::GetPaperMargins(AcGePoint2d& lowerLeft, AcGePoint2d& upperRight)
{
    _variant_t vtlowerLeft;
    _variant_t vtupperRight;
    PyThrowBadHr(impObj()->GetPaperMargins(&vtlowerLeft.GetVARIANT(), &vtupperRight.GetVARIANT()));
    PyThrowBadHr(VariantToAcGePoint2d(vtlowerLeft, lowerLeft));
    PyThrowBadHr(VariantToAcGePoint2d(vtupperRight, upperRight));
}

void PyIAcadPlotConfigurationImpl::GetPaperSize(double& width, double& height)
{
    PyThrowBadHr(impObj()->GetPaperSize(&width, &height));
}

AcGePoint2d PyIAcadPlotConfigurationImpl::GetPlotOrigin()
{
    AcGePoint2d origin;
    _variant_t vtorigin;
    PyThrowBadHr(impObj()->get_PlotOrigin(&vtorigin.GetVARIANT()));
    PyThrowBadHr(VariantToAcGePoint2d(vtorigin, origin));
    return origin;
}

void PyIAcadPlotConfigurationImpl::SetPlotOrigin(const AcGePoint2d& orgin)
{
    AcGePoint2d origin;
    _variant_t vtorigin;
    PyThrowBadHr(AcGePoint2dToVariant(vtorigin, origin));
    PyThrowBadHr(impObj()->put_PlotOrigin(vtorigin));
}

void PyIAcadPlotConfigurationImpl::GetWindowToPlot(AcGePoint2d& lowerLeft, AcGePoint2d& upperRight)
{
    _variant_t vtlowerLeft;
    _variant_t vtupperRight;
    PyThrowBadHr(impObj()->GetWindowToPlot(&vtlowerLeft.GetVARIANT(), &vtupperRight.GetVARIANT()));
    PyThrowBadHr(VariantToAcGePoint2d(vtlowerLeft, lowerLeft));
    PyThrowBadHr(VariantToAcGePoint2d(vtupperRight, upperRight));
}

void PyIAcadPlotConfigurationImpl::SetWindowToPlot(const AcGePoint2d& lowerLeft, const AcGePoint2d& upperRight)
{
    _variant_t vtlowerLeft;
    _variant_t vtupperRight;
    PyThrowBadHr(AcGePoint2dToVariant(vtlowerLeft, lowerLeft));
    PyThrowBadHr(AcGePoint2dToVariant(vtupperRight, upperRight));
    PyThrowBadHr(impObj()->SetWindowToPlot(vtlowerLeft, vtupperRight));
}

bool PyIAcadPlotConfigurationImpl::GetPlotWithPlotStyles() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_PlotWithPlotStyles(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::SetPlotWithPlotStyles(bool val)
{
    PyThrowBadHr(impObj()->put_PlotWithPlotStyles(val ? VARIANT_TRUE : VARIANT_FALSE));
}

bool PyIAcadPlotConfigurationImpl::GetModelType() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_ModelType(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadPlotConfigurationImpl::CopyFrom(const PyIAcadPlotConfigurationImpl& val)
{
    PyThrowBadHr(impObj()->CopyFrom(val.impObj()));
}

wstringArray PyIAcadPlotConfigurationImpl::GetCanonicalMediaNames() const
{
    _variant_t vtstrs;
    wstringArray strs;
    PyThrowBadHr(impObj()->GetCanonicalMediaNames(&vtstrs.GetVARIANT()));
    PyThrowBadHr(VariantTowstringArray(vtstrs, strs));
    return strs;
}

wstringArray PyIAcadPlotConfigurationImpl::GetPlotDeviceNames() const
{
    _variant_t vtstrs;
    wstringArray strs;
    PyThrowBadHr(impObj()->GetPlotDeviceNames(&vtstrs.GetVARIANT()));
    PyThrowBadHr(VariantTowstringArray(vtstrs, strs));
    return strs;
}

wstringArray PyIAcadPlotConfigurationImpl::GetPlotStyleTableNames() const
{
    _variant_t vtstrs;
    wstringArray strs;
    PyThrowBadHr(impObj()->GetPlotStyleTableNames(&vtstrs.GetVARIANT()));
    PyThrowBadHr(VariantTowstringArray(vtstrs, strs));
    return strs;
}

void PyIAcadPlotConfigurationImpl::RefreshPlotDeviceInfo()
{
    PyThrowBadHr(impObj()->RefreshPlotDeviceInfo());
}

CString PyIAcadPlotConfigurationImpl::GetLocaleMediaName(const CString& name) const
{
    _bstr_t bstrVal;
    _bstr_t bstrname{ name };
    PyThrowBadHr(impObj()->GetLocaleMediaName(bstrname, &bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

IAcadPlotConfiguration* PyIAcadPlotConfigurationImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadPlotConfiguration*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadLayoutImpl
PyIAcadLayoutImpl::PyIAcadLayoutImpl(IAcadLayout* ptr)
    : PyIAcadPlotConfigurationImpl(ptr)
{
}

PyIAcadBlockPtr PyIAcadLayoutImpl::GetBlock() const
{
    IAcadBlock* pBlock = nullptr;
    PyThrowBadHr(impObj()->get_Block(&pBlock));
    return std::make_unique<PyIAcadBlockImpl>(pBlock);
}

long PyIAcadLayoutImpl::GetTabOrder() const
{
    long rtval = 0;
    PyThrowBadHr(impObj()->get_TabOrder(&rtval));
    return rtval;
}

void PyIAcadLayoutImpl::SetTabOrder(long val)
{
    PyThrowBadHr(impObj()->put_TabOrder(val));
}

IAcadLayout* PyIAcadLayoutImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadLayout*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadSectionSettingsImpl
PyIAcadSectionSettingsImpl::PyIAcadSectionSettingsImpl(IAcadSectionSettings* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyAcSectionType PyIAcadSectionSettingsImpl::GetCurrentSectionType() const
{
    AcSectionType rtVal = (AcSectionType)PyAcSectionType::pyacSectionTypeLiveSection;
    PyThrowBadHr(impObj()->get_CurrentSectionType(&rtVal));
    return (PyAcSectionType)rtVal;
}

void PyIAcadSectionSettingsImpl::SetCurrentSectionType(PyAcSectionType val) const
{
    PyThrowBadHr(impObj()->put_CurrentSectionType((AcSectionType)val));
}

PyIAcadSectionTypeSettingsPtr PyIAcadSectionSettingsImpl::GetSectionTypeSettings(PyAcSectionType secType) const
{
    IAcadSectionTypeSettings* ptr = nullptr;
    PyThrowBadHr(impObj()->GetSectionTypeSettings((AcSectionType)secType, &ptr));
    return std::make_unique<PyIAcadSectionTypeSettingsImpl>(ptr);
}

IAcadSectionSettings* PyIAcadSectionSettingsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadSectionSettings*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadViewImpl
PyIAcadViewImpl::PyIAcadViewImpl(IAcadView* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

AcGePoint3d PyIAcadViewImpl::GetCenter() const
{
    _variant_t vtval;
    AcGePoint3d rtVal;
    PyThrowBadHr(impObj()->get_Center(&vtval));
    PyThrowBadHr(VariantToAcGePoint3d(vtval, rtVal));
    return rtVal;
}

void PyIAcadViewImpl::SetCenter(const AcGePoint3d& val)
{
    _variant_t vtval;
    PyThrowBadHr(AcGePoint3dToVariant(vtval.GetVARIANT(), val));
    PyThrowBadHr(impObj()->put_Center(vtval));
}

double PyIAcadViewImpl::GetHeight() const
{
    double rtval = 0.0;
    PyThrowBadHr(impObj()->get_Height(&rtval));
    return rtval;
}

void PyIAcadViewImpl::SetHeight(double val)
{
    PyThrowBadHr(impObj()->put_Height(val));
}

double PyIAcadViewImpl::GetWidth() const
{
    double rtval = 0.0;
    PyThrowBadHr(impObj()->get_Width(&rtval));
    return rtval;
}

void PyIAcadViewImpl::SetWidth(double val)
{
    PyThrowBadHr(impObj()->put_Width(val));
}

AcGePoint3d PyIAcadViewImpl::GetTarget() const
{
    _variant_t vtval;
    AcGePoint3d rtVal;
    PyThrowBadHr(impObj()->get_Target(&vtval));
    PyThrowBadHr(VariantToAcGePoint3d(vtval, rtVal));
    return rtVal;
}

void PyIAcadViewImpl::SetTarget(const AcGePoint3d& val)
{
    _variant_t vtval;
    PyThrowBadHr(AcGePoint3dToVariant(vtval.GetVARIANT(), val));
    PyThrowBadHr(impObj()->put_Target(vtval));
}

AcGeVector3d PyIAcadViewImpl::GetDirection() const
{
    AcGeVector3d val;
    _variant_t coord;
    PyThrowBadHr(impObj()->get_Direction(&coord.GetVARIANT()));
    PyThrowBadHr(VariantToAcGeVector3d(coord, val));
    return val;

}

void PyIAcadViewImpl::SetDirection(const AcGeVector3d& val)
{
    _variant_t vtval;
    PyThrowBadHr(AcGeVector3dToVariant(vtval.GetVARIANT(), val));
    PyThrowBadHr(impObj()->put_Direction(vtval));
}

CString PyIAcadViewImpl::GetName() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_Name(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadViewImpl::SetName(const CString& val)
{
    _bstr_t bstrval{ val };
    PyThrowBadHr(impObj()->put_Name(bstrval));
}

CString PyIAcadViewImpl::GetCategoryName() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_CategoryName(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadViewImpl::SetCategoryName(const CString& val)
{
    _bstr_t bstrval{ val };
    PyThrowBadHr(impObj()->put_CategoryName(bstrval));
}

AcDbObjectId PyIAcadViewImpl::GetLayoutId() const
{
    AcDbObjectId id;
    LONG_PTR rtval = 0;
    PyThrowBadHr(impObj()->get_LayoutId(&rtval));
    id.setFromOldId(rtval);
    return id;
}

void PyIAcadViewImpl::SetLayoutId(const AcDbObjectId& val)
{
    PyThrowBadHr(impObj()->put_LayoutId(val.asOldId()));
}

CString PyIAcadViewImpl::GetLayerState() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_LayerState(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadViewImpl::SetLayerState(const CString& val)
{
    _bstr_t bstrval{ val };
    PyThrowBadHr(impObj()->put_LayerState(bstrval));
}

bool PyIAcadViewImpl::GetHasVpAssociation() const
{
    VARIANT_BOOL rtVal = VARIANT_FALSE;
    PyThrowBadHr(impObj()->get_HasVpAssociation(&rtVal));
    return rtVal != VARIANT_FALSE;
}

void PyIAcadViewImpl::SetHasVpAssociation(bool val)
{
    PyThrowBadHr(impObj()->put_HasVpAssociation(val ? VARIANT_TRUE : VARIANT_FALSE));
}

IAcadView* PyIAcadViewImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadView*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadGroupImpl
PyIAcadGroupImpl::PyIAcadGroupImpl(IAcadGroup* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadGroup* PyIAcadGroupImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadGroup*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadGroupsImpl
PyIAcadGroupsImpl::PyIAcadGroupsImpl(IAcadGroups* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadGroupPtr PyIAcadGroupsImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadGroup* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadGroupImpl>(ptr);
}

long PyIAcadGroupsImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadGroupPtr PyIAcadGroupsImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadGroup* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadGroupImpl>(ptr);
}

PyIAcadGroupPtrArray PyIAcadGroupsImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadGroupPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadGroupImpl>((IAcadGroup*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadGroups* PyIAcadGroupsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadGroups*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadDimStyleImpl
PyIAcadDimStyleImpl::PyIAcadDimStyleImpl(IAcadDimStyle* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadDimStyle* PyIAcadDimStyleImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadDimStyle*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadDimStylesImpl
PyIAcadDimStylesImpl::PyIAcadDimStylesImpl(IAcadDimStyles* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadDimStylePtr PyIAcadDimStylesImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadDimStyle* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadDimStyleImpl>(ptr);
}

long PyIAcadDimStylesImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadDimStylePtr PyIAcadDimStylesImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadDimStyle* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadDimStyleImpl>(ptr);

}

PyIAcadDimStylePtrArray PyIAcadDimStylesImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadDimStylePtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadDimStyleImpl>((IAcadDimStyle*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadDimStyles* PyIAcadDimStylesImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadDimStyles*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadLayerImpl
PyIAcadLayerImpl::PyIAcadLayerImpl(IAcadLayer* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadLayer* PyIAcadLayerImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadLayer*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadLayersImpl
PyIAcadLayersImpl::PyIAcadLayersImpl(IAcadLayers* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadLayerPtr PyIAcadLayersImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadLayer* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadLayerImpl>(ptr);
}

long PyIAcadLayersImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadLayerPtr PyIAcadLayersImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadLayer* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadLayerImpl>(ptr);
}

PyIAcadLayerPtrArray PyIAcadLayersImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadLayerPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadLayerImpl>((IAcadLayer*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadLayers* PyIAcadLayersImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadLayers*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadLineTypeImpl
PyIAcadLineTypeImpl::PyIAcadLineTypeImpl(IAcadLineType* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadLineType* PyIAcadLineTypeImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadLineType*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadLineTypesImpl
PyIAcadLineTypesImpl::PyIAcadLineTypesImpl(IAcadLineTypes* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadLineTypePtr PyIAcadLineTypesImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadLineType* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadLineTypeImpl>(ptr);

}

long PyIAcadLineTypesImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadLineTypePtr PyIAcadLineTypesImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadLineType* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadLineTypeImpl>(ptr);
}

PyIAcadLineTypePtrArray PyIAcadLineTypesImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadLineTypePtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadLineTypeImpl>((IAcadLineType*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadLineTypes* PyIAcadLineTypesImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadLineTypes*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadXRecordImpl
PyIAcadXRecordImpl::PyIAcadXRecordImpl(IAcadXRecord* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadXRecord* PyIAcadXRecordImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadXRecord*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadDictionaryImpl
PyIAcadDictionaryImpl::PyIAcadDictionaryImpl(IAcadDictionary* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadDictionary* PyIAcadDictionaryImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadDictionary*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadDictionariesImpl
PyIAcadDictionariesImpl::PyIAcadDictionariesImpl(IAcadDictionaries* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadDictionaryPtr PyIAcadDictionariesImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadObject* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadDictionaryImpl>((IAcadDictionary*)ptr);
}

long PyIAcadDictionariesImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadDictionaryPtr PyIAcadDictionariesImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadDictionary* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadDictionaryImpl>((IAcadDictionary*)ptr);

}

PyIAcadDictionaryPtrArray PyIAcadDictionariesImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadDictionaryPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadDictionaryImpl>((IAcadDictionary*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadDictionaries* PyIAcadDictionariesImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadDictionaries*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadRegisteredApplicationImpl
PyIAcadRegisteredApplicationImpl::PyIAcadRegisteredApplicationImpl(IAcadRegisteredApplication* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

CString PyIAcadRegisteredApplicationImpl::GetName() const
{
    _bstr_t bstrVal;
    PyThrowBadHr(impObj()->get_Name(&bstrVal.GetBSTR()));
    return (LPCTSTR)bstrVal;
}

void PyIAcadRegisteredApplicationImpl::SetName(const CString& val) const
{
    _bstr_t bstrVal{ val };
    PyThrowBadHr(impObj()->put_Name(bstrVal));
}

IAcadRegisteredApplication* PyIAcadRegisteredApplicationImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadRegisteredApplication*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadRegisteredApplicationsImpl
PyIAcadRegisteredApplicationsImpl::PyIAcadRegisteredApplicationsImpl(IAcadRegisteredApplications* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadRegisteredApplicationPtr PyIAcadRegisteredApplicationsImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadRegisteredApplication* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadRegisteredApplicationImpl>(ptr);
}

long PyIAcadRegisteredApplicationsImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadRegisteredApplicationPtr PyIAcadRegisteredApplicationsImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadRegisteredApplication* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadRegisteredApplicationImpl>(ptr);
}

PyIAcadRegisteredApplicationPtrArray PyIAcadRegisteredApplicationsImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadRegisteredApplicationPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadRegisteredApplicationImpl>((IAcadRegisteredApplication*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadRegisteredApplications* PyIAcadRegisteredApplicationsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadRegisteredApplications*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadTextStyleImpl
PyIAcadTextStyleImpl::PyIAcadTextStyleImpl(IAcadTextStyle* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadTextStyle* PyIAcadTextStyleImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadTextStyle*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadTextStylesImpl
PyIAcadTextStylesImpl::PyIAcadTextStylesImpl(IAcadTextStyles* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadTextStylePtr PyIAcadTextStylesImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadTextStyle* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadTextStyleImpl>(ptr);
}

long PyIAcadTextStylesImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;

}

PyIAcadTextStylePtr PyIAcadTextStylesImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadTextStyle* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadTextStyleImpl>(ptr);

}

PyIAcadTextStylePtrArray PyIAcadTextStylesImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadTextStylePtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadTextStyleImpl>((IAcadTextStyle*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadTextStyles* PyIAcadTextStylesImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadTextStyles*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadUCSImpl
PyIAcadUCSImpl::PyIAcadUCSImpl(IAcadUCS* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadUCS* PyIAcadUCSImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadUCS*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadUCSsImpl
PyIAcadUCSsImpl::PyIAcadUCSsImpl(IAcadUCSs* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadUCSPtr PyIAcadUCSsImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadUCS* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadUCSImpl>(ptr);
}

long PyIAcadUCSsImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadUCSPtr PyIAcadUCSsImpl::Add(const AcGePoint3d& origin, const AcGeVector3d& xAxis, const AcGeVector3d& yAxis, const CString& name) const
{
    variant_t vtorigin;
    variant_t vtxAxis;
    variant_t vtyAxis;
    _bstr_t bstrVal{ name };
    IAcadUCS* ptr = nullptr;
    PyThrowBadHr(AcGePoint3dToVariant(vtorigin.GetVARIANT(), origin));
    PyThrowBadHr(AcGeVector3dToVariant(vtxAxis.GetVARIANT(), xAxis));
    PyThrowBadHr(AcGeVector3dToVariant(vtyAxis.GetVARIANT(), yAxis));
    PyThrowBadHr(impObj()->Add(vtorigin, vtxAxis, vtyAxis, bstrVal, &ptr));
    return std::make_unique<PyIAcadUCSImpl>(ptr);
}

PyIAcadUCSPtrArray PyIAcadUCSsImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadUCSPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadUCSImpl>((IAcadUCS*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadUCSs* PyIAcadUCSsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadUCSs*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadViewsImpl
PyIAcadViewsImpl::PyIAcadViewsImpl(IAcadViews* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadViewPtr PyIAcadViewsImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadView* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadViewImpl>(ptr);
}

long PyIAcadViewsImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadViewPtr PyIAcadViewsImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadView* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadViewImpl>(ptr);
}

PyIAcadViewPtrArray PyIAcadViewsImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadViewPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadViewImpl>((IAcadView*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadViews* PyIAcadViewsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadViews*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadViewportImpl
PyIAcadViewportImpl::PyIAcadViewportImpl(IAcadViewport* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadViewport* PyIAcadViewportImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadViewport*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadViewportsImpl
PyIAcadViewportsImpl::PyIAcadViewportsImpl(IAcadViewports* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadViewportPtr PyIAcadViewportsImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadViewport* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadViewportImpl>(ptr);
}

long PyIAcadViewportsImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadViewportPtr PyIAcadViewportsImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadViewport* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadViewportImpl>(ptr);
}

PyIAcadViewportPtrArray PyIAcadViewportsImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadViewportPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadViewportImpl>((IAcadViewport*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadViewports* PyIAcadViewportsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadViewports*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadPlotConfigurationsImpl
PyIAcadPlotConfigurationsImpl::PyIAcadPlotConfigurationsImpl(IAcadPlotConfigurations* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadPlotConfigurationPtr PyIAcadPlotConfigurationsImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadPlotConfiguration* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadPlotConfigurationImpl>(ptr);
}

long PyIAcadPlotConfigurationsImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadPlotConfigurationPtr PyIAcadPlotConfigurationsImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadPlotConfiguration* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, vtMissing, &ptr));
    return std::make_unique<PyIAcadPlotConfigurationImpl>(ptr);
}

PyIAcadPlotConfigurationPtrArray PyIAcadPlotConfigurationsImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadPlotConfigurationPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadPlotConfigurationImpl>((IAcadPlotConfiguration*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadPlotConfigurations* PyIAcadPlotConfigurationsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadPlotConfigurations*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadSectionManagerImpl
PyIAcadSectionManagerImpl::PyIAcadSectionManagerImpl(IAcadSectionManager* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadSectionManager* PyIAcadSectionManagerImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadSectionManager*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadMaterialImpl
PyIAcadMaterialImpl::PyIAcadMaterialImpl(IAcadMaterial* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadMaterial* PyIAcadMaterialImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadMaterial*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadMaterialsImpl
PyIAcadMaterialsImpl::PyIAcadMaterialsImpl(IAcadMaterials* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadMaterialPtr PyIAcadMaterialsImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadMaterial* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadMaterialImpl>(ptr);
}

long PyIAcadMaterialsImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadMaterialPtr PyIAcadMaterialsImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadMaterial* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadMaterialImpl>(ptr);
}

PyIAcadMaterialPtrArray PyIAcadMaterialsImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadMaterialPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadMaterialImpl>((IAcadMaterial*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadMaterials* PyIAcadMaterialsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadMaterials*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadTableStyleImpl
PyIAcadTableStyleImpl::PyIAcadTableStyleImpl(IAcadTableStyle* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadTableStyle* PyIAcadTableStyleImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadTableStyle*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadMLeaderStyleImpl
PyIAcadMLeaderStyleImpl::PyIAcadMLeaderStyleImpl(IAcadMLeaderStyle* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadMLeaderStyle* PyIAcadMLeaderStyleImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadMLeaderStyle*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadLayoutsImpl
PyIAcadLayoutsImpl::PyIAcadLayoutsImpl(IAcadLayouts* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

PyIAcadLayoutPtr PyIAcadLayoutsImpl::GetItem(long ind) const
{
    _variant_t vtind{ ind };
    IAcadLayout* ptr = nullptr;
    PyThrowBadHr(impObj()->Item(vtind, &ptr));
    return std::make_unique<PyIAcadLayoutImpl>(ptr);
}

long PyIAcadLayoutsImpl::GetCount() const
{
    long ind = 0;
    PyThrowBadHr(impObj()->get_Count(&ind));
    return ind;
}

PyIAcadLayoutPtr PyIAcadLayoutsImpl::Add(const CString& name) const
{
    _bstr_t bstrVal{ name };
    IAcadLayout* ptr = nullptr;
    PyThrowBadHr(impObj()->Add(bstrVal, &ptr));
    return std::make_unique<PyIAcadLayoutImpl>(ptr);
}

PyIAcadLayoutPtrArray PyIAcadLayoutsImpl::GetIter() const
{
    IUnknownPtr pUnk;
    IEnumVARIANTPtr vtenum;
    PyIAcadLayoutPtrArray vec;
    PyThrowBadHr(impObj()->get__NewEnum((IUnknown**)&pUnk));
    PyThrowBadHr(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&vtenum));
    {
        HRESULT hr = S_OK;
        for (unsigned long idx = 0, iout = 0; hr == S_OK; idx++)
        {
            _variant_t item;
            if (hr = vtenum->Next(1, &item.GetVARIANT(), &iout); hr == S_OK)
                vec.emplace_back(std::make_shared<PyIAcadLayoutImpl>((IAcadLayout*)(IDispatch*)item));
        }
    }
    return vec;
}

IAcadLayouts* PyIAcadLayoutsImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadLayouts*>(m_pimpl.GetInterfacePtr());
}

//------------------------------------------------------------------------------------
//PyIAcadSortentsTableImpl
PyIAcadSortentsTableImpl::PyIAcadSortentsTableImpl(IAcadSortentsTable* ptr)
    : PyIAcadObjectImpl(ptr)
{
}

IAcadSortentsTable* PyIAcadSortentsTableImpl::impObj(const std::source_location& src /*= std::source_location::current()*/) const
{
    if (m_pimpl == nullptr) [[unlikely]] {
        throw PyNullObject(src);
    }
    return static_cast<IAcadSortentsTable*>(m_pimpl.GetInterfacePtr());
}

