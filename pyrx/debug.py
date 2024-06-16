import os
import sys
import traceback

try:
    import debugpy
except ImportError as e:
    raise ImportError(
        "Optional dependency not installed. "
        "Install debugpy (python -m pip install debugpy)"
    ) from e
import wx

DEBUG_HOST = "127.0.0.1"
DEBUG_PORT = 5678
PYTHON_PATH = sys.base_prefix + "\\python.exe"


def startListener(
    host: str = DEBUG_HOST,
    port: int = DEBUG_PORT,
    python_path: str = PYTHON_PATH,
    show_msgbox: bool = True,
):
    """
    Start the debug server.
    Allows you to debug a module loaded with the `PYLOAD` command.

    Attributes:
        host: debugger host
        port: debugger port
        python_path: path to the python executable
        show_msgbox: if True, displays a dialog box before starting the server

    Visual Studio Code configuration:
    ```
    {
        "name": "Remote Attach",
        "type": "debugpy",
        "request": "attach",
        "connect": {
            "host": <host>,  # default: "127.0.0.1"
            "port": <port>,  # default: 5678
        },
        "justMyCode": false
    }
    ```

    Examples:

        ```
        # add this in the PYLOADed module
        def PyRxCmd_debug():
            from pyrx.debug import startListener

            startListener(show_msgbox=False)

        # load the python module using the `PYLOAD` command, then invoke
        # the `DEBUG` command in the CAD application and run the debugger in vscode.
        ```
    """

    try:
        if show_msgbox:
            result = wx.MessageDialog(
                None,
                "This will start the debug Listener for the session.\n"
                "Now is a good time to run your debugger from vs code:",
                "Confirm",
                wx.YES_NO | wx.NO_DEFAULT | wx.ICON_QUESTION,
            ).ShowModal()

            if result != wx.ID_YES:
                return

        os.environ["PYDEVD_DISABLE_FILE_VALIDATION"] = "1"
        debugpy.configure(python=python_path)
        debugpy.listen((host, port))
        print("dubugger running...")

    except Exception:
        traceback.print_exc()
