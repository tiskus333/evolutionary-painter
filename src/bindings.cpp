#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>

#include "Member.hpp"
#include <EvolAlg.hpp>

char const *greet() { return "hello World"; }

namespace python = boost::python;

// Boost.Python wrapper defining the module name, classes  and methods
BOOST_PYTHON_MODULE(bindings)
{
    python::class_<Member>("Member", python::no_init)
        .def(python::init<unsigned int, const unsigned int, const unsigned int>())
        .def("isModified", &Member::isModified);
    python::class_<EvolAlg, boost::noncopyable>("EvolAlg", python::no_init)
        .def(python::init<const std::string, const unsigned int, const unsigned int, const unsigned int, bool>())
        //.def("getInputImage", &EvolAlg::getInputImage);
        .def("isRunning", &EvolAlg::isRunning)
        .def("stop", &EvolAlg::stop)
        .def("run", &EvolAlg::run);
    python::def("greet", greet);
}
