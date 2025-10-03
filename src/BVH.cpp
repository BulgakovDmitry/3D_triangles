#include "BVH/BVH.hpp"
#include "BVH/node.hpp"
#include "triangle.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <span>
#include <stdexcept>

using namespace triangle;
using namespace bin_tree;

Axis BVH::longest_axis(const AABB &box) {
    Vector v(box.p_max, box.p_min);
    float  v_x = v.get_x();
    float  v_y = v.get_y();
    float  v_z = v.get_z();

    if (v_x >= v_y && v_x >= v_z) {
        return Axis::axis_x;
    } else if (v_y >= v_z) {
        return Axis::axis_y;
    } else {
        return Axis::axis_z;
    }
}

void BVH::build() {
    if (triangles_.empty()) {
        root_.reset();
        return;
    }
    root_ = build_node(0, triangles_.size());
}

std::unique_ptr<Node> BVH::build_node(std::size_t start, std::size_t end) {
    std::span<Triangle> triangles(triangles_.begin() + start, triangles_.begin() + end);

    auto                node = std::make_unique<Node>();

    AABB                box  = calculate_bounding_box(triangles);
    node->set_box(box);

    const std::size_t count = end - start;
    if (count <= max_number_of_triangles_in_leaf) {
        std::span<triangle::Triangle> all{triangles_.data(), triangles_.size()};
        node->set_triangles(all.subspan(start, count));
        return node;
    }

    const Axis axis = longest_axis(box);

    auto       comp = [axis](const triangle::Triangle &a, const triangle::Triangle &b) {
        const std::size_t i = static_cast<std::size_t>(axis);
        return a.get_box().get_center()[i] < b.get_box().get_center()[i];
    };

    triangle::Triangle *first = triangles_.data() + start;
    triangle::Triangle *last  = triangles_.data() + end;
    triangle::Triangle *midIt = first + count / 2;

    std::nth_element(first, midIt, last, comp);
    const std::size_t mid = start + count / 2;

    node->set_left(build_node(start, mid));
    node->set_right(build_node(mid, end));

    return node;
}

void BVH::dump_graph() const {
    std::ofstream gv(dump_file_gv);
    if (!gv)
        throw std::runtime_error("open gv file - error");

    gv << "digraph G {\n"
       << "    rankdir=TB;\n"
       << "    node [style=filled, fontname=\"Helvetica\", fontcolor=darkblue, "
          "fillcolor=peachpuff, color=\"#252A34\", penwidth=2.5];\n"
       << "    bgcolor=\"lemonchiffon\";\n\n";

    dump_graph_list_nodes(root_, gv);
    gv << "\n";
    dump_graph_connect_nodes(root_, gv);

    gv << "}\n";
    gv.close();

    std::system(("dot " + dump_file_gv + " -Tpng -o " + dump_file_png).c_str());
}

void BVH::dump_graph_list_nodes(const std::unique_ptr<Node> &node, std::ofstream &gv) const {
    if (!node)
        return;

    const auto &bbox = node->get_box();
    const auto &pmin = bbox.p_min;
    const auto &pmax = bbox.p_max;
    if (!node->is_branch()) {

        gv << "    node_" << node.get()
           << " [shape=Mrecord; style=filled; fillcolor=palegreen; color=\"#000000\"; "
              "fontcolor=\"#000000\"; "
           << "label=\"{ node_" << node.get() << " | p_min (" << pmin.get_x() << ", "
           << pmin.get_y() << ", " << pmin.get_z() << ") | p_max (" << pmax.get_x() << ", "
           << pmax.get_y() << ", " << pmax.get_z() << ") | { left: " << node->get_left().get()
           << " | right: " << node->get_right().get() << " } }\"" << "];\n";
    } else {
        gv << "    node_" << node.get()
           << " [shape=Mrecord; style=filled; fillcolor=cornflowerblue; color=\"#000000\"; "
              "fontcolor=\"#000000\"; "
           << "label=\"{ node_" << node.get() << " | p_min (" << pmin.get_x() << ", "
           << pmin.get_y() << ", " << pmin.get_z() << ") | p_max (" << pmax.get_x() << ", "
           << pmax.get_y() << ", " << pmax.get_z()
           << ") | num of triangles = " << node->get_number_of_triangles() << "}\"" << "];\n";
    }

    if (node->get_left())
        dump_graph_list_nodes(node->get_left(), gv);
    if (node->get_right())
        dump_graph_list_nodes(node->get_right(), gv);
}

void BVH::dump_graph_connect_nodes(const std::unique_ptr<Node> &node, std::ofstream &gv) const {
    if (!node)
        return;

    if (node->get_left())
        gv << "    node_" << node.get() << " -> node_" << node->get_left().get() << ";\n";
    if (node->get_right())
        gv << "    node_" << node.get() << " -> node_" << node->get_right().get() << ";\n";

    if (node->get_left())
        dump_graph_connect_nodes(node->get_left(), gv);
    if (node->get_right())
        dump_graph_connect_nodes(node->get_right(), gv);
}
