#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "StateStack.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Game
{

	class Application
	{
	public:
		Application();
		void					run();

	private:
		void					processInput();
		void					update(sf::Time dt);
		void					render();

		void					updateStatistics(sf::Time dt);
		void					registerStates();

	private:
		static const sf::Time	TimePerFrame;

		sf::RenderWindow		mWindow;
		TextureHolder			mTextures;
		FontHolder				mFonts;
		SoundBufferHolder		mSoundBuffers;

		StateStack				mStateStack;

		std::unique_ptr<sf::Text> mStatisticsText;
		sf::Time				  mStatisticsUpdateTime;
		std::size_t				  mStatisticsNumFrames;
	};

}