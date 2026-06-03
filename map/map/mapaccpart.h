#ifndef MAPACCPART_H
#define MAPACCPART_H

#include <map>
#include <set>
#include <functional>

template<
    class K,
    class V,
    class C = std::less<K>,
    class Op = std::plus<V>
>
class map_accumulate_partition
{
    std::map<K, V, C>& m;
    std::set<K, C> keys;
    Op op;

    void recompute()
    {
        if (m.empty())
            return;

        typename std::map<K, V, C>::iterator part_begin = m.begin();

        for (typename std::set<K, C>::iterator pk = keys.begin();
            pk != keys.end();
            ++pk)
        {
            typename std::map<K, V, C>::iterator part_end =
                m.lower_bound(*pk);

            if (part_begin != part_end)
            {
                typename std::map<K, V, C>::iterator it = part_begin;
                V acc = it->second;
                ++it;

                for (; it != part_end; ++it)
                {
                    acc = op(acc, it->second);
                    it->second = acc;
                }
            }

            part_begin = part_end;
        }

        if (part_begin != m.end())
        {
            typename std::map<K, V, C>::iterator it = part_begin;
            V acc = it->second;
            ++it;

            for (; it != m.end(); ++it)
            {
                acc = op(acc, it->second);
                it->second = acc;
            }
        }
    }

public:

    map_accumulate_partition(std::map<K, V, C>& mm)
        : m(mm)
    {
    }

    void set_partition_key(const K& k)
    {
        keys.insert(k);
        recompute();
    }
};

#endif