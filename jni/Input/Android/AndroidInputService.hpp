#include "Platform.hpp"

#ifdef ANDROID_PLATFORM

#ifndef ANDROIDINPUTSERVICE_HPP
#define ANDROIDINPUTSERVICE_HPP

#include "Types.hpp"

#include <vector>

namespace Core
{
	struct Finger;

	class AndroidInputService
	{
	public:
		AndroidInputService();
		~AndroidInputService();

		void ClearLinks();

		void Link_FUP(const Param2PtrCallbackStruct &link);
		void Link_FDOWN(const Param2PtrCallbackStruct &link);
		void Link_KEYBACK(const Param1PtrCallbackStruct &link);

		std::vector<Finger*> m_finger;

	public:
		bool onTouchEvent(AInputEvent* pEvent);
		bool onKeyBack();

	private:
		void UpdateFinger(GLuint id, Math::Vector2f pos);
		void AddFinger(GLuint id, Math::Vector2f pos);
		void DeleteFinger(GLuint id);

		std::vector<Param2PtrCallbackStruct> m_callbackFunc_FUP;
		std::vector<Param2PtrCallbackStruct> m_callbackFunc_FDOWN;

		std::vector<Param1PtrCallbackStruct> m_callbackFunc_KEYBACK;
	};
}
#endif

#endif

