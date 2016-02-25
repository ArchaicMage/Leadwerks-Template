#ifndef _APP_H_
	#define _APP_H_

#pragma once
#include "Leadwerks.h"

class Container;

class EventManager;
class InputManager;

class App {
public:
					App			(void);
	virtual 		~App		(void);

	void			Configure(Container* pContainer);

	bool 			Start		(void);
	void			Shutdown	(void);

	void 			preUpdate	(void);
	void 			postUpdate	(void);
	bool 			Update		(float deltaTime);

	void 			preRender	(void);
	void 			postRender	(void);
	void 			Render		(void);

	void 			preDraw		(void);
	void 			postDraw	(void);
	void 			Draw		(void);

private:

	EventManager* m_pEventManager;
	InputManager* m_pInputManager;
}; // end class.

#endif // _APP_H_