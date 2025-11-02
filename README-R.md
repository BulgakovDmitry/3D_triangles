<div align="center">

# УРОВЕНЬ 1
  # Решение задачи пересечения треугольников на C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)

</div>

## Другие языки

1. [Русский](README-R.md)
2. [Английский](README.md)

## Содержание
- [Установка и сборка](#установка-и-сборка)
- [Требования](#требования)
- [Структура проекта](#структура-проекта)
- [Авторы проекта](#авторы-проекта)
  
[**Первый уровень:**](#уровень-1)
- [Введение (первый уровень)](#введение-первый-уровень)
- [Реализация алгоритма](#реализация-алгоритма)
- [Структуры данных для ускорения](#структуры-данных-для-ускорения)
  
[**Второй уровень:**](#уровень-2)
- [Введение (второй уровень)](#введение-второй-уровень)
- [Реализация графики](#реализация-графики)
- [Графические особенности](#графические-особенности)
- [Управление](#управление)
- [Архитектура](#архитектура)

## Установка и сборка

Чтобы установить, выполните:
```bash
git clone https://github.com/BulgakovDmitry/3D_triangles
cd 3D_triangles
```

Сборка:
```bash
cmake -S . -B build 
cmake --build build
```

Вы также можете указать, какая именно сборка вам нужна. Для этого после `cmake -S . -B build` добавьте:
- `-DCMAKE_BUILD_TYPE=Release` или `-DCMAKE_BUILD_TYPE=Debug`, если вам нужен релиз или отладка;
- `-DGRAPHICS=ON`, если вы хотите использовать графический драйвер (по умолчанию включен, при желании можно выключить)

Например:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DGRAPHICS=ON
```

Запуск программы пересечения треугольников:
```bash
./build/3D_triangles
```
Запуск графического драйвера:
```bash
./build/Graphics
```

Запуск модульных тестов:
```bash
ctest
```

Или более подробно:
```bash
ctest -L intersection
ctest -L primitives
ctest -L BVH
```

Запуск сквозных (end_to_end) тестов:
```bash
cmake --build build --target end_to_end
```

## Требования
- C++23 или новее
- CMake 3.20+
- Google Test (для тестирования)
- Graphviz (опционально, для визуализации)
- OpenGL и glad (опционально, для графического драйвера)

## Структура проекта
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

## Авторы проекта

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

## Введение (первый уровень)
Этот проект на `C++` реализует программу для работы с треугольниками в трёхмерном пространстве. Основной функционал включает надёжные алгоритмы вычисления геометрических свойств и обнаружения пересечений между треугольниками.

Разработка таких инструментов геометрической обработки крайне перспективна, поскольку они составляют базовый уровень для широкого спектра критически важных приложений. Среди них — обнаружение столкновений в физических движках для видеоигр и симуляций, 3D‑моделирование и системы автоматизированного проектирования (`CAD`), трассировка лучей для компьютерной графики и даже прокладка маршрутов в робототехнике. Эффективная и точная работа с треугольниками, следовательно, необходима для прогресса в этих технологически значимых областях.

## Реализация алгоритма
### Шаг 1: Проверка треугольника `T1` относительно плоскости `π₂`
Вычисляются три предиката `orient_3d`, чтобы определить положение вершин `T1` относительно плоскости `π₂`, заданной `T2`. Если все вершины лежат строго по одну сторону плоскости, пересечения нет. Если все три предиката равны нулю, треугольники компланарны, и задача сводится к `2D`‑тесту на пересечение. В противном случае алгоритм продолжается.

<table>
  <tr>
    <td align="center">
      <img src="img/intersect.png" width="400">
      <br>
      <em>Рис. 1. Стандартный случай пересечения треугольников по линии пересечения плоскостей. Линия пересечения плоскостей выделена зелёным, области, в которых лежат соответствующие треугольники, выделены синим и красным.</em>
    </td>
    <td align="center">
      <img src="img/intersect_vertex.png" width="400">
      <br>
      <em>Рис. 2. Пересечение треугольников в одной вершине. Линия пересечения плоскостей выделена зелёным, области, в которых лежат соответствующие треугольники, выделены синим и красным.</em>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td align="center">
      <img src="img/parrallel.png" width="400">
      <br>
      <em>Рис. 3. Параллельный случай — пересечения нет. Области, в которых лежат соответствующие треугольники, выделены синим и красным.</em>
    </td>
    <td align="center">
      <img src="img/no_intersect.png" width="400">
      <br>
      <em>Рис. 4. Пересечения нет. Красный треугольник пересекает плоскость синего в одной вершине.</em>
    </td>
  </tr>
</table>


### Шаг 2: Проверка треугольника `T2` относительно плоскости `π₁`
Аналогично, вычисляются три предиката `orient_3d`, чтобы определить положение вершин `T2` относительно плоскости `π₁` треугольника `T1`. Если все вершины по одну сторону — пересечения нет. Если все предикаты равны нулю — треугольники компланарны (`2D‑случай`). Если этот шаг пройден, оба треугольника гарантированно пересекают линию пересечения (`L`) своих плоскостей.

### Шаг 3: Канонический порядок вершин
Вершины каждого треугольника переставляются для получения канонической конфигурации. Для каждого треугольника требуется, чтобы одна вершина (P) находилась по одну сторону плоскости другого треугольника, а две другие вершины (`Q` и `R`) — по противоположную. Это гарантирует, что рёбра `P-Q` и `P-R` пересекают плоскость, задавая отрезок на прямой `L`.

### Шаг 4: Проверка перекрытия интервалов через предикаты
Перекрытие двух отрезков на прямой `L` определяется с помощью двух предикатов ориентации: `orient_3d(P₁, Q₁, P₂, Q₂) > 0` и `orient_3d(P₁, R₁, R₂, P₂) > 0`. Если оба условия истинны, отрезки перекрываются, что означает пересечение треугольников.

### Функция проверки пересечения треугольников
Алгоритм проверки пересечения треугольников в трёхмерном пространстве реализован следующим образом:
<details>
<summary>Показать/скрыть код</summary>

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

## Структуры данных для ускорения
Для ускорения проекта реализован класс BVH (иерархия ограничивающих объёмов) — двоичное дерево.
На самых нижних уровнях дерева находятся 1–3 треугольника, заключённые в ограничивающий бокс.
Для наглядной демонстрации и получения полной информации об этих боксах (в том числе для удобной отладки) реализован графический дамп, пример которого показан ниже:
<div align="center">
  <img src="img/dump_tree.png" alt="bin_tree" width="700">
</div>

<div align="center">


# УРОВЕНЬ 2
  # Визуализация пересечений треугольников на OpenGL
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)
  ![OpenGL](https://img.shields.io/badge/OpenGL-4.6+-orange?style=for-the-badge&logo=opengl)

</div>

## Введение (второй уровень)
Данный проект представляет собой интерактивную программу для визуализации треугольных полигонов в трёхмерном пространстве. Разработанный с использованием библиотеки OpenGL и фреймворка GLFW, он позволяет в реальном времени отображать, трансформировать (перемещать, вращать, масштабировать) и исследовать 3D-модели, представленные в виде сетки треугольников.

Проект служит наглядной демонстрацией работы с графическим конвейером, основами 3D-математики (матрицы преобразований, системы координат) и принципами работы современных графических API

## Реализация графики

Для графической реализации треугольников создан класс Graphics_driver. Он имеет следующую структуру
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
Как можно видеть, для модульности класса создано несколько RAII оболочек (Window, Mesh, Shader_program и Camera).

## Графические особенности

### Визуализация и рендеринг
#### Система отрисовки треугольников

- Разделение по типам: Обычные треугольники отображаются синим цветом (0.30f, 0.50f, 0.60f), пересекающиеся — красным (0.70f, 0.35f, 0.25f)
- Буферизация геометрии: Вершинные данные хранятся в VBO (Vertex Buffer Object) и управляются через VAO (Vertex Array Object)
- Глубина тестирования: Включено тестирование глубины (GL_DEPTH_TEST) для корректного отображения 3D-сцены

### Шейдерная система
#### Вершинный шейдер
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
- Преобразования: Применяет матрицы вида и проекции к вершинам
- Расчёт нормалей: Вычисляет нормали для использования во фрагментном шейдере

#### Фрагментный шейдер
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
- Модель освещения: Упрощённая модель Фонга с ambient и diffuse компонентами
- Направленный источник света: Фиксированное направление (0.5, 1.0, 0.8)
- Ambient освещение: Постоянная составляющая 20% (vec3(0.2))
- Diffuse рассеяние: Расчёт по закону Ламберта через dot product нормалей и направления света

### Система камеры

#### Матричные преобразования

- Матрица вида (View Matrix): Рассчитывается через `glm::lookAt()` на основе позиции камеры, направления взгляда и вектора "вверх"
- Матрица проекции (Projection Matrix): Перспективная проекция через `glm::perspective()` с параметрами:
  - FOV (Field of View): Динамически изменяется через зум (от 1.0° до 90.0°)
  - Соотношение сторон: Фиксированное 1000×800 (5:4)
  - Ближняя и дальняя плоскости: 0.1f и 100.0f соответственно

#### Управление камерой
- Позиционирование: Начальная позиция (0.0f, 0.0f, 3.0f)
- Система координат: Правосторонняя с мировым "верхним" вектором (0.0f, 1.0f, 0.0f)
- Углы Эйлера: Yaw (-90.0°) и Pitch (0.0°) для управления направлением взгляда

#### Параметры движения
- Скорость перемещения: 2.5f единиц в секунду
- Скорость зума: 15.0f единиц в секунду
- Чувствительность мыши: 0.1f для плавного вращения
- Ограничение pitch: ±89.0° для предотвращения "гимбального замка"

### Управление графической памятью

#### Буферные объекты
- VAO (Vertex Array Object): Хранит конфигурацию вершинных атрибутов
- VBO (Vertex Buffer Object): Содержит сырые вершинные данные
- Формат вершин: По 3 компонента float (x, y, z) на вершину

#### Шейдерные программы
- Автоматическая компиляция: Проверка статуса компиляции и линковки
- Униформа передачи: Матрицы view и projection, цвет материала `material_color`
- Очистка ресурсов: Автоматическое удаление шейдеров после линковки программы

### Особенности рендеринга
#### Основной цикл отрисовки
```cpp
while (!glfwWindowShouldClose(window_)) {
    // Обработка ввода
    process_input(delta_time);
    
    // Расчёт матриц
    glm::mat4 view = camera_.get_view_matrix();
    glm::mat4 projection = glm::perspective(/*...*/);
    
    // Очистка буферов
    glClearColor(0.75f, 0.85f, 0.90f, 0.0f);  // Светло-голубой фон
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Отрисовка треугольников
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    
    // Смена буферов
    glfwSwapBuffers(window_);
}
```

#### Обработка ошибок
- Проверка OpenGL ошибок: После критических операций через `check_GL_error()`
- Валидация шейдеров: Проверка компиляции и линковки с выводом информационных сообщений
- Проверка uniform локаций: Подтверждение корректной привязки uniform‑переменных

## Управление
### Перемещение камеры
- W/S: Вверх/вниз
- A/D: Влево/вправо
- Z/X или Page Up/Page Down: Вперёд/назад

### Вращение камеры
- Зажать левую кнопку мыши + двигать: Поворот камеры

### Зум
- Колесо мыши: Приближение/отдаление
- +/-: Аналогично колесу мыши

## Архитектура
#### Проект использует объектно-ориентированный подход с чётким разделением ответственности:

- `Graphics_driver`: Главный координатор графического конвейера
- `Camera`: Логика управления видом и проекцией
- `Mesh`: Управление буферами вершин
- `Shader_program`: Компиляция и линковка шейдеров
- `Window`: Абстракция над окном GLFW

Все компоненты используют современные практики C++ включая move‑семантику, RAII и типобезопасные перечисления.
