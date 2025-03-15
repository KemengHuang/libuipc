#include <pyuipc/core/feature.h>
#include <uipc/core/feature.h>
#include <uipc/common/smart_pointer.h>

namespace pyuipc::core
{
using namespace uipc::core;
PyFeature::PyFeature(py::module& m)
{
    auto class_IFeature = py::class_<IFeature, S<IFeature>>(m, "Feature");
    class_IFeature.def("name", &IFeature::name);
    class_IFeature.def("type_name", &IFeature::type_name);
}
}  // namespace pyuipc::core
