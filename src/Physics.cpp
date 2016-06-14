#define GLM_SWIZZLE

#include "Physics.h"
#include "Camera\Camera.h"
#include "Render.h"

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Gizmos.h"

#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <PxPhysicsAPI.h>
#include <PxScene.h>

using namespace physx; 

#define Assert(val) if (val){}else{ *((char*)0) = 0;}
#define ArrayCount(val) (sizeof(val)/sizeof(val[0]))

using glm::vec3;
using glm::vec4;
using glm::mat4;

/*bool Physics::startup()
{
    if (Application::startup() == false)
    {
        return false;
    }
	
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    Gizmos::create();

    m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
    m_camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;

	m_renderer = new Renderer();
	
    return true;
}

void Physics::shutdown()
{
	delete m_renderer;
    Gizmos::destroy();
    Application::shutdown();
}

bool Physics::update()
{
    if (Application::update() == false)
    {
        return false;
    }

    Gizmos::clear();

    float dt = (float)glfwGetTime();
    m_delta_time = dt;
    glfwSetTime(0.0);

    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10),
            i == 10 ? white : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i),
            i == 10 ? white : black);
    }

    m_camera.update(1.0f / 60.0f);


    return true;
} */

bool Physics::Create()
{
	/*if (!LoadShader("./data/shaders/main_shader.vs", 0, "./data/shaders/main_shader.fs", &main_shader))
	{
		return false;
	}*/

	SetupPhysX();
	SetupIntroductionToPhysX();

	//TODO: toggle add/ comment
	if (isRBD)
	{
		SetupRBDTutorial();
	}

	return true;
} 

void Physics::Shutdown()
{
	m_physics_scene->release();
	m_physics_foundation->release();
	m_physics->release();
	m_controller_manager->purgeControllers();
	//delete m_renderer;
	//Gizmos::destroy();
	//Application::shutdown();
}

bool Physics::Update(GLfloat deltaTime)
{
	float dt = (float)glfwGetTime();

	if (dt > 0)
	{
		m_physics_scene->simulate(dt > 0.033f ? 0.033f : dt);
		while (m_physics_scene->fetchResults() == false);
	}

	if (isRBD)
	{
		//transform
		//mat4 camera = m_camera.getTransform();
		vec3 cam_pos = m_camera.getTransform()[3].xyz();
		vec3 box_vel = -m_camera.getTransform()[2].xyz() * 20.0f;
		PxTransform box_transform(PxVec3(cam_pos.x, cam_pos.y, cam_pos.z));

		// Geometry
		PxSphereGeometry sphere(0.5f);

		//Density
		float fDensity = 10.0f;
		m_fTimer += dt;
		if (m_fTimer >= 15.0f)
		{
			PxRigidDynamic* new_actor = PxCreateDynamic(*m_physics, box_transform, sphere, *m_physics_material, fDensity);

			PxReal muzzleSpeed = -50;
			// balls velocity
			vec3 v3Direction(-m_camera.getTransform()[2]);
			physx::PxVec3 velocity = physx::PxVec3(v3Direction.x, v3Direction.y, v3Direction.z) * muzzleSpeed;
			new_actor->setLinearVelocity(velocity, true);
			m_physics_scene->addActor(*new_actor); //TODO: new_box
		}
		m_fTimer = 0;
	}
	
	return true;
}

void Physics::Draw(const Camera& a_camState)
{
	renderGizmos(m_physics_scene);
}

void Renderer::RenderAndClear(mat4 view_proj)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
    //Gizmos::draw(m_camera.proj, m_camera.view);

    //m_renderer->RenderAndClear(m_camera.view_proj);
	glUseProgram(main_shader);

	int view_proj_loc = glGetUniformLocation(main_shader, "view_proj");

	int model_loc = glGetUniformLocation(main_shader, "model");
	int model_view_proj_loc = glGetUniformLocation(main_shader, "model_view_proj");

	int diffuse_loc = glGetUniformLocation(main_shader, "diffuse_tex");
	int normal_loc = glGetUniformLocation(main_shader, "normal_tex");
	int specular_loc = glGetUniformLocation(main_shader, "specular_tex");

	int ambient_location = glGetUniformLocation(main_shader, "ambient_light");
	int light_dir_location = glGetUniformLocation(main_shader, "light_dir");
	int light_color_location = glGetUniformLocation(main_shader, "light_color");
	int spec_pow_location = glGetUniformLocation(main_shader, "specular_power");

	float sq_3 = sqrt(3.f);

	glUniform3f(ambient_location, 0.2f, 0.2f, 0.2f);
	glUniform3f(light_dir_location, sq_3, -sq_3, sq_3);
	glUniform3f(light_color_location, 0.8f, 0.8f, 0.8f);
	glUniform1f(spec_pow_location, 15.0f);

	glUniformMatrix4fv(view_proj_loc, 1, GL_FALSE, (float*)&view_proj); //a_camState.getProjectionView());//
	glUniform1i(diffuse_loc, 0);
	glUniform1i(normal_loc, 1);
	glUniform1i(specular_loc, 2);

	for (unsigned int i = 0; i < render_queue.size(); ++i)
	{
		RenderItem* item = &render_queue[i];

		mat4 model_view_proj = view_proj * item->transform;

		Meshes* mesh = item->mesh;

		glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float*)&item->transform);
		glUniformMatrix4fv(model_view_proj_loc, 1, GL_FALSE, (float*)&model_view_proj);

		for (unsigned int j = 0; j < mesh->sub_mesh_count; ++j)
		{
			SubMesh* sub_mesh = mesh->sub_meshes + j;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, sub_mesh->material.diffuse_texture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, sub_mesh->material.normal_texture);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, sub_mesh->material.specular_texture);

			glBindVertexArray(sub_mesh->vao);
			glDrawElements(GL_TRIANGLES, sub_mesh->index_count, GL_UNSIGNED_INT, 0);
		}
	}

	glBindVertexArray(0);

	render_queue.clear();

    //glfwSwapBuffers(m_window);
    //glfwPollEvents();
}

/// --------------------------------------------------------------------
/// <summary>
/// <para><param>P1: format: GL_R32F.</param></para>
/// <example> </example>
/// </summary>
/// --------------------------------------------------------------------
void AddWidget(PxShape* shape, PxRigidActor* actor, vec4 geo_color)
{
    PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
    vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
    glm::quat actor_rotation(full_transform.q.w,
        full_transform.q.x,
        full_transform.q.y,
        full_transform.q.z);
    glm::mat4 rot(actor_rotation);

    mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

    PxGeometryType::Enum geo_type = shape->getGeometryType();

    switch (geo_type)
    {
    case (PxGeometryType::eBOX) :
    {
        PxBoxGeometry geo;
        shape->getBoxGeometry(geo);
        vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
        Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
    } break;
    case (PxGeometryType::eCAPSULE) :
    {
        PxCapsuleGeometry geo;
        shape->getCapsuleGeometry(geo);
        Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::eSPHERE) :
    {
        PxSphereGeometry geo;
        shape->getSphereGeometry(geo);
        Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::ePLANE) :
    {

    } break;
    }
}

void Physics::SetupPhysX()
{
	m_default_filter_shader = physx::PxDefaultSimulationFilterShader;
	// Singleton that instatiates the (actual) physics system...
	m_physics_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_default_allocator, m_default_error_callback);
	//... store the pointer in m_physics
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_physics_foundation, PxTolerancesScale());

	// Initialise the PxPhysics extension library
	PxInitExtensions(*m_physics);

	// Create a physX material.
	// P1: Static Friction
	// P2: Dynamic Friction
	// P3: Restitution (how "bouncy" an object is.
	// Applied to bodies in our scene and mimic how materials in the real world react.
	m_physics_material = m_physics->createMaterial(1, 1, 0);
	m_physics_cooker = PxCreateCooking(PX_PHYSICS_VERSION, *m_physics_foundation, PxCookingParams(PxTolerancesScale()));
}

/// --------------------------------------------------------------------
/// <summary>
/// <para><param>P1: format: GL_R32F.</param></para>
/// <para>Create a scene description object. This contains the parameters which control the scene.</para>
/// <para>Set gravity to 10 m / s / s along the y axis, gravity is a vector which can point in any direction and of any length.</para>
/// <para>The filter shader is a PhysX callback function which allows us to catch triggered events(such as when an object enters a trigger zone or collides with another object).We�ll discuss this in a later tutorial.</para>
/// <para>The CPU dispatcher tell PhysX that we are going to use the CPU for PhysX calculations.It�s possible to use a suitable GPU instead of the CPU or multiple CPU cores.We cover this in a later tutorial.</para>
/// <para>Create our physX scene and return the pointer to it.</para>
/// </summary>
/// --------------------------------------------------------------------
PxScene* Physics::CreateDefaultScene()
{
	PxSceneDesc scene_desc(m_physics->getTolerancesScale());
	scene_desc.gravity = PxVec3(0, -9.807f, 0);
	scene_desc.filterShader = &PxDefaultSimulationFilterShader; //TODO: myFilterShader;
	scene_desc.cpuDispatcher = PxDefaultCpuDispatcherCreate(8); //Note/ TODO: can use GPU or multiple CPU cores.
	PxScene* result = m_physics->createScene(scene_desc);
	return result;
}

void Physics::SetupIntroductionToPhysX()
{
	m_physics_scene = CreateDefaultScene();

	//PxSimulationEventCallback* mycollisionCallBack = new MyCollisionCallback();
	//m_physics_scene->setSimulationEventCallback(mycollisionCallBack);

#pragma region Adding Plane And BathTub
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi * 1.0f, PxVec3(0.0f, 0.0f, 1.0f))); //TOOD: *1.0f?
	PxRigidStatic* plane = PxCreateStatic(*m_physics, pose, PxPlaneGeometry(), *m_physics_material);

	//const PxU32 numShapes = plane->getNbShapes();
#pragma region Boxes
	//add it to the physX scene
	m_physics_scene->addActor(*plane);

	// add a box
	float density = 10.0f;
	PxBoxGeometry box(2, 2, 2);
	PxTransform transform(PxVec3(0, 5, 0)); //PxTransform transform(PxVec3(5, 5, 5));
	PxRigidDynamic* dynamicActor = PxCreateDynamic(*m_physics, transform, box, *m_physics_material, density);

	//add it to the physX scene
	m_physics_scene->addActor(*dynamicActor);
	//////////////////////////
	
	//Adding Second Box
	PxBoxGeometry box3(2, 2, 2);
	PxTransform transform2(PxVec3(5, 5, 5));
	PxRigidDynamic* dynamicActor1 = PxCreateDynamic(*m_physics, transform2, box3, *m_physics_material, density);
	//add it to the physX scene
	m_physics_scene->addActor(*dynamicActor1);

#pragma region Filtering/ Boxes
	/*SetUpFiltering(plane, FilterGroup::eGROUND, FilterGroup::ePLAYER);

	PxBoxGeometry side1(4.5, 3, .5);
	PxBoxGeometry side2(0.5, 3, 4.5);

	//1st Side
	pose = PxTransform(PxVec3(0, 0, 4.0f));
	PxRigidStatic* box = PxCreateStatic(*m_physics, pose, side1, *m_physics_material);
	box->setName("Box");
	box->userData = this;
	m_physics_scene->addActor(*box);

	//2nd Side
	pose = PxTransform(PxVec3(0.0f, 0.5, -4.0f));
	box = PxCreateStatic(*m_physics, pose, side1, *m_physics_material);
	box->setName("Box");
	box->userData = this;
	m_physics_scene->addActor(*box);

	//3rd Side
	pose = PxTransform(PxVec3(4.0f, 0.5, 0));
	box = PxCreateStatic(*m_physics, pose, side2, *m_physics_material);
	box->setName("Box");
	box->userData = this;
	m_physics_scene->addActor(*box);

	//4th Side
	pose = PxTransform(PxVec3(-4.0f, 0.5, 0));
	box = PxCreateStatic(*m_physics, pose, side2, *m_physics_material);
	box->setName("Box");
	box->userData = this;
	SetShapeAsTrigger(box);
	m_physics_scene->addActor(*box); */
#pragma endregion
#pragma endregion
#pragma endregion
/*
#pragma region Player Controller ( Capsule )
	m_myHitReport = new ControllerHitReport();
	m_controller_manager = PxCreateControllerManager(*m_physics_scene);

	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = m_physics_material;
	desc.reportCallback = m_myHitReport;
	desc.density = 10;

	m_playerController = m_controller_manager->createController(desc);

	m_playerController->setPosition(PxExtendedVec3(5, 2, 5));

	m_characterYvelocity = 0;
	m_characterRotation = 0;
	m_playerGravity = -0.5f;


	m_myHitReport->clearPlayerContactNormal();

	m_physics_scene->addActor(*m_playerController->getActor());
#pragma endregion

#pragma region Ragdoll
	//Adding A ragdoll into the scene
	Ragdoll* ragdoll = new Ragdoll();
	ragdollArticulation = ragdoll->makeRagdoll(m_physics, ragdoll->ragdollData, PxTransform(PxVec3(10, 5, 20)), .15f, m_physics_material);

	m_physics_scene->addArticulation(*ragdollArticulation);

	//5th Side
	pose = PxTransform(PxVec3(10, 0.5, 10));
	PxBoxGeometry triggerSize(1, 0.1f, 1);
	box = PxCreateStatic(*m_physics, pose, triggerSize, *m_physics_material);
	box->setName("RagDollTrigger");
	box->userData = this;
	SetShapeAsTrigger(box);
	m_physics_scene->addActor(*box);
#pragma endregion

#pragma region Particle System
	PxU32 maxParticles = 20000;
	PxParticleFluid* pf;
	//Particle System//////////////////////////////////////////////////////////////////////
	// create particle system in PhysX SDK

	// set immutable properties.
	bool perParticleRestOffset = false;
	pf = m_physics->createParticleFluid(maxParticles, perParticleRestOffset);


	pf->setRestParticleDistance(0.3f);
	pf->setDynamicFriction(0.1f);
	pf->setStaticFriction(0.1f);
	pf->setDamping(0.1f);
	pf->setParticleMass(0.001f);
	pf->setRestitution(0);
	pf->setStiffness(50);


	//pf->setParticleReadDataFlag(PxParticleReadDataFlag::eDENSITY_BUFFER, true);
	pf->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);

	if (pf)
	{
		m_physics_scene->addActor(*pf);
		m_particleEmitter = new ParticleFluidEmitter(maxParticles, PxVec3(0, 10, 0), pf, .001f);
		m_particleEmitter->setStartVelocityRange(-0.001f, -250.0f, -0.001f, 0.001f, -250.0f, 0.001f);
	}
	/////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region Joints/Springs
	////PxSpring* spring = new PxSpring(PxReal(2), PxReal(0.1f));
	//PxJoint* joint = PxRevoluteJointCreate(*m_physics, dynamicActor, dynamicActor->,);
	//joint->setActors(dynamicActor1, dynamicActor);
	//m_physics_scene->addActor(*joint);
#pragma endregion

#pragma region Filtering
	SetUpFiltering(m_playerController->getActor(), FilterGroup::ePLAYER, FilterGroup::eGROUND | FilterGroup::ePLATFORM);
	SetUpFiltering(plane, FilterGroup::eGROUND, FilterGroup::ePLAYER);
#pragma endregion */
}

void Physics::SetupRBDTutorial()
{
	m_physics_scene = CreateDefaultScene();
	//add a plane to the scene
	PxTransform transform = PxTransform(PxVec3(0, 0, 0), PxQuat((float)PxHalfPi, PxVec3(0, 0, 1)));
	PxRigidStatic* plane = PxCreateStatic(*m_physics, transform, PxPlaneGeometry(), *m_physics_material);
	m_physics_scene->addActor(*plane);
}

void Physics::renderGizmos(PxScene* physics_scene)
{
    PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
    PxU32 actor_count = physics_scene->getNbActors(desiredTypes);
    PxActor** actor_list = new PxActor*[actor_count];
	physics_scene->getActors(desiredTypes, actor_list, actor_count);
    
    vec4 geo_color(1, 0, 0, 1);
    for (int actor_index = 0;
        actor_index < (int)actor_count;
        ++actor_index)
    {
        PxActor* curr_actor = actor_list[actor_index];
        if (curr_actor->isRigidActor())
        {
            PxRigidActor* rigid_actor = (PxRigidActor*)curr_actor;
            PxU32 shape_count = rigid_actor->getNbShapes();
            PxShape** shapes = new PxShape*[shape_count];
            rigid_actor->getShapes(shapes, shape_count);

            for (int shape_index = 0;
                shape_index < (int)shape_count;
                ++shape_index)
            {
                PxShape* curr_shape = shapes[shape_index];
                AddWidget(curr_shape, rigid_actor, geo_color);
            }

            delete[]shapes;
        }
    }

    delete[] actor_list;

    int articulation_count = physics_scene->getNbArticulations();

    for (int a = 0; a < articulation_count; ++a)
    {
        PxArticulation* articulation;
		physics_scene->getArticulations(&articulation, 1, a);

        int link_count = articulation->getNbLinks();

        PxArticulationLink** links = new PxArticulationLink*[link_count];
        articulation->getLinks(links, link_count);

        for (int l = 0; l < link_count; ++l)
        {
            PxArticulationLink* link = links[l];
            int shape_count = link->getNbShapes();

            for (int s = 0; s < shape_count; ++s)
            {
                PxShape* shape;
                link->getShapes(&shape, 1, s);
                AddWidget(shape, link, geo_color);
            }
        }
        delete[] links;
    }
}

