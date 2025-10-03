#ifndef INCLUDE_BVH_HPP
#define INCLUDE_BVH_HPP

#include "node.hpp"
#include "triangle.hpp"
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace bin_tree {

constexpr std::size_t max_number_of_triangles_in_leaf = 3;
const std::string     dump_file_gv                    = "../dump/graph_dump.gv";
const std::string     dump_file_png                   = "../dump/graph_dump.png";

enum class Axis { axis_x = 0, axis_y = 1, axis_z = 2 };

/* ---------- Bounding Volume Hierarchy ---------- */
class BVH {
private:
    std::unique_ptr<Node>           root_ = nullptr;
    std::vector<triangle::Triangle> triangles_;
    std::set<std::size_t>           intersecting_triangles_;

public:
    BVH(std::vector<triangle::Triangle> &&triangles) : triangles_(std::move(triangles)) {}

    void build();
    void dump_graph() const;

private:
    std::unique_ptr<Node> build_node(size_t start, size_t end);
    Axis                  longest_axis(const AABB &box);
    void dump_graph_list_nodes(const std::unique_ptr<Node> &node, std::ofstream &gv) const;
    void dump_graph_connect_nodes(const std::unique_ptr<Node> &node, std::ofstream &gv) const;
};

} // namespace bin_tree

#endif // INCLUDE_BVH_HPP
