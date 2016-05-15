/// <summary>
/// File Name:		Renderable.cpp
/// Author: 		David Azouz
/// Date Created: 	10/05/16
/// Date Modified: 	10/05/16
///----------------------------------------------------------
/// Brief: An Abstract Renderable Class defining the interface all renderable objects must implement.
/// Enables more than one object to share the same mesh or texture etc.
/// viewed: http://www.dofactory.com/net/factory-method-design-pattern
/// http://www.ogre3d.org/docs/api/1.9/class_ogre_1_1_renderable.html#details
///
/// ***EDIT***
/// - Renderable created	 	- David Azouz 10/05/16
/// -  	- David Azouz 5/02/16
/// -  	- David Azouz 6/02/16
/// 
/// TODO: 
/// Reff to Mesh
/// Ref to Textures
/// ref to mesh = Mesh* - maybe? [] - hold onto different submeshes - FBX models
///	ref to textures[]
///	ref to shader prog(which is) m_programID
/// </summary>
/// ----------------------------------------------------------

#include "Renderable.h"



Renderable::Renderable()
{
}


Renderable::~Renderable()
{
}
