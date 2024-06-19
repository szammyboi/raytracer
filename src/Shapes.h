#pragma once

#include "Primitives.h"

#include <vector>

class Shape : public Object
{
public:
    Shape() = default;
    ~Shape() = default;

    void RenderEditUI() = 0;
public:
    std::vector<Primitive*> m_Primitives;
};


class Pyramid : public Shape
{
public:
    Pyramid();
    ~Pyramid() = default;

    void RenderEditUI() override;
};