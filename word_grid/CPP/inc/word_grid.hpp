#ifndef __WORD_GRID_HPP__
#define __WORD_GRID_HPP__

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include "cli_args.hpp"
#include "SimpleLogger.h"
#include "uo_set_utils.hpp"
#include "utils.h"


namespace Vas_LC_Puzzles {

    using vas_lib::utils::str_p;
    using vas_lib::utils::int_p;

    using vas_lib::utils::to_str_p;
    using vas_lib::utils::str_vec_p;
    using vas_lib::uo_set;

    struct WordGridSearch {
        using grid_vec = std::vector<str_vec_p>;

        struct Vertex {
            int x;
            int y;
        };

        Vertex v_fst[8] = {
            {-1, -1},
            {-1, 0},
            {-1, 1},
            {0, -1},
            {0, 1},
            {1, -1},
            {1, 0},
            {1, 1}
        };

        uo_set words;
        str_p grid;
        str_p dict_fname;
        SimpleLogger_p logger;
        std::ifstream dict_fd;
        Vertex st_pos;

        WordGridSearch(str_p dictfname, SimpleLogger_p logger) : dict_fname(dictfname),
                                                                logger(logger)
        {
            dict_fd.open(*dict_fname);
            if (dict_fd.is_open() != true) {
                throw std::runtime_error("Cannot open dictionary file");
            }
        }

        ~WordGridSearch()
        {
            if (dict_fd.is_open()) {
                dict_fd.close();
            }
        }

        int LoadWords(void);
        int LoadGrid(str_p gridstr);
        int FindWordsInGrid(void);
        int findLetterInGrid(const char ch);
        int findNextLetterInGrid(const char ch);
    };
    
}; // namespace Vas_LC_Puzzles

#endif /* __WORD_GRID_HPP__ */