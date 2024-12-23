#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include <GLM/vec3.hpp>
#include <GLM/gtc/quaternion.hpp>

#include <assimp/scene.h>

// Cada osso presente no objeto terá n keyframes que determinarão informações
// relativas às suas transformações ao longo do tempo.
// Entre keyframes são feitas interpolações para manter a animação suave.

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