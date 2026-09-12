#pragma once

#include <SFML/Audio/Music.hpp>

namespace Game
{
    struct Settings;

    class MusicPlayer
    {
    public:
        enum class Id
        {
            None,
            Menu,
            Game,
        };

        explicit MusicPlayer(Settings& settings);

        void play(Id id);
        void stop();
        void pause();
        void resume();
        void apply();

        Id current() const { return mCurrent; }

    private:
        bool enabled(Id id) const;

        Settings& mSettings;
        sf::Music mMusic;
        Id mRequested = Id::None;
        Id mCurrent = Id::None;
    };
}
