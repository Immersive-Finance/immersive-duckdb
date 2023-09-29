from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension("immersive-test-duckdb",
                      sources=["./immersive.cpp"],
                      # define_macros=[...],
                      ),
]

setup(
    name="mmersive-test-duckdb",
    description="Immersive Risk Python Extension",
    ext_modules=ext_modules,
)
