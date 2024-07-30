#include <pybind11/pybind11.h>
#include <functional>
#include <uipc/common/type_define.h>

namespace pyuipc
{
using namespace uipc;
namespace py = pybind11;
class Module
{
  public:
    Module(std::function<void(py::module&)>&& reg)
    {
        creators().push_back(reg);
    }
    friend class ModuleLoader;

  private:
    using Creator = std::function<void(py::module&)>;
    static std::list<Creator>& creators()
    {
        static std::list<Creator> creators;
        return creators;
    }
};
}  // namespace pyuipc