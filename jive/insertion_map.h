/**
  * @file insertion_map.h
  *
  * @brief InsertionMap sorts keys in insertion order.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 26 Apr 2024
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include <map>
#include <string>


namespace jive
{


template<typename Key>
class OrderedKey
{
public:
    OrderedKey(size_t sortOrder, const Key &key)
        :
        key_(key),
        sortOrder_(sortOrder)
    {

    }

    OrderedKey() = default;

    OrderedKey(const OrderedKey &other) = default;

    OrderedKey & operator=(const Key &key)
    {
        this->key_ = key;
    }

#ifdef __GNUC__ && __GNUC__ <= 9
    auto operator<(const OrderedKey &other) const
    {
        return (this->sortOrder_ < other.sortOrder_);
    }
#else
    auto operator<=>(const OrderedKey &other) const
    {
        return (this->sortOrder_ <=> other.sortOrder_);
    }
#endif

    const Key & GetKey() const
    {
        return this->key_;
    }

    operator Key () const
    {
        return this->key_;
    }

private:
    Key key_;
    size_t sortOrder_;
};


template<typename Key>
std::ostream & operator<<(
    std::ostream &output,
    const OrderedKey<Key> &orderedKey)
{
    return output << orderedKey.GetKey();
}


template<typename Key, typename T>
class InsertionMap
{
    using Map = std::map<OrderedKey<Key>, T>;
    using Iterator = typename Map::iterator;

    Map map_;

public:
    InsertionMap()
        :
        map_(),
        insertionIndex_(0)
    {

    }

    auto Find(const Key &key)
    {
        return std::find_if(
            std::begin(this->map_),
            std::end(this->map_),
            [&key](const auto &keyValue) -> bool
            {
                return keyValue.first.GetKey() == key;
            });
    }

    auto Find(const Key &key) const
    {
        return std::find_if(
            std::begin(this->map_),
            std::end(this->map_),
            [&key](const auto &keyValue) -> bool
            {
                return keyValue.first.GetKey() == key;
            });
    }

    size_t count(const Key &key) const
    {
        auto it = this->Find(key);

        if (it != std::end(this->map_))
        {
            return 1;
        }

        return 0;
    }

    T & at(const Key &key)
    {
        auto it = this->Find(key);

        if (it == std::end(this->map_))
        {
            throw std::out_of_range("key not found");
        }

        return it->second;

    }

    const T & at(const Key &key) const
    {
        auto it = this->Find(key);

        if (it == std::end(this->map_))
        {
            throw std::out_of_range("key not found");
        }

        return it->second;
    }

    T & operator[](const Key &key)
    {
        auto it = this->Find(key);

        if (it == std::end(this->map_))
        {
            return this->map_[OrderedKey(this->insertionIndex_++, key)];
        }

        return it->second;
    }

    auto begin() const
    {
        return this->map_.begin();
    }

    auto end() const
    {
        return this->map_.end();
    }

    template<typename P>
    std::pair<Iterator, bool> insert(P &&value)
    {
        auto it = this->Find(value.first);

        if (it != std::end(this->map_))
        {
            return std::make_pair(it, false);
        }

        return this->map_.insert(
            std::make_pair(
                OrderedKey(
                    this->insertionIndex_++,
                    value.first),
                value.second));
    }

private:
    size_t insertionIndex_;
};


} // end namespace jive
