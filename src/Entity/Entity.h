/// <summary>
/// File Name:		Entity.h
/// Author: 		David Azouz
/// Date Created: 	18/03/16
/// Date Modified: 	8/06/16
///----------------------------------------------------------
/// Brief: An Entity Class that handles entities/ objects within our scene.
/// viewed: 
/// (Entity) Design Pattern http://gameprogrammingpatterns.com/component.html
/// ***EDIT***
/// - Created: Classes inheriting from Entity	- David Azouz 18/03/16
/// - Classes inheriting from Entity			- David Azouz /03/16 //TODO: date
/// - Classes made pure: Draw and RenderUI		- David Azouz 8/06/16
/// - Entity.cpp made obsolete					- David Azouz 8/06/16
/// 
/// TODO:
/// Physics for each entity?
/// </summary>
/// ----------------------------------------------------------

#pragma once
//#include "VertexData.h"
#include "Camera\Camera.h"
#include "Helpers.h"
//#include "Physics.h"
#include "Renderable.h"
//#include "RenderTarget.h"
//#include "MathCollision.h"

#include <gl_core_4_4.h>
//#include <glm/glm.hpp> //translate
#include <glm/vec3.hpp>
//#include <glm/mat4x4.hpp>
#include <memory>

//class Camera;
//class MathCollision;
class BoundingShape;

class Entity
{
public:
	//--------------------------------------------------------------------------------------
	// Default Constructor
	//--------------------------------------------------------------------------------------
	// The 'default' keyword is a c++11 feature and should be used instead of {}.
	// This way the class can remain aggregate and/ or trivial.
	//--------------------------------------------------------------------------------------
	Entity() : 
		//m_boundShape(std::make_shared<BoundingShape>()),
		m_pRenderable(std::make_shared<Renderable>()),
		//m_pPhysics(std::make_shared<Physics>()),
		m_m4WorldTransform(1)
	{}
	virtual ~Entity() = default;

	// Is an abstract class
	// cannot create instances of this class
	// = 0 makes it a Pure (virtual) function
	virtual bool	Create() = 0; //TODO: GLboolean?
	// not every entity must have an update
	virtual bool	Update(GLfloat deltaTime) { return true; }
	virtual GLvoid	Draw(const Camera& m_pCamState) = 0;
	// Items to be drawn to our Render Target.
	//GLvoid	DrawApp(); //TODO: create a DrawApp for each class that inherits from Entity - and remove from here
	virtual GLvoid	Destroy()	{};
	virtual GLvoid	RenderUI() = 0;

	// ----------------------------------------------------------
	// Getters and Setters
	// ----------------------------------------------------------
	const Renderable* GetRenderable() const { return m_pRenderable.get(); }
	const glm::mat4& GetTransform() const { return m_m4WorldTransform; }
	const glm::vec4& GetPosition() const { return m_m4WorldTransform[3]; }
	
	void SetTransform(const glm::mat4& a_transform) { m_m4WorldTransform = a_transform; }
	void SetPosition(const glm::vec4& a_position) { m_m4WorldTransform[3] = a_position; }
	//TODO: void SetPosition(const glm::vec4& a_position) { m_m4WorldTransform[3] = glm::translate(glm::vec3(a_position.xyz())); }
	// ----------------------------------------------------------

	/// <summary> Function Pointer TODO: what do I do with this?
	/// <param>P1: Program ID</param>
	/// </summary>
	/*Entity */ void(*EntityRender)(GLint) = NULL;

// Protected so that other classes that inherit from
// Entity can have access to certain member variables.
protected:
	// Every entity uses frustum culling.
	//BoundingShape m_boundShape; //TODO: frustum culling

	// Each class/ entity contains a randerable component 
	// which contains a Mesh, ability to render, and a Sampler/ Texture (id)
	std::shared_ptr<Renderable> m_pRenderable;
	//std::shared_ptr<Physics> m_pPhysics;

	//std::shared_ptr<MathCollision> m_pMath;
	/// ----------------------------------------------------------
	// TODO: Transform class?

	// where our entity is in the world
	glm::mat4 m_m4WorldTransform;
private:
	/// ----------------------------------------------------------
	//GLvoid PlanetCreation();
	//GLvoid RenderGeometry();
	/// ----------------------------------------------------------

};

