#pragma once

#ifdef PYRXDEBUG

#include "PyAcad.h" 

//------------------------------------------------------------------------------------
//PyIAcadObjectImpl
class PyIAcadObjectImpl
{
public:
    explicit PyIAcadObjectImpl(IAcadObject* ptr);
    virtual ~PyIAcadObjectImpl() = default;
    IAcadObject* impObj(const std::source_location& src = std::source_location::current()) const;
protected:
    IAcadObjectPtr m_pimpl;
};

//------------------------------------------------------------------------------------
//PyIAcadPlotConfigurationImpl
class PyIAcadPlotConfigurationImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadPlotConfigurationImpl(IAcadPlotConfiguration* ptr);
    virtual ~PyIAcadPlotConfigurationImpl() = default;
    IAcadPlotConfiguration* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadSectionSettingsImpl
class PyIAcadSectionSettingsImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadSectionSettingsImpl(IAcadSectionSettings* ptr);
    virtual ~PyIAcadSectionSettingsImpl() = default;
    IAcadSectionSettings* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadViewImpl
class PyIAcadViewImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadViewImpl(IAcadView* ptr);
    virtual ~PyIAcadViewImpl() = default;
    IAcadView* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadGroupImpl
class PyIAcadGroupImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadGroupImpl(IAcadGroup* ptr);
    virtual ~PyIAcadGroupImpl() = default;
    IAcadGroup* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadGroupsImpl
class PyIAcadGroupsImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadGroupsImpl(IAcadGroups* ptr);
    virtual ~PyIAcadGroupsImpl() = default;
    IAcadGroups* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadDimStyleImpl
class PyIAcadDimStyleImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadDimStyleImpl(IAcadDimStyle* ptr);
    virtual ~PyIAcadDimStyleImpl() = default;
    IAcadDimStyle* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadDimStylesImpl
class PyIAcadDimStylesImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadDimStylesImpl(IAcadDimStyles* ptr);
    virtual ~PyIAcadDimStylesImpl() = default;
    IAcadDimStyles* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadLayerImpl
class PyIAcadLayerImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadLayerImpl(IAcadLayer* ptr);
    virtual ~PyIAcadLayerImpl() = default;
    IAcadLayer* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadLayersImpl
class PyIAcadLayersImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadLayersImpl(IAcadLayers* ptr);
    virtual ~PyIAcadLayersImpl() = default;
    IAcadLayers* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadLineTypeImpl
class PyIAcadLineTypeImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadLineTypeImpl(IAcadLineType* ptr);
    virtual ~PyIAcadLineTypeImpl() = default;
    IAcadLineType* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadLineTypesImpl
class PyIAcadLineTypesImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadLineTypesImpl(IAcadLineTypes* ptr);
    virtual ~PyIAcadLineTypesImpl() = default;
    IAcadLineTypes* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadXRecordImpl
class PyIAcadXRecordImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadXRecordImpl(IAcadXRecord* ptr);
    virtual ~PyIAcadXRecordImpl() = default;
    IAcadXRecord* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadDictionaryImpl
class PyIAcadDictionaryImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadDictionaryImpl(IAcadDictionary* ptr);
    virtual ~PyIAcadDictionaryImpl() = default;
    IAcadDictionary* impObj(const std::source_location& src = std::source_location::current()) const;
};

//------------------------------------------------------------------------------------
//PyIAcadDictionariesImpl
class PyIAcadDictionariesImpl : public PyIAcadObjectImpl
{
public:
    explicit PyIAcadDictionariesImpl(IAcadDictionaries* ptr);
    virtual ~PyIAcadDictionariesImpl() = default;
    IAcadDictionaries* impObj(const std::source_location& src = std::source_location::current()) const;
};






#endif