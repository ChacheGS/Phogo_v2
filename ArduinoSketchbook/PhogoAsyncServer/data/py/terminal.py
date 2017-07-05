# insert this path into sys.path
import sys
from os.path import dirname
sys.path.append(dirname(__file__))

from object_proxy import ObjectProxy
from browser import window, document

from console import Console

console = Console()


class Terminal(ObjectProxy):
    """docstring for Terminal"""

    def __init__(self, term_id):
        target = document[term_id]
        super(Terminal, self).__init__(target)


class FixOutput(object):

    def __init__(self):
        self.append_next = False

    def fix_line_ending(self, term, value):
        append_this = self.append_next
        if term.get_line(-1).endswith('\n'):
            term.update(-1, term.get_line(-1)[:-1])

        if value.endswith('\n'):
            self.append_next = False
        else:
            self.append_next = True
        # console.log(append_this, "_{}_".format(value))
        return append_this


fix = FixOutput()


class StdOut(object):
    """docstring for Output"""

    def __init__(self, terminal):
        self.terminal = terminal

    def write(self, value):
        console.log(value)
        self.terminal.enable()
        self.terminal.pause()
        append = fix.fix_line_ending(self.terminal, value)
        if append:
            self.append(value)
        else:
            self.terminal.echo(value, {"keepWords": True})
        self.terminal.resume()
        self.terminal.disable()

    def append(self, value):
        self.terminal.update(-1, self.terminal.get_line(-1) + value)


class StdErr(StdOut):
    """docstring for Output"""

    def write(self, value):
        console.error(value)
        self.terminal.enable()
        self.terminal.pause()
        self.terminal.error("\n{}\n".format(value), {"keepWords": True})
        self.terminal.resume()
        self.terminal.disable()

sys.stdout = StdOut(document.terminal)
sys.stderr = StdErr(document.terminal)
