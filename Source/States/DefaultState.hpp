/*-------------------------------------------------------
                    <copyright>
    
    File: DefaultState.hpp
    Language: C++
    
    (C) Copyright Eden Softworks
    
    Author: Joshua Allen
    E-Mail: Joshua(AT)EdenSoftworks(DOT)net
    
    Description: Header file for DefaultState class.
                 The DefaultState class exists to provide
                 a concrete example of how other
                 application states can implement to
                 base state class to provide state
                 gameplay-based logic.

---------------------------------------------------------*/

#ifndef _DEFAULT_STATE_HPP_
	#define _DEFAULT_STATE_HPP_

#pragma once
#include "State.hpp"
#include "../Managers/InputManager.hpp"

#include "../Utilities/CameraHandle.hpp"
#include "../Utilities/Container.hpp"
#include "../Utilities/Event.hpp"
#include "../Utilities/IsoSurface.hpp"
#include "../Utilities/Modeler.hpp"
#include "../Utilities/VoxelBuffer.hpp"

#include "../Components/World.hpp"
#include "../Entities/CameraDynamic.hpp"
#include "../Entities/Prop.hpp"

#include "../Utilities/luatables/luatables.h"

#define ISOLEVEL				0.0f
#define VOXELS_PER_CELL			9
#define DATACELL_SIZE			7.0f
#define VOXEL_SIZE				1.0f
#define RENDERCELL_SIZE			64.0f
#define CELLS_PER_ISOSURFACE	8
#define NUM_VOXELS				9

class DefaultState : public State {

	CLASS_TYPE(DefaultState);

public:		

	                       DefaultState(void);

	void                   Configure(Container* pContainer);
	void                   Load(void);
	void                   Close(void);

	bool                   Update(float deltaTime);

	void                   OnKeyDown(Event_KeyDown* pEvent);
	void                   OnKeyUp(Event_KeyUp* pEvent);

private:

	Leadwerks::Light*	  m_pLight;
	Leadwerks::Model*     m_pGround;

	IsoSurface<float>*     m_pIsosurface;
	Leadwerks::Model*      m_pModel;
	VoxelBuffer<float>*    m_pBuffer;

    InputManager*          m_pInputMgr;
	CameraHandle*          m_pCameraHndl;

	Components::World*     m_pWorld;

	uint64_t               m_cameraDynamic;	    

}; // < end class.

DefaultState::DefaultState(void) { }

void DefaultState::Configure(Container* pContainer)
{
	// < Here, we resolve some dependencies from the application container.
	m_pCameraHndl = pContainer->Resolve<CameraHandle>();
    m_pInputMgr = pContainer->Resolve<InputManager>();
}

void DefaultState::Load(void) 
{ 	
	// < Add a light to our sample scene.
	m_pLight = Leadwerks::DirectionalLight::Create();
	m_pLight->SetRotation(35.0f, -35.0f, 0.0f);

	// < Create a base for the sample scene.
	m_pGround = Leadwerks::Model::Box(10.0f, 0.5f, 10.0f);	
	m_pGround->Move(0.0f, 0.0f, 4.0f);

	// < Create the world for our components and a sample camera to move
	// * about our scene.
	m_pWorld = new Components::World();
	m_cameraDynamic = Entities::CameraDynamic::Create(m_pWorld, m_pCameraHndl, "./Scripts/Camera.lua");    
	
	m_pCameraHndl->getInst()->SetDrawMode(DRAW_WIREFRAME);
    
	// < Set the input manager to reset the mouse-position to the center of the
	// * screen every frame.
	m_pInputMgr->ToggleMouseCenter();
	
	// < Initialize our isosurface and the voxel buffer we will be using.
	m_pIsosurface = new IsoSurface<float>();
	m_pBuffer = new VoxelBuffer<float>(NUM_VOXELS, NUM_VOXELS, NUM_VOXELS);

	ModelerInput<float> input;
	Modeler<float> modeler;
	modeler.Assign(*m_pBuffer);

	// < Here, lets use the modeler to feed out voxel data into our isosurface.
	for (unsigned z = 0; z <= 8; z += 1)
		for (unsigned y = 0; y <= 8; y += 1)
			for (unsigned x = 0; x <= 8; x += 1) {
				input.pt[0] = x; input.pt[1] = y; input.pt[2] = z;
				
				// < Formula for a circle.
				if ( (sqrt((x - 4) * (x - 4) + (y - 4) * (y - 4) + (z - 4) * (z - 4)) < 4) )
					input.val = -1.0f;
				else
					input.val = 1.0f;

				modeler.AddInput(input);
			} // end for
	
	modeler.Execute();

	// < Generate our isosurface.
	m_pModel = Leadwerks::Model::Create();	
	m_pModel->Move(-4.0f, 0.0f, 0.0f);
	unsigned nTriangles = m_pIsosurface->GenerateSurface(*m_pModel,
		*m_pBuffer,
		0.0,
		CELLS_PER_ISOSURFACE,
		CELLS_PER_ISOSURFACE,
		CELLS_PER_ISOSURFACE,
		VOXEL_SIZE,
		VOXEL_SIZE,
		VOXEL_SIZE);

}

void DefaultState::Close(void) 
{ 		
	// < Set the input manager to allow the mouse pointer to freely
	// * move about the window.
    m_pInputMgr->ToggleMouseCenter();

	SAFE_DELETE(m_pWorld);

	m_pCameraHndl = nullptr;
    m_pInputMgr = nullptr;

	SAFE_RELEASE(m_pLight);
	SAFE_DELETE(m_pLight);

	SAFE_DELETE(m_pIsosurface);
	SAFE_DELETE(m_pBuffer);

	SAFE_RELEASE(m_pGround);
	SAFE_DELETE(m_pGround);

	SAFE_RELEASE(m_pModel);
	SAFE_DELETE(m_pModel);

}

bool DefaultState::Update(float dt) 
{ 	
    auto& inputComponent = m_pWorld->GetComponents<Components::Input>(m_pWorld, m_cameraDynamic)
        ->front();

	// < Check for any mouse movement. If there is any movement, we should
	// * look to rotate the camera.
    if (m_pInputMgr->DeltaX() < 0) { inputComponent.nMask |= INPUT_ROTATE_LEFT; }
    if (m_pInputMgr->DeltaX() > 0) { inputComponent.nMask |= INPUT_ROTATE_RIGHT; }

    if (m_pInputMgr->DeltaY() < 0) { inputComponent.nMask |= INPUT_ROTATE_DOWN; }
    if (m_pInputMgr->DeltaY() > 0) { inputComponent.nMask |= INPUT_ROTATE_UP; }

	Entities::CameraDynamic::Update(m_pInputMgr, m_pWorld, dt);	

	return true;

}

void DefaultState::OnKeyDown(Event_KeyDown* pEvent)
{
	auto& inputComponent = m_pWorld->GetComponents<Components::Input>(m_pWorld, m_cameraDynamic)
		->front();    

	// < Check for any key presses from the keyboard. If any key is pressed
	// * we should look to move the camera.
	if (pEvent->Key() == Leadwerks::Key::W) { inputComponent.nMask |= INPUT_MOVE_FORWARD; }
	if (pEvent->Key() == Leadwerks::Key::A) { inputComponent.nMask |= INPUT_MOVE_LEFT; }
	if (pEvent->Key() == Leadwerks::Key::S) { inputComponent.nMask |= INPUT_MOVE_BACKWARD; }
	if (pEvent->Key() == Leadwerks::Key::D) { inputComponent.nMask |= INPUT_MOVE_RIGHT; }

	if (pEvent->Key() == Leadwerks::Key::E) { inputComponent.nMask |= INPUT_MOVE_UP; }
	if (pEvent->Key() == Leadwerks::Key::Q) { inputComponent.nMask |= INPUT_MOVE_DOWN; }

}

void DefaultState::OnKeyUp(Event_KeyUp* pEvent)
{
	auto& inputComponent = m_pWorld->GetComponents<Components::Input>(m_pWorld, m_cameraDynamic)
		->front();

	// < Just like key press however, here we pop the movement bitmask to signal 
	// * a key release.
	if (pEvent->Key() == Leadwerks::Key::W) { inputComponent.nMask &= ~INPUT_MOVE_FORWARD; }
	if (pEvent->Key() == Leadwerks::Key::A) { inputComponent.nMask &= ~INPUT_MOVE_LEFT; }
	if (pEvent->Key() == Leadwerks::Key::S) { inputComponent.nMask &= ~INPUT_MOVE_BACKWARD; }
	if (pEvent->Key() == Leadwerks::Key::D) { inputComponent.nMask &= ~INPUT_MOVE_RIGHT; }

	if (pEvent->Key() == Leadwerks::Key::E) { inputComponent.nMask &= ~INPUT_MOVE_UP; }
	if (pEvent->Key() == Leadwerks::Key::Q) { inputComponent.nMask &= ~INPUT_MOVE_DOWN; }

}

#endif _DEFAULT_STATE_HPP_