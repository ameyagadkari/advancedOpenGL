#include  "ConfigurableParameters.h"
#include "UserInput.h"
#include "../Time/Time.h"
#include "../../Game/MyGame/MyGame.h"
#include "../../Game/Gameplay/Gameobject.h"
#include "../Graphics/Scene.h"
#include "../Math/Functions.h"

namespace
{
	const float minWaveSpeed = 0.001f;
	const float maxWaveSpeed = 0.1f;

	const float minTiling = 1.0f;
	const float maxTiling = 10.0f;

	const float minWaveStrength = 0.001f;
	const float maxWaveStrength = 0.1f;

	const float minReflectivity = 0.1f;
	const float maxReflectivity = 2.0f;

	cs6610::Gameplay::GameObject* light = nullptr;
	std::vector<cyPoint3f> circlePoints;
	const uint8_t grayCode[] = { 1,3,2,6,7,5,4 };
	size_t index = 0;
	size_t circleSize = 0;
	void Initialize();
}

namespace cs6610
{
	namespace ConfigurableParameters
	{
		float waveSpeed = 0.03f;
		float tiling = 6.0f;
		float waveStrength = 0.02f;
		float reflectivity = 0.5f;
		bool autoLight = false;
	}
}

void cs6610::ConfigurableParameters::Update()
{
	if (UserInput::keys.test('o'))
		waveSpeed > minWaveSpeed ? waveSpeed -= 0.01f * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame()) : waveSpeed = minWaveSpeed;
	if (UserInput::keys.test('p'))
		waveSpeed < maxWaveSpeed ? waveSpeed += 0.01f * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame()) : waveSpeed = maxWaveSpeed;

	if (UserInput::keys.test('k'))
		tiling > minTiling ? tiling -= static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame()) : tiling = minTiling;
	if (UserInput::keys.test('l'))
		tiling < maxTiling ? tiling += static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame()) : tiling = maxTiling;

	if (UserInput::keys.test('n'))
		waveStrength > minWaveStrength ? waveStrength -= 0.01f * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame()) : waveStrength = minWaveStrength;
	if (UserInput::keys.test('m'))
		waveStrength < maxWaveStrength ? waveStrength += 0.01f * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame()) : waveStrength = maxWaveStrength;

	if (UserInput::keys.test('h'))
		reflectivity > minReflectivity ? reflectivity -= 0.3f * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame()) : reflectivity = minReflectivity;
	if (UserInput::keys.test('j'))
		reflectivity < maxReflectivity ? reflectivity += 0.3f * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame()) : reflectivity = maxReflectivity;

	if (autoLight)
	{
		if (!circleSize)Initialize();
		if (index == circleSize - 1) index = 0;
		light->SetPosition(circlePoints[index++]);
	}
}

namespace
{
	void Initialize()
	{
		light = cs6610::MyGame::mainScene->GetGameobjectByName("Light");
		{
			const float thetaScale = 0.001f;
			const float radius = 40.0f;
			const float rootTwo = sqrtf(2.0f);
			const float lightHeight = light->GetPosition().y;
			float theta = 0.0f;
			circleSize = static_cast<size_t>(1.0f / thetaScale + 1.0f);
			circlePoints.reserve(circleSize);
			for (size_t i = 0; i < circleSize; i++)
			{
				theta += 2.0f * cs6610::Math::Pi * thetaScale;
				float sinTheta = sinf(theta);
				float cosTheta = cosf(theta);
				float onePlusSineSquareTheta = 1.0f + sinTheta * sinTheta;
				float x = radius * rootTwo * cosTheta / onePlusSineSquareTheta;
				float y = 10.0f + (lightHeight - 10.0f)*fabs(sinTheta);
				float z = radius * rootTwo * cosTheta * sinTheta / onePlusSineSquareTheta;
				circlePoints.push_back(cyPoint3f(x, y, z));
			}
		}
	}
}