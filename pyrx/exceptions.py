from __future__ import annotations

import re
import types
import typing as t


class DefaultMessageMixin:
    default_message: str

    def __init__(self, *args, **kwargs) -> None:
        if args:
            super().__init__(*args, **kwargs)
        else:
            super().__init__(self.default_message, **kwargs)


class Error(Exception):
    """Base exception class for all ``pyrx`` errors."""


class ARXException(Error):

    _subclasses: dict[str, type[ARXException]] = {}
    err_num: t.Optional[int] = None
    err_name: t.Optional[str] = None

    def __init_subclass__(cls) -> None:
        super().__init_subclass__()

        err_name = cls.err_name
        if err_name in ARXException._subclasses:
            raise TypeError(f"ARXException subclass with {err_name=!r} already exists")
        ARXException._subclasses[err_name] = cls


class ARXExceptionTranslator:
    RUNTIME_ERROR_PATT = re.compile(r"^\nException!\((?P<err_name>\w+)\), (?P<err_msg>.*)$")

    _instance = None

    def __new__(cls) -> t.Self:
        if cls._instance is None:
            cls._instance = object.__new__(cls)
        return cls._instance

    @classmethod
    def _parse_runtimeerror(cls, e: RuntimeError, /):
        m = cls.RUNTIME_ERROR_PATT.match(str(e))
        if m is None:
            return None
        gd = m.groupdict()
        err_name = gd["err_name"]
        err_msg = gd["err_msg"]
        err = ARXException._subclasses.get(err_name, None)
        if err is None:
            return None
        return err, err_msg

    def __enter__(self):
        return self

    def __exit__(
        self, exc_type: type[Exception] | None, exc_value: Exception | None, traceback: types.TracebackType | None
    ):
        if exc_type is None:
            return
        res = self._parse_runtimeerror(exc_value)
        if res is None:
            raise
        arx_err_type, arx_err_msg = res
        raise arx_err_type(arx_err_msg).with_traceback(traceback) from exc_value


cast_to_arx_exception = ARXExceptionTranslator()


# ############## #
# ARX Exceptions #
# ############## #


class eNotImplementedYet(ARXException):
    err_num = 1
    err_name = "eNotImplementedYet"


class eNotApplicable(ARXException):
    err_num = 2
    err_name = "eNotApplicable"


class eInvalidInput(ARXException):
    err_num = 3
    err_name = "eInvalidInput"


class eAmbiguousInput(ARXException):
    err_num = 4
    err_name = "eAmbiguousInput"


class eAmbiguousOutput(ARXException):
    err_num = 5
    err_name = "eAmbiguousOutput"


class eOutOfMemory(ARXException):
    err_num = 6
    err_name = "eOutOfMemory"


class eBufferTooSmall(ARXException):
    err_num = 7
    err_name = "eBufferTooSmall"


class eInvalidOpenState(ARXException):
    err_num = 8
    err_name = "eInvalidOpenState"


class eEntityInInactiveLayout(ARXException):
    err_num = 9
    err_name = "eEntityInInactiveLayout"


class eHandleExists(ARXException):
    err_num = 10
    err_name = "eHandleExists"


class eNullHandle(ARXException):
    err_num = 11
    err_name = "eNullHandle"


class eBrokenHandle(ARXException):
    err_num = 12
    err_name = "eBrokenHandle"


class eUnknownHandle(ARXException):
    err_num = 13
    err_name = "eUnknownHandle"


class eHandleInUse(ARXException):
    err_num = 14
    err_name = "eHandleInUse"


class eNullObjectPointer(ARXException):
    err_num = 15
    err_name = "eNullObjectPointer"


class eNullObjectId(ARXException):
    err_num = 16
    err_name = "eNullObjectId"


class eNullBlockName(ARXException):
    err_num = 17
    err_name = "eNullBlockName"


class eContainerNotEmpty(ARXException):
    err_num = 18
    err_name = "eContainerNotEmpty"


class eNullEntityPointer(ARXException):
    err_num = 20
    err_name = "eNullEntityPointer"


class eIllegalEntityType(ARXException):
    err_num = 21
    err_name = "eIllegalEntityType"


class eKeyNotFound(ARXException):
    err_num = 22
    err_name = "eKeyNotFound"


class eDuplicateKey(ARXException):
    err_num = 23
    err_name = "eDuplicateKey"


class eInvalidIndex(ARXException):
    err_num = 24
    err_name = "eInvalidIndex"


class eDuplicateIndex(ARXException):
    err_num = 25
    err_name = "eDuplicateIndex"


class eAlreadyInDb(ARXException):
    err_num = 26
    err_name = "eAlreadyInDb"


class eOutOfDisk(ARXException):
    err_num = 27
    err_name = "eOutOfDisk"


class eDeletedEntry(ARXException):
    err_num = 28
    err_name = "eDeletedEntry"


class eNegativeValueNotAllowed(ARXException):
    err_num = 29
    err_name = "eNegativeValueNotAllowed"


class eInvalidExtents(ARXException):
    err_num = 30
    err_name = "eInvalidExtents"


class eInvalidAdsName(ARXException):
    err_num = 31
    err_name = "eInvalidAdsName"


class eInvalidSymbolTableName(ARXException):
    err_num = 32
    err_name = "eInvalidSymbolTableName"


class eInvalidKey(ARXException):
    err_num = 33
    err_name = "eInvalidKey"


class eWrongObjectType(ARXException):
    err_num = 34
    err_name = "eWrongObjectType"


class eWrongDatabase(ARXException):
    err_num = 35
    err_name = "eWrongDatabase"


class eObjectToBeDeleted(ARXException):
    err_num = 36
    err_name = "eObjectToBeDeleted"


class eInvalidDwgVersion(ARXException):
    err_num = 37
    err_name = "eInvalidDwgVersion"


class eAnonymousEntry(ARXException):
    err_num = 38
    err_name = "eAnonymousEntry"


class eIllegalReplacement(ARXException):
    err_num = 39
    err_name = "eIllegalReplacement"


class eEndOfObject(ARXException):
    err_num = 40
    err_name = "eEndOfObject"


class eEndOfFile(ARXException):
    err_num = 41
    err_name = "eEndOfFile"


class eIsReading(ARXException):
    err_num = 42
    err_name = "eIsReading"


class eIsWriting(ARXException):
    err_num = 43
    err_name = "eIsWriting"


class eNotOpenForRead(ARXException):
    err_num = 44
    err_name = "eNotOpenForRead"


class eNotOpenForWrite(ARXException):
    err_num = 45
    err_name = "eNotOpenForWrite"


class eNotThatKindOfClass(ARXException):
    err_num = 46
    err_name = "eNotThatKindOfClass"


class eInvalidBlockName(ARXException):
    err_num = 47
    err_name = "eInvalidBlockName"


class eMissingDxfField(ARXException):
    err_num = 48
    err_name = "eMissingDxfField"


class eDuplicateDxfField(ARXException):
    err_num = 49
    err_name = "eDuplicateDxfField"


class eInvalidDxfCode(ARXException):
    err_num = 50
    err_name = "eInvalidDxfCode"


class eInvalidResBuf(ARXException):
    err_num = 51
    err_name = "eInvalidResBuf"


class eBadDxfSequence(ARXException):
    err_num = 52
    err_name = "eBadDxfSequence"


class eFilerError(ARXException):
    err_num = 53
    err_name = "eFilerError"


class eVertexAfterFace(ARXException):
    err_num = 54
    err_name = "eVertexAfterFace"


class eInvalidFaceVertexIndex(ARXException):
    err_num = 55
    err_name = "eInvalidFaceVertexIndex"


class eInvalidMeshVertexIndex(ARXException):
    err_num = 56
    err_name = "eInvalidMeshVertexIndex"


class eOtherObjectsBusy(ARXException):
    err_num = 57
    err_name = "eOtherObjectsBusy"


class eMustFirstAddBlockToDb(ARXException):
    err_num = 58
    err_name = "eMustFirstAddBlockToDb"


class eCannotNestBlockDefs(ARXException):
    err_num = 59
    err_name = "eCannotNestBlockDefs"


class eDwgRecoveredOK(ARXException):
    err_num = 60
    err_name = "eDwgRecoveredOK"


class eDwgNotRecoverable(ARXException):
    err_num = 61
    err_name = "eDwgNotRecoverable"


class eDxfPartiallyRead(ARXException):
    err_num = 62
    err_name = "eDxfPartiallyRead"


class eDxfReadAborted(ARXException):
    err_num = 63
    err_name = "eDxfReadAborted"


class eDxbPartiallyRead(ARXException):
    err_num = 64
    err_name = "eDxbPartiallyRead"


class eDwgCRCDoesNotMatch(ARXException):
    err_num = 65
    err_name = "eDwgCRCDoesNotMatch"


class eDwgSentinelDoesNotMatch(ARXException):
    err_num = 66
    err_name = "eDwgSentinelDoesNotMatch"


class eDwgObjectImproperlyRead(ARXException):
    err_num = 67
    err_name = "eDwgObjectImproperlyRead"


class eNoInputFiler(ARXException):
    err_num = 68
    err_name = "eNoInputFiler"


class eDwgNeedsAFullSave(ARXException):
    err_num = 69
    err_name = "eDwgNeedsAFullSave"


class eDxbReadAborted(ARXException):
    err_num = 70
    err_name = "eDxbReadAborted"


class eFileLockedByACAD(ARXException):
    err_num = 71
    err_name = "eFileLockedByACAD"


class eFileAccessErr(ARXException):
    err_num = 72
    err_name = "eFileAccessErr"


class eFileSystemErr(ARXException):
    err_num = 73
    err_name = "eFileSystemErr"


class eFileInternalErr(ARXException):
    err_num = 74
    err_name = "eFileInternalErr"


class eFileTooManyOpen(ARXException):
    err_num = 75
    err_name = "eFileTooManyOpen"


class eFileNotFound(ARXException):
    err_num = 76
    err_name = "eFileNotFound"


class eDwkLockFileFound(ARXException):
    err_num = 77
    err_name = "eDwkLockFileFound"


class eWasErased(ARXException):
    err_num = 80
    err_name = "eWasErased"


class ePermanentlyErased(ARXException):
    err_num = 81
    err_name = "ePermanentlyErased"


class eWasOpenForRead(ARXException):
    err_num = 82
    err_name = "eWasOpenForRead"


class eWasOpenForWrite(ARXException):
    err_num = 83
    err_name = "eWasOpenForWrite"


class eWasOpenForUndo(ARXException):
    err_num = 84
    err_name = "eWasOpenForUndo"


class eWasNotifying(ARXException):
    err_num = 85
    err_name = "eWasNotifying"


class eWasOpenForNotify(ARXException):
    err_num = 86
    err_name = "eWasOpenForNotify"


class eOnLockedLayer(ARXException):
    err_num = 87
    err_name = "eOnLockedLayer"


class eMustOpenThruOwner(ARXException):
    err_num = 88
    err_name = "eMustOpenThruOwner"


class eSubentitiesStillOpen(ARXException):
    err_num = 89
    err_name = "eSubentitiesStillOpen"


class eAtMaxReaders(ARXException):
    err_num = 90
    err_name = "eAtMaxReaders"


class eIsWriteProtected(ARXException):
    err_num = 91
    err_name = "eIsWriteProtected"


class eIsXRefObject(ARXException):
    err_num = 92
    err_name = "eIsXRefObject"


class eNotAnEntity(ARXException):
    err_num = 93
    err_name = "eNotAnEntity"


class eHadMultipleReaders(ARXException):
    err_num = 94
    err_name = "eHadMultipleReaders"


class eDuplicateRecordName(ARXException):
    err_num = 95
    err_name = "eDuplicateRecordName"


class eXRefDependent(ARXException):
    err_num = 96
    err_name = "eXRefDependent"


class eSelfReference(ARXException):
    err_num = 97
    err_name = "eSelfReference"


class eMissingSymbolTable(ARXException):
    err_num = 98
    err_name = "eMissingSymbolTable"


class eMissingSymbolTableRec(ARXException):
    err_num = 99
    err_name = "eMissingSymbolTableRec"


class eWasNotOpenForWrite(ARXException):
    err_num = 100
    err_name = "eWasNotOpenForWrite"


class eCloseWasNotifying(ARXException):
    err_num = 101
    err_name = "eCloseWasNotifying"


class eCloseModifyAborted(ARXException):
    err_num = 102
    err_name = "eCloseModifyAborted"


class eClosePartialFailure(ARXException):
    err_num = 103
    err_name = "eClosePartialFailure"


class eCloseFailObjectDamaged(ARXException):
    err_num = 104
    err_name = "eCloseFailObjectDamaged"


class eCannotBeErasedByCaller(ARXException):
    err_num = 105
    err_name = "eCannotBeErasedByCaller"


class eCannotBeResurrected(ARXException):
    err_num = 106
    err_name = "eCannotBeResurrected"


class eWasNotErased(ARXException):
    err_num = 107
    err_name = "eWasNotErased"


class eInsertAfter(ARXException):
    err_num = 110
    err_name = "eInsertAfter"


class eFixedAllErrors(ARXException):
    err_num = 120
    err_name = "eFixedAllErrors"


class eLeftErrorsUnfixed(ARXException):
    err_num = 122
    err_name = "eLeftErrorsUnfixed"


class eUnrecoverableErrors(ARXException):
    err_num = 123
    err_name = "eUnrecoverableErrors"


class eNoDatabase(ARXException):
    err_num = 124
    err_name = "eNoDatabase"


class eXdataSizeExceeded(ARXException):
    err_num = 125
    err_name = "eXdataSizeExceeded"


class eRegappIdNotFound(ARXException):
    err_num = 126
    err_name = "eRegappIdNotFound"


class eRepeatEntity(ARXException):
    err_num = 127
    err_name = "eRepeatEntity"


class eRecordNotInTable(ARXException):
    err_num = 128
    err_name = "eRecordNotInTable"


class eIteratorDone(ARXException):
    err_num = 129
    err_name = "eIteratorDone"


class eNullIterator(ARXException):
    err_num = 130
    err_name = "eNullIterator"


class eNotInBlock(ARXException):
    err_num = 131
    err_name = "eNotInBlock"


class eOwnerNotInDatabase(ARXException):
    err_num = 132
    err_name = "eOwnerNotInDatabase"


class eOwnerNotOpenForRead(ARXException):
    err_num = 133
    err_name = "eOwnerNotOpenForRead"


class eOwnerNotOpenForWrite(ARXException):
    err_num = 134
    err_name = "eOwnerNotOpenForWrite"


class eExplodeBeforeTransform(ARXException):
    err_num = 135
    err_name = "eExplodeBeforeTransform"


class eCannotScaleNonUniformly(ARXException):
    err_num = 136
    err_name = "eCannotScaleNonUniformly"


class eNotInDatabase(ARXException):
    err_num = 137
    err_name = "eNotInDatabase"


class eNotCurrentDatabase(ARXException):
    err_num = 138
    err_name = "eNotCurrentDatabase"


class eIsAnEntity(ARXException):
    err_num = 139
    err_name = "eIsAnEntity"


class eCannotChangeActiveViewport(ARXException):
    err_num = 140
    err_name = "eCannotChangeActiveViewport"


class eNotInPaperspace(ARXException):
    err_num = 141
    err_name = "eNotInPaperspace"


class eCommandWasInProgress(ARXException):
    err_num = 142
    err_name = "eCommandWasInProgress"


class eGeneralModelingFailure(ARXException):
    err_num = 150
    err_name = "eGeneralModelingFailure"


class eOutOfRange(ARXException):
    err_num = 151
    err_name = "eOutOfRange"


class eNonCoplanarGeometry(ARXException):
    err_num = 152
    err_name = "eNonCoplanarGeometry"


class eDegenerateGeometry(ARXException):
    err_num = 153
    err_name = "eDegenerateGeometry"


class eInvalidAxis(ARXException):
    err_num = 154
    err_name = "eInvalidAxis"


class ePointNotOnEntity(ARXException):
    err_num = 155
    err_name = "ePointNotOnEntity"


class eSingularPoint(ARXException):
    err_num = 156
    err_name = "eSingularPoint"


class eInvalidOffset(ARXException):
    err_num = 157
    err_name = "eInvalidOffset"


class eNonPlanarEntity(ARXException):
    err_num = 158
    err_name = "eNonPlanarEntity"


class eCannotExplodeEntity(ARXException):
    err_num = 159
    err_name = "eCannotExplodeEntity"


class eStringTooLong(ARXException):
    err_num = 160
    err_name = "eStringTooLong"


class eInvalidSymTableFlag(ARXException):
    err_num = 161
    err_name = "eInvalidSymTableFlag"


class eUndefinedLineType(ARXException):
    err_num = 162
    err_name = "eUndefinedLineType"


class eInvalidTextStyle(ARXException):
    err_num = 163
    err_name = "eInvalidTextStyle"


class eTooFewLineTypeElements(ARXException):
    err_num = 164
    err_name = "eTooFewLineTypeElements"


class eTooManyLineTypeElements(ARXException):
    err_num = 165
    err_name = "eTooManyLineTypeElements"


class eExcessiveItemCount(ARXException):
    err_num = 166
    err_name = "eExcessiveItemCount"


class eIgnoredLinetypeRedef(ARXException):
    err_num = 167
    err_name = "eIgnoredLinetypeRedef"


class eBadUCS(ARXException):
    err_num = 168
    err_name = "eBadUCS"


class eBadPaperspaceView(ARXException):
    err_num = 169
    err_name = "eBadPaperspaceView"


class eSomeInputDataLeftUnread(ARXException):
    err_num = 170
    err_name = "eSomeInputDataLeftUnread"


class eNoInternalSpace(ARXException):
    err_num = 171
    err_name = "eNoInternalSpace"


class eInvalidDimStyle(ARXException):
    err_num = 172
    err_name = "eInvalidDimStyle"


class eInvalidLayer(ARXException):
    err_num = 173
    err_name = "eInvalidLayer"


class eUserBreak(ARXException):
    err_num = 180
    err_name = "eUserBreak"


class eUserUnloaded(ARXException):
    err_num = 181
    err_name = "eUserUnloaded"


class eDwgNeedsRecovery(ARXException):
    err_num = 190
    err_name = "eDwgNeedsRecovery"


class eDeleteEntity(ARXException):
    err_num = 191
    err_name = "eDeleteEntity"


class eInvalidFix(ARXException):
    err_num = 192
    err_name = "eInvalidFix"


class eFSMError(ARXException):
    err_num = 193
    err_name = "eFSMError"


class eBadLayerName(ARXException):
    err_num = 200
    err_name = "eBadLayerName"


class eLayerGroupCodeMissing(ARXException):
    err_num = 201
    err_name = "eLayerGroupCodeMissing"


class eBadColorIndex(ARXException):
    err_num = 202
    err_name = "eBadColorIndex"


class eBadLinetypeName(ARXException):
    err_num = 203
    err_name = "eBadLinetypeName"


class eBadLinetypeScale(ARXException):
    err_num = 204
    err_name = "eBadLinetypeScale"


class eBadVisibilityValue(ARXException):
    err_num = 205
    err_name = "eBadVisibilityValue"


class eProperClassSeparatorExpected(ARXException):
    err_num = 206
    err_name = "eProperClassSeparatorExpected"


class eBadLineWeightValue(ARXException):
    err_num = 207
    err_name = "eBadLineWeightValue"


class eBadColor(ARXException):
    err_num = 208
    err_name = "eBadColor"


class eBadMaterialName(ARXException):
    err_num = 209
    err_name = "eBadMaterialName"


class ePagerError(ARXException):
    err_num = 210
    err_name = "ePagerError"


class eOutOfPagerMemory(ARXException):
    err_num = 211
    err_name = "eOutOfPagerMemory"


class ePagerWriteError(ARXException):
    err_num = 212
    err_name = "ePagerWriteError"


class eWasNotForwarding(ARXException):
    err_num = 213
    err_name = "eWasNotForwarding"


class eInvalidIdMap(ARXException):
    err_num = 220
    err_name = "eInvalidIdMap"


class eInvalidOwnerObject(ARXException):
    err_num = 221
    err_name = "eInvalidOwnerObject"


class eOwnerNotSet(ARXException):
    err_num = 222
    err_name = "eOwnerNotSet"


class eWrongSubentityType(ARXException):
    err_num = 230
    err_name = "eWrongSubentityType"


class eTooManyVertices(ARXException):
    err_num = 231
    err_name = "eTooManyVertices"


class eTooFewVertices(ARXException):
    err_num = 232
    err_name = "eTooFewVertices"


class eNoActiveTransactions(ARXException):
    err_num = 250
    err_name = "eNoActiveTransactions"


class eNotTopTransaction(ARXException):
    err_num = 251
    err_name = "eNotTopTransaction"


class eTransactionOpenWhileCommandEnded(ARXException):
    err_num = 252
    err_name = "eTransactionOpenWhileCommandEnded"


class eInProcessOfCommitting(ARXException):
    err_num = 253
    err_name = "eInProcessOfCommitting"


class eNotNewlyCreated(ARXException):
    err_num = 254
    err_name = "eNotNewlyCreated"


class eLongTransReferenceError(ARXException):
    err_num = 255
    err_name = "eLongTransReferenceError"


class eNoWorkSet(ARXException):
    err_num = 256
    err_name = "eNoWorkSet"


class eAlreadyInGroup(ARXException):
    err_num = 260
    err_name = "eAlreadyInGroup"


class eNotInGroup(ARXException):
    err_num = 261
    err_name = "eNotInGroup"


class eInvalidREFIID(ARXException):
    err_num = 290
    err_name = "eInvalidREFIID"


class eInvalidNormal(ARXException):
    err_num = 291
    err_name = "eInvalidNormal"


class eInvalidStyle(ARXException):
    err_num = 292
    err_name = "eInvalidStyle"


class eCannotRestoreFromAcisFile(ARXException):
    err_num = 300
    err_name = "eCannotRestoreFromAcisFile"


class eMakeMeProxy(ARXException):
    err_num = 301
    err_name = "eMakeMeProxy"


class eNLSFileNotAvailable(ARXException):
    err_num = 302
    err_name = "eNLSFileNotAvailable"


class eNotAllowedForThisProxy(ARXException):
    err_num = 303
    err_name = "eNotAllowedForThisProxy"


class eNotClonedPrimaryProxy(ARXException):
    err_num = 304
    err_name = "eNotClonedPrimaryProxy"


class eNotSupportedInDwgApi(ARXException):
    err_num = 310
    err_name = "eNotSupportedInDwgApi"


class ePolyWidthLost(ARXException):
    err_num = 311
    err_name = "ePolyWidthLost"


class eNullExtents(ARXException):
    err_num = 312
    err_name = "eNullExtents"


class eExplodeAgain(ARXException):
    err_num = 313
    err_name = "eExplodeAgain"


class eBadDwgHeader(ARXException):
    err_num = 314
    err_name = "eBadDwgHeader"


class eLockViolation(ARXException):
    err_num = 320
    err_name = "eLockViolation"


class eLockConflict(ARXException):
    err_num = 321
    err_name = "eLockConflict"


class eDatabaseObjectsOpen(ARXException):
    err_num = 322
    err_name = "eDatabaseObjectsOpen"


class eLockChangeInProgress(ARXException):
    err_num = 323
    err_name = "eLockChangeInProgress"


class eVetoed(ARXException):
    err_num = 325
    err_name = "eVetoed"


class eNoDocument(ARXException):
    err_num = 330
    err_name = "eNoDocument"


class eNotFromThisDocument(ARXException):
    err_num = 331
    err_name = "eNotFromThisDocument"


class eLISPActive(ARXException):
    err_num = 332
    err_name = "eLISPActive"


class eTargetDocNotQuiescent(ARXException):
    err_num = 333
    err_name = "eTargetDocNotQuiescent"


class eDocumentSwitchDisabled(ARXException):
    err_num = 334
    err_name = "eDocumentSwitchDisabled"


class eInvalidContext(ARXException):
    err_num = 335
    err_name = "eInvalidContext"


class eCreateFailed(ARXException):
    err_num = 337
    err_name = "eCreateFailed"


class eCreateInvalidName(ARXException):
    err_num = 338
    err_name = "eCreateInvalidName"


class eSetFailed(ARXException):
    err_num = 340
    err_name = "eSetFailed"


class eDelDoesNotExist(ARXException):
    err_num = 342
    err_name = "eDelDoesNotExist"


class eDelIsModelSpace(ARXException):
    err_num = 343
    err_name = "eDelIsModelSpace"


class eDelLastLayout(ARXException):
    err_num = 344
    err_name = "eDelLastLayout"


class eDelUnableToSetCurrent(ARXException):
    err_num = 345
    err_name = "eDelUnableToSetCurrent"


class eDelUnableToFind(ARXException):
    err_num = 346
    err_name = "eDelUnableToFind"


class eRenameDoesNotExist(ARXException):
    err_num = 348
    err_name = "eRenameDoesNotExist"


class eRenameIsModelSpace(ARXException):
    err_num = 349
    err_name = "eRenameIsModelSpace"


class eRenameInvalidLayoutName(ARXException):
    err_num = 350
    err_name = "eRenameInvalidLayoutName"


class eRenameLayoutAlreadyExists(ARXException):
    err_num = 351
    err_name = "eRenameLayoutAlreadyExists"


class eRenameInvalidName(ARXException):
    err_num = 352
    err_name = "eRenameInvalidName"


class eCopyDoesNotExist(ARXException):
    err_num = 354
    err_name = "eCopyDoesNotExist"


class eCopyIsModelSpace(ARXException):
    err_num = 355
    err_name = "eCopyIsModelSpace"


class eCopyFailed(ARXException):
    err_num = 356
    err_name = "eCopyFailed"


class eCopyInvalidName(ARXException):
    err_num = 357
    err_name = "eCopyInvalidName"


class eCopyNameExists(ARXException):
    err_num = 358
    err_name = "eCopyNameExists"


class eProfileDoesNotExist(ARXException):
    err_num = 359
    err_name = "eProfileDoesNotExist"


class eInvalidFileExtension(ARXException):
    err_num = 360
    err_name = "eInvalidFileExtension"


class eInvalidProfileName(ARXException):
    err_num = 361
    err_name = "eInvalidProfileName"


class eFileExists(ARXException):
    err_num = 362
    err_name = "eFileExists"


class eProfileIsInUse(ARXException):
    err_num = 363
    err_name = "eProfileIsInUse"


class eCantOpenFile(ARXException):
    err_num = 364
    err_name = "eCantOpenFile"


class eNoFileName(ARXException):
    err_num = 365
    err_name = "eNoFileName"


class eRegistryAccessError(ARXException):
    err_num = 366
    err_name = "eRegistryAccessError"


class eRegistryCreateError(ARXException):
    err_num = 367
    err_name = "eRegistryCreateError"


class eBadDxfFile(ARXException):
    err_num = 368
    err_name = "eBadDxfFile"


class eUnknownDxfFileFormat(ARXException):
    err_num = 369
    err_name = "eUnknownDxfFileFormat"


class eMissingDxfSection(ARXException):
    err_num = 370
    err_name = "eMissingDxfSection"


class eInvalidDxfSectionName(ARXException):
    err_num = 371
    err_name = "eInvalidDxfSectionName"


class eNotDxfHeaderGroupCode(ARXException):
    err_num = 372
    err_name = "eNotDxfHeaderGroupCode"


class eUndefinedDxfGroupCode(ARXException):
    err_num = 373
    err_name = "eUndefinedDxfGroupCode"


class eNotInitializedYet(ARXException):
    err_num = 374
    err_name = "eNotInitializedYet"


class eInvalidDxf2dPoint(ARXException):
    err_num = 375
    err_name = "eInvalidDxf2dPoint"


class eInvalidDxf3dPoint(ARXException):
    err_num = 376
    err_name = "eInvalidDxf3dPoint"


class eBadlyNestedAppData(ARXException):
    err_num = 378
    err_name = "eBadlyNestedAppData"


class eIncompleteBlockDefinition(ARXException):
    err_num = 379
    err_name = "eIncompleteBlockDefinition"


class eIncompleteComplexObject(ARXException):
    err_num = 380
    err_name = "eIncompleteComplexObject"


class eBlockDefInEntitySection(ARXException):
    err_num = 381
    err_name = "eBlockDefInEntitySection"


class eNoBlockBegin(ARXException):
    err_num = 382
    err_name = "eNoBlockBegin"


class eDuplicateLayerName(ARXException):
    err_num = 383
    err_name = "eDuplicateLayerName"


class eBadPlotStyleName(ARXException):
    err_num = 384
    err_name = "eBadPlotStyleName"


class eDuplicateBlockName(ARXException):
    err_num = 385
    err_name = "eDuplicateBlockName"


class eBadPlotStyleType(ARXException):
    err_num = 386
    err_name = "eBadPlotStyleType"


class eBadPlotStyleNameHandle(ARXException):
    err_num = 387
    err_name = "eBadPlotStyleNameHandle"


class eUndefineShapeName(ARXException):
    err_num = 388
    err_name = "eUndefineShapeName"


class eDuplicateBlockDefinition(ARXException):
    err_num = 389
    err_name = "eDuplicateBlockDefinition"


class eMissingBlockName(ARXException):
    err_num = 390
    err_name = "eMissingBlockName"


class eBinaryDataSizeExceeded(ARXException):
    err_num = 391
    err_name = "eBinaryDataSizeExceeded"


class eObjectIsReferenced(ARXException):
    err_num = 392
    err_name = "eObjectIsReferenced"


class eNoThumbnailBitmap(ARXException):
    err_num = 393
    err_name = "eNoThumbnailBitmap"


class eGuidNoAddress(ARXException):
    err_num = 394
    err_name = "eGuidNoAddress"


class eMustBe0to2(ARXException):
    err_num = 395
    err_name = "eMustBe0to2"


class eMustBe0to3(ARXException):
    err_num = 396
    err_name = "eMustBe0to3"


class eMustBe0to4(ARXException):
    err_num = 397
    err_name = "eMustBe0to4"


class eMustBe0to5(ARXException):
    err_num = 398
    err_name = "eMustBe0to5"


class eMustBe0to8(ARXException):
    err_num = 399
    err_name = "eMustBe0to8"


class eMustBe1to8(ARXException):
    err_num = 400
    err_name = "eMustBe1to8"


class eMustBe1to15(ARXException):
    err_num = 401
    err_name = "eMustBe1to15"


class eMustBePositive(ARXException):
    err_num = 402
    err_name = "eMustBePositive"


class eMustBeNonNegative(ARXException):
    err_num = 403
    err_name = "eMustBeNonNegative"


class eMustBeNonZero(ARXException):
    err_num = 404
    err_name = "eMustBeNonZero"


class eMustBe1to6(ARXException):
    err_num = 405
    err_name = "eMustBe1to6"


class eNoPlotStyleTranslationTable(ARXException):
    err_num = 406
    err_name = "eNoPlotStyleTranslationTable"


class ePlotStyleInColorDependentMode(ARXException):
    err_num = 407
    err_name = "ePlotStyleInColorDependentMode"


class eMaxLayouts(ARXException):
    err_num = 408
    err_name = "eMaxLayouts"


class eNoClassId(ARXException):
    err_num = 409
    err_name = "eNoClassId"


class eUndoOperationNotAvailable(ARXException):
    err_num = 410
    err_name = "eUndoOperationNotAvailable"


class eUndoNoGroupBegin(ARXException):
    err_num = 411
    err_name = "eUndoNoGroupBegin"


class eHatchTooDense(ARXException):
    err_num = 420
    err_name = "eHatchTooDense"


class eOpenFileCancelled(ARXException):
    err_num = 430
    err_name = "eOpenFileCancelled"


class eNotHandled(ARXException):
    err_num = 431
    err_name = "eNotHandled"


class eMakeMeProxyAndResurrect(ARXException):
    err_num = 432
    err_name = "eMakeMeProxyAndResurrect"


class eFileSharingViolation(ARXException):
    err_num = 433
    err_name = "eFileSharingViolation"


class eUnsupportedFileFormat(ARXException):
    err_num = 434
    err_name = "eUnsupportedFileFormat"


class eObsoleteFileFormat(ARXException):
    err_num = 435
    err_name = "eObsoleteFileFormat"


class eFileMissingSections(ARXException):
    err_num = 436
    err_name = "eFileMissingSections"


class eRepeatedDwgRead(ARXException):
    err_num = 437
    err_name = "eRepeatedDwgRead"


class eWrongCellType(ARXException):
    err_num = 440
    err_name = "eWrongCellType"


class eCannotChangeColumnType(ARXException):
    err_num = 441
    err_name = "eCannotChangeColumnType"


class eRowsMustMatchColumns(ARXException):
    err_num = 442
    err_name = "eRowsMustMatchColumns"


class eNullNodeId(ARXException):
    err_num = 450
    err_name = "eNullNodeId"


class eNoNodeActive(ARXException):
    err_num = 451
    err_name = "eNoNodeActive"


class eGraphContainsProxies(ARXException):
    err_num = 452
    err_name = "eGraphContainsProxies"


class eDwgShareDemandLoad(ARXException):
    err_num = 500
    err_name = "eDwgShareDemandLoad"


class eDwgShareReadAccess(ARXException):
    err_num = 501
    err_name = "eDwgShareReadAccess"


class eDwgShareWriteAccess(ARXException):
    err_num = 502
    err_name = "eDwgShareWriteAccess"


class eLoadFailed(ARXException):
    err_num = 503
    err_name = "eLoadFailed"


class eDeviceNotFound(ARXException):
    err_num = 504
    err_name = "eDeviceNotFound"


class eNoCurrentConfig(ARXException):
    err_num = 505
    err_name = "eNoCurrentConfig"


class eNullPtr(ARXException):
    err_num = 506
    err_name = "eNullPtr"


class eNoLayout(ARXException):
    err_num = 507
    err_name = "eNoLayout"


class eIncompatiblePlotSettings(ARXException):
    err_num = 508
    err_name = "eIncompatiblePlotSettings"


class eNonePlotDevice(ARXException):
    err_num = 509
    err_name = "eNonePlotDevice"


class eNoMatchingMedia(ARXException):
    err_num = 510
    err_name = "eNoMatchingMedia"


class eInvalidView(ARXException):
    err_num = 511
    err_name = "eInvalidView"


class eInvalidWindowArea(ARXException):
    err_num = 512
    err_name = "eInvalidWindowArea"


class eInvalidPlotArea(ARXException):
    err_num = 513
    err_name = "eInvalidPlotArea"


class eCustomSizeNotPossible(ARXException):
    err_num = 514
    err_name = "eCustomSizeNotPossible"


class ePageCancelled(ARXException):
    err_num = 515
    err_name = "ePageCancelled"


class ePlotCancelled(ARXException):
    err_num = 516
    err_name = "ePlotCancelled"


class eInvalidEngineState(ARXException):
    err_num = 517
    err_name = "eInvalidEngineState"


class ePlotAlreadyStarted(ARXException):
    err_num = 518
    err_name = "ePlotAlreadyStarted"


class eNoErrorHandler(ARXException):
    err_num = 519
    err_name = "eNoErrorHandler"


class eInvalidPlotInfo(ARXException):
    err_num = 520
    err_name = "eInvalidPlotInfo"


class eNumberOfCopiesNotSupported(ARXException):
    err_num = 521
    err_name = "eNumberOfCopiesNotSupported"


class eLayoutNotCurrent(ARXException):
    err_num = 522
    err_name = "eLayoutNotCurrent"


class eGraphicsNotGenerated(ARXException):
    err_num = 523
    err_name = "eGraphicsNotGenerated"


class eCannotPlotToFile(ARXException):
    err_num = 524
    err_name = "eCannotPlotToFile"


class eMustPlotToFile(ARXException):
    err_num = 525
    err_name = "eMustPlotToFile"


class eNotMultiPageCapable(ARXException):
    err_num = 526
    err_name = "eNotMultiPageCapable"


class eBackgroundPlotInProgress(ARXException):
    err_num = 527
    err_name = "eBackgroundPlotInProgress"


class eNotShownInPropertyPalette(ARXException):
    err_num = 528
    err_name = "eNotShownInPropertyPalette"


class eSubSelectionSetEmpty(ARXException):
    err_num = 550
    err_name = "eSubSelectionSetEmpty"


class eNoIntersections(ARXException):
    err_num = 551
    err_name = "eNoIntersections"


class eEmbeddedIntersections(ARXException):
    err_num = 552
    err_name = "eEmbeddedIntersections"


class eNoOverride(ARXException):
    err_num = 570
    err_name = "eNoOverride"


class eNoStoredOverrides(ARXException):
    err_num = 571
    err_name = "eNoStoredOverrides"


class eUnableToRetrieveOverrides(ARXException):
    err_num = 572
    err_name = "eUnableToRetrieveOverrides"


class eUnableToStoreOverrides(ARXException):
    err_num = 573
    err_name = "eUnableToStoreOverrides"


class eUnableToRemoveOverrides(ARXException):
    err_num = 574
    err_name = "eUnableToRemoveOverrides"


class eNoStoredReconcileStatus(ARXException):
    err_num = 580
    err_name = "eNoStoredReconcileStatus"


class eUnableToStoreReconcileStatus(ARXException):
    err_num = 581
    err_name = "eUnableToStoreReconcileStatus"


class eInvalidObjectId(ARXException):
    err_num = 600
    err_name = "eInvalidObjectId"


class eInvalidXrefObjectId(ARXException):
    err_num = 601
    err_name = "eInvalidXrefObjectId"


class eNoViewAssociation(ARXException):
    err_num = 602
    err_name = "eNoViewAssociation"


class eNoLabelBlock(ARXException):
    err_num = 603
    err_name = "eNoLabelBlock"


class eUnableToSetViewAssociation(ARXException):
    err_num = 604
    err_name = "eUnableToSetViewAssociation"


class eUnableToGetViewAssociation(ARXException):
    err_num = 605
    err_name = "eUnableToGetViewAssociation"


class eUnableToSetLabelBlock(ARXException):
    err_num = 606
    err_name = "eUnableToSetLabelBlock"


class eUnableToGetLabelBlock(ARXException):
    err_num = 607
    err_name = "eUnableToGetLabelBlock"


class eUnableToRemoveAssociation(ARXException):
    err_num = 608
    err_name = "eUnableToRemoveAssociation"


class eUnableToSyncModelView(ARXException):
    err_num = 609
    err_name = "eUnableToSyncModelView"


class eDataLinkAdapterNotFound(ARXException):
    err_num = 650
    err_name = "eDataLinkAdapterNotFound"


class eDataLinkInvalidAdapterId(ARXException):
    err_num = 651
    err_name = "eDataLinkInvalidAdapterId"


class eDataLinkNotFound(ARXException):
    err_num = 652
    err_name = "eDataLinkNotFound"


class eDataLinkBadConnectionString(ARXException):
    err_num = 653
    err_name = "eDataLinkBadConnectionString"


class eDataLinkNotUpdatedYet(ARXException):
    err_num = 654
    err_name = "eDataLinkNotUpdatedYet"


class eDataLinkSourceNotFound(ARXException):
    err_num = 655
    err_name = "eDataLinkSourceNotFound"


class eDataLinkConnectionFailed(ARXException):
    err_num = 656
    err_name = "eDataLinkConnectionFailed"


class eDataLinkSourceUpdateNotAllowed(ARXException):
    err_num = 657
    err_name = "eDataLinkSourceUpdateNotAllowed"


class eDataLinkSourceIsWriteProtected(ARXException):
    err_num = 658
    err_name = "eDataLinkSourceIsWriteProtected"


class eDataLinkExcelNotFound(ARXException):
    err_num = 659
    err_name = "eDataLinkExcelNotFound"


class eDataLinkOtherError(ARXException):
    err_num = 660
    err_name = "eDataLinkOtherError"


class eXrefReloaded(ARXException):
    err_num = 700
    err_name = "eXrefReloaded"


class eSecInitializationFailure(ARXException):
    err_num = 1001
    err_name = "eSecInitializationFailure"


class eSecErrorReadingFile(ARXException):
    err_num = 1002
    err_name = "eSecErrorReadingFile"


class eSecErrorWritingFile(ARXException):
    err_num = 1003
    err_name = "eSecErrorWritingFile"


class eSecInvalidDigitalID(ARXException):
    err_num = 1101
    err_name = "eSecInvalidDigitalID"


class eSecErrorGeneratingTimestamp(ARXException):
    err_num = 1102
    err_name = "eSecErrorGeneratingTimestamp"


class eSecErrorComputingSignature(ARXException):
    err_num = 1103
    err_name = "eSecErrorComputingSignature"


class eSecErrorWritingSignature(ARXException):
    err_num = 1104
    err_name = "eSecErrorWritingSignature"


class eSecErrorEncryptingData(ARXException):
    err_num = 1201
    err_name = "eSecErrorEncryptingData"


class eSecErrorCipherNotSupported(ARXException):
    err_num = 1202
    err_name = "eSecErrorCipherNotSupported"


class eSecErrorDecryptingData(ARXException):
    err_num = 1203
    err_name = "eSecErrorDecryptingData"


class eNoAcDbHostApplication(ARXException):
    err_num = 1300
    err_name = "eNoAcDbHostApplication"


class eNoUnderlayHost(ARXException):
    err_num = 1301
    err_name = "eNoUnderlayHost"


class eInetBase(ARXException):
    err_num = 20000
    err_name = "eInetBase"
