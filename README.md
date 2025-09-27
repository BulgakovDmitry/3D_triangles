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
- [3. Implementation of algorithms](#implementation-of-algorithms)
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
./geometry_tests
./triangles_tests
```

If you want a debug build, enter:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cd build
cmake --build .
./3D_triangles
```

## Introduction
This C++ project implements a program for working with triangles in three-dimensional space. Its core functionality includes robust algorithms for calculating geometric properties and detecting intersections between triangles.

The development of such geometric processing tools is highly prospective, as they form the foundational layer for a wide range of critical applications. These include collision detection in physics engines for video games and simulations, 3D modeling and computer-aided design (CAD) software, ray tracing for computer graphics, and even pathfinding in robotics. Efficient and accurate triangle manipulation is, therefore, essential for advancements in these technologically significant fields.

## Implementation of algorithms
The algorithm for checking the intersection of triangles in three-dimensional space is implemented as follows:
<details>
<summary>Click to show/hide code</summary>
  
```cpp
bool intersect(const Triangle &triangle) const {
    // check the position of the vertices of one triangle relative to another
    auto relative_positions = check_relative_positions(triangle);

    if (relative_positions == pozitive || relative_positions == negative)
        return false;

    if (relative_positions == null_sign)
        return intersect_2d(triangle); // coplanar case

    auto canon_main = canonicalize_triangle(*this, triangle);
    auto canon_ref  = canonicalize_triangle(triangle, *this);

    return check_interval_intersect (canon_main, canon_ref);
}
```
</details>

## Project structure
```tree
3D_triangles/
├── CMakeLists.txt
├── include
│   ├── geometry.hpp
│   └── triangle.hpp
├── src
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
