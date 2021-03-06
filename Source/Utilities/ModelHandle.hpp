/*-------------------------------------------------------
                    <copyright>
    
    File: ModelHandle.hpp
    Language: C++
    
    (C) Copyright Eden Softworks
    
    Author: Joshua Allen
    E-Mail: Joshua(AT)EdenSoftworks(DOT)net
    
    Description: Header file for ModelHandle utility.
                 The ModelHandle is a wrapper around
                 the Leadwerks::Model object.
    
    Functions: 1. Leadwerks::Model* getInst(void);                                                     

---------------------------------------------------------*/

#ifndef _MODEL_HANDLE_HPP_
    #define _MODEL_HANDLE_HPP_

#pragma once
#include "Leadwerks.h"
#include "Macros.hpp"

class ModelHandle
{
    CLASS_TYPE(ModelHandle);

public:

    ModelHandle(Leadwerks::Model* pModel)
        :m_pModel(pModel) {

    }

    ~ModelHandle(void) {

        SAFE_RELEASE(m_pModel);
        SAFE_DELETE(m_pModel);

    }

    Leadwerks::Model* getInst(void) { return m_pModel; }

private:

    Leadwerks::Model* m_pModel;

}; // < end class.

#endif _MODEL_HANDLE_HPP_