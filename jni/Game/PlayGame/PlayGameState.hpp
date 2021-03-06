#ifndef PLAYGAME_HPP
#define PLAYGAME_HPP

#include "GameState.hpp"
#include "PGInputController.hpp"
#include "Texture2D.hpp"
#include "Sprite.hpp"
#include "AnimatedSprite.hpp"
#include "TiledBackground.hpp"
#include "Particles.hpp"

#include "Player.hpp"
#include "Enemy.hpp"

using namespace Core;

namespace Game
{
	class PlayGame: public GameState
	{
	public:
		PlayGame();

		int Update();
		void Render();

		~PlayGame();

	private:
		static void Physics_callback(void *param1, void *param2);

		PGInputController	*m_pPGInputController;

		Texture2D		*m_backgroundTexture;
		TiledBackground	*m_background;

		Texture2D 		*m_playerTexture;
		Sprite			*m_playerSprite;
		Player			*m_playerObject;

		Texture2D 		*m_enemyTexture;
		Sprite			*m_enemySprite;
		Enemy			*m_enemyObject;

		Texture2D				*m_particleTex;
		ParticleEmitterCircle 	*m_pEmitter;

		Texture2D		*m_testTexture;
		AnimatedSprite	*m_testSprite;
	};
}

#endif
