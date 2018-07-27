/*
 The zlib/libpng License
 
 Copyright (c) 2005-2007 Phillip Castaneda (pjcast -- www.wreckedgames.com)
 
 This software is provided 'as-is', without any express or implied warranty. In no event will
 the authors be held liable for any damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose, including commercial
 applications, and to alter it and redistribute it freely, subject to the following
 restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that
 you wrote the original software. If you use this software in a product,
 an acknowledgment in the product documentation would be appreciated but is
 not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef OIS_CocoaMouse_H
#define OIS_CocoaMouse_H

#include "OISMouse.h"
#include "mac/CocoaHelpers.h"

#include <Cocoa/Cocoa.h>

@class CocoaMouseView;

namespace OIS
{
	class CocoaMouse : public Mouse
    {
	public:
		CocoaMouse(InputManager* creator, bool buffered, bool hideMouse, bool grabMouse);
		
		virtual ~CocoaMouse();
		
		/** @copydoc Object::setBuffered */
		virtual void setBuffered(bool buffered) override;

		/** @copydoc Object::capture */
		virtual void capture() override;

		/** @copydoc Object::queryInterface */
		virtual inline Interface* queryInterface(Interface::IType type) override
		{
			return nullptr;
		}

		/** @copydoc Object::_initialize */
		virtual void _initialize() override;
        
        inline MouseState* getMouseStatePtr()
		{
			return &(mState);
		}

	protected:
        CocoaMouseView* mResponder;
	};
}

@interface CocoaMouseView : NSView
{
    OIS::CocoaMouse *oisMouseObj;
    OIS::MouseState mTempState;
    bool mNeedsToRegainFocus;
	bool mMouseGrab;
	bool mMouseHide;
	NSTrackingArea* mTrackingArea;
}

- (void)setOISMouseObj:(OIS::CocoaMouse *)obj;
- (void)grabMouse;
- (void)hideMouse;
- (void)capture;

@end

#endif // OIS_CocoaMouse_H
