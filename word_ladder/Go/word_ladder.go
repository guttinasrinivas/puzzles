package main


import (
	"io"
	"os"
	"fmt"
	"bufio"
	"slices"
	"strings"

	"github.com/maxbolgarin/logze/v2"
)


type WordLadder struct {
	fname string
	fh *os.File
	rdr *bufio.Reader
	words []string
	wordq WordQueue[string]
	ladder map[string]string
	logger *logze.Logger
}


func (wl *WordLadder) sanityChecks(fromword string, toword string) bool {
	log := wl.logger
	if len(fromword) != len(toword) {
		log.Error("Start and End words must match the same size")
		return false;
	}

	if fromword == toword {
		/* TODO Log error and throw a runtime exception */
		log.Warn("Both Start and End words are the same. Nothing to do.")
		return false
	}

	if wl.lookUpWord(fromword) {
		log.Warn("Found the Start word in dictionary. Ignoring and continuing...")
	}

	found := slices.Index(wl.words, toword)
	if (found < 0) {
		log.Warn("Could not find the End word in dictionary.")
		return false
	}

	return true
}


func (wl *WordLadder) loadDictionary(fname string) {
	var err error

	wl.fname = fname
	wl.fh, err = os.Open(wl.fname)
	panic_on_error(err, wl.logger)
	wl.rdr = bufio.NewReader(wl.fh)

	for {
		inln, err := wl.rdr.ReadString('\n')

		if (len(inln) > 0) {
			wl.words = append(wl.words, strings.TrimSpace(inln))
		}

		if (err == io.EOF) {
			wl.logger.Info("Loaded dictionary")
			break
		}

		if (err != nil) {
			wl.logger.Error("Dictionary loading failed. Exiting...")
			panic(err)
		}
	}
}


func (wl *WordLadder) printDictionary() {
	for _, word := range wl.words {
		fmt.Print(word, " ")
	}
}


func (wl *WordLadder) lookUpWord(inword string) bool {
	found := slices.Index(wl.words, inword)
	if (found < 0) {
		return false
	}

	wl.words = slices.Delete(wl.words, found, found + 1)
	return true
}


func (wl *WordLadder) enumerateWords(startword *string) {
	for ii := range(len(*startword)) {
		runes := []rune(*startword)
		for _, ch:= range "abcdefghijklmnopqrstuvwxyz" {
			runes[ii] = ch
			tmpword := string(runes)
			if (wl.lookUpWord(tmpword)) &&
			   (*startword != tmpword) {
				wl.wordq.Push(&tmpword)
				wl.ladder[tmpword] = *startword
				wl.logger.Tracef("Enumerated word: %s", tmpword)
			}
		}
	}

	wl.logger.Debugf("Word Queue Length: %d", wl.wordq.Size())
}


func (wl *WordLadder) printAncestry(currword *string) {
	hopctr := 0
	fmt.Print("Hops for: ", *currword)
	prevword := *currword
	for {
		nextword, exists := wl.ladder[prevword]
		if !exists {
			fmt.Println("\nHop Counts: ", hopctr)
			break;
		}

		if prevword == nextword {
			continue
		}

		hopctr++
		fmt.Print(" <- ", nextword)
		prevword = nextword
	}
}


func (wl *WordLadder) findWordLadder(fromword string, toword string) bool {
	wl.wordq.Push(&fromword)
	for {
		nextword := wl.wordq.Pop()
		if nextword == nil {
			break
		}

		if *nextword == toword {
			wl.printAncestry(nextword)
			wl.logger.Info("Found the path")
			return true
		}

		wl.enumerateWords(nextword)
	}

	wl.logger.Warn("Could not find a path between given words")
	return false;
}

