from fastapi import APIRouter

api_base = "/api/v1"
router = APIRouter(prefix=api_base)

__all__ = ["api_base", "router"]
