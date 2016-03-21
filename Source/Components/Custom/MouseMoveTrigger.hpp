#ifndef _MOUSE_MOVE_TRIGGER_COMPONENT_HPP_
	#define _MOUSE_MOVE_TRIGGER_COMPONENT_HPP_

#pragma once
#include "../../Common.hpp"
#include "../../Utilities/Event.hpp"
#include "../../Utilities/Macros.hpp"

#include "../World.hpp"
#include "../Trigger.hpp"

namespace Components
{    
	template <typename T>
	struct MouseMoveTrigger : public Trigger<Components::World&, T, Event_MouseMove*>
	{
        typedef Delegate<Components::World&, T, Event_MouseMove*> MouseMoveEvent;

		CLASS_TYPE(MouseMoveTrigger);

		MouseMoveTrigger(MouseMoveEvent _callback, std::string _cName = "")
			: Trigger(_callback, _cName) { }

	}; // < end class.

} // < end namespace.

#endif _MOUSE_MOVE_TRIGGER_COMPONENT_HPP_