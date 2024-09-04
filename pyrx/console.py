import ctypes
import sys
import typing as t
from contextlib import contextmanager

import win32api
import win32console

from pyrx.exceptions import DefaultMessageMixin
from pyrx.utils.sentinels import Sentinel

_UNSET = Sentinel("Unset")


class ConsoleAlreadyExist(DefaultMessageMixin, Exception):
    default_message = (
        "Cannot access new console. Check if you already have "
        "another instance open (only one console can be attached)"
    )


@contextmanager
def std_redirect(stdin: t.Any = _UNSET, stdout: t.Any = _UNSET, stderr: t.Any = _UNSET):
    """
    Context manager.
    Redirect ``sys.stdin``, ``sys.stdout`` and ``sys.stderr``.

    Examples::

        with open("output.txt", "w") as f:
            with std_redirect(stdout=f):
                print("TEST")
    """
    if stdin is not _UNSET:
        old_stdin = sys.stdin
        sys.stdin = stdin
    if stdout is not _UNSET:
        old_stdout = sys.stdout
        sys.stdout = stdout
    if stderr is not _UNSET:
        old_stderr = sys.stderr
        sys.stderr = stderr

    try:
        yield (sys.stdin, sys.stdout, sys.stderr)
    finally:
        if stdin is not _UNSET:
            sys.stdin = old_stdin
        if stdout is not _UNSET:
            sys.stdout = old_stdout
        if stderr is not _UNSET:
            sys.stderr = old_stderr


@contextmanager
def console(allow_existing=False):
    console_exists = win32console.GetConsoleWindow()
    if console_exists and not allow_existing:
        raise ConsoleAlreadyExist
    try:
        win32console.AllocConsole()
    except win32console.error as e:
        winerr = ctypes.WinError(e.winerror, e.strerror)
        winerr.add_note("Unable to allocate console")
        raise winerr from None
    win32api.SetConsoleCtrlHandler(console_ctrl_handler, True)
    try:
        yield
    finally:
        if not console_exists:
            with open("CONOUT$", "w") as f:
                print("You can close the console . . .", file=f, flush=True)
            win32console.FreeConsole()


def console_ctrl_handler(msg):
    win32console.FreeConsole()
    return True
