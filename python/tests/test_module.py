import uipc
import logging

logging.basicConfig(level=logging.DEBUG)


def test_moidule():
    print(uipc.__doc__)
    print(uipc.__version__)
    assert True
