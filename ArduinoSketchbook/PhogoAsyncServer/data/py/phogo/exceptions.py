#  -*- coding: utf-8 -*-


class PhogoException(BaseException):
    """ Base exception for Phogo """
    pass


class PhogoConnectionError(PhogoException):
    pass


class PhogoTimeoutError(PhogoConnectionError):
    pass


class PhogoProtocolError(PhogoConnectionError):
    pass
