#include <contact_system/contact_system_exporter.h>
#include <contact_system/contact_system_feature.h>

namespace uipc::backend::cuda
{
REGISTER_SIM_SYSTEM(ContactSystemExporter);

void ContactSystemExporter::do_build()
{
    m_global_trajectory_filter   = require<GlobalTrajectoryFilter>();
    m_global_contact_manager     = require<GlobalContactManager>();
    m_half_plane_vertex_reporter = find<HalfPlaneVertexReporter>();

    m_simplex_normal_contact           = find<SimplexNormalContact>();
    m_simplex_frictional_contact       = find<SimplexFrictionalContact>();
    m_vertex_half_plane_normal_contact = find<VertexHalfPlaneNormalContact>();
    m_vertex_half_plane_frictional_contact = find<VertexHalfPlaneFrictionalContact>();

    auto overrider = std::make_shared<ContactSystemFeatureOverrider>(this);
    auto feature   = std::make_shared<core::ContactSystemFeature>(overrider);
    features().insert(feature);

    on_init_scene(
        [&]
        {
            m_simplex_trajectory_filter =
                m_global_trajectory_filter->find<SimplexTrajectoryFilter>();

            m_vertex_half_plane_trajectory_filter =
                m_global_trajectory_filter->find<VertexHalfPlaneTrajectoryFilter>();
        });
}

void ContactSystemExporter::get_contact_gradient(geometry::Geometry& vert_grad)
{
    auto& g = m_global_contact_manager->m_impl.sorted_contact_gradient;

    vert_grad.instances().resize(g.doublet_count());
    auto i = vert_grad.instances().find<IndexT>("i");
    if(!i)
    {
        i = vert_grad.instances().create<IndexT>("i");
    }
    auto grad = vert_grad.instances().find<Vector3>("grad");
    if(!grad)
    {
        grad = vert_grad.instances().create<Vector3>("grad");
    }

    auto i_view = view(*i);
    g.indices().copy_to(i_view.data());

    auto grad_view = view(*grad);
    g.values().copy_to(grad_view.data());
}

void ContactSystemExporter::get_contact_gradient(std::string_view prim_type,
                                                 std::string_view contact_type,
                                                 geometry::Geometry& prim_grad)
{
    Exporter* exporter = find_exporter(prim_type, contact_type);
    if(!exporter)
    {
        spdlog::warn("Exporter for {}+{} not found, ignore output.", prim_type, contact_type);
        return;
    }

    exporter->get_gradient(prim_grad);
}

void ContactSystemExporter::get_contact_hessian(geometry::Geometry& vert_hess)
{
    auto& h = m_global_contact_manager->m_impl.sorted_contact_hessian;

    vert_hess.instances().resize(h.triplet_count());
    auto i = vert_hess.instances().find<IndexT>("i");
    if(!i)
    {
        i = vert_hess.instances().create<IndexT>("i");
    }
    auto j = vert_hess.instances().find<IndexT>("j");
    if(!j)
    {
        j = vert_hess.instances().create<IndexT>("j");
    }
    auto hess = vert_hess.instances().find<Matrix3x3>("hess");
    if(!hess)
    {
        hess = vert_hess.instances().create<Matrix3x3>("hess");
    }
    auto i_view    = view(*i);
    auto j_view    = view(*j);
    auto hess_view = view(*hess);

    h.row_indices().copy_to(i_view.data());
    h.col_indices().copy_to(j_view.data());
    h.values().copy_to(hess_view.data());
}

void ContactSystemExporter::get_contact_hessian(std::string_view prim_type_v,
                                                std::string_view contact_type_v,
                                                geometry::Geometry& prim_hess)
{

    // add type to geometry.meta()
    auto prim_type = prim_hess.meta().find<std::string>("primitive_type");
    if(!prim_type)
    {
        prim_type = prim_hess.meta().create<std::string>("primitive_type");
    }
    view(*prim_type)[0] = prim_type_v;

    auto contact_type = prim_hess.meta().find<std::string>("contact_type");
    if(!contact_type)
    {
        contact_type = prim_hess.meta().create<std::string>("contact_type");
    }
    view(*contact_type)[0] = contact_type_v;
}

void ContactSystemExporter::get_contact_primtives(std::string_view prim_type_v,
                                                  std::string_view contact_type_v,
                                                  geometry::Geometry& prims)
{
    auto supported_types = get_contact_primitive_types();
    if(std::find(supported_types.begin(), supported_types.end(), prim_type_v)
       == supported_types.end())
    {
        spdlog::warn("Unsupported contact primitive type: {}. Supported types are: [{}], ignore output.",
                     prim_type_v,
                     fmt::join(supported_types, ", "));
        return;
    }
    // add type to geometry.meta()
    auto prim_type = prims.meta().find<std::string>("primitive_type");
    if(!prim_type)
    {
        prim_type = prims.meta().create<std::string>("primitive_type");
    }
    view(*prim_type)[0] = prim_type_v;

    if(m_simplex_trajectory_filter)
    {
        if(prim_type_v == "PP")
        {
            auto PPs = m_simplex_trajectory_filter->PPs();

            prims.instances().resize(PPs.size());
            auto topo = prims.instances().find<Vector2i>("topo");
            if(!topo)
            {
                topo = prims.instances().create<Vector2i>("topo");
            }
            auto topo_view = view(*topo);
            PPs.copy_to(topo_view.data());
        }
        else if(prim_type_v == "PE")
        {
            auto PEs = m_simplex_trajectory_filter->PEs();

            prims.instances().resize(PEs.size());
            auto topo = prims.instances().find<Vector3i>("topo");
            if(!topo)
            {
                topo = prims.instances().create<Vector3i>("topo");
            }
            auto topo_view = view(*topo);
            PEs.copy_to(topo_view.data());
        }
        else if(prim_type_v == "PT")
        {
            auto PTs = m_simplex_trajectory_filter->PTs();

            prims.instances().resize(PTs.size());
            auto topo = prims.instances().find<Vector4i>("topo");
            if(!topo)
            {
                topo = prims.instances().create<Vector4i>("topo");
            }
            auto topo_view = view(*topo);
            PTs.copy_to(topo_view.data());
        }
        else if(prim_type_v == "EE")
        {
            auto EEs = m_simplex_trajectory_filter->EEs();

            prims.instances().resize(EEs.size());
            auto topo = prims.instances().find<Vector4i>("topo");
            if(!topo)
            {
                topo = prims.instances().create<Vector4i>("topo");
            }
            auto topo_view = view(*topo);
            EEs.copy_to(topo_view.data());
        }
    }

    if(m_vertex_half_plane_trajectory_filter)
    {
        if(prim_type_v == "PH")
        {
            auto PHs = m_vertex_half_plane_trajectory_filter->PHs();

            prims.instances().resize(PHs.size());
            auto topo = prims.instances().find<Vector2i>("topo");
            if(!topo)
            {
                topo = prims.instances().create<Vector2i>("topo");
            }
            auto topo_view = view(*topo);
            PHs.copy_to(topo_view.data());

            // NOTE:
            // Because the H in PH using the local index of half-plane
            // So we need to adjust the vertex indices by the offset to
            // get the global vertex indices
            auto v_offset = m_half_plane_vertex_reporter->vertex_offset();
            std::ranges::transform(topo_view,
                                   topo_view.begin(),
                                   [v_offset](Vector2i& v)
                                   {
                                       // Adjust vertex indices by the offset
                                       v[1] += v_offset;
                                       return v;
                                   });
        }
    }
}

vector<std::string> ContactSystemExporter::get_contact_primitive_types() const
{
    return {"PP", "PE", "PT", "EE", "PH"};
}

vector<std::string> ContactSystemExporter::get_contact_types() const
{
    return {"N", "F"};
}

void ContactSystemExporter::add_exporter(std::string_view prim_type,
                                         std::string_view contact_type,
                                         Exporter&&       exporter)
{
    auto key = fmt::format("{}+{}", prim_type, contact_type);
    if(m_exporters.find(key) != m_exporters.end())
    {
        spdlog::warn("Exporter for {} already exists, overwriting.", key);
    }
    m_exporters[key] = std::move(exporter);
}

ContactSystemExporter::Exporter* ContactSystemExporter::find_exporter(std::string_view prim_type,
                                                                      std::string_view contact_type)
{
    auto key = fmt::format("{}+{}", prim_type, contact_type);
    auto it  = m_exporters.find(key);
    if(it != m_exporters.end())
    {
        return &it->second;
    }
    else
    {
        spdlog::warn("Exporter for {} not found.", key);
        return nullptr;
    }
}
}  // namespace uipc::backend::cuda