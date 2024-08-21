from typing import TYPE_CHECKING
import warnings


class PyRxModule:
    def __init__(self, name: str):
        self.name = name

    def __getattr__(self, name):
        raise ModuleNotFoundError(
            f"The {self.name!r} module is not available, it must be "
            "invoked from a CAD application."
        )


try:
    import PyRx as Rx  # isort: skip  # type: ignore
    import PyGe as Ge  # isort: skip  # type: ignore
    import PyGi as Gi  # isort: skip  # type: ignore
    import PyDb as Db  # isort: skip  # type: ignore
    import PyAp as Ap  # isort: skip  # type: ignore
    import PyEd as Ed  # isort: skip  # type: ignore
    import PyPl as Pl  # isort: skip  # type: ignore
    import PyGs as Gs  # isort: skip  # type: ignore
except ModuleNotFoundError:
    warnings.warn(
        "PyRx modules are not available, they must be invoked from a CAD application."
    )
    Rx = PyRxModule("PyRx")
    Ge = PyRxModule("PyGe")
    Gi = PyRxModule("PyGi")
    Db = PyRxModule("PyDb")
    Ap = PyRxModule("PyAp")
    Ed = PyRxModule("PyEd")
    Pl = PyRxModule("PyPl")
    Gs = PyRxModule("PyGs")

if TYPE_CHECKING:
    from .PyRxStubs import PyAp as Ap  # noqa: F811  # type: ignore
    from .PyRxStubs import PyDb as Db  # noqa: F811  # type: ignore
    from .PyRxStubs import PyEd as Ed  # noqa: F811  # type: ignore
    from .PyRxStubs import PyGe as Ge  # noqa: F811  # type: ignore
    from .PyRxStubs import PyGi as Gi  # noqa: F811  # type: ignore
    from .PyRxStubs import PyGs as Gs  # noqa: F811  # type: ignore
    from .PyRxStubs import PyPl as Pl  # noqa: F811  # type: ignore
    from .PyRxStubs import PyRx as Rx  # noqa: F811  # type: ignore

__all__ = ("Ap", "Db", "Ed", "Ge", "Gi", "Gs", "Pl", "Rx")

__version__ = "1.3.25"
