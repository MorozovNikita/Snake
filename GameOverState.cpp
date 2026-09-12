#include "GameOverState.h"

#include "Constants.h"
#include "Leaderboard.h"
#include "MusicPlayer.h"
#include "ResourceHolder.h"
#include "Settings.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <string>

namespace Game
{
    namespace
    {
        void centerText(sf::Text& text, float x, float y)
        {
            const auto bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f);
            text.setPosition({ x, y });
        }

        std::string formatRow(int place, const Record& record)
        {
            std::string name = record.name;
            if (name.size() > 10)
                name.resize(10);
            return std::to_string(place) + ". "s + name + "  "s + std::to_string(record.score);
        }
    }

    GameOverState::GameOverState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mDimmer({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) })
        , mPanel({ 560.f, 440.f })
        , mTitle(context.fonts.get(Fonts::Main), context.session.won ? "You win"s : "Game over"s, 32)
        , mScoreText(context.fonts.get(Fonts::Main), "Score "s + std::to_string(context.session.score), 16)
        , mPrompt(context.fonts.get(Fonts::Main), ""s, 16)
        , mNameBox({ 360.f, 36.f })
        , mNameText(context.fonts.get(Fonts::Main), "noname"s, 16)
    {
        mDimmer.setFillColor(sf::Color(0, 0, 0, 160));

        mPanel.setFillColor(sf::Color(24, 24, 24));
        mPanel.setOutlineColor(sf::Color::Green);
        mPanel.setOutlineThickness(3.f);
        mPanel.setOrigin(mPanel.getSize() / 2.f);
        mPanel.setPosition({ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });

        centerText(mTitle, SCREEN_WIDTH / 2.f, 110.f);
        mTitle.setFillColor(sf::Color::White);
        centerText(mScoreText, SCREEN_WIDTH / 2.f, 150.f);
        mScoreText.setFillColor(sf::Color::White);
        mPrompt.setFillColor(sf::Color::Green);

        mNameBox.setFillColor(sf::Color(10, 10, 10));
        mNameBox.setOutlineColor(sf::Color::Green);
        mNameBox.setOutlineThickness(2.f);
        mNameBox.setOrigin(mNameBox.getSize() / 2.f);
        mNameBox.setPosition({ SCREEN_WIDTH / 2.f, 430.f });
        mNameText.setFillColor(sf::Color::White);

        refreshTable();

        getContext().music.play(MusicPlayer::Id::Menu);
        setPhase(context.leaderboard.isRecord(context.session.score) ? Phase::AskName : Phase::Actions);
    }

    GameOverState::~GameOverState()
    {
        getContext().window.setKeyRepeatEnabled(false);
    }

    void GameOverState::refreshTable()
    {
        mTable.clear();
        const auto& records = getContext().leaderboard.records();
        const int count = std::min(Leaderboard::PopupCount, static_cast<int>(records.size()));
        for (int i = 0; i < count; ++i)
        {
            sf::Text row(getContext().fonts.get(Fonts::Main),
                formatRow(i + 1, records[static_cast<std::size_t>(i)]), 14);
            row.setFillColor(sf::Color::White);
            centerText(row, SCREEN_WIDTH / 2.f, 190.f + i * 28.f);
            mTable.push_back(std::move(row));
        }
    }

    void GameOverState::setPhase(Phase phase)
    {
        mPhase = phase;
        mButtons.clear();
        auto& font = getContext().fonts.get(Fonts::Main);

        if (phase == Phase::AskName)
        {
            mPrompt.setString("Save record?"s);
            mButtons.emplace_back(font, "Yes"s, 22);
            mButtons.emplace_back(font, "No"s, 22);
            mSelected = 1;
            getContext().window.setKeyRepeatEnabled(false);
        }
        else if (phase == Phase::TypeName)
        {
            mPrompt.setString("Enter name"s);
            mName = "noname";
            mNameIsDefault = true;
            mNameText.setString(mName);
            mSelected = 0;
            getContext().window.setKeyRepeatEnabled(true);
        }
        else
        {
            mPrompt.setString(""s);
            mButtons.emplace_back(font, "Play again"s, 22);
            mButtons.emplace_back(font, "Main menu"s, 22);
            mSelected = 0;
            getContext().window.setKeyRepeatEnabled(false);
        }

        centerText(mPrompt, SCREEN_WIDTH / 2.f, 350.f);
        refreshButtons();
    }

    void GameOverState::refreshButtons()
    {
        const float startY = (mPhase == Phase::AskName) ? 390.f : 380.f;
        for (int i = 0; i < static_cast<int>(mButtons.size()); ++i)
        {
            mButtons[static_cast<std::size_t>(i)].setFillColor(
                i == mSelected ? sf::Color::Green : sf::Color::White);
            centerText(mButtons[static_cast<std::size_t>(i)], SCREEN_WIDTH / 2.f, startY + i * 36.f);
        }

        const auto nameBounds = mNameText.getLocalBounds();
        mNameText.setOrigin({ nameBounds.position.x, nameBounds.position.y + nameBounds.size.y / 2.f });
        mNameText.setPosition({ SCREEN_WIDTH / 2.f - 160.f, 430.f });
    }

    void GameOverState::applyAskName()
    {
        if (mSelected == 0)
            setPhase(Phase::TypeName);
        else
            setPhase(Phase::Actions);
    }

    void GameOverState::applyTypedName()
    {
        std::string name = mName;
        if (name.empty())
            name = "noname";
        getContext().leaderboard.add(std::move(name), getContext().session.score);
        refreshTable();
        setPhase(Phase::Actions);
    }

    void GameOverState::restartGame()
    {
        requestStackPop();
        requestStackPop();
        requestStackPush(States::Game);
    }

    void GameOverState::backToMenu()
    {
        requestStackPop();
        requestStackPop();
    }

    void GameOverState::applyAction()
    {
        if (mSelected == 0)
            restartGame();
        else
            backToMenu();
    }

    void GameOverState::draw()
    {
        auto& window = getContext().window;
        window.draw(mDimmer);
        window.draw(mPanel);
        window.draw(mTitle);
        window.draw(mScoreText);
        for (const auto& row : mTable)
            window.draw(row);
        window.draw(mPrompt);

        if (mPhase == Phase::TypeName)
        {
            window.draw(mNameBox);
            window.draw(mNameText);
        }
        else
        {
            for (const auto& button : mButtons)
                window.draw(button);
        }
    }

    bool GameOverState::update(sf::Time)
    {
        return false;
    }

    bool GameOverState::handleEvent(const sf::Event& event)
    {
        const auto& input = getContext().settings.input;

        if (mPhase == Phase::TypeName)
        {
            if (const auto* text = event.getIf<sf::Event::TextEntered>())
            {
                const auto unicode = text->unicode;
                if (unicode == '\b')
                {
                    if (!mName.empty())
                    {
                        mName.pop_back();
                        mNameIsDefault = false;
                    }
                }
                else if (unicode >= 32 && unicode < 127 && unicode != '\r' && unicode != '\n')
                {
                    if (mNameIsDefault)
                    {
                        mName.clear();
                        mNameIsDefault = false;
                    }
                    if (mName.size() < 12)
                        mName += static_cast<char>(unicode);
                }
                mNameText.setString(mName);
                refreshButtons();
                return false;
            }
        }

        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return false;

        if (mPhase == Phase::TypeName)
        {
            if (keyPressed->code == input.confirm)
                applyTypedName();
            else if (keyPressed->code == sf::Keyboard::Key::Backspace && !mName.empty())
            {
                mName.pop_back();
                mNameIsDefault = false;
                mNameText.setString(mName);
                refreshButtons();
            }
            return false;
        }

        if (keyPressed->code == input.menuUp)
        {
            mSelected = (mSelected + static_cast<int>(mButtons.size()) - 1) % static_cast<int>(mButtons.size());
            refreshButtons();
        }
        else if (keyPressed->code == input.menuDown)
        {
            mSelected = (mSelected + 1) % static_cast<int>(mButtons.size());
            refreshButtons();
        }
        else if (keyPressed->code == input.confirm)
        {
            if (mPhase == Phase::AskName)
                applyAskName();
            else
                applyAction();
        }
        else if (keyPressed->code == input.back)
        {
            if (mPhase == Phase::AskName)
                setPhase(Phase::Actions);
            else
                backToMenu();
        }

        return false;
    }
}
