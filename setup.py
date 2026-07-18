from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "ptui.core",
        sources=["ptui/bindings.cpp"],
        include_dirs=[
            pybind11.get_include(),
            pybind11.get_include(user=True),
        ],
        language="c++",
        extra_compile_args=["-std=c++20", "-O3"] if not __import__('sys').platform.startswith('win') else ["/std:c++20", "/O2"],
    ),
]

setup(
    ext_modules=ext_modules,
    zip_safe=False,
)