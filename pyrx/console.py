import sys
import typing as t
from contextlib import contextmanager

from pyrx.utils.sentinels import Sentinel

_UNSET = Sentinel("Unset")


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
