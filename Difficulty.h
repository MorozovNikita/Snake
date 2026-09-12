#pragma once

#include "Settings.h"

namespace Game
{
    struct DifficultyParams
    {
        float V = 7.f;  // speed
        int   P = 10;   // points
        int   L = 1;    // length per apple
    };

    inline constexpr float ReadyDelay = 3.f;

    inline constexpr int DifficultyCount = 5;

    inline constexpr DifficultyParams Difficulties[DifficultyCount] = {
        { 4.f,  10, 1 },
        { 5.5f, 15, 1 },
        { 7.f,  20, 1 },
        { 9.f,  30, 1 },
        { 12.f, 50, 1 },
    };

    inline const DifficultyParams& difficultyAt(int level)
    {
        if (level < 1)
            level = 1;
        if (level > DifficultyCount)
            level = DifficultyCount;
        return Difficulties[level - 1];
    }

    inline const DifficultyParams& currentDifficulty(const Settings& settings)
    {
        return difficultyAt(settings.difficultyLevel);
    }
}
