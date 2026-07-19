#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "PYTUINT/PYTUINT.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
    m.doc() = "PYTUINT Core C++ Engine";
    
    // Hooks
    m.def("useState", &PYTUINT::useState, py::arg("initial"));
    m.def("useEffect", &PYTUINT::useEffect, py::arg("callback"), py::arg("deps") = py::list());
    m.def("useMemo", &PYTUINT::useMemo, py::arg("factory"), py::arg("deps") = py::list());
    m.def("useRef", &PYTUINT::useRef, py::arg("initial") = py::none());
    m.def("useReducer", &PYTUINT::useReducer, py::arg("reducer"), py::arg("initial"));
    
    // Components
    py::class_<PYTUINT::View>(m, "View")
        .def(py::init<std::map<std::string, py::object>&, py::list&>())
        .def("render", &PYTUINT::View::render);
    
    py::class_<PYTUINT::Text>(m, "Text")
        .def(py::init<const std::string&, std::map<std::string, py::object>&>());
    
    py::class_<PYTUINT::Button>(m, "Button")
        .def(py::init<const std::string&, py::function&, std::map<std::string, py::object>&>());
    
    // Add other components...
    
    // App runner
    m.def("run", &PYTUINT::run, py::arg("app"));
}