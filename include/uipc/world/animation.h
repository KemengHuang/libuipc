#pragma once
#include <uipc/common/type_define.h>
#include <uipc/world/object.h>
#include <uipc/common/span.h>
#include <functional>

namespace uipc::backend
{
class AnimatorVisitor;
}

namespace uipc::world
{
class World;
class UIPC_CORE_API Animation
{
  public:
    class UIPC_CORE_API UpdateHint
    {
      public:
        void fixed_vertices_changing(bool v) noexcept;

      private:
        friend class Animation;
        bool m_fixed_vertices_changing = false;
    };


    class UIPC_CORE_API UpdateInfo
    {
      public:
        Object&                         object() const noexcept;
        span<S<geometry::GeometrySlot>> geo_slots() const noexcept;
        span<S<geometry::GeometrySlot>> rest_geo_slots() const noexcept;
        SizeT                           frame() const noexcept;
        UpdateHint&                     hint() noexcept;

      private:
        UpdateInfo(Animation& animation) noexcept;
        friend class Animation;
        Animation* m_animation = nullptr;
        UpdateHint m_hint;
    };

    using ActionOnUpdate = std::function<void(UpdateInfo&)>;

  private:
    friend class Animator;
    friend class backend::AnimatorVisitor;
    void init();
    void update();

    Animation(Scene& scene, Object& object, ActionOnUpdate&& on_update) noexcept;

    Object*        m_object = nullptr;
    Scene*         m_scene  = nullptr;
    ActionOnUpdate m_on_update;

    mutable vector<S<geometry::GeometrySlot>> m_temp_geo_slots;
    mutable vector<S<geometry::GeometrySlot>> m_temp_rest_geo_slots;
};
}  // namespace uipc::world