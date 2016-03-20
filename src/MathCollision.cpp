#include "MathCollision.h"
#include "Camera\Camera.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;


MathCollision::MathCollision()
{
}


MathCollision::~MathCollision()
{
}

bool MathCollision::Create()
{
	m_pMath = std::make_shared<MathCollision>();
	return false;
}

GLvoid MathCollision::Update(Camera* cam)
{
	///----------------------------------------------------------
	BoundingSphere bound;
	BoundingSphere sphere;
	sphere.v3Centre = vec3(0, cosf((GLfloat)glfwGetTime()) + 1, 0);
	sphere.fRadius = 0.5f;

	vec4 v4FillColor(1, 0, 1, 1);
	// a plane that faces up
	// offset from (0, 0, 0) by 1
	glm::vec4 plane(0, 1, 0, -1);

	//ax + by + cz + d = 0
	// 0 + 1 + 0 + -1 = 0

	GLfloat d = glm::dot(vec3(plane), sphere.v3Centre) + plane.w;

	GLushort usGrid = 8;
	Gizmos::addSphere(sphere.v3Centre, sphere.fRadius, usGrid, usGrid, v4FillColor);

	vec4 v4PlaneColor(1, 1, 0, 1);
	if (d > sphere.fRadius)
	{
		v4PlaneColor = vec4(0, 1, 1, 1);
		//printf("Front\n");
	}
	else if (d < -sphere.fRadius)
	{
		v4PlaneColor = vec4(1, 0, 0, 1);
		//printf("Back\n");
	}
	else
	{
		//printf("On the plane\n");
	}

	Gizmos::addTri(vec3(4, 1, 4), vec3(-4, 1, -4), vec3(-4, 1, 4), v4PlaneColor);
	Gizmos::addTri(vec3(4, 1, 4), vec3(4, 1, -4), vec3(-4, 1, -4), v4PlaneColor);

	vec4 v4Planes[6];
	bound.GetFrustumPlanes(cam->getProjectionView(), v4Planes);

	for (GLushort s = 0; s < 6; s++)
	{
		float d = glm::dot(vec3(v4Planes[s]), sphere.v3Centre) + v4Planes[s].w;

		if (d < -sphere.fRadius)
		{
			//printf("Behind, don't render it!\n");
			glClearColor(0.25f, 1, 0.25f, 1);
			break;
		}
		else if (d < sphere.fRadius)
		{
			//printf("Touching, we still need to render it!\n");
		}
		else
		{
			//printf("Front, fully visible so render it!\n");
			glClearColor(0.25f, 0.25f, 0.25f, 1);
		}
	}
}

#pragma region AABB

void AABB::Reset()
{
	min.x = min.y = min.z = 1e37f;
	max.x = max.y = max.z = -1e37f;
}

void AABB::Fit(const std::vector<glm::vec3>& a_v3Points)
{
	for (auto& p : a_v3Points)
	{
		if (p.x < min.x)
		{
			min.x = p.x;
		}
		if (p.y < min.y)
		{
			min.y = p.y;
		}
		if (p.z < min.z)
		{
			min.z = p.z;
		}
		if (p.x < min.x)
		{
			max.x = p.x;
		}
		if (p.y < min.y)
		{
			max.y = p.y;
		}
		if (p.z < min.z)
		{
			max.z = p.z;
		}
	}
}

#pragma endregion

#pragma region BoundingSphere

void BoundingSphere::Fit(const std::vector<glm::vec3>& a_v3Points)
{
	glm::vec3 min(1e37f), max(-1e37f);

	for (auto& p : a_v3Points)
	{
		if (p.x < min.x)
		{
			min.x = p.x;
		}
		if (p.y < min.y)
		{
			min.y = p.y;
		}
		if (p.z < min.z)
		{
			min.z = p.z;
		}
		if (p.x < min.x)
		{
			max.x = p.x;
		}
		if (p.y < min.y)
		{
			max.y = p.y;
		}
		if (p.z < min.z)
		{
			max.z = p.z;
		}
	}

	v3Centre = (min + max) * 0.5f;
	fRadius = glm::distance(min, v3Centre);
}

void BoundingSphere::GetFrustumPlanes(const glm::mat4& a_m4Tramsform, glm::vec4* a_v3Planes)
{
	// right side
	a_v3Planes[0] = glm::vec4(	a_m4Tramsform[0][3] - a_m4Tramsform[0][0],
								a_m4Tramsform[1][3] - a_m4Tramsform[1][0],
								a_m4Tramsform[2][3] - a_m4Tramsform[2][0],
								a_m4Tramsform[3][3] - a_m4Tramsform[3][0]);
	// left side
	a_v3Planes[1] = glm::vec4(	a_m4Tramsform[0][3] + a_m4Tramsform[0][0],
								a_m4Tramsform[1][3] + a_m4Tramsform[1][0],
								a_m4Tramsform[2][3] + a_m4Tramsform[2][0],
								a_m4Tramsform[3][3] + a_m4Tramsform[3][0]);
	// top
	a_v3Planes[2] = glm::vec4(	a_m4Tramsform[0][3] - a_m4Tramsform[0][1],
								a_m4Tramsform[1][3] - a_m4Tramsform[1][1],
								a_m4Tramsform[2][3] - a_m4Tramsform[2][1],
								a_m4Tramsform[3][3] - a_m4Tramsform[3][1]);
	// bottom
	a_v3Planes[3] = glm::vec4(	a_m4Tramsform[0][3] + a_m4Tramsform[0][1],
								a_m4Tramsform[1][3] + a_m4Tramsform[1][1],
								a_m4Tramsform[2][3] + a_m4Tramsform[2][1],
								a_m4Tramsform[3][3] + a_m4Tramsform[3][1]);
	// far
	a_v3Planes[4] = glm::vec4(	a_m4Tramsform[0][3] - a_m4Tramsform[0][2],
								a_m4Tramsform[1][3] - a_m4Tramsform[1][2],
								a_m4Tramsform[2][3] - a_m4Tramsform[2][2],
								a_m4Tramsform[3][3] - a_m4Tramsform[3][2]);
	// near
	a_v3Planes[5] = glm::vec4(	a_m4Tramsform[0][3] - a_m4Tramsform[0][2],
								a_m4Tramsform[1][3] - a_m4Tramsform[1][2],
								a_m4Tramsform[2][3] - a_m4Tramsform[2][2],
								a_m4Tramsform[3][3] - a_m4Tramsform[3][2]);
	for (GLint i = 0; i < 6; i++)
	{
		//a_v3Planes[i] = glm::normalize(a_v3Planes[i]);
		GLfloat fLen = glm::length(glm::vec3(a_v3Planes[i]));
		a_v3Planes[i] /= fLen;
	}
}

#pragma endregion