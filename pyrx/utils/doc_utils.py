from __future__ import annotations

import collections.abc as c
import re

PARAM_PATT = re.compile(r"(?P<name>\w+)(?::(?P<type>[\w|.]+))?(?:=(?P<default>[^,]+))?,?")
BASE_SIGNATURE_PATT = re.compile(r"!\[\((.*?)\)\]!", re.IGNORECASE)
OVERLOADS_PATT = re.compile(r"<\[\(Overloads:\n(.*)\)\]\>", re.IGNORECASE | re.DOTALL)
DOCSTRING_ID_PATT = re.compile(r"<\[{(?P<id>-?\d+)}\]>", re.IGNORECASE)


class Signature:
    """Represents a function signature with a list of parameters."""

    def __init__(self, *params: Parameter) -> None:
        self.params = params

    def __eq__(self, other):  # for tests
        if not isinstance(other, Signature):
            return NotImplemented
        return self.params == other.params


class Parameter:
    """Represents a parameter in a function signature."""

    def __init__(
        self, name: str, type_hint: str | None = None, default: str | None = None
    ) -> None:
        self.name = name
        self.type_hint = type_hint
        self.default = default

    def __eq__(self, other):  # for
        if not isinstance(other, Parameter):
            return NotImplemented
        return (
            self.name,
            self.type_hint,
            self.default,
        ) == (
            other.name,
            other.type_hint,
            other.default,
        )


def parse_docstring(docstring: str) -> tuple[tuple[Signature, ...], str | None]:
    """Return signatures and docstring ID from raw docstring."""
    base_signature, overloads, docstring_id = get_docstring_sections(docstring)
    if base_signature is None:
        if overloads is not None:
            raise ValueError("Overloads provided without base signature.")
        signatures = ()
    else:
        signatures = tuple(_get_signatures(base_signature, overloads))
    return signatures, docstring_id


def _get_signatures(
    base_signature: str, overloads: str | None
) -> c.Generator[Signature, None, None]:
    base_params = tuple(parse_params(base_signature))
    if overloads is None:
        yield Signature(*base_params)
    else:
        for line in overloads.splitlines():
            line = line.strip().lstrip("-")
            overload_params = parse_params(line)
            yield Signature(*base_params, *overload_params)


def get_docstring_sections(docstring: str, /) -> c.Generator[str | None, None, None]:
    """
    Extract base signature, overloads, and docstring ID from a docstring.
    """
    docstring = docstring.replace(" ", "")
    for patt in (BASE_SIGNATURE_PATT, OVERLOADS_PATT, DOCSTRING_ID_PATT):
        m = patt.search(docstring)
        yield m.group(1).strip() if m else None


def parse_params(docstring_section: str) -> c.Generator[Parameter, None, None]:
    """
    Parse parameters from a docstring section (base signature or one
    overloads line)
    """
    for m in PARAM_PATT.finditer(docstring_section.replace(" ", "")):
        gd = m.groupdict()
        name = gd["name"]
        type_hint = gd["type"] or None
        default = gd["default"] or None
        yield Parameter(name, type_hint, default)
