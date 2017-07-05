import sys
import time
import traceback
import javascript

from browser import document as doc, window, alert
from browser.ajax import ajax

editor = doc.editor

def default_contents():
    try:
        return open('empty_default.py').read()
    except Exception as e:
        print(str(e))

    return '# Here goes your Python code'

if hasattr(window, 'localStorage'):
    from browser.local_storage import storage
else:
    storage = None


def reset_src():
    if storage is not None and "py_src" in storage:
        editor.setValue(storage["py_src"])
    else:
        editor.setValue(default_contents())
    # editor.resize()
    if doc['loading']:
        doc['loading'].style.display = 'none'
    editor.navigateFileEnd()
    editor.focus()


class cOutput:

    def write(self, data):
        doc["console"].value += str(data)

    def flush(self):
        pass

if "console" in doc:
    sys.stdout = cOutput()
    sys.stderr = cOutput()


def to_str(xx):
    return str(xx)

doc['version'].html = "{}.{}.{}".format(*sys.implementation.version[:3])

output = ''


def show_console(ev):
    doc["console"].value = output
    doc["console"].cols = 60

# load a Python script


def load_script(evt):
    _name = evt.target.value + '?foo=%s' % time.time()
    editor.setValue(open(_name).read())

# run a script, in global namespace if in_globals is True


def run(*args):
    global output
    doc["console"].value = ''
    src = editor.getValue()
    if storage is not None:
        storage["py_src"] = src

    t0 = time.perf_counter()
    try:
        ns = {'__name__': '__main__'}
        exec(src, ns)
        state = 1
    except Exception as exc:
        traceback.print_exc(file=sys.stderr)
        state = 0
    output = doc["console"].value

    print('<completed in %6.2f ms>' % ((time.perf_counter() - t0) * 1000.0))
    return state


def show_js(ev):
    src = editor.getValue()
    doc["console"].value = javascript.py2js(src, '__main__')

reset_src()

