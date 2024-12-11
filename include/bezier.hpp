//Bibliotecas externas
#include <GLM/vec4.hpp>


#pragma once

class Bezier {


public:
    glm::vec4 p0, p1, p2, p3;

    // Construtor
    Bezier(const glm::vec4& start,
        const glm::vec4& control1,
        const glm::vec4& control2,
        const glm::vec4& end);

    glm::vec4 getPoint(float t) const;

};