package main

import (
	"fmt"
	"io"
	"os"

	"github.com/maxbolgarin/logze/v2"
	"github.com/alexflint/go-arg"
)


type inCliArgs struct {
	DictFname 	 string `arg:"--dictionary,env:DICTIONARY"`
	StartWord    string `arg:"positional,required"`
	EndWord      string `arg:"positional,required"`
	Verbosity    int32  `arg:"env:VERBOSITY"`
}


func panic_on_error(inerr error, logger *logze.Logger) {
    if inerr == nil {
		return
	}

	if logger != nil {
		logger.Error("Cannot continue", inerr)
	}

	panic(inerr)
}


func setupLogging(logfname string, verbosity int32) (*logze.Logger, io.Closer) {
	var conf logze.Config
	fmt.Println("Logging to " + logfname)
	filewriter, err := os.OpenFile(logfname, os.O_RDWR | os.O_APPEND | os.O_CREATE, 0644)
	panic_on_error(err, nil)
	baseconf := logze.C(filewriter).WithConsole().WithNoDiode()

	/* TODO Setup logging based on levels */
	if verbosity > 4 {
		conf = baseconf.WithLevel(logze.LevelTrace)
	} else {
		conf = baseconf.WithLevel(logze.LevelInfo)
	}

	logger := logze.New(conf)
	logger.Info("Starting up...")
	return &logger, filewriter
}


func parseCliArgs(envprefix string) *inCliArgs {
	var cliargs inCliArgs
	conf := arg.Config{
		EnvPrefix: envprefix,
	}

	parser, err := arg.NewParser(conf, &cliargs)
	if err != nil {
		fmt.Println("Error: ", err, ". Cannot continue...")
		panic(err)
	}

	parser.MustParse(os.Args[1:])

	return &cliargs
}


func main() {
	cliargs := parseCliArgs("WORD_LADDER_")
	logger, closer := setupLogging(os.Args[0] + ".log", cliargs.Verbosity)
	defer closer.Close()

	logger.Infof("Using dictionary file: %s", cliargs.DictFname)

	wl := WordLadder{logger: logger}
	wl.ladder = make(map[string]string)
	wl.loadDictionary(cliargs.DictFname)
	defer wl.fh.Close()

	check := wl.sanityChecks(cliargs.StartWord, cliargs.EndWord)
	if (!check) {
		logger.Error("Sanity checks failed. Exiting...")
		os.Exit(2)
	}

	logger.Debug("Sanity checks passed.")
	logger.Infof("Attempting to find the path from %s to %s now", cliargs.StartWord, cliargs.EndWord)

	wl.findWordLadder(cliargs.StartWord, cliargs.EndWord)

	os.Exit(0)
}

