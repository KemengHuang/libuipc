# -*- coding: utf-8 -*-
# @file lifespan.py
# @brief The Server Lifespan
# @author sailing-innocent
# @date 2025-02-08
# @version 1.0
# ---------------------------------

from fastapi import FastAPI
from contextlib import asynccontextmanager
import logging


@asynccontextmanager
async def lifespan(app: FastAPI):
    # init
    ## format uvicorn logger
    logger = logging.getLogger("uvicorn.access")
    handler = logging.handlers.RotatingFileHandler(
        "output/server.log", mode="a", maxBytes=100 * 1024, backupCount=3
    )
    handler.setFormatter(logging.Formatter("%(asctime)s - %(levelname)s - %(message)s"))
    logger.addHandler(handler)

    yield
    ## remove uvicorn logger
    logger.removeHandler(handler)
