
#define GLM_SWIZZLE
#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

Camera::Camera(float fovY, float aspectRatio, float near, float far)
	: m_speed(10),
	m_fRotation(0.01f),
	m_up(0,1,0),
	m_transform(1),
	m_view(1),
	m_v3Movement(0)
{
	setPerspective(fovY, aspectRatio, near, far);
}

Camera::~Camera()
{

}

void Camera::setPerspective(float fovY, float aspectRatio, float near, float far)
{
	m_projection = glm::perspective(fovY, aspectRatio, near, far);
	m_projectionView = m_projection * m_view;
}

void Camera::setLookAtFrom(const glm::vec3& from, const glm::vec3& to)
{
	m_view = glm::lookAt(from, to, glm::vec3(0, 1, 0));
	m_transform = glm::inverse(m_view);
	m_projectionView = m_projection * m_view;
}

void Camera::update(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();

	float frameSpeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? deltaTime * m_speed * 2 : deltaTime * m_speed;	

	if (glfwGetKey(window, GLFW_KEY_KP_ADD))
	{
		m_speed += m_speed * 0.1f;
		printf("Speed: %f.\n", m_speed);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT))
	{
		m_speed -= m_speed * 0.1f;
		printf("Speed: %f.\n", m_speed);
	}

	// translate Old: if (glfwGetKey(window, 'A') == GLFW_PRESS)
	// ---------------------------------------------------------------------------------------
#pragma region Movement
	// ---------------------------------------------------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
	{
		m_transform[3].x += m_transform[1].x * frameSpeed;
		m_transform[3].z += m_transform[1].z * frameSpeed;
	}
	if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) || 
		glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN))
	{
		m_transform[3].x -= m_transform[1].x * frameSpeed;
		m_transform[3].z -= m_transform[1].z * frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT))
	{
		m_transform[3] -= m_transform[0] * frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		m_transform[3] += m_transform[0] * frameSpeed;
	}
	// Rise & Fall
	if (glfwGetKey(window, GLFW_KEY_R))
	{
		m_transform[3] += m_transform[1] * frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_F))
	{
		m_transform[3] -= m_transform[1] * frameSpeed;
	}
	// Zoom
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_W))
	{
		m_transform[3] -= m_transform[2] * frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_S))
	{
		m_transform[3] += m_transform[2] * frameSpeed;
	}
#pragma endregion

#pragma region Orbit

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_Z))
	{
		m_transform[3] -= m_transform[0] * frameSpeed;
		setLookAtFrom(m_transform[3].xyz, glm::vec3(0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_C))
	{
		m_transform[3] += m_transform[0] * frameSpeed;
		setLookAtFrom(m_transform[3].xyz, glm::vec3(0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_Q))
	{
		if (m_transform[3].x != 0 || m_transform[3].z != 0)//glm::vec3(0))
		{
			m_transform[3].xz = 0;// = glm::vec3(0, 5, 0);
			m_transform[3].y = 5;// = glm::vec3(0, 5, 0);
		}
		m_transform[3] -= m_transform[0] * frameSpeed;
		//setLookAtFrom(glm::vec3(0, 5, 0), m_transform[3].xyz);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_E))
	{
		m_transform[3] += m_transform[0] * frameSpeed;
		setLookAtFrom(glm::vec3(0, 5, 0), m_transform[3].xyz);
	}
#pragma endregion

	if (glfwGetKey(window, GLFW_KEY_X))
	{
		m_transform = glm::rotate(m_transform, m_fRotation, glm::vec3(1, 0, 1));
	}
	if (glfwGetKey(window, GLFW_KEY_V))
	{
		m_transform = glm::rotate(m_transform, m_fRotation, glm::vec3(-1, 0, -1));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		setLookAtFrom(glm::vec3(10,10,10), glm::vec3(0));
	}
	// ---------------------------------------------------------------------------------------
	/// <summary> zooming in is easy. Get the second index of the world matrix of the camera and it'll give you the LookAt vector. 
	/// This is essentially where that camera is looking at. 
	/// E.g. World[2] = lookAt vector combine that vector with your camera matrix to zoom in/out </summary> 
	// ---------------------------------------------------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_COMMA))
	{
		m_transform -= glm::rotate(m_fRotation, glm::vec3(1.0f, 0.0f, 1.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_PERIOD))
	{
		m_transform += glm::rotate(m_fRotation, glm::vec3(1.0f, 0.0f, 1.0f));
	}

	// ---------------------------------------------------------------------------------------
	// Rotation
	// ---------------------------------------------------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		m_transform = glm::rotate(m_transform, m_fRotation, glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		m_transform = glm::rotate(m_transform, m_fRotation, glm::vec3(0, -1, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_Y))
	{
		m_transform = glm::rotate(m_transform, m_fRotation, glm::vec3(-0.5f, 0, -0.5f));
		//m4pos = glm::yawPitchRoll(0.0f, mApp->fDeltaTime * m_fRotation, m_fSpeed) * m4pos;
		//m_transform = m4pos + m_transform;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_H))
	{
		m_transform = glm::rotate(m_transform, m_fRotation, glm::vec3(0.5f, 0, 0.5f));
		//m4pos = glm::yawPitchRoll(0.0f, mApp->fDeltaTime * m_fRotation, -m_fSpeed) * m4pos;
		//m_transform = m4pos + m_transform;
	}
	if (glfwGetKey(window, GLFW_KEY_T))
	{
		m_transform[3] += m_transform[2] * frameSpeed;

		/*m_v3Movement.x += m_fRotation;
		//m_transform[0].x += m_fRotation;
		//m_transform[2].z += m_fRotation;
		//m_transform = glm::rotate(m_transform, m_fRotation, m_v3Movement);

		//m4pos = glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0) * m4pos;
		//m4pos = mat4(glm::eulerAngles(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0)), 0) * m4pos;
		//m4pos = glm::mat4_cast(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0)) * m4pos;
		// ---------------------------------------------------------------------------------------
		// TRY THIS AGAIN //m4pos = glm::mat4_cast(glm::quat::(glm::highp_vec3 &glm::eulerAngles(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0))) * m4pos;
		////m4pos = glm::mat4_cast(glm::quat(glm::toQuat(glm::orientate4(glm::eulerAngles(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0)))))) * m4pos;

		m4pos *= glm::mat4_cast(qEulerRot);// *m4pos;

		//m4pos = glm::orientate4(glm::eulerAngles(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0))) * m4pos;

		// Working #1
		////m4pos = glm::yawPitchRoll(m_fSpeed, mApp->fDeltaTime * m_fRotation, 0.0f) * m4pos;

		//m4pos = glm::yawPitchRoll(mApp->fDeltaTime * m_fRotation, 0.0f, 0.0f) * m4pos;

		// ---------------------------------------------------------------------------------------
		// "Working version"
		//m4pos = glm::mat4_cast(QuatFinalOrient) * m4pos;

		//v3pos = glm::eulerAngles(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0)) * v3pos;
		//glm::quat_cast;
		m_transform = m4pos + m_transform; */
	}
	if (glfwGetKey(window, GLFW_KEY_G))
	{
		m_transform[3] -= m_transform[2] * frameSpeed;
		/* ---------------------------------------------------------------------------------------
		// TRY THIS AGAIN //
		//m4pos = glm::mat4_cast(glm::quat::(glm::highp_vec3 &glm::eulerAngles(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0))) * m4pos;
		//m4pos = glm::mat4_cast(glm::quat(glm::toQuat(glm::orientate4(glm::eulerAngles(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0)))))) * m4pos;
		//m4pos = glm::orientate4(glm::eulerAngles(glm::quat(0.0f, mApp->fDeltaTime * m_fRotation, 0, 0))) * m4pos;

		// Pals
		//m4pos += glm::yawPitchRoll(m_fSpeed, mApp->fDeltaTime * m_fRotation, 0.0f);// *m4pos; //m_transform = m4pos + m_transform;
		//m_transform += m4pos;// +m_transform; //*/
	}
	//glm::vec3 m_v3Movement = glm::vec3(0);
	// ---------------------------------------------------------------------------------------
#pragma region Random Rotations
	// ---------------------------------------------------------------------------------------
	int iYes = 0;
	if (glfwGetKey(window, GLFW_KEY_K))
	{
		m_v3Movement.z += frameSpeed;
		m_transform[iYes].z = m_v3Movement.z;
		m_transform[iYes + 1].z = m_v3Movement.z;
		m_transform[iYes + 2].z = m_v3Movement.z;
		m_transform[iYes + 3].z = m_v3Movement.z;
		//m_transform = glm::rotateX(m_transform,  ;
	}
	if (glfwGetKey(window, GLFW_KEY_L))
	{
		m_v3Movement.z -= frameSpeed;
		m_transform[iYes].z = m_v3Movement.z;
		m_transform[iYes + 1].z = m_v3Movement.z;
		m_transform[iYes + 2].z = m_v3Movement.z;
		m_transform[iYes + 3].z = m_v3Movement.z;
	}

	if (glfwGetKey(window, GLFW_KEY_Y))
	{
		m_v3Movement.x -= m_fRotation;
		m_transform = glm::rotate(m_transform, m_fRotation, m_v3Movement);
		//m_transform[iYes].w = m_v3Movement.x;
	}
	if (glfwGetKey(window, GLFW_KEY_H))
	{
		m_v3Movement.x += m_fRotation;
		m_transform = glm::rotate(m_transform, m_fRotation, m_v3Movement);
		//m_transform[iYes].w = m_v3Movement.x;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_X))
	{
		m_v3Movement.z += m_fRotation;
		m_transform = glm::rotate(m_transform, m_fRotation, m_v3Movement);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_V))
	{
		m_v3Movement.z -= m_fRotation;
		m_transform = glm::rotate(m_transform, m_fRotation, m_v3Movement);
		//m_transform[iYes].w = m_v3Movement.x;
	} //*/

	  // Wacky Waving Inflatable Arm Flaling Tube Man
	if (glfwGetKey(window, GLFW_KEY_O))
	{
		m_v3Movement += frameSpeed;
		m_transform[0].w = m_v3Movement.x;
	}
	if (glfwGetKey(window, GLFW_KEY_P))
	{
		m_v3Movement -= frameSpeed;
		m_transform[0].w = m_v3Movement.x;
	}
	// Let her rip
	if (glfwGetKey(window, GLFW_KEY_U))
	{
		m_transform[0] -= m_transform[3] * frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_J))
	{
		m_transform[0] += m_transform[3] * frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_U))
	{
		m_transform[1] -= m_transform[3] * frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_J))
	{
		m_transform[1] += m_transform[3] * frameSpeed;
	}
#pragma endregion

#pragma region Perspective

	if (glfwGetKey(window, GLFW_KEY_1))
	{
		//setPerspective(fovY, aspectRatio, near, far);
		m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
		m_projectionView = m_projection * m_view;
	}
	if (glfwGetKey(window, GLFW_KEY_2))
	{
		//setPerspective(fovY, aspectRatio, near, far);
		m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 10.f, 15.f);
		m_projectionView = m_projection * m_view;
	}
	if (glfwGetKey(window, GLFW_KEY_3))
	{
		//setPerspective(fovY, aspectRatio, near, far);
		m_projection = glm::perspective(glm::pi<float>() * 0.75f, 16 / 9.f, 0.1f, 1000.f);
		m_projectionView = m_projection * m_view;
	}
	if (glfwGetKey(window, GLFW_KEY_4))
	{
		//setPerspective(fovY, aspectRatio, near, far);
		m_projection = glm::ortho(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
		m_projectionView = m_projection * m_view;
	}
#pragma endregion

	// check for rotation
	static bool sbMouseButtonDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) || glfwGetKey(window, GLFW_KEY_M))
	{
		static double siPrevMouseX = 0;
		static double siPrevMouseY = 0;

		if (sbMouseButtonDown == false)
		{
			sbMouseButtonDown = true;
			glfwGetCursorPos(window,&siPrevMouseX,&siPrevMouseY);
		}

		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(window,&mouseX,&mouseY);

		double iDeltaX = mouseX - siPrevMouseX;
		double iDeltaY = mouseY - siPrevMouseY;

		siPrevMouseX = mouseX;
		siPrevMouseY = mouseY;

		glm::mat4 mMat;
		
		// pitch
		if (iDeltaY != 0)
		{
			mMat = glm::axisAngleMatrix(m_transform[0].xyz(), (float)-iDeltaY / 150.0f);
			m_transform[0] = mMat * m_transform[0];
			m_transform[1] = mMat * m_transform[1];
			m_transform[2] = mMat * m_transform[2];
		}

		// yaw
		if (iDeltaX != 0)
		{
			mMat = glm::axisAngleMatrix( m_up, (float)-iDeltaX / 150.0f );
			m_transform[0] = mMat * m_transform[0];
			m_transform[1] = mMat * m_transform[1];
			m_transform[2] = mMat * m_transform[2];
		}
	}
	else
	{
		sbMouseButtonDown = false;
	}

	m_view = glm::inverse(m_transform);
	m_projectionView = m_projection * m_view;
}

glm::vec3 Camera::screenPositionToDirection(float x, float y) const {
	
	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projection[0][0];
	screenPos.y /= m_projection[1][1];

	return glm::normalize(m_transform * glm::vec4(screenPos, 0)).xyz();
}

glm::vec3 Camera::pickAgainstPlane(float x, float y, const glm::vec4& plane) const {

	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projection[0][0];
	screenPos.y /= m_projection[1][1];

	glm::vec3 dir = glm::normalize(m_transform * glm::vec4(screenPos, 0)).xyz();

	float d = (plane.w - glm::dot(m_transform[3].xyz(), plane.xyz()) / glm::dot(dir, plane.xyz()));

	return m_transform[3].xyz() + dir * d;
}