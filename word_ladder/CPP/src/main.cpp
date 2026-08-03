#include <iostream>
#include <memory>
#include "cli_args.hpp"
#include "SimpleLogger.h"
#include "word_list_impl.hpp"


using ArgParser::str_p;
using ArgParser::int_p;


struct WordLadder {
    str_p dict_fname_;
    std::vector<str_p> words_;
    ManagedWordList_p word_list_;

    WordLadder(str_p dictfname) : dict_fname_(dictfname) {}

    int loadWords(void);
    int findWordLadder(std::string inword);
    int printWords(void);
};


int WordLadder::loadWords(void)
{
    word_list_ = ManagedWordList::make(*dict_fname_);
    word_list_->ReadList();
    word_list_->PrintWords();
    return 0;
}


int WordLadder::printWords(void)
{
    for (auto word : words_) {
        std::cout << word << "\n";
    }

    return 0;
}


int main(int ac, char* av[])
{
    auto logger = SimpleLogger::getLogger();
    logger->enableConsoleLogging();

    try {
        auto argparser = ArgParser::CLI_Args(ac, av);
        auto help_flag = argparser.add_flag("-h", "--help", "Print help and exit");
        auto verbose_log = argparser.add_int("-v", "--verbose", "Logging level", INFO);
        auto dict_fname = argparser.add_str("", "--dict-fname", "dictionary file name");
        argparser.parse_args();

        logger->setLevel(*verbose_log);
        if (*help_flag) {
            std::cout << "Usage: " << av[0] << " <args>\n";
            std::cout << *argparser.help();
            exit(0);
        }


        LOG_Info("TODO: Implement word_ladder in CPP");
        LOG_Debug("Using dictionary file %s", dict_fname->c_str());
        auto wl = std::make_shared<WordLadder>(dict_fname);
        wl->loadWords();

    } catch (std::exception &exc) {
        LOG_Error("Ran into error: %s", exc.what());
    }

    return 0;
}


/* End of file */
