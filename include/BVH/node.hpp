#ifndef INCLUDE_NODE_HPP
#define INCLUDE_NODE_HPP

#include <span>
#include <vector>
#include <memory>
#include <iostream>
#include "triangle.hpp"
#include "AABB.hpp"

namespace BVH {

class Node {
private:
    AABB box_;
    std::span<Triangle> triangles_;
    bool is_leaf = true;

    std::unique_ptr<Node> left_  = nullptr;
    std::unique_ptr<Node> right_ = nullptr; 

public:


};

} // namespace BVH

#endif // INCLUDE_NODE_HPP