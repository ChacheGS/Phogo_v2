#  -*- coding: utf-8 -*-
import sys

from .exceptions import PhogoException

_HAVE_AJAX_ = False
_HAVE_REQUESTS_ = False
try:
    # try to import browser stuff first because Brython is
    # slower than Python, so we don't try to import requests
    # and have all the overhead everytime
    from browser.ajax import ajax
    _HAVE_AJAX_ = True
except ImportError as e:
    print("Not running in browser, or Brython is not accessible", file=sys.stderr)
    raise e
    # this is for the Python interpreter implementation. Will be made later
    # try:
    #     import requests
    #     _HAVE_REQUESTS_ = True
    # except ImportError as e:
    #     print(
    #         "Phogo needs the 'requests' module to be installed. You can install it by typing\n" +
    #         "\t'pip install requests' or 'pip3 install requests' in a terminal.",
    #         file=sys.stderr)
    #     raise e


class Connection(object):
    """docstring for Connection"""

    def __init__(self, host, *args, **kwargs):
        self._host = host
        self._timeout = kwargs.get('timeout', 5)

    def set_timeout(self, timeout=5):
        self._timeout = timeout

    def connect(self):
        raise NotImplementedError

    def disconnect(self):
        raise NotImplementedError

    def send(self, data):
        raise NotImplementedError

    def receive(self):
        raise NotImplementedError

    def __repr__(self):
        return "<{} '{}' [{}]>".format(
            self.__class__.__name__, self._host, self._timeout)


def AjaxConnection(*args, **kwargs):
    return _AjaxConnection(*args, **kwargs)


class _AjaxConnection(Connection):
    """docstring for _AjaxConnection"""

    def __init__(self, *args, **kwargs):
        super(_AjaxConnection, self).__init__(*args, **kwargs)
        self._conn = None

    def send(self, data, timeout=5):

        self._conn = ajax()
        self._conn.set_timeout(self._timeout)
        # False means sync (blocking)
        self._conn.open('POST', self._host, False)

        # server doesn't support the OPTIONS verb
        # if I set this to 'application/json' (as I should),
        # browser will first send an OPTIONS request and server
        # won't answer, so no 'application/json'
        self._conn.set_header('content-type', 'text/plain')

        self._conn.send(data)

    def receive(self):
        response = self._conn.text
        self._conn = None
        return response

    def __repr__(self):
        return super(_AjaxConnection, self).__repr__()


# def RequestsConnection(*args, **kwargs):
#     return _RequestsConnection(*args, **kwargs)


# class _RequestsConnection(Connection):
#     """docstring for _RequestsConnection"""

#     def __init__(self, *args, **kwargs):
#         super(_AjaxConnection, self).__init__(*args, **kwargs)

#     def send(self, data):
#         headers = {'Content-type': 'application/json'}
#         json_foo = json.dumps(data)
#         self._resp = requests.post(self._host, data=json_foo, headers=headers)

#     def receive(self):
#         # print(response.text)
#         return json.loads(self._resp.text)
