from logging import getLogger
from collections import deque
from argparse import ArgumentParser
from common.logger import add_logger_args
from common.logger import setup_logging_cliargs


class WordList:
    def __init__(self, dictfname):
        self.dict_fname = dictfname
        self.dict_fd = open(dictfname, "r")
        self.dict_words = set()
        self.enum_words = deque()
        self.ancestry = {}
        self.start_word = ""
        self.end_word = ""

    def lookup(self, inword):
        try:
            idx = self.dict_words.remove(inword)
        except ValueError:
            return False
        except KeyError:
            return False
        return True

    def enum_chars(self, currword, pos):
        currbuf = bytearray(currword.encode())
        for ch in range(ord('a'), ord('z') + 1):
            currbuf[pos] = ch
            nextword = currbuf.decode()
            if not self.lookup(nextword):
                continue
            if nextword == currword:
                continue
            self.enum_words.append(nextword)
            self.ancestry[nextword] = currword

    def enumerate(self, inword):
        for ii in range(len(inword)):
            self.enum_chars(inword, ii)

    def print_ancestry(self):
        ostr = self.end_word
        currword = self.end_word
        while currword != self.start_word:
            currword = self.ancestry[currword]
            ostr += f" <- {currword}"
        return ostr

    def sanity_checks(self):
        if self.start_word == self.end_word:
            raise RuntimeError("Both start and end words are the same")

        if len(self.start_word) != len(self.end_word):
            raise RuntimeError("Both start and end words must be of same length")

        if self.lookup(self.start_word):
            log = getLogger(__name__)
            log.warning("Start word is already in the dictionary. Ignoring and continuing...")

        # Relying on implicit raise
        self.dict_words.index(self.end_word)

    def reset(self):
        self.dict_fd.seek(0)
        self.dict_words = deque([inln.strip() for inln in self.dict_fd])
        self.enum_words = deque()
        self.ancestry = {}

    def find_ladder(self, startword, endword):
        self.start_word = startword
        self.end_word = endword
        self.reset()
        self.sanity_checks()
        self.enum_words.append(startword)
        while len(self.enum_words) > 0:
            currword = self.enum_words.popleft()
            if currword == endword:
                print("Found the path")
                return self.print_ancestry()
            self.enumerate(currword)


def parse_cli_args():
    parser = ArgumentParser(description="World ladder puzzle")
    add_logger_args(parser)
    parser.add_argument("--dict-file", type=str, help="Path to the dictionary file, one word per line", required=True)
    parser.add_argument("startword", type=str, help="The starting word for the ladder")
    parser.add_argument("endword", type=str, help="The ending word for the ladder")
    return parser.parse_args()


def main():
    cliargs = parse_cli_args()
    setup_logging_cliargs(cliargs)
    wl = WordList(cliargs.dict_file)
    print(wl.find_ladder(cliargs.startword, cliargs.endword))


if __name__ == "__main__":
    main()


# End of file
