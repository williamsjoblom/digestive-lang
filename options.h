#pragma once

#include <string>

static const std::string version = "0.1.0";

extern bool verbose;
extern bool runTests;


/**
 * Dump.
 */
extern bool dumpParseDesugaredGrammar; // -d bnf
extern bool dumpParseSets;             // -d ps
extern bool dumpParseStateTree;        // -d pst
extern bool dumpJitHandles;            // -d h
