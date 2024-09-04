class DefaultMessageMixin:
    default_message: str

    def __init__(self, *args, **kwargs) -> None:
        if args:
            super().__init__(*args, **kwargs)
        else:
            super().__init__(self.default_message, **kwargs)
