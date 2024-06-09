#include <uipc/geometry/attribute_collection.h>
#include <uipc/common/log.h>

namespace uipc::geometry
{
P<IAttributeSlot> AttributeCollection::share(std::string_view name, const IAttributeSlot& slot)
{
    auto n  = std::string{name};
    auto it = m_attributes.find(n);

    if(size() != slot.size())
        throw GeometryAttributeError{
            fmt::format("Attribute size mismatch, "
                        "Attribute Collision size is {}, input slot size is {}.",
                        size(),
                        slot.size())};

    if(it != m_attributes.end())
        throw GeometryAttributeError{
            fmt::format("Attribute with name [{}] already exist!", name)};
    return m_attributes[n] = slot.clone();
}

void AttributeCollection::destroy(std::string_view name)
{
    auto it = m_attributes.find(std::string{name});
    if(it == m_attributes.end())
    {
        UIPC_WARN_WITH_LOCATION("Destroying non-existing attribute [{}]", name);
        return;
    }

    if(!it->second->allow_destroy())
        throw GeometryAttributeError{
            fmt::format("Attribute [{}] don't allow destroy!", name)};

    m_attributes.erase(it);
}

P<IAttributeSlot> AttributeCollection::find(std::string_view name)
{
    auto it = m_attributes.find(std::string{name});
    return it != m_attributes.end() ? it->second : nullptr;
}


P<const IAttributeSlot> AttributeCollection::find(std::string_view name) const
{
    auto it = m_attributes.find(std::string{name});
    return it != m_attributes.end() ? it->second : nullptr;
}

void AttributeCollection::resize(SizeT N)
{
    for(auto& [name, slot] : m_attributes)
    {
        slot->make_owned();
        slot->attribute().resize(N);
    }
    m_size = N;
}

void AttributeCollection::reorder(span<const SizeT> O)
{
    for(auto& [name, slot] : m_attributes)
    {
        slot->make_owned();
        slot->attribute().reorder(O);
    }
}

SizeT AttributeCollection::size() const
{
    return m_size;
}

void AttributeCollection::clear()
{
    for(auto& [name, slot] : m_attributes)
    {
        slot->make_owned();
        slot->attribute().clear();
    }
}

void AttributeCollection::reserve(SizeT N)
{
    for(auto& [name, slot] : m_attributes)
    {
        slot->attribute().reserve(N);
    }
}

AttributeCollection::AttributeCollection(const AttributeCollection& o)
{
    for(auto& [name, attr] : o.m_attributes)
    {
        m_attributes[name] = attr->clone();
    }
    m_size = o.m_size;
}

AttributeCollection& AttributeCollection::operator=(const AttributeCollection& o)
{
    if(std::addressof(o) == this)
        return *this;
    for(auto& [name, attr] : o.m_attributes)
    {
        m_attributes[name] = attr->clone();
    }
    m_size = o.m_size;
    return *this;
}

AttributeCollection::AttributeCollection(AttributeCollection&& o) noexcept
    : m_attributes(std::move(o.m_attributes))
    , m_size(o.m_size)
{
    o.m_size = 0;
}

AttributeCollection& AttributeCollection::operator=(AttributeCollection&& o) noexcept
{
    if(std::addressof(o) == this)
        return *this;
    m_attributes = std::move(o.m_attributes);
    m_size       = o.m_size;
    o.m_size     = 0;
    return *this;
}

}  // namespace uipc::geometry

namespace fmt
{
appender formatter<uipc::geometry::AttributeCollection>::format(
    const uipc::geometry::AttributeCollection& collection, format_context& ctx)
{
    auto size = collection.size();

    fmt::format_to(ctx.out(), "[", size);

    for(const auto& [name, slot] : collection.m_attributes)
    {
        char star = slot->allow_destroy() ? ' ' : '*';
        fmt::format_to(ctx.out(), "{}'{}' ", star, name);
    }

    fmt::format_to(ctx.out(), "]");

    return ctx.out();
}
}  // namespace fmt