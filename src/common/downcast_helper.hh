#pragma once

#include <concepts>

#include <type_traits>
#include <utility>
namespace plane_quest {

/** This class is used as an efficient alternative for RTTI dynamic_cast.
Subclasses identify themselves by key which is hashed */
template <class Base, class Key = std::string_view> class DowncastHelper {
    std::size_t type_hash;

  protected:
    DowncastHelper(const Key &type_key) {
        type_hash = std::hash<Key>{}(type_key);
    }

  public:
    template <std::derived_from<Base> T> T &&try_downcast() {
        std::size_t downcast_type_hash = std::hash<Key>{}(T::type_key());
        if (this->type_hash != downcast_type_hash)
            throw std::bad_cast();

        return static_cast<T &&>(*this);
    }
};

} // namespace plane_quest