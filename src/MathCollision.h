#pragma once
#include "Entity\Entity.h"
#include "gl_core_4_4.h"
#include "Gizmos.h"

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <vector>
#include <memory>

class Camera;

class MathCollision
{
public:
	MathCollision();
	~MathCollision();

	bool Create();
	GLvoid Update(Camera* cam);
};

/*class BoundingShape 
{
public:
	bool Create();
	//GLvoid Update(Camera* cam);
}; */

class AABB// : public BoundingShape
{
public:
	AABB() { Reset(); }
	~AABB() {}

	void Reset();

	void Fit(const std::vector<glm::vec3>& a_v3Points);

	glm::vec3 min, max;
};

class BoundingSphere// : public BoundingShape
{
public:
	BoundingSphere();// : v3Centre(0), fRadius(0) {}
	~BoundingSphere() {}

	void Fit(const std::vector<glm::vec3>& a_v3Points);

	void GetFrustumPlanes(const glm::mat4& a_m4Tramsform, glm::vec4* a_v3Planes);

	glm::vec3	v3Centre;
	GLfloat		fRadius;

private:
	std::shared_ptr<AABB> m_pAABB;
};
