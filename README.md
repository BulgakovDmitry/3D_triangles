<div align="center">

# LEVEL 1
  # Solving the Triangle Intersection Problem in C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)

</div>

## Other languages

1. [Русский](/README-R.md)
2. [English](/README.md)

## Contents
- [Installation and Build](#installation-and-build)
- [Requirements](#requirements)
- [Project Structure](#project-structure)
- [Project Authors](#project-creators)
  
[**Level One:**](#level-1)
- [Introduction (Level 1)](#introduction-first-level)
- [Algorithm Implementation](#implementation-of-algorithm)
- [Acceleration Data Structures](#data-structure-for-acceleration)
  
[**Level Two:**](#level-2)
- [Introduction (Level 2)](#introduction-second-level)
- [Graphics Implementation](#implementation-of-graphics)
- [Graphics Features](#graphic-features)
- [Controls](#control)
- [Architecture](#architecture)

## Installation and Build <a id="installation-and-build"></a>

To install:
```bash
git clone https://github.com/BulgakovDmitry/3D_triangles
cd 3D_triangles
```

Build:
```bash
cmake -S . -B build 
cmake --build build
```

You can also specify the desired build configuration. After `cmake -S . -B build`, add:
- `-DCMAKE_BUILD_TYPE=Release` or `-DCMAKE_BUILD_TYPE=Debug` for release or debug;
- `-DGRAPHICS=ON` if you want to use the graphics driver (enabled by default; you can turn it off if needed).

Example:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DGRAPHICS=ON
```

Run the triangle‑intersection program:
```bash
./build/3D_triangles
```
Run the graphics driver:
```bash
./build/Graphics
```

Run unit tests:
```bash
ctest
```

Or more specifically:
```bash
ctest -L intersection
ctest -L primitives
ctest -L BVH
```

Run end_to_end tests:
```bash
cmake --build build --target end_to_end
```

## Requirements <a id="requirements"></a>
- C++23 or newer
- CMake 3.20+
- Google Test (for testing)
- Graphviz (optional, for visualization)
- OpenGL and glad (optional, for the graphics driver)

## Project Structure <a id="project-structure"></a>
```tree
3D_triangles/
├── CMakeLists.txt
├── include
|   ├── BVH
|   |  ├── AABB.hpp
|   |  ├── BVH.hpp
|   |  └── node.hpp
|   ├── primitives
|   |  ├── point.hpp
|   |  ├── vector.hpp
|   |  ├── triangle
|   |  └── line.hpp
|   ├── common
|   |  └── cmp.hpp
|   ├── graphics
|   |  ├── camera.hpp
|   |  ├── glfw_guard.hpp.hpp
|   |  ├── mesh.hpp
|   |  ├── graphics_driver.hpp.hpp
|   |  ├── shader_program.hpp
|   |  ├── shaders.hpp
|   |  ├── utils.hpp
|   |  └── window.hpp
|   ├── intersection
|   |  ├── point_to_segment.hpp
|   |  ├── point_to_triangle.hpp
|   |  ├── segment_to_segment.hpp
|   |  ├── triangle_to_triangle_2d.hpp
|   |  └── triangle_to_triangle.hpp
│   └── driver.hpp
├── src
|   ├── driver.cpp
|   ├── BVH.cpp
│   └── main.cpp
├── dump
|   ├── graph_dump.gv
│   └── graph_dump.png
└── tests
    ├── CMakeLists.txt
    ├── end_to_end/
    |   └──...
    ├── BVH/
    |   └──...
    ├── primitives/
    |   └──...
    └── intersection/
        └──...
```

## Project Authors <a id="project-creators"></a>

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

## Introduction (Level 1) <a id="introduction-first-level"></a>
This C++ project implements a program for working with triangles in three-dimensional space. The core functionality includes robust algorithms for computing geometric properties and detecting intersections between triangles.

Developing such geometric processing tools is highly valuable because they form a foundational layer for a wide range of critical applications — including collision detection in physics engines for video games and simulations, 3D modeling and computer‑aided design (CAD) systems, ray tracing for computer graphics, and even pathfinding in robotics. Efficient and accurate triangle handling is therefore essential for progress in these technologically significant areas.

## Algorithm Implementation <a id="implementation-of-algorithm"></a>
### Step 1: Check triangle `T1` with respect to plane `π₂`
Compute three `orient_3d` predicates to determine the positions of the vertices of `T1` relative to the plane `π₂` defined by `T2`. If all vertices lie strictly on one side of the plane, there is no intersection. If all three predicates are zero, the triangles are coplanar, and the problem reduces to a `2D` intersection test. Otherwise, the algorithm proceeds.

<table>
  <tr>
    <td align="center">
      <img src="img/intersect.png" width="400">
      <br>
      <em>Fig. 1. Standard case: triangles intersect along the line of intersection of the planes. The line of intersection is shown in green; the regions containing the corresponding triangles are shown in blue and red.</em>
    </td>
    <td align="center">
      <img src="img/intersect_vertex.png" width="400">
      <br>
      <em>Fig. 2. Triangles intersect at a single vertex. The line of intersection is shown in green; the regions containing the corresponding triangles are shown in blue and red.</em>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td align="center">
      <img src="img/parrallel.png" width="400">
      <br>
      <em>Fig. 3. Parallel case — no intersection. The regions containing the corresponding triangles are shown in blue and red.</em>
    </td>
    <td align="center">
      <img src="img/no_intersect.png" width="400">
      <br>
      <em>Fig. 4. No intersection. The red triangle intersects the plane of the blue triangle at a single vertex.</em>
    </td>
  </tr>
</table>


### Step 2: Check triangle `T2` with respect to plane `π₁`
Similarly, compute three `orient_3d` predicates to determine the positions of the vertices of `T2` relative to the plane `π₁` of triangle `T1`. If all vertices are on one side, there is no intersection. If all predicates are zero, the triangles are coplanar (the `2D` case). If this step passes, both triangles are guaranteed to intersect the line `L` where their planes meet.

### Step 3: Canonical vertex ordering
Reorder the vertices of each triangle to obtain a canonical configuration. For each triangle, one vertex (`P`) must lie on one side of the other triangle’s plane, and the other two vertices (`Q` and `R`) on the opposite side. This guarantees that the edges `P-Q` and `P-R` intersect the plane, defining a segment on the line `L`.

### Step 4: Interval overlap via predicates
The overlap of the two segments on line `L` is determined using two orientation predicates: `orient_3d(P₁, Q₁, P₂, Q₂) > 0` and `orient_3d(P₁, R₁, R₂, P₂) > 0`. If both conditions are true, the segments overlap, which means the triangles intersect.

### Triangle intersection check function
The algorithm for checking triangle–triangle intersection in three-dimensional space is implemented as follows:
<details>
<summary>Show/Hide code</summary>

```cpp
inline bool intersect(const Triangle &first, const Triangle &second) {
    if (first.get_type() == TypeTriangle::point)
        return point_inside_triangle(second, first.get_vertices()[0]);
    if (second.get_type() == TypeTriangle::point)
        return point_inside_triangle(first, second.get_vertices()[0]);
    if (first.get_type() == TypeTriangle::interval)
        return segment_intersect_triangle(/*triangle=*/second, /*interval=*/first);
    if (second.get_type() == TypeTriangle::interval)
        return segment_intersect_triangle(/*triangle=*/first, /*interval=*/second);

    Sign relative_positions = check_relative_positions(first, second);

    if (relative_positions == Sign::pozitive || relative_positions == Sign::negative)
        return false;

    if (relative_positions == Sign::common_plane)

        return intersection_2d::intersect_2d(first, second); // 2d case

    if (relative_positions == Sign::common_vertice_other_poz_or_neg)
        return intersect_one_vertice_in_plane(first, second);

    auto canon_main = canonicalize_triangle(first, second);
    auto canon_ref = canonicalize_triangle(second, first);

    return check_segments_intersect(canon_main, canon_ref);
}
```
</details>

## Acceleration Data Structures <a id="data-structure-for-acceleration"></a>
To speed up computations, a BVH (Bounding Volume Hierarchy) — a binary tree — is implemented.
At the lowest levels of the tree there are 1–3 triangles enclosed in bounding boxes.
For a visual demonstration and to obtain complete information about these boxes (including convenient debugging), a graphical dump is implemented; an example is shown below:
<div align="center">
  <img src="img/dump_tree.png" alt="bin_tree" width="700">
</div>

<div align="center">

# LEVEL 2
  # Visualizing Triangle Intersections with OpenGL
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)
  ![OpenGL](https://img.shields.io/badge/OpenGL-4.6+-orange?style=for-the-badge&logo=opengl)

</div>

## Introduction (Level 2) <a id="introduction-second-level"></a>
This project is an interactive application for visualizing triangular polygons in three‑dimensional space. Built with OpenGL and GLFW, it allows real‑time display, transformation (translation, rotation, scaling), and exploration of 3D models represented as triangle meshes.

The project provides a clear demonstration of working with the graphics pipeline, basic 3D mathematics (transformation matrices, coordinate systems), and the principles of modern graphics APIs.

## Graphics Implementation <a id="implementation-of-graphics"></a>

For the graphical rendering of triangles, a `Graphics_driver` class is created. Its structure is as follows:
```cpp
class Graphics_driver {
  private:
    Window window_;

    Mesh blue_mesh_;
    Mesh red_mesh_;

    Shader_program shader_;

    Camera camera_;

    std::vector<float> vec1_;
    std::vector<float> vec2_;
...
```
As you can see, several RAII wrappers (Window, Mesh, Shader_program, and Camera) are used to keep the class modular.

## Graphics Features <a id="graphic-features"></a>

### Visualization and Rendering
#### Triangle rendering system

- Type‑based separation: Regular triangles are rendered in blue (0.30f, 0.50f, 0.60f); intersecting triangles — in red (0.70f, 0.35f, 0.25f).
- Geometry buffering: Vertex data is stored in a VBO (Vertex Buffer Object) and managed via a VAO (Vertex Array Object).
- Depth testing: GL_DEPTH_TEST is enabled for correct 3D rendering.

### Shader System
#### Vertex shader
```cpp
#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 normal;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    normal = normalize(aPos);
}
```
- Transformations: Applies view and projection matrices to vertices.
- Normal calculation: Computes normals for use in the fragment shader.

#### Fragment shader
```cpp
#version 460 core
in vec3 normal;
out vec4 frag_color;

uniform vec3 material_color;

void main() {
    vec3 light_direction = normalize(vec3(0.5, 1.0, 0.8));
    
    vec3 ambient = vec3(0.2);
    float diffuse = max(dot(normalize(normal), light_direction), 0.0);
    
    vec3 result = (ambient + diffuse) * material_color;
    frag_color = vec4(result, 1.0);
}
```
- Lighting model: Simplified Phong model with ambient and diffuse components.
- Directional light: Fixed direction (0.5, 1.0, 0.8).
- Ambient lighting: Constant 20% (vec3(0.2)).
- Diffuse scattering: Lambertian shading via the dot product of the normal and light direction.

### Camera System

#### Matrix transformations

- View matrix: Computed via `glm::lookAt()` based on camera position, look direction, and the world‑up vector.
- Projection matrix: Perspective projection via `glm::perspective()` with parameters:
  - FOV (Field of View): Dynamically adjusted via zoom (from 1.0° to 90.0°).
  - Aspect ratio: Fixed at 1000×800 (5:4).
  - Near and far planes: 0.1f and 100.0f respectively.

#### Camera controls
- Position: Initial position (0.0f, 0.0f, 3.0f).
- Coordinate system: Right‑handed with world “up” vector (0.0f, 1.0f, 0.0f).
- Euler angles: Yaw (‑90.0°) and Pitch (0.0°) to control the view direction.

#### Movement parameters
- Movement speed: 2.5f units per second.
- Zoom speed: 15.0f units per second.
- Mouse sensitivity: 0.1f for smooth rotation.
- Pitch clamp: ±89.0° to prevent gimbal lock.

### Graphics Memory Management

#### Buffer objects
- VAO (Vertex Array Object): Stores vertex attribute configurations.
- VBO (Vertex Buffer Object): Contains raw vertex data.
- Vertex format: 3 float components per vertex (x, y, z).

#### Shader programs
- Automatic compilation: Checks compilation and linkage status.
- Uniform upload: View and projection matrices; `material_color`.
- Resource cleanup: Automatic deletion of shaders after program linkage.

### Rendering Details
#### Main render loop
```cpp
while (!glfwWindowShouldClose(window_)) {
    // Input processing
    process_input(delta_time);
    
    // Matrix calculations
    glm::mat4 view = camera_.get_view_matrix();
    glm::mat4 projection = glm::perspective(/*...*/);
    
    // Clear buffers
    glClearColor(0.75f, 0.85f, 0.90f, 0.0f);  // Light‑blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw triangles
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    
    // Swap buffers
    glfwSwapBuffers(window_);
}
```

#### Error handling
- OpenGL error checks: After critical operations via `check_GL_error()`.
- Shader validation: Compilation and linkage checks with informative messages.
- Uniform locations: Verifies correct binding of uniform variables.

## Controls <a id="control"></a>
### Camera movement
- W/S: Up/Down
- A/D: Left/Right
- Z/X or Page Up/Page Down: Forward/Backward

### Camera rotation
- Hold left mouse button + move: Rotate camera

### Zoom
- Mouse wheel: Zoom in/out
- +/-: Same as the mouse wheel

## Architecture <a id="architecture"></a>
#### The project follows an object‑oriented approach with clear separation of concerns:

- `Graphics_driver`: Main coordinator of the graphics pipeline.
- `Camera`: View and projection logic.
- `Mesh`: Vertex buffer management.
- `Shader_program`: Shader compilation and linkage.
- `Window`: Abstraction over the GLFW window.

All components use modern C++ practices including move semantics, RAII, and type‑safe enumerations.
