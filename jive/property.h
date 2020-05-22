#pragma

#include <functional>

namespace jive
{

template<typename T>
class Property
{
public:
    using OnAssign = std::function<void (const T &)>;

    Property()
        :
        value_{},
        onAssign_{[](const T &){}}
    {

    }
    
    // The initializer tag is needed to disambiguate the implicitly deleted
    // operator=(const Property &) from the provided operator=(const T &).  If
    // Property provides a constructor from const T &, the compiler tries to
    // create a temporary Property<T> and then cannot call the implicitly
    // deleted operator=.
    explicit Property(const T &value)
        :
        value_{value},
        onAssign_{[](const T &){}}
    {

    }
    
    void RegisterAssign(OnAssign &&onAssign)
    {
        this->onAssign_ = std::move(onAssign);
    }

    operator const T & () const { return this->value_; }

    const Property & operator=(const T &other) const
    {
        const_cast<Property *>(this)->value_ = other;
        this->onAssign_(other);
        return *this;
    }

private:
    T value_;
    OnAssign onAssign_;
};

} // end namespace jive
