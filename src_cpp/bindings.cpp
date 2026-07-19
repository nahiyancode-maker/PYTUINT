#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "ptui/ptui.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
    m.doc() = "PTUI Core C++ Engine";
    
    // Hooks
    m.def("useState", &ptui::useState, py::arg("initial"));
    m.def("useEffect", &ptui::useEffect, py::arg("callback"), py::arg("deps") = py::list());
    m.def("useMemo", &ptui::useMemo, py::arg("factory"), py::arg("deps") = py::list());
    m.def("useRef", &ptui::useRef, py::arg("initial") = py::none());
    m.def("useReducer", &ptui::useReducer, py::arg("reducer"), py::arg("initial"));
    
    // Components
    py::class_<ptui::View>(m, "View")
        .def(py::init<std::map<std::string, py::object>&, py::list&>())
        .def("render", &ptui::View::render);
    
    py::class_<ptui::Text>(m, "Text")
        .def(py::init<const std::string&, std::map<std::string, py::object>&>());
    
    py::class_<ptui::Button>(m, "Button")
        .def(py::init<const std::string&, py::function&, std::map<std::string, py::object>&>());
    
    // Add other components...
    
    // App runner
    m.def("run", &ptui::run, py::arg("app"));
}