#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

namespace sf
{
	class RenderWindow;
}

namespace Game
{
	class MusicPlayer;
	class Leaderboard;
	struct Settings;
	struct SessionResult;
}

class StateStack;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
			    SoundBufferHolder& soundBuffers, Game::MusicPlayer& music, Game::Settings& settings,
			    Game::Leaderboard& leaderboard, Game::SessionResult& session);

		sf::RenderWindow&  window;
		TextureHolder&	   textures;
		FontHolder&		   fonts;
		SoundBufferHolder& soundBuffers;
		Game::MusicPlayer& music;
		Game::Settings&	   settings;
		Game::Leaderboard& leaderboard;
		Game::SessionResult& session;
	};

public:
	State(StateStack& stack, Context& context);
	virtual				~State();

	virtual void		draw() = 0;
	virtual bool		update(sf::Time dt) = 0;
	virtual bool		handleEvent(const sf::Event& event) = 0;

protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext() const;

private:
	StateStack* mStack;
	Context&	mContext;
};
