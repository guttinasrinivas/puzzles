#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include "cli_args.hpp"
#include "SimpleLogger.h"
#include <assert.h>
#include "utils.h"
#include "uo_set_utils.hpp"


using vas_lib::utils::str_p;
using vas_lib::utils::int_p;

using vas_lib::utils::to_str_p;
using vas_lib::uo_set;


struct WordLadder {
    /* Utilities */
    /* Methods */
    WordLadder(str_p dictfname, SimpleLogger_p logger) : dict_fname_(dictfname),
                                                        logger(logger)
    {
    }

    int loadWords(void)
    {
        auto infp = std::ifstream(*dict_fname_);
        if (infp.is_open() != true) {
            throw std::runtime_error("Cannot open dictionary file");
        }

        std::string inln;
        while (std::getline(infp, inln)) {
            words_.insert(to_str_p(inln));
        }

        return 0;
    }

    int printWords(void)
    {
        for (auto word : words_) {
            std::cout << *word << "\n";
        }

        return 0;
    }

    bool lookUpWord(str_p inword)
    {
        auto found = words_.find(inword);
        if (found == words_.end()) {
            return false;
        }

        words_.erase(found);
        return true;
    }

    void enumerateWords(str_p inword)
    {
        LOG_Debug("Finding enumerations for: %s in %d words", inword->c_str(), (int) words_.size());

        for (size_t ii = 0; ii < inword->size(); ii++) {
            for (char ch = 'a'; ch <= 'z'; ch++) {
                str_p currword = to_str_p(*inword);
                auto nextword = to_str_p(currword->replace(ii, 1, 1, ch));

                if (!lookUpWord(nextword)) {
                    continue;
                }
                
                if (*nextword == *inword) {
                    throw std::runtime_error("Duplicate word");
                }

                word_tree_[*nextword] = inword;
                words_q_.push_back(nextword);
                LOG_Debug("Enumerated: %s", nextword->c_str());
            }
        }
    }

    void printLadder(str_p inword)
    {
        std::cout << "Ladder: " << *inword;
        auto it = word_tree_.find(*inword);
        while (it != word_tree_.end()) {
            std::cout << " <- " << *(it->second);
            it = word_tree_.find(*(it->second));
        }
    }

    bool sanityChecks(str_p startword, str_p endword)
    {
        if (startword->size() != endword->size()) {
            throw std::runtime_error("Start and end words must be of same size");
        }

        if (lookUpWord(startword)) {
            LOG_Warning("Found starting word %s in dictionary!!!", startword->c_str());
        }

        auto found = words_.find(endword);
        if (found == words_.end()) {
            LOG_Error("End target word %s not found in dictionary", endword->c_str());
            throw std::runtime_error("End word not found in dictionary");
        }

        if (*startword == *endword) {
            LOG_Warning("Start word %s is the end target word %s!!!", startword->c_str(), endword->c_str());
            return false;
        }

        return true;
    }

    bool findLadder(str_p startword, str_p endword)
    {
        LOG_Info("Attempting for find a ladder from %s to %s...", startword->c_str(), endword->c_str());
        if (!sanityChecks(startword, endword)) {
            return false;
        }

        words_q_.push_back(startword);
        while (!words_q_.empty()) {
            auto currword = words_q_.front();
            words_q_.erase(words_q_.begin());
            if (*currword == *endword) {
                LOG_Info("Found ladder");
                printLadder(currword);
                return true;
            }
            enumerateWords(currword);
            LOG_Debug("Queue Depth: %ld", words_q_.size());
        }

        LOG_Warning("Could not find a viable path");

        return false;
    }

    /* Members */
    str_p dict_fname_;
    uo_set words_;
    std::map<std::string, str_p> word_tree_;
    std::vector<str_p> words_q_;
    SimpleLogger_p logger;
};


int main(int ac, char* av[])
{
    auto logger = SimpleLogger::getLogger();
    logger->enableConsoleLogging();

    try {
        auto argparser = ArgParser::CLI_Args(ac, av);
        auto help_flag = argparser.add_flag("-h", "--help", "Print help and exit");
        auto verbose_log = argparser.add_int("-v", "--verbose", "Logging level", INFO);
        auto dict_fname = argparser.add_str("", "--dict-fname", "dictionary file name");
        auto use_wl_lib = argparser.add_flag("", "--use-wl-lib", "Use low level memory mapped library", false);
        argparser.parse_args();

        logger->setLevel(*verbose_log);
        if (*help_flag) {
            std::cout << "Usage: " << av[0] << " <args>\n";
            std::cout << *argparser.help();
            exit(0);
        }

        LOG_Info("TODO: Implement word_ladder in CPP");
        LOG_Debug("Using dictionary file %s", dict_fname->c_str());
        auto wl = std::make_shared<WordLadder>(dict_fname, logger);
        wl->loadWords();
        if (*verbose_log > INFO) {
            wl->printWords();
        }
        auto posargs = argparser.positional_args();
        wl->findLadder(posargs[0], posargs[1]);

    } catch (std::exception &exc) {
        LOG_Error("Ran into error: %s", exc.what());
    }

    return 0;
}


/* End of file */
