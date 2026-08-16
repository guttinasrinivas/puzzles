#include "word_grid.hpp"


int Vas_LC_Puzzles::WordGridSearch::LoadWords(void)
{
    std::string line;
    while (std::getline(dict_fd, line)) {
        words.insert(to_str_p(line));
    }
    return 0;
}


int Vas_LC_Puzzles::WordGridSearch::LoadGrid(str_p gridstr)
{
    if (gridstr->length() != 16) {
        LOG_Error("For now only 4x4 grid is supported. Cannot process given grid.");
        return -1;
    }

    std::stringstream ss;
    for (int ii = 0; ii < 16; ii += 4) {
        ss << gridstr->substr(ii, 4) << std::endl;
    }

    LOG_Info("Loaded Grid:\n%s", ss.str().c_str());

    grid = gridstr;

    return 0;
}

int Vas_LC_Puzzles::WordGridSearch::FindWordsInGrid(void)
{
    for (auto word : words) {
        auto idx = 0;
        LOG_Debug("Checking word: %s", word->c_str());
        auto ret = findLetterInGrid(word->at(idx));

        if (ret != 0) {
            LOG_Debug("Word %s not found in grid", word->c_str());
            continue;
        }

        for (idx = 1; idx < word->length(); idx++) {
            ret = findNextLetterInGrid(word->at(idx));
            if (ret != 0) {
                LOG_Debug("Word %s not found in grid", word->c_str());
                break;
            }
        }

        if (idx == word->length()) {
            LOG_Info("Found word: %s", word->c_str());
        }
    }

    return 0;
}

int Vas_LC_Puzzles::WordGridSearch::findLetterInGrid(const char ch)
{
    for (int ii = 0; ii < grid->length(); ii++) {
        int xx = ii / 4;
        int yy = ii % 4;

        if (grid->at(ii) == ch) {
            LOG_Debug("Found letter %c at (%d, %d)", ch, xx, yy);
            st_pos.x = xx;
            st_pos.y = yy;
            return 0;
        }
    }

    LOG_Debug("Letter %c not found in grid", ch);
    return -1;
}


int Vas_LC_Puzzles::WordGridSearch::findNextLetterInGrid(const char ch)
{
    for (auto ii: v_fst) {
        Vertex pos = {st_pos.x + ii.x, st_pos.y + ii.y};
        if (pos.x < 0 || pos.x >= 4 || pos.y < 0 || pos.y >= 4) {
            continue;
        }

        if (grid->at(pos.x * 4 + pos.y) == ch) {
            st_pos = pos;
            return 0;
        }
    }

    return -1;
}

/* End of file */