#!/usr/bin/env python3

import argparse
import sys
import subprocess

from os import listdir
from os import linesep
from os.path import isfile, join


class TestResult:
    """
    Test result.
    """
    
    def __init__(self, test, failed_lines):
        """
        Constructor.
        Takes a list of tuples consisting of (actual line, expected line)
        """
        self.test = test
        self.failed_lines = failed_lines

        
    def passed(self):
        """
        Return true if this test passed
        """
        return not self.failed_lines

    
    def dump(self):
        """
        Print result.
        """
        MAX_FAILED_LINES = 4
        
        n = min(MAX_FAILED_LINES, len(self.failed_lines))

        print("%s:" % self.test.path)
        for i in range(n):
            actual, expected = self.failed_lines[i]
            print("%s%d: expected '%s', got '%s'." %
                  (" "*4, i + 1, expected, actual))

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

        failed_lines = []            
        for i in range(len(self.expected_lines)):
            e = self.expected_lines[i].strip()
            if (i >= len(output)):
                failed_lines.append(("", e))
                continue
            
            a = output[i].decode("utf-8").strip()
            
            if e != a:
                failed_lines.append((a, e))

        return TestResult(self, failed_lines)
        
        
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
        result = t.run();
        if result.passed():
            passed.append(result)
        else:
            failed.append(result)

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
        for result in failed:
            result.dump()
    else:
        print("All tests passed! ヾ(⌐■_■)ノ♪")    
    
        

        
