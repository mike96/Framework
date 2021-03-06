#include "Platform.hpp"

#ifdef ANDROID_PLATFORM

#include "PGAndroidInput.hpp"

#include "Context.hpp"
#include "Camera2D.hpp"

PGAndroidInput::PGAndroidInput()
{
	m_requestExit = false;
	Global::pContext->pInputService->Link_KEYBACK(Param1PtrCallbackStruct(onBackKey,this));

	m_joystickInnerTex=new Texture2D;
	m_joystickOuterTex=new Texture2D;

	m_joystickInnerTex->Load("@Sprites/joystick_inner.png");
	m_joystickOuterTex->Load("@Sprites/joystick_outer.png");

	//Joystick setup
	m_joystick=new Joystick;
	m_joystick->SetTextures(m_joystickInnerTex,m_joystickOuterTex);
	m_joystick->SetMaxDistance(100.0f);

	//Setup input surface
	m_fingers[0]=m_fingers[1]=NULL;
	Global::pContext->pInputService->Link_FDOWN(Param2PtrCallbackStruct(DOWN_callback,this));
	Global::pContext->pInputService->Link_FUP(Param2PtrCallbackStruct(UP_callback,this));
}

PGAndroidInput::~PGAndroidInput()
{
	delete m_joystickInnerTex;
	delete m_joystickOuterTex;

	delete m_joystick;
}

void PGAndroidInput::Update(GLfloat delta)
{
	m_joystick->Update();

	if(m_fingers[1])
	{
		GLfloat firstDistance=m_firstScalePosition[0].Distance(m_firstScalePosition[1]);
		GLfloat currentDistance=m_fingers[0]->m_pos.Distance(m_fingers[1]->m_pos);
		GLfloat ratio=currentDistance/firstDistance;
		Global::pActiveCamera->SetScale(m_firstScale*ratio);

		GLfloat lastAngle=(m_lastFramePosition[1]-m_lastFramePosition[0]).Angle();
		GLfloat	currentAngle=(m_fingers[1]->m_pos-m_fingers[0]->m_pos).Angle();
		GLfloat angle=currentAngle-lastAngle;
		Global::pActiveCamera->RotateDirection(angle);
	}

	if(m_fingers[0])
	{
		m_lastFramePosition[0]=m_fingers[0]->m_pos;
	}
	if(m_fingers[1])
	{
		m_lastFramePosition[1]=m_fingers[1]->m_pos;
	}
}

void PGAndroidInput::Render()
{
	m_joystick->Render();
}

Vector2f PGAndroidInput::GetMovementDirection() const
{
	return (m_joystick->m_dir*5.0f).Rotated(Global::pActiveCamera->GetDirection().Angle());
}

Vector2f PGAndroidInput::GetShootingDirection() const
{
	return Vector2f();
}

bool PGAndroidInput::IsUserRequestingExit() const
{
	return m_requestExit;
}

void PGAndroidInput::onBackKey(void *pointer)
{
	PGAndroidInput *pPGAndroidInput=(PGAndroidInput*)(pointer);
	pPGAndroidInput->m_requestExit=true;
}

void PGAndroidInput::DOWN_callback(void *param1, void *param2)
{
	PGAndroidInput *pPGAndroidInput=(PGAndroidInput*)(param1);
	Finger *pFinger=(Finger*)(param2);
	if(pFinger->m_flag==Finger::LOCKED)
	{
		return;
	}
	if(pPGAndroidInput->m_fingers[0]==NULL)
	{
		pFinger->m_flag=Finger::LOCKED;
		pPGAndroidInput->m_lastFramePosition[0]=pFinger->m_pos;
		pPGAndroidInput->m_fingers[0]=pFinger;
		return;
	}
	if(pPGAndroidInput->m_fingers[1]==NULL)
	{
		pFinger->m_flag==Finger::LOCKED;
		pPGAndroidInput->m_firstScalePosition[0]=pPGAndroidInput->m_fingers[0]->m_pos;
		pPGAndroidInput->m_firstScalePosition[1]=pFinger->m_pos;
		pPGAndroidInput->m_lastFramePosition[1]=pFinger->m_pos;
		pPGAndroidInput->m_fingers[1]=pFinger;
		pPGAndroidInput->m_firstScale=Global::pActiveCamera->GetScale();
	}
}

void PGAndroidInput::UP_callback(void *param1, void *param2)
{
	PGAndroidInput *pPGAndroidInput=(PGAndroidInput*)(param1);
	Finger *pFinger=(Finger*)(param2);
	if(pPGAndroidInput->m_fingers[1]==pFinger)
	{
		pFinger->m_flag=Finger::FREE;
		pPGAndroidInput->m_fingers[1]=NULL;
		return;
	}
	if(pPGAndroidInput->m_fingers[0]==pFinger)
	{
		pFinger->m_flag==Finger::FREE;
		if(pPGAndroidInput->m_fingers[1]!=NULL)
		{
			pPGAndroidInput->m_fingers[0]=pPGAndroidInput->m_fingers[1];
			pPGAndroidInput->m_lastFramePosition[0]=pPGAndroidInput->m_lastFramePosition[1];
			pPGAndroidInput->m_fingers[1]=NULL;
		}
		else
		{
			pPGAndroidInput->m_fingers[0]=NULL;
		}
	}
}

#endif
