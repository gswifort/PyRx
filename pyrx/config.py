import logging
import os
from pathlib import Path
from tomllib import TOMLDecodeError

from pydantic import ValidationError
from pydantic_settings import BaseSettings, SettingsConfigDict, TomlConfigSettingsSource

logger = logging.getLogger(__name__)


def _get_settings_file_dirs():
    yield Path.cwd()
    if (_appdata := os.getenv("APPDATA", None)) is not None:
        yield Path(_appdata) / "pyrx"


class PyRxSettings(BaseSettings):
    disable_onload: bool = False
    load_repl: bool = False

    model_config = SettingsConfigDict(env_prefix="PYRX_")

    @classmethod
    def settings_customise_sources(
        cls, settings_cls, init_settings, env_settings, dotenv_settings, file_secret_settings
    ):
        toml_sources = []
        for dir_ in _get_settings_file_dirs():
            toml_file = dir_ / "pyrx.toml"
            if not toml_file.exists():
                continue
            try:
                toml_sources.append(TomlConfigSettingsSource(settings_cls, dir_ / "pyrx.toml"))
            except (TOMLDecodeError, ValidationError):
                logger.exception(f"Failed to load pyrx settings from {toml_file}")

        return (
            init_settings,
            env_settings,
            dotenv_settings,
            *toml_sources,
            file_secret_settings,
        )


_pyrx_settings: PyRxSettings | None = None


def get_pyrx_settings() -> PyRxSettings:
    global _pyrx_settings
    if _pyrx_settings is None:
        _pyrx_settings = PyRxSettings()
    return _pyrx_settings.model_copy()
