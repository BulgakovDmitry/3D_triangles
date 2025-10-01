#ifndef INCLUDE_NODE_HPP
#define INCLUDE_NODE_HPP

#include <span>
#include <vector>
#include <memory>
#include <iostream>
#include "triangle.hpp"
#include "AABB.hpp"

namespace bin_tree {

class Node {
private:
    AABB box_;
    std::span<Triangle> triangles_;
    bool is_branch_ = true;

    std::unique_ptr<Node> left_  = nullptr;
    std::unique_ptr<Node> right_ = nullptr; 

public:
    void set_left(std::unique_ptr<Node> left) {
        is_branch_ = false;
        left_ = std::move(left);
    }

    void set_right(std::unique_ptr<Node> right) {
        is_branch_ = false;
        right_ = std::move(right);
    }

    void set_box(const AABB& box) {
        box_ = box;
    }

    void set_triangles(const std::span<Triangle>& triangles) {
        is_branch_ = true;
        triangles_ = triangles;
    }

    const std::unique_ptr<Node>& get_left() const noexcept {
        return left_;
    }

    const std::unique_ptr<Node>& get_right() const noexcept {
        return right_;
    }

    bool is_branch() const noexcept {
        return is_branch_;
    }

    std::span<Triangle> get_triangles() const noexcept {
        return triangles_;
    }

    AABB get_box() const noexcept {
        return box_;
    }
};

} // namespace bin_tree

#endif // INCLUDE_NODE_HPP