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

#ifndef OIS_CocoaKeyboard_H
#define OIS_CocoaKeyboard_H

#include "OISKeyboard.h"
#include "mac/CocoaHelpers.h"

#include <list>
#include <Cocoa/Cocoa.h>

@class CocoaKeyboardView;

namespace OIS
{
    class CocoaKeyStackEvent
    {
        friend class CocoaKeyboard;
        
    public:
        inline CocoaKeyStackEvent(KeyEvent event, MacEventType type) :
			Event(event),
			Type(type)
		{
			// Nothing to do in here
		}

        inline const MacEventType type()
		{
			return Type;
		}

        inline const KeyEvent event()
		{
			return Event;
		}

    private:
        MacEventType Type;
        KeyEvent Event;

    };
    
    class CocoaKeyboard : public Keyboard
    {
    public:
        CocoaKeyboard(InputManager* creator, bool buffered, bool repeat);

        virtual ~CocoaKeyboard();

        // Sets buffered mode
        virtual void setBuffered(bool buffered) override;

        // Unbuffered keydown check
        virtual bool isKeyDown(KeyCode key) const override;

        // This will send listener events if buffered is on.
        // Note that in the mac implementation, unbuffered input is
        // automatically updated without calling this.
        virtual void capture() override;

        // Copies the current key buffer
        virtual void copyKeyStates(char keys[256]) const override;

        // Returns a description of the given key
        virtual std::string& getAsString(KeyCode key) override;

        virtual inline Interface* queryInterface(Interface::IType type) override
		{
			return nullptr;
		}

        // Public but reserved for internal use:
        virtual void _initialize() override;
        
        inline unsigned int & _getModifiers()
		{
			return mModifiers;
		}

    protected:
        CocoaKeyboardView* mResponder;
        std::string getString;
    };
}

typedef std::map<unsigned short, OIS::KeyCode> VirtualtoOIS_KeyMap;
typedef std::list<OIS::CocoaKeyStackEvent> eventStack;

@interface CocoaKeyboardView : NSResponder
{
    OIS::CocoaKeyboard *oisKeyboardObj;
    VirtualtoOIS_KeyMap keyConversion;

    char KeyBuffer[256];
    NSUInteger prevModMask;
    
    // buffered events, fifo stack
    eventStack pendingEvents;
    bool useRepeat;
}

- (void)setOISKeyboardObj:(OIS::CocoaKeyboard *)obj;
- (void)populateKeyConversion;
- (void)capture;
- (void)injectEvent:(OIS::KeyCode)kc eventTime:(unsigned int)time eventType:(OIS::MacEventType)type;
- (void)injectEvent:(OIS::KeyCode)kc eventTime:(unsigned int)time eventType:(OIS::MacEventType)type eventText:(unsigned int)txt;
- (void)copyKeyStates:(char [256])keys;
- (bool)isKeyDown:(OIS::KeyCode)key;
- (void)setUseRepeat:(bool)repeat;
- (VirtualtoOIS_KeyMap)keyConversionMap;

@end

#endif
