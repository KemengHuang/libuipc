#include <pyuipc/world/object.h>
#include <pyuipc/as_numpy.h>
#include <uipc/geometry/simplicial_complex_slot.h>
#include <uipc/geometry/implicit_geometry_slot.h>
#include <uipc/world/object.h>

namespace pyuipc::world
{
using namespace uipc::world;
using namespace uipc::geometry;
PyObject::PyObject(py::module& m)
{
    auto class_Object = py::class_<Object, S<Object>>(m, "Object");

    auto class_Geometries = py::class_<Object::Geometries>(class_Object, "Geometries");

    class_Object
        .def("name", &Object::name)  //
        .def("id", &Object::id);     //

    class_Object.def(
        "geometries", [](Object& self) { return self.geometries(); }, py::return_value_policy::move);

    // For Simplicial Complex

    class_Geometries.def("create",
                         [](Object::Geometries& self, SimplicialComplex& sc)
                         {
                             auto [geo, rest_geo] = std::move(self).create(sc);
                             return std::make_pair(std::move(geo), std::move(rest_geo));
                         });

    class_Geometries.def(
        "create",
        [](Object::Geometries& self, SimplicialComplex& sc, SimplicialComplex& rest_sc)
        {
            auto [geo, rest_geo] = std::move(self).create(sc, rest_sc);
            return std::make_pair(std::move(geo), std::move(rest_geo));
        });

    // For Implicit Geometry

    class_Geometries.def("create",
                         [](Object::Geometries& self, ImplicitGeometry& ig)
                         {
                             auto [geo, rest_geo] = std::move(self).create(ig);
                             return std::make_pair(std::move(geo), std::move(rest_geo));
                         });

    class_Geometries.def(
        "create",
        [](Object::Geometries& self, ImplicitGeometry& ig, ImplicitGeometry& rest_ig)
        {
            auto [geo, rest_geo] = std::move(self).create(ig, rest_ig);
            return std::make_pair(std::move(geo), std::move(rest_geo));
        });

    class_Geometries.def("ids",
                         [](Object::Geometries& self) {
                             return as_numpy(std::move(self).ids(), py::cast(self));
                         });
}
}  // namespace pyuipc::world
