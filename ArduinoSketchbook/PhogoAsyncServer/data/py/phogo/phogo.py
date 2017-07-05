#!/usr/bin/python3
#  -*- coding: utf-8 -*-
import sys
import json
from .exceptions import PhogoException

from urllib.parse import urlparse

BROWSER = False
try:
    from browser.ajax import ajax
    BROWSER = True

    from ._connections import AjaxConnection as Connection

except Exception as e:
    # not in browser
    from ._connections import RequestsConnection as Connection


def Phogo(*args, **kwargs):
    return _Phogo(*args, **kwargs)


class _Phogo(object):
    """docstring for _Phogo"""

    def __init__(self, host, port=80, url='/'):
        self._cmd_id = 0

        host = host if host.startswith(
            '//') else '//' + host  # needed by urlparse to conform to RFC
        endpoint = urlparse(host, scheme='http')

        self.com = Connection(endpoint.geturl())

        print("Phogo@{}".format(endpoint.netloc))

    def _send_command(self, data):
        data = json.dumps(data)
        self.com.send(data)
        # print(response.text)
        return json.loads(self.com.receive())

    def print_command(self, cmd):
        params = cmd["cmd"].get("params", "")
        if params:
            params = ", ".join("{}={}".format(k, v) for k, v in params.items())
        print(
            "{:.<4} {:.10}({:.15})".format(
                cmd["id"],
                cmd["cmd"]["action"],
                params).ljust(30),
            "-> ",
            end="", sile=sys.stderr)

    def print_response(self, resp):
        # sys.stdout.append("{}".format(resp["result"]))
        print("{}".format(resp["result"]))

    def _build_cmd(self, action, params=None):
        cmd = {
            'id': self._cmd_id,
            'cmd': {
                'action': action
            }
        }
        if params and isinstance(params, dict):
            cmd['cmd']['params'] = params

        self._cmd_id += 1
        return cmd

    # defeinition of the possible commands
    def distance(self):
        """ Does not take parameters. Returns the distance to the object in front """
        action = "distance"

        cmd = self._build_cmd(action)
        self.print_command(cmd)
        response = self._send_command(cmd)
        self.print_response(response)
        return response

    def forward(self, units=10):
        action = "forward"
        params = {"units": units}

        cmd = self._build_cmd(action, params)
        self.print_command(cmd)
        response = self._send_command(cmd)
        self.print_response(response)
        return response

    def backward(self, units=10):
        action = "backward"
        params = {"units": units}

        cmd = self._build_cmd(action, params)
        self.print_command(cmd)
        response = self._send_command(cmd)
        self.print_response(response)
        return response

    def right(self, degrees=90):
        action = "right"
        params = {"degrees": degrees}

        cmd = self._build_cmd(action, params)
        self.print_command(cmd)
        response = self._send_command(cmd)
        self.print_response(response)
        return response

    def left(self, degrees=90):
        action = "left"
        params = {"degrees": degrees}

        cmd = self._build_cmd(action, params)
        self.print_command(cmd)
        response = self._send_command(cmd)
        self.print_response(response)
        return response

    def pen_up(self, degrees=90):
        action = "pen_up"

        cmd = self._build_cmd(action)
        self.print_command(cmd)
        response = self._send_command(cmd)
        self.print_response(response)
        return response

    def pen_down(self, degrees=90):
        action = "pen_down"

        cmd = self._build_cmd(action)
        self.print_command(cmd)
        response = self._send_command(cmd)
        self.print_response(response)
        return response

# TODO control de errores
# FIXME añadir los comandos que faltan


def main():
    test()


def test():
    import sys

    from browser import window
    # header Access-Control-Allow-Origin: * must be set in the response
    host = '//{}:5000/json'.format(window.location.hostname)

    ph = Phogo(host)

    try:
        for i in range(10):
            resp = ph.distance()
            # if i % 2:
            #     print(resp, flush=True)
            # else:
            #     print(resp, file=sys.stderr, flush=True)
    except Exception as e:
        print("Exception:", str(e), file=sys.stderr)

if __name__ == '__main__':
    main()
