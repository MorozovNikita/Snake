#pragma once

#include <string>
#include <vector>

namespace Game
{
    struct Record
    {
        std::string name;
        int score = 0;
    };

    struct SessionResult
    {
        int score = 0;
        bool won = false;
    };

    class Leaderboard
    {
    public:
        static constexpr int StoredCount = 10;
        static constexpr int PopupCount = 5;

        explicit Leaderboard(std::string path);

        void load();
        void save() const;

        bool isRecord(int score) const;
        void add(std::string name, int score);

        const std::vector<Record>& records() const { return mRecords; }

    private:
        void sortAndTrim();

        std::string mPath;
        std::vector<Record> mRecords;
    };
}
