#pragma once
#include <string_view>
#include <uipc/geometry/attribute_collection.h>
#include <uipc/geometry/attribute_friend.h>
#include <uipc/geometry/geometry_friend.h>
namespace uipc::backend
{
class GeometryVisitor;
}
namespace uipc::geometry
{
/**
 * @brief An abstract class for geometries
 */
class UIPC_CORE_API IGeometry
{
    template <typename T>
    friend class GeometryFriend;
    friend class backend::GeometryVisitor;

  public:
    /**
     * @brief Get the type of the geometries, check the type to downcast the geometries to a specific type
     * 
     * @return a string_view of the type of the geometries
     */
    [[nodiscard]] std::string_view type() const noexcept;
    virtual ~IGeometry() = default;
    [[nodiscard]] Json to_json() const;

  protected:
    [[nodiscard]] virtual std::string_view get_type() const noexcept = 0;
    virtual Json                           do_to_json() const        = 0;
    virtual void do_collect_attribute_collections(vector<std::string>& names,
                                                  vector<AttributeCollection*>& collections) = 0;

  private:
    /**
     * @brief Provide a way to use a generic way to iterate over the attribute collections.
     * 
     * @param[out] names The names of the attribute collections.
     * @param[out] collections The attribute collections.
     */
    void collect_attribute_collections(vector<std::string>& names,
                                       vector<AttributeCollection*>& collections);
};

/**
 * @brief A base geometries class that contains the instance attributes and the meta attributes.
 */
class UIPC_CORE_API Geometry : public IGeometry
{
    template <typename T>
    friend class GeometryFriend;

  public:
    /**
     * @brief A wrapper class for the meta attributes of a geometries.
     */
    template <bool IsConst>
    class MetaAttributesT
    {
        friend struct fmt::formatter<MetaAttributesT<IsConst>>;

        using AutoAttributeCollection =
            std::conditional_t<IsConst, const AttributeCollection, AttributeCollection>;

        template <bool _IsConst>
        friend class MetaAttributesT;

        template <typename T>
        friend class AttributeFriend;

      public:
        MetaAttributesT(AutoAttributeCollection& attributes)
            : m_attributes(attributes)
        {
        }

        template <bool OtherIsConst>
        MetaAttributesT(const MetaAttributesT<OtherIsConst>& o) noexcept
            requires(IsConst)
            : m_attributes(o.m_attributes)
        {
        }

        MetaAttributesT(const MetaAttributesT& o)            = default;
        MetaAttributesT(MetaAttributesT&& o)                 = default;
        MetaAttributesT& operator=(const MetaAttributesT& o) = default;
        MetaAttributesT& operator=(MetaAttributesT&& o)      = default;

        /**
         * @brief Find an attribute by type and name, if the attribute does not exist, return nullptr.
         */
        template <typename T>
        [[nodiscard]] auto find(std::string_view name) &&
        {
            return m_attributes.template find<T>(name);
        }

        /**
         * @brief Create an attribute with the given name.
         */
        template <typename T>
        decltype(auto) create(std::string_view name, const T& init_value = {}) &&
        {
            return m_attributes.template create<T>(name, init_value);
        }

        template <typename T>
        decltype(auto) share(std::string_view name, const AttributeSlot<T>& slot)
            requires(!IsConst)
        {
            return m_attributes.template share<T>(name, slot);
        }

        /**
         * @sa AttributeCollection::destroy
         */
        void destroy(std::string_view name) &&
            requires(!IsConst)
        {
            m_attributes.destroy(name);
        }

        void copy_from(MetaAttributesT<true> other,
                       const AttributeCopy&  copy          = {},
                       span<const string>    include_names = {},
                       span<const string>    exclude_names = {}) &&
            requires(!IsConst)
        {
            m_attributes.copy_from(other.m_attributes, copy, include_names, exclude_names);
        }

        Json to_json() const;

      private:
        AutoAttributeCollection& m_attributes;
    };

    using MetaAttributes  = MetaAttributesT<false>;
    using CMetaAttributes = MetaAttributesT<true>;

    /**
     * @brief A wrapper class for the instance attributes of a geometries.
     */
    template <bool IsConst>
    class InstanceAttributesT
    {
        friend struct fmt::formatter<InstanceAttributesT<IsConst>>;

        template <bool _IsConst>
        friend class InstanceAttributesT;

        template <typename T>
        friend class AttributeFriend;

        using AutoAttributeCollection =
            std::conditional_t<IsConst, const AttributeCollection, AttributeCollection>;

      public:
        InstanceAttributesT(AutoAttributeCollection& attributes)
            : m_attributes(attributes)
        {
        }

        template <bool OtherIsConst>
        InstanceAttributesT(const InstanceAttributesT<OtherIsConst>& o) noexcept
            requires(IsConst)
            : m_attributes(o.m_attributes)
        {
        }

        InstanceAttributesT(const InstanceAttributesT& o)            = default;
        InstanceAttributesT(InstanceAttributesT&& o)                 = default;
        InstanceAttributesT& operator=(const InstanceAttributesT& o) = default;
        InstanceAttributesT& operator=(InstanceAttributesT&& o)      = default;

        /**
         * @sa AttributeCollection::resize
         */
        void resize(size_t size) &&
            requires(!IsConst);
        /**
         * @sa AttributeCollection::reserve
         */
        void reserve(size_t size) &&
            requires(!IsConst);
        /**
         * @sa AttributeCollection::clear
         */
        void clear() &&
            requires(!IsConst);
        /**
         * @sa AttributeCollection::size
         */
        [[nodiscard]] SizeT size() &&;

        /**
         * @sa AttributeCollection::destroy
         */
        void destroy(std::string_view name) &&
            requires(!IsConst);

        /**
         * @brief Find an attribute by type and name, if the attribute does not exist, return nullptr.
         */
        template <typename T>
        [[nodiscard]] auto find(std::string_view name) &&
        {
            return m_attributes.template find<T>(name);
        }

        /**
         * @brief Create an attribute with the given name.
         */
        template <typename T>
        decltype(auto) create(std::string_view name, const T& init_value = {}) &&
        {
            return m_attributes.template create<T>(name, init_value);
        }

        template <typename T>
        decltype(auto) share(std::string_view name, const AttributeSlot<T>& slot)
            requires(!IsConst)
        {
            return m_attributes.template share<T>(name, slot);
        }

        void copy_from(InstanceAttributesT<true> other,
                       const AttributeCopy&      copy          = {},
                       span<const string>        include_names = {},
                       span<const string>        exclude_names = {}) &&
            requires(!IsConst)
        {
            m_attributes.copy_from(other.m_attributes, copy, include_names, exclude_names);
        }

        Json to_json() const;

      private:
        AutoAttributeCollection& m_attributes;
    };

    using InstanceAttributes  = InstanceAttributesT<false>;
    using CInstanceAttributes = InstanceAttributesT<true>;

    Geometry();

    // allow copy_from and move on construction, because the geometry truely empty
    Geometry(const Geometry& o) = default;
    Geometry(Geometry&& o)      = default;

    // no copy_from or move assignment, because the geometry is no longer empty
    Geometry& operator=(const Geometry& o) = delete;
    Geometry& operator=(Geometry&& o)      = delete;

    /**
     * @brief Get the meta attributes of the geometries.
     * 
     * @return The meta attributes of the geometries. 
     */
    [[nodiscard]] MetaAttributes meta();

    [[nodiscard]] CMetaAttributes meta() const;


    /**
     * @brief Get the instance attributes of the geometries.
     * 
     * @return  The instance attributes of the geometries.
     */
    [[nodiscard]] InstanceAttributes instances();

    [[nodiscard]] CInstanceAttributes instances() const;

    template <std::derived_from<Geometry> T>
    [[nodiscard]] T* as();

  protected:
    virtual Json do_to_json() const override;
    virtual void do_collect_attribute_collections(vector<std::string>& names,
                                                  vector<AttributeCollection*>& collections) override;

    AttributeCollection m_intances;
    AttributeCollection m_meta;
};
}  // namespace uipc::geometry


namespace fmt
{
template <bool IsConst>
struct formatter<uipc::geometry::Geometry::MetaAttributesT<IsConst>>
    : public formatter<string_view>
{
    auto format(const uipc::geometry::Geometry::MetaAttributesT<IsConst>& attr,
                format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", attr.m_attributes);
    }
};

template <bool IsConst>
struct formatter<uipc::geometry::Geometry::InstanceAttributesT<IsConst>>
    : public formatter<string_view>
{
    auto format(const uipc::geometry::Geometry::InstanceAttributesT<IsConst>& attr,
                format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", attr.m_attributes);
    }
};

template <>
struct UIPC_CORE_API formatter<uipc::geometry::Geometry> : public formatter<string_view>
{
    appender format(const uipc::geometry::Geometry& geo, format_context& ctx) const;
};
}  // namespace fmt

#include "details/geometry.inl"