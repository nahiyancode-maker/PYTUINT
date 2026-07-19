from setuptools import setup, Extension, find_packages
import pybind11
import sys

extra = ["/std:c++20"] if sys.platform == "win32" else ["-std=c++20"]

setup(
    name="PYTUINT",
    version="0.1.0",
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    python_requires=">=3.8",
    install_requires=["pybind11>=2.10"],
    ext_modules=[
        Extension(
            "PYTUINT",
            ["include/PYTUINT/PYTUINT.hpp"],
            include_dirs=[pybind11.get_include(), "include"],
            define_macros=[("PYTUINT_PYTHON_BINDINGS", "1")],
            language="c++",
            extra_compile_args=extra,
        )
    ],
)