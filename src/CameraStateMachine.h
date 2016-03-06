/// <summary>
/// File Name:		CameraStateMachine.h
/// Author: 		David Azouz
/// Date Created: 	5/03/16
/// Date Modified: 	5/03/16
///----------------------------------------------------------
/// Brief: A CameraStateMachine Class that selects a camera's behaviour
/// viewed: 
/// 
/// ***EDIT***
/// - Camera	 	- David Azouz 2/02/16
/// 
/// TODO: CameraStateMachine Constructor is providing errors?
/// </summary>
/// ----------------------------------------------------------

#pragma once

#include <glm/vec4.hpp>

#include <memory>

class Camera;
class FlyCamera;

/// <summary> 
/// Enumeration to decide which method to draw our scene with.
/// </summary> TODO: GLenum?
enum E_CAMERA_MODE_STATE
{
	E_CAMERA_MODE_STATE_STATIC,
	E_CAMERA_MODE_STATE_FLYCAMERA, /* Move around scene */
	E_CAMERA_MODE_STATE_ORBIT, /* Orbit */

	E_CAMERA_MODE_STATE_COUNT,
};

class CameraStateMachine
{
public:
	CameraStateMachine(glm::vec4 a_v4Perspective);
	~CameraStateMachine();

	void Update(float fDeltaTime);
	void ChangeState(E_CAMERA_MODE_STATE eState);

	Camera* GetCurrentCamera() const { return m_cameraStatesArray[m_eCurrentState].get(); }
	E_CAMERA_MODE_STATE GetCurrentCameraMode() const { return m_eCurrentState; }

private:
	E_CAMERA_MODE_STATE m_eCurrentState;
	std::shared_ptr<Camera> m_cameraStatesArray[E_CAMERA_MODE_STATE_COUNT];
};

