#include <collision_detection/simplex_trajectory_filter.h>

namespace uipc::backend::cuda
{
void SimplexTrajectoryFilter::do_build()
{
    m_impl.global_vertex_manager = &require<GlobalVertexManager>();
    m_impl.global_simplicial_surface_manager = &require<GlobalSimpicialSurfaceManager>();
    m_impl.global_contact_manager = &require<GlobalContactManager>();
    auto global_trajectory_filter = &require<GlobalTrajectoryFilter>();

    global_trajectory_filter->add_filter(this);
}

void SimplexTrajectoryFilter::do_detect(GlobalTrajectoryFilter::DetectInfo& info)
{
    DetectInfo this_info{&m_impl};
    this_info.m_alpha = info.alpha();
    do_detect(this_info);
}
void SimplexTrajectoryFilter::do_filter_active(GlobalTrajectoryFilter::FilterActiveInfo& info)
{
    FilterActiveInfo this_info{&m_impl};
    do_filter_active(this_info);

    spdlog::info("SimplexTrajectoryFilter PTs: {}, EEs: {}, PEs: {}, PPs: {}",
                 m_impl.PTs.size(),
                 m_impl.EEs.size(),
                 m_impl.PEs.size(),
                 m_impl.PPs.size());
}
void SimplexTrajectoryFilter::do_filter_toi(GlobalTrajectoryFilter::FilterTOIInfo& info)
{
    FilterTOIInfo this_info{&m_impl};
    this_info.m_alpha = info.alpha();
    this_info.m_toi   = info.toi();
    do_filter_toi(this_info);
}

void SimplexTrajectoryFilter::Impl::record_friction_candidates(
    GlobalTrajectoryFilter::RecordFrictionCandidatesInfo& info)
{
    loose_resize(friction_PT, PTs.size());
    friction_PT.view().copy_from(PTs);

    loose_resize(friction_EE, EEs.size());
    friction_EE.view().copy_from(EEs);

    loose_resize(friction_PE, PEs.size());
    friction_PE.view().copy_from(PEs);

    loose_resize(friction_PP, PPs.size());
    friction_PP.view().copy_from(PPs);

    spdlog::info("SimplexTrajectoryFilter Friction PT: {}, EE: {}, PE: {}, PP: {}",
                 friction_PT.size(),
                 friction_EE.size(),
                 friction_PE.size(),
                 friction_PP.size());
}

void SimplexTrajectoryFilter::do_record_friction_candidates(GlobalTrajectoryFilter::RecordFrictionCandidatesInfo& info)
{
    m_impl.record_friction_candidates(info);
}

Float SimplexTrajectoryFilter::BaseInfo::d_hat() const noexcept
{
    return m_impl->global_contact_manager->d_hat();
}

muda::CBufferView<Vector3> SimplexTrajectoryFilter::BaseInfo::positions() const noexcept
{
    return m_impl->global_vertex_manager->positions();
}

muda::CBufferView<IndexT> SimplexTrajectoryFilter::BaseInfo::surf_vertices() const noexcept
{
    return m_impl->global_simplicial_surface_manager->surf_vertices();
}

muda::CBufferView<Vector2i> SimplexTrajectoryFilter::BaseInfo::surf_edges() const noexcept
{
    return m_impl->global_simplicial_surface_manager->surf_edges();
}

muda::CBufferView<Vector3i> SimplexTrajectoryFilter::BaseInfo::surf_triangles() const noexcept
{
    return m_impl->global_simplicial_surface_manager->surf_triangles();
}

muda::CBufferView<Vector4i> SimplexTrajectoryFilter::PTs() const noexcept
{
    return m_impl.PTs;
}
muda::CBufferView<Vector4i> SimplexTrajectoryFilter::EEs() const noexcept
{
    return m_impl.EEs;
}
muda::CBufferView<Vector3i> SimplexTrajectoryFilter::PEs() const noexcept
{
    return m_impl.PEs;
}
muda::CBufferView<Vector2i> SimplexTrajectoryFilter::PPs() const noexcept
{
    return m_impl.PPs;
}

muda::CBufferView<Vector4i> SimplexTrajectoryFilter::friction_PTs() const noexcept
{
    return m_impl.friction_PT;
}

muda::CBufferView<Vector4i> SimplexTrajectoryFilter::friction_EEs() const noexcept
{
    return m_impl.friction_EE;
}

muda::CBufferView<Vector3i> SimplexTrajectoryFilter::friction_PEs() const noexcept
{
    return m_impl.friction_PE;
}

muda::CBufferView<Vector2i> SimplexTrajectoryFilter::friction_PPs() const noexcept
{
    return m_impl.friction_PP;
}
muda::CBufferView<Vector3> SimplexTrajectoryFilter::DetectInfo::displacements() const noexcept
{
    return m_impl->global_vertex_manager->displacements();
}

void SimplexTrajectoryFilter::FilterActiveInfo::PTs(muda::CBufferView<Vector4i> PTs) noexcept
{
    m_impl->PTs = PTs;
}

void SimplexTrajectoryFilter::FilterActiveInfo::EEs(muda::CBufferView<Vector4i> EEs) noexcept
{
    m_impl->EEs = EEs;
}

void SimplexTrajectoryFilter::FilterActiveInfo::PEs(muda::CBufferView<Vector3i> PEs) noexcept
{
    m_impl->PEs = PEs;
}

void SimplexTrajectoryFilter::FilterActiveInfo::PPs(muda::CBufferView<Vector2i> PPs) noexcept
{
    m_impl->PPs = PPs;
}
muda::VarView<Float> SimplexTrajectoryFilter::FilterTOIInfo::toi() noexcept
{
    return m_toi;
}
}  // namespace uipc::backend::cuda
