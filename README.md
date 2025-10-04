<div align="center">

  # Solution to the triangle intersection problem in C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)
  
</div>

## Other Languages

1. [Russian](/README-R.md)
2. [English](/README.md)

## Content
- [1. Installation and Build](#installation-and-build)
- [2. Introduction](#introduction)
- [3. Implementation of algorithm](#implementation-of-algorithm)
- [4. Project structure](#project-structure)
- [5. Project Creators](#project-creators)

## Installation and Build:

To install, compile, and run enter:
```bash
git clone https://github.com/BulgakovDmitry/3D_triangles
cd 3D_triangles
cmake -S . -B build
cd build
cmake --build .
./3D_triangles
```
To run tests, enter:
```bash
./tests/geometry
./tests/triangles
```

If you want a debug build, enter:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cd build
cmake --build .
./3D_triangles
```

## Introduction
This `C++` project implements a program for working with triangles in three-dimensional space. Its core functionality includes robust algorithms for calculating geometric properties and detecting intersections between triangles.

The development of such geometric processing tools is highly prospective, as they form the foundational layer for a wide range of critical applications. These include collision detection in physics engines for video games and simulations, 3D modeling and computer-aided design (`CAD`) software, ray tracing for computer graphics, and even pathfinding in robotics. Efficient and accurate triangle manipulation is, therefore, essential for advancements in these technologically significant fields.

## Implementation of algorithm
### Step 1: Testing Triangle `T1` against Plane `π₂`
Three `orient_3d` predicates are computed to determine the position of the vertices of `T1` relative to the plane `π₂` defined by `T2`. If all vertices lie strictly on the same side of the plane, no intersection exists. If all three predicates return zero, the triangles are coplanar, and the problem is reduced to a `2D` intersection test. Otherwise, the algorithm proceeds.

<table>
  <tr>
    <td align="center">
      <img src="img/intersect.png" width="400">
      <br>
      <em>Fig. 1. Standard case of intersection of triangles on the line of intersection of planes. The line of intersection of the planes is marked in green, the areas in which the corresponding triangles lie are marked in blue and red.</em>
    </td>
    <td align="center">
      <img src="img/intersect_vertex.png" width="400">
      <br>
      <em>Fig. 2. Intersection of triangles in one vertex. The line of intersection of the planes is marked in green, the areas in which the corresponding triangles lie are marked in blue and red.</em>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td align="center">
      <img src="img/parrallel.png" width="400">
      <br>
      <em>Fig. 3. Parallel case, there is no intersection here. The areas in which the corresponding triangles lie are marked in blue and red</em>
    </td>
    <td align="center">
      <img src="img/no_intersect.png" width="400">
      <br>
      <em>Fig. 4. There is no intersection here. The red triangle intersects the plane of the blue at one vertex.</em>
    </td>
  </tr>
</table>


### Step 2: Testing Triangle `T2` against Plane `π₁`
Similarly, three `orient_3d` predicates are computed to determine the position of `T2's` vertices relative to the plane `π₁` of `T1`. If all vertices are on one side, there is no intersection. If all predicates are zero, the triangles are coplanar (`2D case`). If this step is passed, both triangles are guaranteed to intersect the line of intersection (`L`) of their respective planes.

### Step 3: Canonical Vertex Ordering
The vertices of each triangle are permuted to achieve a canonical configuration. For each triangle, the goal is to have one vertex (P) on one side of the other triangle's plane, and the other two vertices (`Q` and `R`) on the opposite side. This ensures that edges `P-Q` and `P-R` intersect the plane, defining a segment on the line `L`.

### Step 4: Interval Overlap Test via Predicates
The overlap of the two segments on line `L` is determined by evaluating two orientation predicates: `orient_3d(P₁, Q₁, P₂, Q₂) > 0` and `orient_3d(P₁, R₁, R₂, P₂) > 0`. If both conditions are true, the segments overlap, indicating that the triangles intersect.

### Function to check intersect of triangles
The algorithm for checking the intersection of triangles in three-dimensional space is implemented as follows:
<details>
<summary>Click to show/hide code</summary>
  
```cpp
bool intersect(const Triangle &triangle) const {
    Sign relative_positions = check_relative_positions(triangle);

    if (relative_positions == Sign::pozitive || relative_positions == Sign::negative)
        return false;

    if (relative_positions == Sign::common_plane)
        return intersect_2d(triangle);

    if (relative_positions == Sign::common_vertice_other_poz_or_neg)
        return intersect_one_vertice_in_plane(triangle);

    auto canon_main = canonicalize_triangle(*this, triangle);
    auto canon_ref  = canonicalize_triangle(triangle, *this);

    return check_interval_intersect(canon_main, canon_ref);
}
```
</details>

## Project structure
```tree
3D_triangles/
├── CMakeLists.txt
├── include
|   ├── primitives
|   |  ├── point.hpp
|   |  ├── vector.hpp
|   |  └── line.hpp
|   ├── BVH.hpp
│   ├── driver.hpp
|   └── triangle.hpp
├── src
|   ├── driver.cpp
│   └── main.cpp
└── tests
    ├── geometry_tests.cpp
    └── triangles_tests.cpp
```

## Project Creators

<div align="center">

  <a href="https://github.com/RTCupid">
    <img src="https://raw.githubusercontent.com/BulgakovDmitry/3D_triangles/main/img/A.jpeg" width="160" height="160" style="border-radius: 50%;">
  </a>
  <a href="https://github.com/BulgakovDmitry">
    <img src="https://raw.githubusercontent.com/BulgakovDmitry/3D_triangles/main/img/D.jpeg" width="160" height="160" style="border-radius: 50%;">
  </a>
  <br>
  <a href="https://github.com/RTCupid"><strong>@RTCupid, </strong></a>
  <a href="https://github.com/BulgakovDmitry"><strong>@BulgakovDmitry</strong></a>  
  <br>
</div>
