#ifndef VERTEXKEY_H
#define VERTEXKEY_H

#include <cstddef>
#include <functional>

// Used for duplication of vertices in model loading
struct VertexKey
{
    unsigned int pos;
    unsigned int normal;


    // Hash function, used by hash table to compare equivalence
    bool operator==(const VertexKey& other) const noexcept {
        return pos == other.pos &&
            normal == other.normal;
    }
};

// Hash specialization must be in std namespace
namespace std {
    template<>
    struct hash<VertexKey>
    {
        size_t operator()(const VertexKey& k) const noexcept
        {
            // Classic hash-combine pattern (same as in Boost)
            size_t h1 = std::hash<unsigned int>{}(k.pos);
            size_t h2 = std::hash<unsigned int>{}(k.normal);
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}
#endif // !VERTEXKEY_H

