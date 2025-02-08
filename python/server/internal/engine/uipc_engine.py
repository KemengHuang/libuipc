# -*- coding: utf-8 -*-
# @file uipc_engine.py
# @brief The UIPC Engine Singleton
# @author sailing-innocent
# @date 2025-02-08
# @version 1.0
# ---------------------------------
import uipc
import time
import threading
import numpy as np
import logging
from models.uipcrt import UIPCRunParam
import json
import os
import numpy as np
import uipc

# from uipc.gui import SceneGUI
from uipc import view
from uipc import Logger, Timer
from uipc import Vector3, Vector2, Transform, Quaternion, AngleAxis
from uipc import builtin
from uipc.core import Engine, World, Scene
from uipc.geometry import trimesh, label_surface
from uipc.constitution import AffineBodyConstitution
from uipc.unit import MPa, GPa


def thread_function(name, scene_path):
    logging.info(f"Thread {name}: starting")
    start_time = time.time()
    # Both threads will try to perform CPU-intensive tasks
    workspace = scene_path
    meta_f = os.path.join(scene_path, "scene.json")
    data_f = os.path.join(scene_path, "scene_data.json")
    with open(meta_f, "r") as f:
        meta = json.load(f)
    with open(data_f, "r") as f:
        data = json.load(f)

    engine = Engine("cuda", workspace)
    world = World(engine)
    config = Scene.default_config()
    dt = 0.02
    config["dt"] = dt
    config["gravity"] = [[0.0], [-9.8], [0.0]]
    scene = Scene(config)
    abd = AffineBodyConstitution()
    scene.contact_tabular().default_model(0.5, 1.0 * GPa)
    default_element = scene.contact_tabular().default_element()
    body_elem = scene.contact_tabular().create("body")
    scene.contact_tabular().default_model(0.01, 1e12)
    scene.contact_tabular().insert(body_elem, body_elem, 0, 0, False)
    geo_slots = []
    N = len(data.keys())

    scene.contact_tabular().default_model(0.5, 1.0 * GPa)
    default_element = scene.contact_tabular().default_element()
    body_elem = scene.contact_tabular().create("body")
    scene.contact_tabular().default_model(0.01, 1e12)
    scene.contact_tabular().insert(body_elem, body_elem, 0, 0, False)

    for key in data.keys():
        print(data[key])
        Vs = data[key]["Vs"]
        Fs = data[key]["Fs"]
        mesh = trimesh(np.array(Vs), np.array(Fs))

        loc = data[key]["loc"]
        pos_view = view(mesh.positions())
        pos_view += Vector3.UnitY() * loc[2]
        pos_view += Vector3.UnitZ() * loc[0]
        pos_view += Vector3.UnitX() * loc[1]

    end_time = time.time()
    logging.info(
        f"Thread {name}: finishing. Time taken: {end_time - start_time:.2f} seconds"
    )


class UIPCEngine:
    _instance = None
    _status = None
    _threads = []

    @staticmethod
    def get_instance():
        if not UIPCEngine._instance:
            UIPCEngine._instance = UIPCEngine()
        return UIPCEngine._instance

    def __init__(self):
        if UIPCEngine._instance:
            raise Exception("This class is a singleton!")
        else:
            UIPCEngine._instance = self

        self._status = "idle"

    def run_scene(self, scene_path):
        # start a new thread to run the scene
        # TODO: check duplicated scene
        self._status = "running"
        tname = str(len(self._threads))
        x = threading.Thread(target=thread_function, args=(tname, scene_path))
        self._threads.append(x)
        x.start()

    def get_status(self):
        # check threads status
        for t in self._threads:
            if t.is_alive():
                return "running"
        self._status = "idle"
        return self._status

    def __str__(self):
        return "UIPCEngine Instance with status: " + str(self._status)


def engine_func():
    return UIPCEngine.get_instance()
