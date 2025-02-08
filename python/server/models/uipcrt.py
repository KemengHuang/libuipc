# -*- coding: utf-8 -*-
# @file uipcrt.py
# @brief The UIPC Runtime Model
# @author sailing-innocent
# @date 2025-02-08
# @version 1.0
# ---------------------------------

from pydantic import BaseModel


class UIPCStatus(BaseModel):
    health: str
    status: str


class UIPCRunParam(BaseModel):
    scene_path: str
