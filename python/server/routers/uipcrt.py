# -*- coding: utf-8 -*-
# @file uipcrt.py
# @brief The UIPC application
# @author sailing-innocent
# @date 2025-02-08
# @version 1.0
# ---------------------------------


from fastapi import Depends, HTTPException
from .base import router
from models.uipcrt import UIPCStatus, UIPCRunParam
from internal.engine.uipc_engine import engine_func


@router.get("/status", response_model=UIPCStatus)
def get_status(engine=Depends(engine_func)):
    status = engine.get_status()
    return {"health": "ok", "status": status}


@router.post("/run", response_model=UIPCStatus)
def run(param: UIPCRunParam, engine=Depends(engine_func)):
    engine.run_scene(param.scene_path)
    status = engine.get_status()
    return {"health": "ok", "status": status}
