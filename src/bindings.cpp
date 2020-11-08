#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>

#include "Member.hpp"

char const *greet() { return "hello World"; }

namespace python = boost::python;

// Boost.Python wrapper defining the module name, classes  and methods
BOOST_PYTHON_MODULE(bindings)
{
    python::class_<Member>("Member", python::no_init)
        .def(python::init<unsigned int, const unsigned int, const unsigned int>())
        .def("isModified", &Member::isModified);
    python::def("greet", greet);
}
