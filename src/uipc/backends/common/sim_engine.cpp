#include <uipc/backends/common/sim_engine.h>
#include <uipc/backends/common/sim_system_auto_register.h>
#include <uipc/backends/common/module.h>
#include <filesystem>
#include <fstream>

namespace uipc::backend
{
Json SimEngine::do_to_json() const
{
    Json j;
    j["sim_systems"] = m_system_collection.to_json();
    return j;
}

void SimEngine::build_systems()
{
    auto& funcs = SimSystemAutoRegister::creators().entries;
    for(auto& f : funcs)
    {
        auto uptr = f(*this);
        if(uptr)
            m_system_collection.create(std::move(uptr));
    }

    m_system_collection.build_systems();
}

void SimEngine::dump_system_info() const
{
    namespace fs = std::filesystem;

    spdlog::info("Built systems:\n{}", m_system_collection);

    auto     workspace = ModuleInfo::instance().workspace();
    fs::path p         = fs::absolute(fs::path{workspace} / "systems.json");
    {
        std::ofstream ofs(p);
        ofs << to_json().dump(4);
    }
    spdlog::info("System info dumped to {}", p.string());
}

ISimSystem* SimEngine::find_system(ISimSystem* ptr)
{
    if(ptr)
    {
        if(!ptr->is_valid())
        {
            ptr = nullptr;
        }
        else
        {
            ptr->set_engine_aware();
        }
    }
    return ptr;
}

ISimSystem* SimEngine::require_system(ISimSystem* ptr)
{
    if(ptr)
    {
        if(!ptr->is_valid())
        {
            throw SimEngineException(fmt::format("SimSystem [{}] is invalid", ptr->name()));
        }
        else
        {
            ptr->set_engine_aware();
        }
    }
    return ptr;
}
}  // namespace uipc::backend