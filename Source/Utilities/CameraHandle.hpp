/*-------------------------------------------------------
                    <copyright>
    
    File: CameraHandle.hpp
    Language: C++
    
    (C) Copyright Eden Softworks
    
    Author: Joshua Allen
    E-Mail: Joshua(AT)EdenSoftworks(DOT)net
    
    Description: Header file for CameraHandle utility.
                 The CameraHandle is a wrapper around
                 the Leadwerks::Camera object.
    
    Functions: 1. Leadwerks::Camera* getInst(void);                                                     

---------------------------------------------------------*/

#ifndef _CAMERA_HANDLE_HPP_
	#define _CAMERA_HANDLE_HPP_

#pragma once
#include "Leadwerks.h"
#include "Macros.hpp"

class CameraHandle
{
	CLASS_TYPE(CameraHandle);

public:

	CameraHandle(Leadwerks::Camera* pCamera) 
		:m_pCamera(pCamera){

	}

	~CameraHandle(void) {

		SAFE_RELEASE(m_pCamera);
		SAFE_DELETE(m_pCamera);

	}

	Leadwerks::Camera* getInst(void) { return m_pCamera; }

private:

	Leadwerks::Camera* m_pCamera;

}; // < end class.

#endif _CAMERA_HANDLE_HPP_