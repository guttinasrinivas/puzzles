/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <string>
#include <mutex>
#include <memory>
#include <thread>
#include <vector>
#include "SimpleLogger.h"
#ifdef USE_CLI_ARGS
#include "CLI11.hpp"
#elif defined USE_VAS_LIB
#include "cli_args.hpp"
#endif /* USE_CLI_ARGS */


using val_ptr = std::shared_ptr<int>;
using thread_ptr = std::shared_ptr<std::thread>;


struct ThreadSyncTest
{
    public:
        ThreadSyncTest(int inmaxval, int inverbosity=0): 
            maxval(inmaxval), verbosity(inverbosity) {}
        virtual ~ThreadSyncTest() {}

    protected:
        int maxval;
        int verbosity;
        std::mutex mm;
        int currval;
        std::vector<thread_ptr> threads;

       
    protected:
        int print_val(std::string prefix, int seq)
        {
            /* TODO Release this or move to a different scope!!! */
            std::lock_guard<std::mutex> guard(mm);
            
            if (currval < 0) {
                return false;
            }
            
            if (currval >= maxval) {
                return false;
            }
            
            if ((currval % 3) != seq) {
                if (verbosity > 0) {
                    std::cout << '.';
                }
                return true;
            }
            
            currval += 1;
            std::cout << prefix << currval;
            
            return true;
        }


        void print_thread(std::string prefix, int seq)
        {
            while (true) {
                if (!print_val(prefix, seq)) {
                    return;
                }
            }
        }
        

    public:
        void launch_threads(int startval = 0)
        {
            currval = startval;
            threads.push_back(std::make_shared<std::thread>(&ThreadSyncTest::print_thread, this, "A", 0));
            threads.push_back(std::make_shared<std::thread>(&ThreadSyncTest::print_thread, this, "B", 1));
            threads.push_back(std::make_shared<std::thread>(&ThreadSyncTest::print_thread, this, "C", 2));
            return;
        }

        void wait_for_threads(void)
        {
            for (auto thrd: threads) {
                thrd->join();
            }
        }
};


void usage(char *av[], ArgParser::CLI_Args& ap)
{
#if defined USE_VAS_LIB
    std::cout << std::endl << "Usage: " << av[0];
    std::cout << " " << *ap.exp_args();
    std::cout << std::endl << *ap.help();
    std::cout << std::endl;
#endif /* USE_VAS_LIB */
}


int main(int argc, char *av[])
{
    int maxval = 10;
    int verbose = 0;
    auto logger = std::make_shared<SimpleLogger>();

#ifdef USE_CLI_ARGS
    CLI::App app{"LeetCode puzzle: Thread sync"};
    try {
        auto argv = app.ensure_utf8(av);
        app.add_option("--max-val", maxval, "Run up to and stop at <max-va,>");
        app.add_option("--verbose", verbose, "Debug log verbosity level");
        CLI11_PARSE(app, argc, argv);
#elif defined USE_VAS_LIB
    ArgParser::CLI_Args app(argc, av);
    try {
        auto in_maxval = app.add_int("", "--max-val");
        auto in_verbose = app.add_int("", "--verbose");
        auto in_help = app.add_flag("-h", "--help");
        LOG_Debug("Using CLI Args: %s", app.get_args()->c_str());
        app.parse_args();
        if (*in_help) {
            usage(av, app);
            exit(0);
        }
        maxval = ((*in_maxval) > 0) ? *in_maxval : maxval;
        verbose = ((*in_verbose) > 0) ? *in_verbose : verbose;
#endif /* USE_CLI_ARGS */

        auto ts_test = ThreadSyncTest(maxval, verbose);
        ts_test.launch_threads();
        ts_test.wait_for_threads();
    } catch (std::exception &exc) {
        std::cout << "Error: " << exc.what();
        usage(av, app);
    }


    std::cout << "\nDone.\n";

    return 0;
}


/* End of file */

