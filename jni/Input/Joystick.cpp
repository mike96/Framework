#include "Joystick.hpp"

#include "Render.hpp"

Joystick::Joystick(Vector2f point, GLfloat radius)
	: m_pFinger(NULL)
{
	m_actionCircleCenter=point;
	m_actionCircleRadius=radius;

	Global::pContext->pInputService->Link_FDOWN(Param2PtrCallbackStruct(DOWN_callback,this));
	Global::pContext->pInputService->Link_FUP(Param2PtrCallbackStruct(UP_callback,this));
}

void Joystick::SetTextures(Texture2D *innerTex, Texture2D *outerTex)
{
	m_outerSprite.SetTexture(outerTex);
	m_innerSprite.SetTexture(innerTex);

	m_outerSprite.SetSize(70.0f,70.0f);
	m_innerSprite.SetSize(60.0f,60.0f);

	m_outerSprite.m_spriteShadingType=SpriteVertex::ALPHA_BLEND;
	m_innerSprite.m_spriteShadingType=SpriteVertex::ALPHA_BLEND;
}

void Joystick::SetMaxDistance(GLfloat maxDist)
{
	m_maxDistance=maxDist;
}

void Joystick::Update()
{
	if(m_pFinger)
	{
		m_currentPos=m_pFinger->m_pos;
		m_dir=m_currentPos-m_firstPos;
		if(m_dir.Length()>m_maxDistance)
		{
			m_dir.Normalize();
			m_dir*=m_maxDistance;
			m_currentPos=m_firstPos+m_dir;
		}
	}
}

void Joystick::Render()
{
	if(m_pFinger)
	{
		m_outerSprite.m_position=m_firstPos;
		m_innerSprite.m_position=m_currentPos;

		m_outerSprite.Render();
		m_innerSprite.Render();
	}
}

Joystick::~Joystick()
{
}

void Joystick::DOWN_callback(void *param1, void *param2)
{
	Joystick *pJoystick=(Joystick*)(param1);
	if(!pJoystick->m_pFinger)
	{
		Finger *pFinger=(Finger*)(param2);
		if(pJoystick->m_actionCircleCenter.Distance(pFinger->m_pos)<=pJoystick->m_actionCircleRadius)
		{
			pJoystick->m_pFinger=pFinger;
			pJoystick->m_firstPos=pFinger->m_pos;
		}
	}
}

void Joystick::UP_callback(void *param1, void *param2)
{
	Joystick *pJoystick=(Joystick*)(param1);
	Finger *pFinger=(Finger*)(param2);
	if(pJoystick->m_pFinger==pFinger)
	{
		pJoystick->m_pFinger=NULL;
		pJoystick->m_dir=Vector2f();
	}
}
