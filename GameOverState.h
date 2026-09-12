#pragma once

#include "State.h"
#include "Leaderboard.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>
#include <vector>

namespace Game
{
    class GameOverState : public State
    {
    public:
        GameOverState(StateStack& stack, State::Context& context);
        ~GameOverState() override;

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        enum class Phase
        {
            AskName,
            TypeName,
            Actions,
        };

        void setPhase(Phase phase);
        void refreshTable();
        void refreshButtons();
        void applyAskName();
        void applyTypedName();
        void applyAction();
        void restartGame();
        void backToMenu();

        Phase mPhase = Phase::Actions;
        int mSelected = 0;
        std::string mName = "noname";
        bool mNameIsDefault = true;

        sf::RectangleShape mDimmer;
        sf::RectangleShape mPanel;
        sf::Text mTitle;
        sf::Text mScoreText;
        sf::Text mPrompt;
        std::vector<sf::Text> mTable;
        std::vector<sf::Text> mButtons;
        sf::RectangleShape mNameBox;
        sf::Text mNameText;
    };
}
