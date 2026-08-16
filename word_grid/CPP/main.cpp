#include <iostream>
#include <memory>
#include "cli_args.hpp"
#include "SimpleLogger.h"
#include <assert.h>
#include "word_grid.hpp"


int main(int ac, char* av[])
{
    auto logger = SimpleLogger::getLogger();
    logger->enableConsoleLogging();

    try {
        auto argparser = ArgParser::CLI_Args(ac, av);
        auto help_flag = argparser.add_flag("-h", "--help", "Print help and exit");
        auto verbose_log = argparser.add_int("-v", "--verbose", "Logging level", INFO);
        auto dict_fname = argparser.add_str("", "--dict-fname", "dictionary file name");
        auto grid_str = argparser.add_str("", "--grid", "4x4 grid as 16 characters");        
        argparser.parse_args();

        logger->setLevel(*verbose_log);
        if (*help_flag) {
            std::cout << "Usage: " << av[0] << " <args>\n";
            std::cout << *argparser.help();
            exit(0);
        }

        auto wg = std::make_shared<Vas_LC_Puzzles::WordGridSearch>(dict_fname, logger);
        wg->LoadWords();
        wg->LoadGrid(grid_str);
        wg->FindWordsInGrid();

        return 0;
    } catch (std::exception& e) {
        LOG_Error("Exception: %s", e.what());
        return -1;
    }
}