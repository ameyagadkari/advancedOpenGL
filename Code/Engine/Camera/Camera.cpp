#include "Camera.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Time/Time.h"

float cs6610::Camera::Camera::ms_aspectRatio = 0.0f;

cs6610::Camera::Camera::Camera(
	cyPoint3f i_position,
	cyPoint3f i_eularAngles,
	float i_fieldOfView,
	float i_nearPlaneDistance,
	float i_farPlaneDistance,
	float i_topPlaneDistance,
	float i_bottomPlaneDistance,
	float i_leftPlaneDistance,
	float i_rightPlaneDistance)
	:
	m_position(i_position),
	m_eularAngles(i_eularAngles),
	m_fieldOfView(i_fieldOfView),
	m_nearPlaneDistance(i_nearPlaneDistance),
	m_farPlaneDistance(i_farPlaneDistance),
	m_topPlaneDistance(i_topPlaneDistance),
	m_bottomPlaneDistance(i_bottomPlaneDistance),
	m_leftPlaneDistance(i_leftPlaneDistance),
	m_rightPlaneDistance(i_rightPlaneDistance)
{
	CalculateOrthographicProjectionMatrix();
	UpdateLocalCameraAxes();
}

cs6610::Camera::Camera::~Camera() {}

#pragma region Gets
cyPoint3f cs6610::Camera::Camera::GetPosition()const
{
	return m_position;
}
cyPoint3f cs6610::Camera::Camera::GetEularAngles() const
{
	return m_eularAngles;
}
float cs6610::Camera::Camera::GetFieldOfView()const
{
	return m_fieldOfView;
}
float cs6610::Camera::Camera::GetNearPlaneDistance()const
{
	return m_nearPlaneDistance;
}
float cs6610::Camera::Camera::GetFarPlaneDistance()const
{
	return m_farPlaneDistance;
}
cyMatrix4f cs6610::Camera::Camera::GetOrthographicProjectionMatrix()const
{
	return m_orthographicProjection;
}
cyMatrix4f cs6610::Camera::Camera::GetPerspectiveProjectionMatrix()const
{
	return cyMatrix4f::MatrixPerspective(m_fieldOfView, ms_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance);
}
#pragma endregion

#pragma region Sets
void cs6610::Camera::Camera::SetPosition(cyPoint3f i_position)
{
	m_position = i_position;
}
#pragma endregion

void cs6610::Camera::Camera::UpdateCurrentCameraPosition()
{
	cyPoint3f localOffset = cyPoint3f(0.0f);

	//localOffset.z += zOffset;

	if (UserInput::keys.test('w'))
		localOffset += m_localCameraAxis.m_front;
	if (UserInput::keys.test('s'))
		localOffset -= m_localCameraAxis.m_front;
	if (UserInput::keys.test('d'))
		localOffset += m_localCameraAxis.m_right;
	if (UserInput::keys.test('a'))
		localOffset -= m_localCameraAxis.m_right;

	const float speed_unitsPerSecond = 10.0f;
	const float offsetModifier = speed_unitsPerSecond * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame());
	localOffset *= offsetModifier;
	m_position += localOffset;
}
void cs6610::Camera::Camera::UpdateCurrentCameraOrientation(bool constrainPitch)
{
	cyPoint3f localOffset = cyPoint3f(0.0f);

	//localOffset.y += xOffset;
	//localOffset.x += yOffset;

	if (UserInput::keys.test('h'))
		localOffset.y += 1.0f;
	if (UserInput::keys.test('f'))
		localOffset.y -= 1.0f;
	if (UserInput::keys.test('g'))
		localOffset.x += 1.0f;
	if (UserInput::keys.test('t'))
		localOffset.x -= 1.0f;

	const float speed_unitsPerSecond = 10.0f;
	const float offsetModifier = speed_unitsPerSecond * static_cast<float>(Time::GetElapsedTimeDuringPreviousFrame());
	localOffset *= offsetModifier;
	m_eularAngles += localOffset;

	/*if (constrainPitch)
	{
		if (m_eularAngles.x > 89.0f)
		{
			m_eularAngles.x = 89.0f;
		}
		if (m_eularAngles.x < -89.0f)
		{
			m_eularAngles.x = -89.0f;
		}
	}*/
	UpdateLocalCameraAxes();
}

cyMatrix4f cs6610::Camera::Camera::GetViewMatrix() const
{
	return cyMatrix4f::MatrixView(m_position, m_position + m_localCameraAxis.m_front, const_cast<cyPoint3f&> (m_localCameraAxis.m_up));
}

void cs6610::Camera::Camera::UpdateLocalCameraAxes()
{
	m_localCameraAxis.m_front.z = -(cosf(Math::ConvertDegreesToRadians(m_eularAngles.y)) * cosf(Math::ConvertDegreesToRadians(m_eularAngles.x)));
	m_localCameraAxis.m_front.y = -sinf(Math::ConvertDegreesToRadians(m_eularAngles.x));
	m_localCameraAxis.m_front.x = sinf(Math::ConvertDegreesToRadians(m_eularAngles.y)) * cosf(Math::ConvertDegreesToRadians(m_eularAngles.x));

	m_localCameraAxis.m_front.Normalize();
	m_localCameraAxis.m_right = (m_localCameraAxis.m_front.Cross(cyPoint3f(0.0f, 1.0f, 0.0f))).GetNormalized();
	m_localCameraAxis.m_up = (m_localCameraAxis.m_right.Cross(m_localCameraAxis.m_front)).GetNormalized();
}

void cs6610::Camera::Camera::CalculateOrthographicProjectionMatrix()
{
	cyMatrix4f scale = cyMatrix4f::MatrixScale(
		(2.0f / m_rightPlaneDistance - m_leftPlaneDistance),
		(2.0f / m_topPlaneDistance - m_bottomPlaneDistance),
		(2.0f / m_farPlaneDistance - m_nearPlaneDistance));

	cyMatrix4f trans = cyMatrix4f::MatrixTrans(cyPoint3f(
		(m_leftPlaneDistance + m_rightPlaneDistance)*-0.5f,
		(m_topPlaneDistance + m_bottomPlaneDistance)*-0.5f,
		(m_farPlaneDistance + m_nearPlaneDistance)*-0.5f));

	m_orthographicProjection = scale*trans;
}
