#include "MusicPlayer.h"

#include "Constants.h"
#include "Settings.h"

#include <filesystem>
#include <stdexcept>
#include <string>

namespace Game
{
    MusicPlayer::MusicPlayer(Settings& settings)
        : mSettings(settings)
    {
    }

    bool MusicPlayer::enabled(Id id) const
    {
        switch (id)
        {
        case Id::Menu: return mSettings.menuMusic;
        case Id::Game: return mSettings.gameMusic;
        default:       return false;
        }
    }

    void MusicPlayer::stop()
    {
        mMusic.stop();
        mCurrent = Id::None;
    }

    void MusicPlayer::pause()
    {
        if (mMusic.getStatus() == sf::SoundSource::Status::Playing)
            mMusic.pause();
    }

    void MusicPlayer::resume()
    {
        if (mMusic.getStatus() == sf::SoundSource::Status::Paused)
            mMusic.play();
    }

    void MusicPlayer::apply()
    {
        play(mRequested);
    }

    void MusicPlayer::play(Id id)
    {
        mRequested = id;
        stop();
        if (id == Id::None || !enabled(id))
            return;

        const std::string stem = (id == Id::Menu) ? "menuMusic"s : "gameMusic"s;
        const std::string base = RESOURCES_PATH + stem;

        constexpr const char* exts[] = { ".ogg", ".wav", ".flac", ".mp3" };

        bool opened = false;
        bool hadMp3 = false;
        for (const char* ext : exts)
        {
            const std::filesystem::path path = base + ext;
            if (!std::filesystem::exists(path))
                continue;
            if (ext == ".mp3"s)
                hadMp3 = true;
            if (mMusic.openFromFile(path))
            {
                opened = true;
                break;
            }
        }

        if (!opened)
        {
            std::string message = "MusicPlayer: failed to open " + base + " (.ogg/.wav/.flac)";
            if (hadMp3)
                message += ". SFML does not support MP3 — convert the file to OGG or WAV.";
            throw std::runtime_error(message);
        }

        mMusic.setLooping(true);
        mMusic.setVolume(40.f);
        mMusic.play();
        mCurrent = id;
    }
}
