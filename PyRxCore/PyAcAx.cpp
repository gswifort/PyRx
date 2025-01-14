#include "stdafx.h"
#include "PyAcAx.h"
#include "PyAcadObject.h"
#include "PyAcadEntity.h"
#include "PyAcadApplication.h"
using namespace boost::python;

BOOST_PYTHON_MODULE(PyAx)
{
    makePyAcadStateWrapper();
    makePyAcadObjectWrapper();
    makePyAcadEntityWrapper();
    makePyAcadDatabaseWrapper();
    makePyAcadDocumentWrapper();
    makePyAcadDocumentsWrapper();
    makePyAcadApplicationWrapper();

    enum_<PyAcSectionGeneration>("AcSectionGeneration")
        .value("acSectionGenerationSourceAllObjects", PyAcSectionGeneration::pyacSectionGenerationSourceAllObjects)
        .value("acSectionGenerationSourceSelectedObjects", PyAcSectionGeneration::pyacSectionGenerationSourceSelectedObjects)
        .value("acSectionGenerationDestinationNewBlock", PyAcSectionGeneration::pyacSectionGenerationDestinationNewBlock)
        .value("acSectionGenerationDestinationReplaceBlock", PyAcSectionGeneration::pyacSectionGenerationDestinationReplaceBlock)
        .value("acSectionGenerationDestinationFile", PyAcSectionGeneration::pyacSectionGenerationDestinationFile)
        .export_values()
        ;

    enum_<PyAcWindowState>("AcWindowState")
        .value("acNorm", PyAcWindowState::pyacNorm)
        .value("acMin", PyAcWindowState::pyacMin)
        .value("acMax", PyAcWindowState::pyacMax)
        .export_values()
        ;

    enum_<PyAcColorMethod>("AcColorMethod")
        .value("acColorMethodByLayer", PyAcColorMethod::pyacColorMethodByLayer)
        .value("acColorMethodByBlock", PyAcColorMethod::pyacColorMethodByBlock)
        .value("acColorMethodByRGB", PyAcColorMethod::pyacColorMethodByRGB)
        .value("acColorMethodByACI", PyAcColorMethod::pyacColorMethodByACI)
        .value("acColorMethodForeground", PyAcColorMethod::pyacColorMethodForeground)
        .export_values()
        ;

    enum_<PyAcColor>("AcColor")
        .value("acByBlock", PyAcColor::pyacByBlock)
        .value("acRed", PyAcColor::pyacRed)
        .value("acYellow", PyAcColor::pyacYellow)
        .value("acGreen", PyAcColor::pyacGreen)
        .value("acCyan", PyAcColor::pyacCyan)
        .value("acBlue", PyAcColor::pyacBlue)
        .value("acMagenta", PyAcColor::pyacMagenta)
        .value("acWhite", PyAcColor::pyacWhite)
        .value("acByLayer", PyAcColor::pyacByLayer)
        .export_values()
        ;

    enum_<PyAcZoomScaleType>("AcZoomScaleType")
        .value("acZoomScaledAbsolute", PyAcZoomScaleType::pyacZoomScaledAbsolute)
        .value("acZoomScaledRelative", PyAcZoomScaleType::pyacZoomScaledRelative)
        .value("acZoomScaledRelativePSpace", PyAcZoomScaleType::pyacZoomScaledRelativePSpace)
        .export_values()
        ;

}
void initPyAxModule()
{
    PyImport_AppendInittab(PyAxNamespace, &PyInit_PyAx);
}
