#ifndef BEZIER_HPP
#define BEZIER_HPP

#include <GLM/vec4.hpp>

class Bezier {
public:
    glm::vec3 p0, p1, p2, p3;
    float duration;

    Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float d) {
        this->p0 = p0;
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->duration = d;
    }

    glm::vec3 getPoint(float t) const {
        // Mapeamento de t para [0, duration)
        float time = fmod(t, duration * 2.0f);

        // Evita que o objeto se teletransporte após chegar em t = duration
        if (time > duration) {
            float extra = time - duration;
            time = duration - extra;
        }

        time /= duration;

        glm::vec3 a = powf(1 - time, 3) * p0;
        glm::vec3 b = 3.0f * powf(1 - time, 2) * time * p1;
        glm::vec3 c = 3.0f * (1 - time) * powf(time, 2) * p2;
        glm::vec3 d = powf(time, 3) * p3;

        return a + b + c + d;
    }
};

#endif