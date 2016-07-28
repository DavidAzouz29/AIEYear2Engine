#define GLM_SWIZZLE

#include "Physics.h"
#include "BaseApplication.h"
#include "Camera\Camera.h"
#include "ControllerHitReport.h"
#include "PhysXComponent.h"
#include "Render.h"

#include "Gizmos.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "imgui.h"

#include <assert.h>
#include <GLFW/glfw3.h>
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
	m_pPhysics_scene->release();
	m_pPhysics_foundation->release(); //TODO" 178: ...pending module ref. Close .. dep mod first
	m_pPhysics->release();
	//m_controller_manager->purgeControllers();
	//delete m_renderer;
	//Gizmos::destroy();
	//Application::shutdown();
}

bool Physics::Update(GLfloat a_deltaTime, const Camera& a_camera)
{
	//TODO: make 'deltaTime' useful?
	float dt = (float)glfwGetTime();

	if (dt > 0)
	{
		m_pPhysics_scene->simulate(dt > 0.033f ? 0.033f : dt);
		while (m_pPhysics_scene->fetchResults() == false);
	}

	if (isRBD)
	{
		//transform
		//mat4 camera = a_camera.getTransform();
		vec3 cam_pos = a_camera.getTransform()[3].xyz();
		vec3 box_vel = -a_camera.getTransform()[2].xyz() * 20.0f;
		PxTransform box_transform(PxVec3(cam_pos.x, cam_pos.y, cam_pos.z));

		// Geometry
		PxSphereGeometry sphere(0.5f);

		//Density
		float fDensity = 10.0f;
		m_fTimer += dt;
		if (m_fTimer >= 15.0f)
		{
			PxRigidDynamic* new_actor = PxCreateDynamic(*m_pPhysics, box_transform, sphere, *m_pPhysics_material, fDensity);

			PxReal muzzleSpeed = -50;
			// balls velocity
			vec3 v3Direction(-a_camera.getTransform()[2]);
			physx::PxVec3 velocity = physx::PxVec3(v3Direction.x, v3Direction.y, v3Direction.z) * muzzleSpeed;
			new_actor->setLinearVelocity(velocity, true);
			m_pPhysics_scene->addActor(*new_actor); //TODO: new_box
		}
		m_fTimer = 0;
	}
	
	return true;
}

void Physics::Draw(const Camera& a_camState)
{
	renderGizmos(m_pPhysics_scene);
}

void Physics::RenderUI()
{
	if (ImGui::CollapsingHeader("Physics"))
	{
		//ImGui::DragFloat4("World Trans data", m_worldTransform[3].data, 1.1f, -FLT_MAX, FLT_MAX);
		PxVec3 &pos = PxVec3(physx::toVec3(m_v3PhysicsWorldTransform));
		//TODO: we're not adjusting the actual position - just viewing
		glm::vec3 &wPos = glm::vec3(pos.x, pos.y, pos.z);
		ImGui::DragFloat3("World Trans Physics", glm::value_ptr(wPos), 1.1f, -FLT_MAX, FLT_MAX);
		ImGui::Separator(); 
	}
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

void Physics::SetupPhysX()
{
	m_default_filter_shader = physx::PxDefaultSimulationFilterShader;
	// Singleton that instatiates the (actual) physics system...
	m_pPhysics_foundation = &PxGetFoundation();// PxCreateFoundation(PX_PHYSICS_VERSION, m_default_allocator, m_default_error_callback);
	//... store the pointer in m_physics
	m_pPhysics = &PxGetPhysics();// PxCreatePhysics(PX_PHYSICS_VERSION, *m_pPhysics_foundation, PxTolerancesScale());

	// Initialise the PxPhysics extension library
	PxInitExtensions(*m_pPhysics);

	// Create a physX material.
	// P1: Static Friction
	// P2: Dynamic Friction
	// P3: Restitution (how "bouncy" an object is.
	// Applied to bodies in our scene and mimic how materials in the real world react.
	m_pPhysics_material = m_pPhysics->createMaterial(1, 1, 0);
	m_pPhysics_cooker = PxCreateCooking(PX_PHYSICS_VERSION, *m_pPhysics_foundation, PxCookingParams(PxTolerancesScale()));
}

/// --------------------------------------------------------------------
/// <summary>
/// <para><param>P1: format: GL_R32F.</param></para>
/// <para>Create a scene description object. This contains the parameters which control the scene.</para>
/// <para>Set gravity to 10 m / s / s along the y axis, gravity is a vector which can point in any direction and of any length.</para>
/// <para>The filter shader is a PhysX callback function which allows us to catch triggered events(such as when an object enters a trigger zone or collides with another object).We’ll discuss this in a later tutorial.</para>
/// <para>The CPU dispatcher tell PhysX that we are going to use the CPU for PhysX calculations.It’s possible to use a suitable GPU instead of the CPU or multiple CPU cores.We cover this in a later tutorial.</para>
/// <para>Create our physX scene and return the pointer to it.</para>
/// </summary>
/// --------------------------------------------------------------------
PxScene* Physics::CreateDefaultScene()
{
	PxSceneDesc scene_desc(m_pPhysics->getTolerancesScale());
	scene_desc.gravity = PxVec3(0, -9.807f, 0);
	scene_desc.filterShader = &PxDefaultSimulationFilterShader; //TODO: myFilterShader;
	//unsigned int uiThreads = BaseApplication::GetNumOfThreadsAvailable();
	scene_desc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2); //was 8 - Note/ TODO: can use GPU or multiple CPU cores.
	PxScene* result = m_pPhysics->createScene(scene_desc);
	return result;
}

void Physics::SetupIntroductionToPhysX()
{
	m_pPhysics_scene = CreateDefaultScene();

	//PxSimulationEventCallback* mycollisionCallBack = new MyCollisionCallback();
	//m_physics_scene->setSimulationEventCallback(mycollisionCallBack);

#pragma region Adding Plane And BathTub
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi * 1.0f, PxVec3(0.0f, 0.0f, 1.0f))); //TOOD: *1.0f?
	PxRigidStatic* plane = PxCreateStatic(*m_pPhysics, pose, PxPlaneGeometry(), *m_pPhysics_material);

	//const PxU32 numShapes = plane->getNbShapes();
#pragma region Boxes
	//add it to the physX scene
	m_pPhysics_scene->addActor(*plane);

	// add a box
	float density = 10.0f;
	PxBoxGeometry box(2, 2, 2);
	PxTransform transform(PxVec3(7, 5, 7)); //TODO: stop colliding with other entities upon start
	PxRigidDynamic* dynamicActor = PxCreateDynamic(*m_pPhysics, transform, box, *m_pPhysics_material, density);

	//add it to the physX scene
	m_pPhysics_scene->addActor(*dynamicActor);
	//////////////////////////
	
	//Adding Second Box
	PxBoxGeometry box3(2, 2, 2);
	PxTransform transform2(PxVec3(-7, 5, -5));
	PxRigidDynamic* dynamicActor1 = PxCreateDynamic(*m_pPhysics, transform2, box3, *m_pPhysics_material, density);
	//add it to the physX scene
	m_pPhysics_scene->addActor(*dynamicActor1);

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

#pragma region Player Controller ( Capsule )
	m_pMyHitReport = std::make_shared<ControllerHitReport>(); //TODO: was 'new'ed
	m_pController_manager = PxCreateControllerManager(*m_pPhysics_scene);

	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = m_pPhysics_material;
	desc.reportCallback = m_pMyHitReport.get(); // connect it to our collision detection routine
	desc.density = 10;
	// Create the layer controller
	m_pPlayerController = m_pController_manager->createController(desc);
	m_v3PhysicsWorldTransform = PxExtendedVec3(5, 2, 5);
	//m_worldTransform = glm::translate(glm::vec3(-m_iGrid * 0.5f, -m_fAmplitude, -m_iGrid * 0.5f));
	m_pPlayerController->setPosition(m_v3PhysicsWorldTransform);

	// Set up some variables to control our player with
	m_characterYvelocity = 0;	// initialize character velocity
	m_characterRotation = 0;	// and rotation
	m_playerGravity = -0.5f;	// set up the player gravity

	// Initialize the contact normal (what we are in contact with)
	m_pMyHitReport->ClearPlayerContactNormal(); 

	m_pPhysics_scene->addActor(*m_pPlayerController->getActor()); // so we can draw it's Gizmo

	PxCapsuleGeometry capsule(desc.radius, desc.height); //TODO: halfHeight?
	PxRigidDynamic* capsuleActor = PxCreateDynamic(*m_pPhysics, transform2, capsule, *desc.material, desc.density);
	m_pPhysics_scene->addActor(*capsuleActor);

#pragma endregion
/*
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
	m_pPhysics_scene = CreateDefaultScene();
	//add a plane to the scene
	PxTransform transform = PxTransform(PxVec3(0, 0, 0), PxQuat((float)PxHalfPi, PxVec3(0, 0, 1)));
	PxRigidStatic* plane = PxCreateStatic(*m_pPhysics, transform, PxPlaneGeometry(), *m_pPhysics_material);
	m_pPhysics_scene->addActor(*plane);
}

bool Physics::UpdatePlayerController(float a_deltaTime)
{
	bool isOnGround; // set to true if we are on the ground
	float fMovementSpeed = 10.0f; // forward and back movement speed
	float fMovementSpeedSlowDown = fMovementSpeed - 2.0f; // slow down our speed if going uphill
	float fRotationSpeed = 1.0f; // turn speed
	float fJumpForce = 25.0f;
	float fJumpForceMax = fJumpForce;// *2;

	//check if we have a contact normal. 
	//If 'y' is greater than .3 we assume this is solid ground.
	// This is a rather primitive way to do this. TODO: Can you do better?
	// Check if we are currently on the ground.
	// if we are then raise us above the plane
	if (m_pMyHitReport->GetPlayerContactNormal().y > 0.3f)
	{
		m_characterYvelocity = 0.1f;
		fJumpForce = fJumpForceMax;
		isOnGround = true;
	}
	else
	{
		m_characterYvelocity += m_playerGravity * a_deltaTime;
		isOnGround = false;
	}
	m_pMyHitReport->ClearPlayerContactNormal();
	const PxVec3 up(0, 1, 0);
	// scan the keys and set up our intended velocity based on a global transform
	PxVec3 velocity(0, m_characterYvelocity, 0);
	m_pWindow = glfwGetCurrentContext();
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		velocity.z += fMovementSpeed * a_deltaTime;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		velocity.z -= fMovementSpeed * a_deltaTime;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		velocity.x += fMovementSpeed * a_deltaTime;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		velocity.x -= fMovementSpeed * a_deltaTime;
	}
	// Jumping
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		//if (isOnGround) //m_playerGravity
		velocity.y -= fJumpForce * a_deltaTime;// fMovementSpeed * a_deltaTime;
		fJumpForceMax -= 1.0f;
		// TODO: further research asymptotes
		//float x = velocity.y;
		// x^2
		//float fJumpAsymptote = ((x * x) - 3 * x) / ((2 * x) - 2);
	}

	float fMinDistance = 0.001f;
	PxControllerFilters filter;
	// make controls relative to player facing
	PxQuat rotation(m_characterRotation, up);
	//PxVec3 velocity(0, m_characterYvelocity, 0); //TODO: what do with this? redef?
	m_pPlayerController->setStepOffset(2.0f);
	// move the controller
	m_pPlayerController->move(rotation.rotate(velocity), fMinDistance, a_deltaTime, filter);

	return true;
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
				m_pPhysXComponent->AddWidget(curr_shape, rigid_actor, geo_color);
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
				m_pPhysXComponent->AddWidget(shape, link, geo_color);
            }
        }
        delete[] links;
    }
}

//
void Physics::AttachedRigidBodyConvex(float density, PxMaterial* g_PhysicsMaterial, bool isDynamic)
{
	//need a placeholder box 
	PxBoxGeometry box = PxBoxGeometry(1, 1, 1); 
	//PxTransform transform(*(PxMat44*)(&m_worldTransform[0])); //PhysX and GLM matricies are the same internally so we simply cast between them; 
	PxTransform transform(*(PxMat44*)(&m_v3PhysicsWorldTransform)); //PhysX and GLM matricies are the same internally so we simply cast between them; 
	if (isDynamic) 
	{
		_pXactor = PxCreateDynamic(*m_pPhysics, transform, box, *g_PhysicsMaterial, density);
	}
	else
	{
		_pXactorS = PxCreateStatic(*m_pPhysics, transform, box, *g_PhysicsMaterial);
		//PxRigidActor* //TODO: 
	}
	_pXactor->userData = this; //link the PhysX actor to our FBX model

	int numberVerts = 0;
	//find out how many verts there are in total in tank model 
	for (unsigned int i = 0; i < _FBXModel->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = _FBXModel->getMeshByIndex(i);
		numberVerts += mesh->m_vertices.size();
	}
	//reserve space for vert buffer 
	PxVec3 *verts = new PxVec3[numberVerts]; //temporary buffer for our verts 
	int vertIDX = 0;
	//copy our verts from all the sub meshes 
	//and tranform them into the same space 
	for (unsigned int i = 0; i < _FBXModel->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = _FBXModel->getMeshByIndex(i);
		int numberVerts = mesh->m_vertices.size();
		for (int vertCount = 0; vertCount< numberVerts; vertCount++)
		{
			glm::vec4 temp = mesh->m_globalTransform * mesh->m_vertices[vertCount].position;
			verts[vertIDX++] = PxVec3(temp.x, temp.y, temp.z);
		}
	}

	if (isDynamic)
	{
		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = numberVerts;
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.points.data = verts;
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
		convexDesc.vertexLimit = 128;
		PxDefaultMemoryOutputStream* buf = new PxDefaultMemoryOutputStream();
		assert(m_pPhysics_cooker->cookConvexMesh(convexDesc, *buf));
		PxU8* contents = buf->getData();
		PxU32 size = buf->getSize();
		PxDefaultMemoryInputData input(contents, size);
		PxConvexMesh* convexMesh = m_pPhysics->createConvexMesh(input);
		PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f));
		PxShape* convexShape = _pXactor->createShape(PxConvexMeshGeometry(convexMesh), *g_PhysicsMaterial, pose);
		//remove the placeholder box we started with 
		int numberShapes = _pXactor->getNbShapes();
		PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numberShapes, 16);
		_pXactor->getShapes(shapes, numberShapes);
		_pXactor->detachShape(**shapes); delete(verts);
		//delete our temporary vert buffer. 
		//Add it to the scene 
		m_pPhysics_scene->addActor(*_pXactor);
	}
	else
	{
		// Less efficient than convex hulls
		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = numberVerts;
		meshDesc.points.stride = sizeof(PxVec3);
		meshDesc.points.data = verts;
		int triCount = numberIndex / 3;
		meshDesc.triangles.count = triCount;
		meshDesc.triangles.stride = 3 * sizeof(PxU32);
		meshDesc.triangles.data = indexes; //WhaT F

		PxDefaultMemoryOutputStream* buf = new PxDefaultMemoryOutputStream();
		assert(m_pPhysics_cooker->cookTriangleMesh(meshDesc, *buf));
		PxU8* contents = buf->getData();
		PxU32 size = buf->getSize();
		PxDefaultMemoryInputData input(contents, size);
		PxTriangleMesh* triangleMesh = m_pPhysics->createTriangleMesh(input);
		PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f));
		PxShape* convexShape = _pXactorS->createShape(PxTriangleMeshGeometry(triangleMesh), *g_PhysicsMaterial, pose);
	}
}

/// --------------------------------------------------------------------
/// <summary>
/// <para><param>P1 + 2: rows + columns.</param></para>
/// <para><param>P3: terrain: int* perlin_data.</param></para>
/// <para><param>P4: _heightScalePX = *number next to height* i.e. height * x.</param></para>
/// <para><param>P5: size: .</param></para>
///
/// Height-map i.e. Terrain
/// <example> </example>
/// </summary> //PxReal
/// --------------------------------------------------------------------
void Physics::TerrainCollision(unsigned int _rows, unsigned int _cols, int* _samples, int _heightScalePX, int _size)//, PxMaterial* g_PhysicsMaterial = GetPxMat())
{
	PxHeightFieldDesc hfDesc;
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = _cols;
	hfDesc.nbRows = _rows;
	hfDesc.samples.data = _samples;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);
	hfDesc.thickness = -100.0f;
	PxHeightField* aHeightField = m_pPhysics->createHeightField(hfDesc);
	PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), _heightScalePX, _size, _size);
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f));
	PxShape* heightmap = _pXactorS->createShape((PxHeightFieldGeometry)hfGeom, *m_pPhysics_material, pose);
	assert(heightmap);
}

//g_Physics