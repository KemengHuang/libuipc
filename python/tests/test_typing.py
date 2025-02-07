import pytest
import numpy as np
import uipc


@pytest.mark.typing
def test_typings():
    assert type(uipc.Float.One()) is float
    assert uipc.Float.One() == 1.0
    assert uipc.Float.Zero() == 0.0
    assert uipc.Float.Value(3.14) == 3.14
    assert type(uipc.Vector3.Zero()) is np.ndarray
    assert np.all(uipc.Vector3.Zero() == np.zeros(3))
    assert np.all(uipc.Vector3.Ones() == np.ones(3))
    assert np.all(uipc.Vector3.Values([1, 2, 3]) == np.array([1, 2, 3]).reshape(3, 1))
    assert np.all(uipc.Vector3.Identity() == np.array([1, 0, 0]).reshape(3, 1))
    assert np.all(uipc.Vector3.UnitY() == np.array([0, 1, 0]).reshape(3, 1))
