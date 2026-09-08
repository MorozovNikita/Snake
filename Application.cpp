#include "Application.h"

#include "Constants.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "MenuState.h"
#include "GameState.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace Game
{

	const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

	Application::Application()
		: mWindow(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "Gameplay"s, sf::Style::Close)
		, mTextures()
		, mFonts()
		, mSoundBuffers()
		, mStateStack(State::Context(mWindow, mTextures, mFonts, mSoundBuffers))
		, mStatisticsText()
		, mStatisticsUpdateTime()
		, mStatisticsNumFrames(0)
	{
		mWindow.setKeyRepeatEnabled(false);

		mFonts.load(Fonts::Main, RESOURCES_PATH + "\\Fonts\\PressStart2P-Regular.ttf"s);

		mTextures.load(Textures::Grass, RESOURCES_PATH + "\\grass.png"s);
		mTextures.load(Textures::SnakeHead, RESOURCES_PATH + "\\head.png"s);
		mTextures.load(Textures::Wall, RESOURCES_PATH + "\\wall.jpg"s);

		mStatisticsText = std::make_unique<sf::Text>(mFonts.get(Fonts::Main));
		mStatisticsText->setPosition({ 5.f, 5.f });
		mStatisticsText->setCharacterSize(10u);

		registerStates();
		mStateStack.pushState(States::Menu);
	}

	void Application::run()
	{
		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		while (mWindow.isOpen())
		{
			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;
			while (timeSinceLastUpdate > TimePerFrame)
			{
				timeSinceLastUpdate -= TimePerFrame;

				processInput();
				update(TimePerFrame);

				// Check inside this loop, because stack might be empty before update() call
				if (mStateStack.isEmpty())
					mWindow.close();
			}

			updateStatistics(dt);
			render();
		}
	}

	void Application::processInput()
	{
		while (std::optional<sf::Event> event = mWindow.pollEvent())
		{
			mStateStack.handleEvent(*event);

			if (event->is<sf::Event::Closed>())
				mWindow.close();
		}
	}

	void Application::update(sf::Time dt)
	{
		mStateStack.update(dt);
	}

	void Application::render()
	{
		mWindow.clear();

		mStateStack.draw();

		mWindow.setView(mWindow.getDefaultView());
		mWindow.draw(*mStatisticsText);

		mWindow.display();
	}

	void Application::updateStatistics(sf::Time dt)
	{
		mStatisticsUpdateTime += dt;
		mStatisticsNumFrames += 1;
		if (mStatisticsUpdateTime >= sf::seconds(1.0f))
		{
			mStatisticsText->setString("FPS: " + std::to_string(mStatisticsNumFrames));

			mStatisticsUpdateTime -= sf::seconds(1.0f);
			mStatisticsNumFrames = 0;
		}
	}

	void Application::registerStates()
	{
		mStateStack.registerState<MenuState>(States::Menu);
		mStateStack.registerState<GameState>(States::Game);
	}

}
