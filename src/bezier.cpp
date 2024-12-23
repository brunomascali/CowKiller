//Bibliotecas externas
#include <GLM/vec4.hpp>
#include <cmath>
//Bibliotecas personalizadas
#include "../include/bezier.hpp"


#pragma once

Bezier::Bezier(const glm::vec4& start,
    const glm::vec4& control1,
    const glm::vec4& control2,
    const glm::vec4& end)
    : p0(start), p1(control1), p2(control2), p3(end) {
    p0.w = 1.0f;
    p1.w = 1.0f;
    p2.w = 1.0f;
    p3.w = 1.0f;
}

// Cubic Bezier curve point calculation
glm::vec4 Bezier::getPoint(float t) const {
    t = std::fmax(0.0f, std::fmin(1.0f, t));
    //Bernstein polynomials of degree 3
    float b03 = pow((1 - t), 3);
    float b13 = (3 * t) * pow(1 - t, 2);
    float b23 = 3 * t * t * (1 - t);
    float b33 = pow(t, 3);
    return p0 * b03 + b13 * p1 + b23 * p2 + b33 * p3;
}
