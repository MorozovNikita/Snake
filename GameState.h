#pragma once

#include "State.h"
#include "Grid.h"
#include "Snake.h"
#include "Apple.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Game
{
    class GameState : public State
    {
    public:
        GameState(StateStack& stack, State::Context& context);
        ~GameState() override;

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        void finish(sf::Sound& jingle);
        void openGameOver();
        void refreshHud();
        void refreshReadyText();
        bool isReady() const { return mReadyRemaining > 0.f; }

        Grid mGrid;
        Snake mSnake;
        Apple mApple;

        int mScore = 0;
        int mApplesEaten = 0;
        float mReadyRemaining = 0.f;
        bool mWon = false;
        bool mOpenedGameOver = false;

        sf::Sprite mTrophyIcon;
        sf::Sprite mAppleIcon;
        sf::Text mScoreText;
        sf::Text mApplesText;
        sf::Text mReadyText;
        sf::RectangleShape mReadyDimmer;

        sf::Sound mEatSound;
        sf::Sound mDeathSound;
        sf::Sound mWinSound;
        sf::Sound* mEndSound = nullptr;
    };
}
