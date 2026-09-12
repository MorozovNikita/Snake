#include "Leaderboard.h"

#include <algorithm>
#include <fstream>

namespace Game
{
    Leaderboard::Leaderboard(std::string path)
        : mPath(std::move(path))
    {
        load();
    }

    void Leaderboard::sortAndTrim()
    {
        std::sort(mRecords.begin(), mRecords.end(), [](const Record& a, const Record& b)
        {
            return a.score > b.score;
        });
        if (static_cast<int>(mRecords.size()) > StoredCount)
            mRecords.resize(static_cast<std::size_t>(StoredCount));
    }

    void Leaderboard::load()
    {
        mRecords.clear();
        std::ifstream in(mPath);
        if (!in)
            return;

        std::string line;
        while (std::getline(in, line))
        {
            if (line.empty())
                continue;

            const auto tab = line.find('\t');
            if (tab == std::string::npos)
                continue;

            Record record;
            try
            {
                record.score = std::stoi(line.substr(0, tab));
            }
            catch (...)
            {
                continue;
            }
            record.name = line.substr(tab + 1);
            if (record.name.empty())
                record.name = "noname";
            mRecords.push_back(std::move(record));
        }

        sortAndTrim();
    }

    void Leaderboard::save() const
    {
        std::ofstream out(mPath, std::ios::trunc);
        if (!out)
            return;

        for (const auto& record : mRecords)
            out << record.score << '\t' << record.name << '\n';
    }

    bool Leaderboard::isRecord(int score) const
    {
        if (static_cast<int>(mRecords.size()) < StoredCount)
            return true;
        return score > mRecords.back().score;
    }

    void Leaderboard::add(std::string name, int score)
    {
        if (name.empty())
            name = "noname";
        mRecords.push_back(Record{ std::move(name), score });
        sortAndTrim();
    }
}
