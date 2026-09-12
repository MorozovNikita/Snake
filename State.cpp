#include "State.h"

#include "StateStack.h"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
	SoundBufferHolder& soundBuffers, Game::MusicPlayer& music, Game::Settings& settings,
	Game::Leaderboard& leaderboard, Game::SessionResult& session)
	: window(window)
	, textures(textures)
	, fonts(fonts)
	, soundBuffers(soundBuffers)
	, music(music)
	, settings(settings)
	, leaderboard(leaderboard)
	, session(session)
{
}

State::State(StateStack& stack, Context& context)
	: mStack(&stack)
	, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}