from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "ptui",
        sources=["ptui/bindings.cpp"],
        include_dirs=[
            pybind11.get_include(),
            pybind11.get_include(user=True),
        ],
        language="c++",
        cxx_std=20,
        # Windows-specific
        extra_compile_args=["-O3"] if not __import__('sys').platform.startswith('win') else [],
        extra_link_args=["-O3"] if not __import__('sys').platform.startswith('win') else [],
    ),
]

setup(
    ext_modules=ext_modules,
    zip_safe=False,
)