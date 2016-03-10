/// <summary>
/// File Name:		CameraStateMachine.cpp
/// Author: 		David Azouz
/// Date Created: 	5/03/16
/// Date Modified: 	6/03/16
///----------------------------------------------------------
/// Brief: A Camera State Class that Switches between various Camera modes
/// viewed: 
/// Camera States https://github.com/richard-stern/SpaceCowboy/blob/master/Project1/StateMachine.cpp
/// 
/// ***EDIT***
/// - Camera Modes implemented	 	- David Azouz 5/03/16
/// 
/// TODO:
/// 
/// </summary>
/// ----------------------------------------------------------

#include "CameraStateMachine.h"
#include "Camera.h"
#include "FlyCamera.h"
#include "OrbitCamera.h"
#include "TravelCamera.h"

#include <memory> //shared_ptr

CameraStateMachine::CameraStateMachine(glm::vec4 a_v4Perspective) :
	m_eCurrentState(E_CAMERA_MODE_STATE_FLYCAMERA)
{
	if (m_eCurrentState == NULL)
	{
		E_CAMERA_MODE_STATE_FLYCAMERA;
	}

	m_cameraStatesArray[E_CAMERA_MODE_STATE_STATIC] = std::make_shared<Camera>(a_v4Perspective);
	m_cameraStatesArray[E_CAMERA_MODE_STATE_FLYCAMERA] = std::make_shared<FlyCamera>(a_v4Perspective);
	m_cameraStatesArray[E_CAMERA_MODE_STATE_ORBIT] = std::make_shared<OrbitCamera>(a_v4Perspective);
	m_cameraStatesArray[E_CAMERA_MODE_STATE_TRAVEL] = std::make_shared<TravelCamera>(a_v4Perspective);

	m_cameraStatesArray[m_eCurrentState]->Enter();
}
//	m_cameraStatesArray[E_CAMERA_MODE_STATE_STATIC] = std::unique_ptr<Camera>();


CameraStateMachine::~CameraStateMachine()
{
	m_cameraStatesArray[m_eCurrentState]->Exit();
}

void CameraStateMachine::Update(float fDeltaTime)
{
	m_cameraStatesArray[m_eCurrentState]->Update(fDeltaTime);
	//GetCurrentCamera()->Update(fDeltaTime); //TODO
}

void CameraStateMachine::ChangeState(E_CAMERA_MODE_STATE eState)
{
	m_cameraStatesArray[m_eCurrentState]->Exit();
	m_eCurrentState = eState;
	m_cameraStatesArray[m_eCurrentState]->Enter();
}