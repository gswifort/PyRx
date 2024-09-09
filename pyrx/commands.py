import inspect
import traceback
from functools import wraps


def command(func=None, *, name=None, cmdflags=None):
    def decorator(func):
        sig = inspect.signature(func)
        sig_cmd_flags = None
        for key in sig.parameters:
            if key.lower() == "cmdflags":
                sig_cmd_flags = [key, sig.parameters[key].default]
                break
        if sig_cmd_flags:
            if cmdflags is not None:
                sig_cmd_flags[1] = cmdflags

            @wraps(func)
            def wrapper(cmdflags=sig_cmd_flags[1]):
                try:
                    return func(**dict((sig_cmd_flags,)))
                except Exception:
                    traceback.print_exc()

        else:

            @wraps(func)
            def wrapper():
                try:
                    return func()
                except Exception:
                    traceback.print_exc()

        command_name = f"PyRxCmd_{name or func.__qualname__.lstrip("_")}"
        func.__globals__[command_name] = wrapper
        return wrapper

    if func is None:  # @command(...)
        return decorator
    else:  # @command
        return decorator(func)
