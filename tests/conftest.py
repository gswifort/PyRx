import pytest

from pyrx import Db


@pytest.fixture
def db():
    db = Db.Database(True, True)
    yield db
