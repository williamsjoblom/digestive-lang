#!/usr/bin/env python3

import argparse
import sys
import subprocess

from os import listdir
from os import linesep
from os.path import isfile, join

class Test:
    """
    Test.
    """
    
    def __init__(self, path, expected_lines, name="", description=""):
        """
        Constructor.
        """
        self.path = path
        self.name = name
        self.description = description
        self.expected_lines = expected_lines

        
    def run(self):
        """
        Run test.
        """
        process = subprocess.Popen(["dg", self.path], stdout=subprocess.PIPE)
        output = process.stdout.readlines()

        if len(output) != len(self.expected_lines):
            return False
        
        for i in range(len(output)):
            if output[i].decode("utf-8").strip() != self.expected_lines[i].strip():
                return False

        return True
        
        
def parse_test(path):
    """
    Parse test.
    """
    with open(path, 'r') as infile:
        source = infile.read()

    name = ""
    description = ""
    expected_lines = []
        
    lines = source.splitlines()
    # Parse header
    header = (line for line in lines if line.startswith("#@"))
    for line in header:
        value = line.split(":", 1)[1]

        if line.startswith("#@ name:"):
            name += linesep + value.strip()
        elif line.startswith("#@ description:"):
            description += linesep + value.strip()
        else:
            print("Unknown header parameter:", line)
            sys.exit(1)

    name = name.strip()
    description = description.strip()
    
    # Parse expected output
    expected = (line for line in lines if line.startswith("#$"))
    for line in expected:
        value = line.split("#$", 1)[1]
        expected_lines.append(value)

    if len(expected_lines) == 0:
        print("No expected output for file:", path)
        sys.exit(1)

    return Test(path, expected_lines, name, description)
        

def test(tests):
    """
    Run tests.
    """
    passed, failed = [], []
    
    for t in tests:
        if t.run():
            passed.append(t)
        else:
            failed.append(t)

    return (passed, failed)


def test_files(files):
    """
    Parse and run test files.
    """
    tests = []
    for f in files:
        t = parse_test(f)
        tests.append(t)

    passed, failed = test(tests)
    return (passed, failed)

    
if __name__ == "__main__":
    parser = argparse.ArgumentParser("pit")
    parser.add_argument("directory", nargs="+")
    args = parser.parse_args()
    
    dirs = args.directory
    
    files = []  
    for d in dirs:
        try:
            files.extend( [join(d, f) for f in listdir(d) if isfile(join(d, f)) and
                           f.split(".")[-1] == "dgr"] )
        except FileNotFoundError:
            print("Directory '" + d + "' not found!")
            sys.exit(1)

    passed, failed = test_files(files)

    
    if not failed and not passed:
        print("No tests were ran")

    if passed:
        print("-"*8, "Passed tests:", len(passed), "-"*8)
        
    if failed:
        print("-"*8, "Failed tests:", len(failed), "-"*8)
        for t in failed:
            print(" "*4, t.path)
    else:
        print("All tests passed! ヾ(⌐■_■)ノ♪")    
    
        

        
