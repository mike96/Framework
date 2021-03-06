#include "Platform.hpp"

#ifdef ANDROID_PLATFORM

#ifndef ANDROIDEVENTLOOP_HPP
#define ANDROIDEVENTLOOP_HPP

#include "Types.hpp"
#include "EventLoop.hpp"

namespace Core
{
	class ActivityHandler;
}

namespace Core
{
    class AndroidEventLoop : public EventLoop
    {
    public:
        AndroidEventLoop(void *data);
        Status Run(ActivityHandler* pActivityHandler);

    protected:
        void Activate();
        void Deactivate();

        void ProcessAppEvent(int32_t pCommand);
        int32_t ProcessInputEvent(AInputEvent* pEvent);
        void ProcessSensorEvent();

        Status InitializeDisplay();
        void DestroyDisplay();

    private:
        // Private callbacks handling events occuring in the thread loop.
        static void callback_event(android_app* pApplication, int32_t pCommand);
        static int32_t callback_input(android_app* pApplication, AInputEvent* pEvent);

    private:
        // Saves application state when application is active/paused.
        bool mEnabled;
        // Indicates if the event handler wants to exit.
        bool mQuit;

        EGLDisplay mDisplay;
		EGLSurface mSurface;
		EGLContext mContext;
    };
}
#endif

#endif

