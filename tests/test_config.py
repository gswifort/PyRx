import logging
import sys
import typing as t
from pathlib import Path

import pytest

from pyrx.config import PyRxSettings, get_pyrx_settings

if t.TYPE_CHECKING:
    from pyrx.config import PyRxSettings


@pytest.fixture
def appdata(tmp_path: Path, monkeypatch):
    """Fixture to create a temporary appdata directory."""
    appdata_dir = tmp_path / "appdata"
    appdata_dir.mkdir(parents=True, exist_ok=True)
    monkeypatch.setenv("APPDATA", str(appdata_dir))
    return appdata_dir


@pytest.fixture
def cwd(tmp_path: Path, monkeypatch: pytest.MonkeyPatch):
    """Fixture to create a temporary current working directory."""
    cwd_dir = tmp_path / "cwd"
    cwd_dir.mkdir(parents=True, exist_ok=True)
    monkeypatch.chdir(cwd_dir)
    return cwd_dir


class TestPyRxSettings:
    @pytest.fixture
    def setup_env(self, appdata: Path, cwd: Path, monkeypatch: pytest.MonkeyPatch):
        """Setup method to ensure PYRX_DISABLE_ONLOAD and PYRX_LOAD_REPL are unset."""
        pyrx_config_module_name = "pyrx.config"
        if pyrx_config_module_name in sys.modules:
            sys.modules.pop(pyrx_config_module_name)
        from pyrx.config import PyRxSettings

        monkeypatch.delenv("PYRX_DISABLE_ONLOAD", False)
        monkeypatch.delenv("PYRX_LOAD_REPL", False)

        return PyRxSettings, appdata, cwd

    def test_settings_order(
        self, setup_env: tuple[type[PyRxSettings], Path, Path], monkeypatch: pytest.MonkeyPatch
    ):
        PyRxSettings, appdata, cwd = setup_env

        default_settings = PyRxSettings()
        assert default_settings.disable_onload is False
        assert default_settings.load_repl is False

        appdata_file = appdata / "pyrx/pyrx.toml"
        appdata_file.parent.mkdir(parents=True, exist_ok=True)
        with open(appdata_file, "w") as f:
            f.write("disable_onload = true\nload_repl = true\n")

        settings = PyRxSettings()
        assert settings.disable_onload is True
        assert settings.load_repl is True

        cwd_file = cwd / "pyrx.toml"
        with open(cwd_file, "w") as f:
            f.write("load_repl = false\n")

        settings = PyRxSettings()
        assert settings.disable_onload is True  # from appdata
        assert settings.load_repl is False

        monkeypatch.setenv("PYRX_DISABLE_ONLOAD", "ala")

        settings = PyRxSettings.model_construct()
        assert settings.disable_onload is False

        settings = PyRxSettings(disable_onload=False, load_repl=True)
        assert settings.disable_onload is False
        assert settings.load_repl is True

    def test_skipping_invalid_toml(
        self, setup_env: tuple[type[PyRxSettings], Path, Path], caplog: pytest.LogCaptureFixture
    ):
        PyRxSettings, appdata, cwd = setup_env

        default_settings = PyRxSettings()

        appdata_file = appdata / "pyrx/pyrx.toml"
        appdata_file.parent.mkdir(parents=True, exist_ok=True)
        with open(appdata_file, "w") as f:
            f.write("disable_onload = yes\nload_repl = true\n")  # ValidationError

        cwd_file = cwd / "pyrx.toml"
        with open(cwd_file, "w") as f:
            f.write("disable_onload=false\nload_repl = false\n")

        caplog.set_level(logging.WARNING)
        caplog.clear()
        settings = PyRxSettings()
        assert caplog.records[0].message == f"Failed to load pyrx settings from {appdata_file}"
        assert settings.disable_onload is False
        assert settings.load_repl is False

        with open(cwd_file, "w") as f:
            f.write("disable_onload==false\nload_repl = false\n")  # TOMLDecodeError

        caplog.clear()
        settings = PyRxSettings()
        assert caplog.records[0].message == f"Failed to load pyrx settings from {cwd_file}"
        assert caplog.records[1].message == f"Failed to load pyrx settings from {appdata_file}"

        settings == default_settings

    def test_get_pyrx_settings(self):
        settings_1 = get_pyrx_settings()
        settings_2 = get_pyrx_settings()

        assert isinstance(settings_1, PyRxSettings)
        assert isinstance(settings_2, PyRxSettings)

        assert settings_1 == settings_2
        assert settings_1 is not settings_2
