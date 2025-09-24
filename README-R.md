<div align="center">

  # Решение задачи о пересечении треугольников на C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)
  
</div>

## Другие языки

1. [Русский](/README-R.md)
2. [Английский](/README.md)

## Содержание
- [1. Установка и сборка](#установка-и-сборка)
- [2. Введение](#введение)
- [3. Реализация алгоритмов](#реализация-алгоритмов)
- [4. Структура проекта](#структура-проекта)
- [5. Создатели проекта](#создатели-проекта)

## Установка и сборка:

Чтобы установить, скомпилировать и запустить проект, выполните:
```bash
git clone https://github.com/BulgakovDmitry/3D_triangles
cd 3D_triangles
cmake -S . -B build
cd build
cmake --build .
./3D_triangles
```

Чтобы запустить тесты, выполните:
```cpp
./geometry_tests
./triangles_tests
```

Если вам нужна отладочная сборка (debug), выполните:
```cpp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cd build
cmake --build .
./3D_triangles
```

## Введение
Этот проект на C++ реализует программу для работы с треугольниками в трехмерном пространстве. Его основная функциональность включает надежные алгоритмы для вычисления геометрических свойств и обнаружения пересечений между треугольниками.

Разработка подобных инструментов для геометрической обработки является высоко перспективной, поскольку они формируют базовый уровень для широкого спектра важных приложений. К ним относятся обнаружение столкновений в физических движках для видеоигр и симуляций, 3D-моделирование и программное обеспечение для автоматизированного проектирования (CAD), трассировка лучей в компьютерной графике и даже поиск пути в робототехнике. Следовательно, эффективная и точная работа с треугольниками необходима для прогресса в этих технологически значимых областях.

## Реализация алгоритмов
Алгоритм проверки пересечения треугольников в трехмерном пространстве реализован следующим образом:

<details>
<summary>Click to show/hide code</summary>
  
```cpp
bool Triangle::intersect(const Triangle &triangle) const {
    auto   vertices_2     = triangle.get_vertices();

    Vector fst_vectors[3] = {
        {vertices_[0], vertices_[1]}, {vertices_[1], vertices_[2]}, {vertices_[2], vertices_[0]}};

    Vector scd_vectors[3]   = {{vertices_2[0], vertices_2[1]},
                               {vertices_2[1], vertices_2[2]},
                               {vertices_2[2], vertices_2[0]}};

    auto   normal_fst_trngl = vector_product(fst_vectors[0], fst_vectors[1]);

    if (!check_scalar_productions(normal_fst_trngl, scd_vectors))
        return false;

    auto normal_scd_trngl = vector_product(scd_vectors[0], scd_vectors[1]);

    if (!check_scalar_productions(normal_scd_trngl, fst_vectors))
        return false;

    return true;
}
```
</details>

## Структура проекта
```txt
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

## Создатели проекта

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



