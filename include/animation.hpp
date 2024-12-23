#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include <GLM/vec3.hpp>
#include <GLM/gtc/quaternion.hpp>

#include <assimp/scene.h>

// Cada osso presente no objeto ter� n keyframes que determinar�o informa��es
// relativas �s suas transforma��es ao longo do tempo.
// Entre keyframes s�o feitas interpola��es para manter a anima��o suave.

struct KeyframePosition {
	float timeStamp;
	glm::vec3 position;
	glm::vec3 scale;
};

struct KeyframeRotation {
	float timeStamp;
	glm::quat rotation;
};

struct KeyframeScale {
	float timeStamp;
	glm::vec3 scale;
};


struct AnimationChannel {
	std::string boneName;
	std::vector<KeyframePosition> keyP;
	std::vector<KeyframeRotation> keyR;
	std::vector<KeyframeScale> keyS;
};

glm::mat4 interpolateAnimationChannel(const AnimationChannel& channel, float t);

class Animation {
public:
	double duration;
	double ticksPerSecond;
	std::unordered_map<std::string, AnimationChannel> channels;

	Animation() = default;
	Animation(const aiAnimation* animation);
};

#endif