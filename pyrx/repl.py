from __future__ import annotations

import abc
import builtins
import os
import queue
import sys
import threading
import time
import traceback
import types
import typing as t
from code import InteractiveConsole

import win32api
import win32con
import win32gui
from prompt_toolkit.input import create_input
from prompt_toolkit.output import create_output
from ptpython.repl import PythonRepl
from pyrx_imp import Ap, Db, Ed, Ge, Gi, Gs, Pl, Rx, Sm
from PyRxDebug import PyRxCmd_debug  # noqa: F401

from pyrx.console import console, std_redirect
from pyrx.utils.sentinels import Sentinel

_UNSET = Sentinel("Unset")


class ReplMixin(abc.ABC):
    """
    A mixin for implementing REPLs that run in a separate CAD application thread and
    invoke compiled code in the main thread.

    Derived classes must implement the `.run_code_in_main_thread()` method, which is run
    in the main thread, and takes the compiled code passed by the
    `.send_code_and_receive_result()` method as an argument (also to be implemented by
    derived classes).

    Additionally, derived classes must implement the `.run_repl()` method that starts
    the main REPL loop, this method is called by the `.run()` method.
    """

    WND_PROC_MSG = win32con.WM_USER + 1

    def __init__(self, stdin: t.TextIO = _UNSET, stdout: t.TextIO = _UNSET, stderr: t.TextIO = _UNSET) -> None:
        self._res_queue = queue.Queue(1)
        self._code_queue: queue.Queue[types.CodeType] = queue.Queue(1)
        self.stdin = stdin
        self.stdout = stdout
        self.stderr = stderr

    @abc.abstractmethod
    def run_code_in_main_thread(self, code: types.CodeType) -> t.Any:
        """
        Function to call the compiled code in the main thread.
        """

    @abc.abstractmethod
    def run_repl(self, *args, **kwargs) -> None:
        """
        Function called by the ``.run()`` function, the main function that starts the
        interpreter loop.
        """

    def run(self, *args, **kwargs) -> None:
        self._add_wnd_proc_hook()
        try:
            self.run_repl(*args, **kwargs)
        finally:
            self._restore_wnd_proc_hook()

    def send_code_and_receive_result(self, code: types.CodeType) -> t.Any:
        """
        Send compiled code to call in CAD application main thread and receive result.
        """
        self._code_queue.put(code)
        # send a message to the main thread to execute the code
        win32gui.PostMessage(Ap.Application.mainWnd(), self.WND_PROC_MSG, 0, 0)
        success, res = self._res_queue.get()
        if success:
            return res
        else:
            raise res

    def _std_redirect(self):
        kwargs = {
            k: v
            for k, v in (("stdin", self.stdin), ("stdout", self.stdout), ("stderr", self.stderr))
            if v is not _UNSET
        }
        return std_redirect(**kwargs)

    def _runcode_in_main_thread(self) -> None:
        code = self._code_queue.get_nowait()
        if not isinstance(code, types.CodeType):
            raise TypeError(f"code must be a compiled python code, not {code.__class__.__name__}")
        try:
            with self._std_redirect():
                res = self.run_code_in_main_thread(code)
        except BaseException as e:
            self._res_queue.put((False, e))
        else:
            self._res_queue.put((True, res))

    def is_repl_msg(self, msg, w_param, l_param) -> bool:
        return msg == self.WND_PROC_MSG

    def my_wnd_proc(self, hwnd, msg, w_param, l_param):
        if self.is_repl_msg(msg, w_param, l_param):
            try:
                self._runcode_in_main_thread()
            except Exception:
                pass  # call self._old_wnd_proc
            else:
                return 0
        return win32gui.CallWindowProc(self._old_wnd_proc, hwnd, msg, w_param, l_param)

    def _add_wnd_proc_hook(self) -> None:
        self._old_wnd_proc = win32gui.SetWindowLong(Ap.Application.mainWnd(), win32con.GWL_WNDPROC, self.my_wnd_proc)

    def _restore_wnd_proc_hook(self) -> None:
        win32api.SetWindowLong(Ap.Application.mainWnd(), win32con.GWL_WNDPROC, self._old_wnd_proc)


class StdLibRepl(ReplMixin, InteractiveConsole):
    WND_PROC_MSG = 1111

    def __init__(self, *, stdin=_UNSET, stdout=_UNSET, stderr=_UNSET, locals=None, filename="<console>") -> None:
        ReplMixin.__init__(self, stdin=stdin, stdout=stdout, stderr=stderr)
        InteractiveConsole.__init__(self, locals=locals, filename=filename)

    def run_repl(self, *args, **kwargs) -> None:
        InteractiveConsole.interact(self, *args, **kwargs)

    def run_code_in_main_thread(self, code: types.CodeType) -> None:
        InteractiveConsole.runcode(self, code)

    # override InteractiveConsole.runcode
    def runcode(self, code: types.CodeType) -> t.Any:
        return self.send_code_and_receive_result(code)

    # override InteractiveConsole.raw_input
    def raw_input(self, prompt: str = "") -> str:
        if prompt:
            self.stdout.write(prompt)
            self.stdout.flush()
        return self.stdin.readline()

    def write(self, data) -> int:
        return self.stdout.write(data)


class PtPythonRepl(ReplMixin, PythonRepl):
    def __init__(self, *, stdin=_UNSET, stdout=_UNSET, stderr=_UNSET, globals=None, locals=None, **kwargs) -> None:

        if globals is None:
            globals = {
                "__name__": "__main__",
                "__package__": None,
                "__doc__": None,
                "__builtins__": builtins,
            }

        locals = locals or globals

        def get_globals():
            return globals

        def get_locals():
            return locals

        ReplMixin.__init__(self, stdin=stdin, stdout=stdout, stderr=stderr)
        PythonRepl.__init__(self, get_globals=get_globals, get_locals=get_locals, **kwargs)

    def run_code_in_main_thread(self, code: types.CodeType) -> None:
        return eval(code, self.get_globals(), self.get_locals())

    # override PythonRepl.runcode
    def eval(self, line: str) -> t.Any:
        """
        Evaluate the line and print the result.
        """
        # WORKAROUND: Due to a bug in Jedi, the current directory is removed
        # from sys.path. See: https://github.com/davidhalter/jedi/issues/1148
        if "" not in sys.path:
            sys.path.insert(0, "")

        if line.lstrip().startswith("!"):
            # Run as shell command
            os.system(line[1:])
        else:
            # Try eval first
            try:
                code = self._compile_with_flags(line, "eval")
            except SyntaxError:
                pass
            else:
                # No syntax errors for eval. Do eval.
                result = self.send_code_and_receive_result(code)  # override !

                self._store_eval_result(result)
                return result

            # If not a valid `eval` expression, run using `exec` instead.
            # Note that we shouldn't run this in the `except SyntaxError` block
            # above, then `sys.exc_info()` would not report the right error.
            # See issue: https://github.com/prompt-toolkit/ptpython/issues/435
            code = self._compile_with_flags(line, "exec")
            result = self.send_code_and_receive_result(code)  # override !

        return None

    def run_repl(self, *args, **kwargs) -> None:
        PythonRepl.run(self, *args, **kwargs)


def _get_repl_namespace():
    return {
        "Ap": Ap,
        "Db": Db,
        "Ed": Ed,
        "Ge": Ge,
        "Gi": Gi,
        "Gs": Gs,
        "Pl": Pl,
        "Rx": Rx,
        "Sm": Sm,
        "entsel": entsel,
        "select": select,
        "curdb": curdb,
    }


def run_std_lib_repl():
    time.sleep(0.1)
    with console(allow_existing=False):
        conin = open("CONIN$", "r")
        conout = open("CONOUT$", "w")
        repl = StdLibRepl(
            locals=_get_repl_namespace(),
            stdin=conin,
            stdout=conout,
            stderr=conout,
        )
        repl.run()


def run_ptpython_repl():
    time.sleep(0.1)
    with console(allow_existing=False):
        conin = open("CONIN$", "r")
        conout = open("CONOUT$", "w")
        output = create_output(conout)
        with std_redirect(stdin=conin):
            input = create_input()
        repl = PtPythonRepl(
            globals=_get_repl_namespace(),
            input=input,
            output=output,
            stdin=conin,
            stdout=conout,
            stderr=conout,
        )
        repl.run()


# shortcut functions


def entsel(prompt="Select object: ") -> tuple[Ed.PromptStatus, Db.ObjectId, Ge.Point3d]:
    """Select single entity."""
    return Ed.Editor.entSel(prompt)


def select(prompt="Select objects: ", filters=()) -> tuple[Ed.PromptStatus, Ed.SelectionSet]:
    """Select multiple entities."""
    return Ed.Editor.selectPrompt(prompt, "", list(filters))


def curdb() -> Db.Database:
    """Return current database."""
    return Db.curDb()


# CAD commands


def PyRxCmd_stdrepl():
    try:
        t = threading.Thread(target=run_std_lib_repl)
        t.start()
    except Exception:
        traceback.print_exc()


def PyRxCmd_ptrepl():
    try:
        t = threading.Thread(target=run_ptpython_repl)
        t.start()
    except Exception:
        traceback.print_exc()
