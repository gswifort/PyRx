from pyrx.console import std_redirect


class Test_std_redirect:
    def test_valid(self):
        import sys

        old_stdin = sys.stdin
        old_stdout = sys.stdout
        old_stderr = sys.stderr
        new_stdin = object()
        new_stdout = object()
        new_stderr = object()
        try:
            with std_redirect(new_stdin, new_stdout, new_stderr) as (stdin, stdout, stderr):
                assert sys.stdin == stdin == new_stdin
                assert sys.stdout == stdout == new_stdout
                assert sys.stderr == stderr == new_stderr
                raise RuntimeError
        except RuntimeError:
            assert sys.stdin == old_stdin
            assert sys.stdout == old_stdout
            assert sys.stderr == old_stderr

    def test_valid_with_one_arg(self):
        import sys

        old_stdin = sys.stdin
        old_stdout = sys.stdout
        old_stderr = sys.stderr
        new_stdin = object()
        try:
            with std_redirect(new_stdin) as (stdin, stdout, stderr):
                assert sys.stdin == stdin == new_stdin
                assert sys.stdout == stdout == old_stdout
                assert sys.stderr == stderr == old_stderr
                raise RuntimeError
        except RuntimeError:
            assert sys.stdin == old_stdin
            assert sys.stdout == old_stdout
            assert sys.stderr == old_stderr
