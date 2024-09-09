import pytest

from pyrx import Ap
from pyrx.commands import command


@command
def command_1():
    print(1)
    return 1


@command(name="cmd2")
def _command_2():
    print(2)
    return 2


@command(cmdflags=Ap.CmdFlags.SESSION)
def _command_3():
    return 3


@command(cmdflags=Ap.CmdFlags.SESSION)
def _command_4(cmdflags):
    return cmdflags


@command(cmdflags=Ap.CmdFlags.SESSION)
def _command_5(cmdflags=Ap.CmdFlags.NOBEDIT):
    return cmdflags


@command
def _command_6(cmdflags=Ap.CmdFlags.NOBEDIT):
    return cmdflags


@command(name="cmd7", cmdflags=Ap.CmdFlags.SESSION)
def _command_7(cmdflags=Ap.CmdFlags.NOBEDIT):
    return cmdflags


@command
def _command_8(cmdflags=Ap.CmdFlags.NOBEDIT):
    return cmdflags


@command()
def _command_9(cmdflags=Ap.CmdFlags.NOBEDIT):
    return cmdflags


class Test_command_decorator:

    @pytest.mark.parametrize(
        "func, orig_name, pyrx_name, return_val",
        (
            pytest.param(command_1, "command_1", "PyRxCmd_command_1", 1, id="001"),
            pytest.param(_command_2, "_command_2", "PyRxCmd_cmd2", 2, id="002"),
            pytest.param(_command_3, "_command_3", "PyRxCmd_command_3", 3, id="003"),
            pytest.param(_command_4, "_command_4", "PyRxCmd_command_4", Ap.CmdFlags.SESSION, id="004"),
            pytest.param(_command_5, "_command_5", "PyRxCmd_command_5", Ap.CmdFlags.SESSION, id="005"),
            pytest.param(_command_6, "_command_6", "PyRxCmd_command_6", Ap.CmdFlags.NOBEDIT, id="006"),
            pytest.param(_command_7, "_command_7", "PyRxCmd_cmd7", Ap.CmdFlags.SESSION, id="007"),
            pytest.param(_command_8, "_command_8", "PyRxCmd_command_8", Ap.CmdFlags.NOBEDIT, id="008"),
            pytest.param(_command_9, "_command_9", "PyRxCmd_command_9", Ap.CmdFlags.NOBEDIT, id="009"),
        ),
    )
    def test_valid(self, func, orig_name, pyrx_name, return_val):
        assert func.__qualname__ == orig_name
        assert pyrx_name in globals()
        assert globals()[pyrx_name] is func
        res = func()
        assert res == return_val
        # TODO: load into new CAD instance and execute commands
