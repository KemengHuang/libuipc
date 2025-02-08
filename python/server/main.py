# -*- coding: utf-8 -*-
# @file main.py
# @brief The main entry of libuipc server
# @author sailing-innocent
# @date 2025-02-08
# @version 1.0
# ---------------------------------

from fastapi import FastAPI
from routers import uipcrt
from internal.util.middleware import add_middleware
from internal.util.lifespan import lifespan

port = 7803
app = FastAPI(lifespan=lifespan)
add_middleware(app)
app.include_router(uipcrt.router)

if __name__ == "__main__":
    import uvicorn

    uvicorn.run(app, host="0.0.0.0", port=port)
