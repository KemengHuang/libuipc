#pragma once
#include <contact_system/contact_reporter.h>
#include <line_search/line_searcher.h>
#include <contact_system/contact_coeff.h>
#include <collision_detection/simplex_trajectory_filter.h>

namespace uipc::backend::cuda
{
class SimplexFrictionalContact : public ContactReporter
{
  public:
    using ContactReporter::ContactReporter;

    class Impl;

    class BaseInfo
    {
      public:
        BaseInfo(Impl* impl) noexcept
            : m_impl(impl)
        {
        }

        muda::CBuffer2DView<ContactCoeff> contact_tabular() const;
        muda::CBufferView<Vector4i>       friction_PTs() const;
        muda::CBufferView<Vector4i>       friction_EEs() const;
        muda::CBufferView<Vector3i>       friction_PEs() const;
        muda::CBufferView<Vector2i>       friction_PPs() const;
        muda::CBufferView<Vector3>        positions() const;
        muda::CBufferView<Vector3>        prev_positions() const;
        muda::CBufferView<Vector3>        rest_positions() const;
        muda::CBufferView<Float>          thicknesses() const;
        muda::CBufferView<IndexT>         contact_element_ids() const;
        Float                             d_hat() const;
        Float                             dt() const;
        Float                             eps_velocity() const;

      private:
        friend class SimplexFrictionalContact;
        Impl* m_impl;
    };

    class ContactInfo : public BaseInfo
    {
      public:
        ContactInfo(Impl* impl) noexcept
            : BaseInfo(impl)
        {
        }
        auto friction_PT_gradients() const noexcept { return m_PT_gradients; }
        auto friction_PT_hessians() const noexcept { return m_PT_hessians; }
        auto friction_EE_gradients() const noexcept { return m_EE_gradients; }
        auto friction_EE_hessians() const noexcept { return m_EE_hessians; }
        auto friction_PE_gradients() const noexcept { return m_PE_gradients; }
        auto friction_PE_hessians() const noexcept { return m_PE_hessians; }
        auto friction_PP_gradients() const noexcept { return m_PP_gradients; }
        auto friction_PP_hessians() const noexcept { return m_PP_hessians; }

      private:
        friend class SimplexFrictionalContact;
        muda::DoubletVectorView<Float, 3> m_PT_gradients;
        muda::TripletMatrixView<Float, 3> m_PT_hessians;

        muda::DoubletVectorView<Float, 3> m_EE_gradients;
        muda::TripletMatrixView<Float, 3> m_EE_hessians;

        muda::DoubletVectorView<Float, 3> m_PE_gradients;
        muda::TripletMatrixView<Float, 3> m_PE_hessians;

        muda::DoubletVectorView<Float, 3> m_PP_gradients;
        muda::TripletMatrixView<Float, 3> m_PP_hessians;
    };


    class BuildInfo
    {
      public:
    };

    class EnergyInfo : public BaseInfo
    {
      public:
        EnergyInfo(Impl* impl) noexcept
            : BaseInfo(impl)
        {
        }

        muda::BufferView<Float> friction_PT_energies() const noexcept
        {
            return m_PT_energies;
        }
        muda::BufferView<Float> friction_EE_energies() const noexcept
        {
            return m_EE_energies;
        }
        muda::BufferView<Float> friction_PE_energies() const noexcept
        {
            return m_PE_energies;
        }
        muda::BufferView<Float> friction_PP_energies() const noexcept
        {
            return m_PP_energies;
        }

      private:
        friend class SimplexFrictionalContact;
        muda::BufferView<Float> m_PT_energies;
        muda::BufferView<Float> m_EE_energies;
        muda::BufferView<Float> m_PE_energies;
        muda::BufferView<Float> m_PP_energies;
    };

    class Impl
    {
      public:
        SimSystemSlot<GlobalTrajectoryFilter> global_trajectory_filter;
        SimSystemSlot<GlobalContactManager>   global_contact_manager;
        SimSystemSlot<GlobalVertexManager>    global_vertex_manager;

        SimSystemSlot<SimplexTrajectoryFilter> simplex_trajectory_filter;

        SizeT PT_count = 0;
        SizeT EE_count = 0;
        SizeT PE_count = 0;
        SizeT PP_count = 0;
        Float dt       = 0;

        muda::CBufferView<Float>           PT_energies;
        muda::CDoubletVectorView<Float, 3> PT_gradients;
        muda::CTripletMatrixView<Float, 3> PT_hessians;

        muda::CBufferView<Float>           EE_energies;
        muda::CDoubletVectorView<Float, 3> EE_gradients;
        muda::CTripletMatrixView<Float, 3> EE_hessians;

        muda::CBufferView<Float>           PE_energies;
        muda::CDoubletVectorView<Float, 3> PE_gradients;
        muda::CTripletMatrixView<Float, 3> PE_hessians;

        muda::CBufferView<Float>           PP_energies;
        muda::CDoubletVectorView<Float, 3> PP_gradients;
        muda::CTripletMatrixView<Float, 3> PP_hessians;
    };

    muda::CBufferView<Vector4i>        PTs() const;
    muda::CBufferView<Float>           PT_energies() const;
    muda::CDoubletVectorView<Float, 3> PT_gradients() const;
    muda::CTripletMatrixView<Float, 3> PT_hessians() const;

    muda::CBufferView<Vector4i>        EEs() const;
    muda::CBufferView<Float>           EE_energies() const;
    muda::CDoubletVectorView<Float, 3> EE_gradients() const;
    muda::CTripletMatrixView<Float, 3> EE_hessians() const;

    muda::CBufferView<Vector3i>        PEs() const;
    muda::CBufferView<Float>           PE_energies() const;
    muda::CDoubletVectorView<Float, 3> PE_gradients() const;
    muda::CTripletMatrixView<Float, 3> PE_hessians() const;

    muda::CBufferView<Vector2i>        PPs() const;
    muda::CBufferView<Float>           PP_energies() const;
    muda::CDoubletVectorView<Float, 3> PP_gradients() const;
    muda::CTripletMatrixView<Float, 3> PP_hessians() const;

  protected:
    virtual void do_build(BuildInfo& info)           = 0;
    virtual void do_compute_energy(EnergyInfo& info) = 0;
    virtual void do_assemble(ContactInfo& info)      = 0;

  private:
    virtual void do_report_energy_extent(GlobalContactManager::EnergyExtentInfo& info) override final;
    virtual void do_compute_energy(GlobalContactManager::EnergyInfo& info) override final;
    virtual void do_report_gradient_hessian_extent(
        GlobalContactManager::GradientHessianExtentInfo& info) override final;
    virtual void do_assemble(GlobalContactManager::GradientHessianInfo& info) override final;
    virtual void do_build(ContactReporter::BuildInfo& info) override final;

    Impl m_impl;
};
}  // namespace uipc::backend::cuda
