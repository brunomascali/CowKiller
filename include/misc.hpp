#ifndef MISC_HPP
#define MISC_HPP

float lastTime = 0.0f;

void updateDT(std::shared_ptr<float> dt) {
    float currentTime = static_cast<float>(glfwGetTime());
    *dt = currentTime - lastTime;
    lastTime = currentTime;
}

#endif