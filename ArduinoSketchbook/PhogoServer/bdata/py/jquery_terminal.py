# insert this path into sys.path
import sys
from os.path import dirname
sys.path.append(dirname(__file__))

from object_proxy import ObjectProxy
from browser import window, document


class Terminal(ObjectProxy):
    """docstring for Terminal"""

    def __init__(self, div_id, handlers, options={}):
        target = document[div_id]
        _term = window.jQuery('#' + div_id).terminal(handlers, options)
        super(Terminal, self).__init__(_term)


class PythonREPL(object):

    def __init__(self, terminal, ps1='>>> ', ps2='... '):
        self.term = terminal
        self.execution_ns = {
            'credits': lambda: 'credits',
            'copyright': lambda: 'copyright',
            'license': lambda: 'license',
            '__name__': '__main__'
        }
        self.ps1 = ps1
        self.ps2 = ps2

    def run(self, line):
        try:
            _ = self.execution_ns['_'] = eval(line, self.execution_ns)
            if _ is not None:
                log(self.term, repr(_))
            self.term.set_prompt(self.ps1)
            _status = "main"
        except IndentationError:
            self.term.set_prompt(self.ps2)
            _status = "block"
        except SyntaxError as msg:
            if str(msg) == 'invalid syntax : triple string end not found' or \
                    str(msg).startswith('Unbalanced bracket'):
                self.term.set_prompt(self.ps2)
                _status = "3string"
            elif str(msg) == 'eval() argument must be an expression':
                try:
                    exec(line, self.execution_ns)
                except:
                    traceback.print_exc()
                self.term.set_prompt(self.ps1)
                _status = "main"
            elif str(msg) == 'decorator expects function':
                self.term.set_prompt(self.ps2)
                _status = "block"
            else:
                traceback.print_exc()
                self.term.set_prompt(self.ps1)
                _status = "main"
        except:
            traceback.print_exc()
            self.term.set_prompt(self.ps1)
            _status = "main"


class StdOut(object):
    """docstring for Output"""

    def __init__(self, terminal):
        self.terminal = terminal

    def write(self, value):
        self.terminal.pause()
        self.terminal.echo(value)
        self.terminal.resume()


class StdErr(StdOut):
    """docstring for Output"""

    def write(self, value):
        self.terminal.pause()
        self.terminal.error(value)
        self.terminal.resume()


browser = lambda: window.navigator.appName + window.navigator.appVersion
python_interpreter_options = {
    'greetings': "Brython {}.{}.{} on {}".format(*sys.implementation.version, browser()),
    'prompt': '>>> '
}



def python_interpreter(command, term):
    # log(term, 'stdout:', command)
    # error(term, 'stderr:', command)
    repl.run(command)


def main_interpreter(command, term):
    # normalize input
    command = list(map(lambda x: x.strip(), command.split()))
    cmd, args = command[0], command[1:]
    command = ' '.join(command)
    if cmd == 'python':
        term.push(python_interpreter, python_interpreter_options)
    else:
        log(term, command)


main_interpreter_options = {
    'greetings': 'Terminal\n',
    'prompt': '>> '
}

window.terminal = t = Terminal(
    'terminal',
    python_interpreter,
    options=python_interpreter_options)

sys.stdout = StdOut(t)
sys.stderr = StdErr(t)

repl = PythonREPL(t)

def log(term, *args, end='', sep=' ', flush=False):
    print(*args, end=end, sep=sep, file=sys.stdout, flush=flush)


def error(term, *args, end='', sep=' ', flush=False):
    print(*args, end=end, sep=sep, file=sys.stderr, flush=flush)

log(t, window.navigator.appName)
error(t, window.navigator.appVersion)

# if __name__ == '__main__':
# main()
