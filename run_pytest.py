import importlib
import sys
import traceback
from pathlib import Path

import pytest
from pyrx_imp import *  # noqa
from PyRxDebug import startListener as PyRxCmd_debug  # noqa

from pyrx.console import console, std_redirect

BASE_DIR = Path(__file__).parent
PYTEST_ARGS_FILE = BASE_DIR / "pytest_args.txt"


def PyRxCmd_pytest():
    try:
        reload_tests_and_pyrx()
        with console(allow_existing=False):
            conin = open("CONIN$", "r")
            conout = open("CONOUT$", "w")
            with std_redirect(conin, conout, conout):
                pytest_args = PYTEST_ARGS_FILE.read_text("utf-8").splitlines() if PYTEST_ARGS_FILE.exists() else []
                print(f"{pytest_args=}\n")
                pytest.main(pytest_args)
                input("Press any key to continue . . . ")
    except Exception:
        traceback.print_exc()
    finally:
        conin.close()
        conout.close()


def OnPyReload() -> None:
    reload_tests_and_pyrx()


def reload_tests_and_pyrx():

    to_reload = tuple(
        (name, module) for name, module in sys.modules.items() if name.startswith("tests") or name.startswith("pyrx")
    )
    for name, _ in to_reload:
        sys.modules.pop(name, None)

    for name, _ in to_reload:
        importlib.import_module(name)
