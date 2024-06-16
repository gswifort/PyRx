import re
from pathlib import Path

from setuptools import find_packages, setup

BASE_DIR = Path(__file__).parent
PYTHON_PACKAGE_DIR = BASE_DIR / "pyrx"
BIN_DIR = BASE_DIR / "PyRxBin"

with open(PYTHON_PACKAGE_DIR / "__init__.py", "r", encoding="utf-8") as f:
    version = (
        re.compile(r'^__version__\s*=\s*["\'](.*?)["\']$', re.MULTILINE)
        .search(f.read())
        .group(1)
    )


install_requires = ("wxpython",)

extras_require = {"activex": ("pywin32",)}

setup(
    name="pyrx",
    version=version,
    url="https://pyarx.blogspot.com/",
    project_urls={
        "Source": "https://github.com/CEXT-Dan/PyRx",
    },
    license="",  # !!!!!!!!!!!!!!!
    author="Daniel",
    author_email="dwmcabinets@yahoo.com",
    description="Python wrappers for Autocad ObjectARX® and clones.",
    packages=find_packages(include=("pyrx*",)),
    include_package_data=True,
    keywords=["dwg", "dxf", "autocad", "bricscad", "gstarcad", "zwcad"],
    install_requires=install_requires,
    extras_require=extras_require,
    python_requires=">=3.12",
)
