#include "GameState.h"

#include "Constants.h"
#include "Difficulty.h"
#include "Leaderboard.h"
#include "MusicPlayer.h"
#include "ResourceHolder.h"
#include "Settings.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cmath>
#include <string>

namespace Game
{
    namespace
    {
        constexpr float HudMargin = 16.f;
        constexpr float HudY = 10.f;
        constexpr float HudIconSize = 22.f;
        constexpr float HudIconTextGap = 6.f;
        constexpr float HudGroupGap = 18.f;

        void fitHudIcon(sf::Sprite& sprite, const sf::Texture& texture)
        {
            const auto size = texture.getSize();
            const float scale = HudIconSize / static_cast<float>(size.y);
            sprite.setScale({ scale, scale });
        }
    }

    GameState::GameState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mGrid(context.textures)
        , mSnake(context.textures, { 5, 5 },
            1.f / currentDifficulty(context.settings).V,
            SCREEN_WIDTH / CELL_SIZE, SCREEN_HEIGHT / CELL_SIZE)
        , mApple(context.textures)
        , mTrophyIcon(context.textures.get(Textures::Score))
        , mAppleIcon(context.textures.get(Textures::Apple))
        , mScoreText(context.fonts.get(Fonts::Main), "0"s, 16)
        , mApplesText(context.fonts.get(Fonts::Main), "0"s, 16)
        , mReadyText(context.fonts.get(Fonts::Main), ""s, 72)
        , mReadyDimmer({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) })
        , mEatSound(context.soundBuffers.get(SoundBuffers::Eat))
        , mDeathSound(context.soundBuffers.get(SoundBuffers::Death))
        , mWinSound(context.soundBuffers.get(SoundBuffers::Win))
    {
        mReadyRemaining = ReadyDelay;
        mReadyDimmer.setFillColor(sf::Color(0, 0, 0, 120));
        mScoreText.setFillColor(sf::Color::White);
        mApplesText.setFillColor(sf::Color::White);
        mReadyText.setFillColor(sf::Color::White);

        fitHudIcon(mTrophyIcon, context.textures.get(Textures::Score));
        fitHudIcon(mAppleIcon, context.textures.get(Textures::Apple));

        refreshHud();
        refreshReadyText();

        mApple.respawn(mGrid, mSnake);
        getContext().music.play(MusicPlayer::Id::Game);
    }

    GameState::~GameState()
    {
        getContext().music.play(MusicPlayer::Id::Menu);
    }

    void GameState::refreshHud()
    {
        mScoreText.setString(std::to_string(mScore));
        mApplesText.setString(std::to_string(mApplesEaten));

        float x = static_cast<float>(SCREEN_WIDTH) - HudMargin;
        const float textY = HudY + 3.f;

        auto placeGroup = [&](sf::Text& text, sf::Sprite& icon)
        {
            const auto textBounds = text.getLocalBounds();
            text.setOrigin({ textBounds.position.x + textBounds.size.x, textBounds.position.y });
            text.setPosition({ x, textY });
            x -= textBounds.size.x + HudIconTextGap;

            const auto iconBounds = icon.getGlobalBounds();
            icon.setPosition({ x - iconBounds.size.x, HudY });
            x -= iconBounds.size.x + HudGroupGap;
        };

        placeGroup(mScoreText, mTrophyIcon);
        placeGroup(mApplesText, mAppleIcon);
    }

    void GameState::refreshReadyText()
    {
        const int n = static_cast<int>(std::ceil(mReadyRemaining));
        mReadyText.setString(n > 0 ? std::to_string(n) : ""s);

        const auto bounds = mReadyText.getLocalBounds();
        mReadyText.setOrigin(bounds.position + bounds.size / 2.f);
        mReadyText.setPosition({ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });
    }

    void GameState::openGameOver()
    {
        if (mOpenedGameOver)
            return;

        mOpenedGameOver = true;
        getContext().session.score = mScore;
        getContext().session.won = mWon;
        requestStackPush(States::GameOver);
    }

    void GameState::finish(sf::Sound& jingle)
    {
        if (mEndSound || mOpenedGameOver)
            return;

        mWon = (&jingle == &mWinSound);
        getContext().music.stop();
        mEatSound.stop();

        if (!getContext().settings.sfx)
        {
            openGameOver();
            return;
        }

        mEndSound = &jingle;
        mEndSound->play();
    }

    void GameState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(40, 40, 40));

        mGrid.draw(window);
        mApple.draw(window);
        mSnake.draw(window);

        window.draw(mTrophyIcon);
        window.draw(mScoreText);
        window.draw(mAppleIcon);
        window.draw(mApplesText);

        if (isReady())
        {
            window.draw(mReadyDimmer);
            window.draw(mReadyText);
        }
    }

    bool GameState::update(sf::Time dt)
    {
        if (mEndSound)
        {
            if (mEndSound->getStatus() != sf::SoundSource::Status::Playing)
                openGameOver();
            return true;
        }

        if (isReady())
        {
            mReadyRemaining -= dt.asSeconds();
            if (mReadyRemaining < 0.f)
                mReadyRemaining = 0.f;
            refreshReadyText();
            return true;
        }

        mSnake.update(dt);

        if (auto entered = mSnake.popEnteredCell())
        {
            const Cell cell = mGrid.get(*entered);

            if (cell == Cell::Wall || mSnake.occupies(*entered, false))
            {
                finish(mDeathSound);
                return true;
            }

            if (cell == Cell::Apple)
            {
                const auto& difficulty = currentDifficulty(getContext().settings);
                mScore += difficulty.P;
                ++mApplesEaten;
                mSnake.grow(difficulty.L);
                refreshHud();

                if (getContext().settings.sfx)
                    mEatSound.play();

                if (!mApple.respawn(mGrid, mSnake))
                    finish(mWinSound);
            }
        }

        return true;
    }

    bool GameState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return true;

        if (mEndSound)
        {
            const auto& input = getContext().settings.input;
            if (keyPressed->code == input.back || keyPressed->code == input.confirm)
            {
                mEndSound->stop();
                openGameOver();
            }
            return false;
        }

        const auto& input = getContext().settings.input;
        if (keyPressed->code == input.moveUp)
            mSnake.setInput(Direction::Up);
        else if (keyPressed->code == input.moveDown)
            mSnake.setInput(Direction::Down);
        else if (keyPressed->code == input.moveLeft)
            mSnake.setInput(Direction::Left);
        else if (keyPressed->code == input.moveRight)
            mSnake.setInput(Direction::Right);
        else if (keyPressed->code == input.pause && !isReady())
            requestStackPush(States::Pause);
        else if (keyPressed->code == input.back)
            requestStackPop();

        return false;
    }
}
