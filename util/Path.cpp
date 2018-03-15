#include "Path.h"

std::string rootDirPath() {
    char* cstr = getenv("DGROOT");
    if (cstr != nullptr)
	return std::string(cstr);
    else
	return "";
}


bool hasRootDirPath() {
    return !rootDirPath().empty();
}


std::string coreBNFFilePath() {
    return rootDirPath() + "/lang/dg.bnf";
}


std::string bootFilePath() {
    return rootDirPath() + "/core/boot.dg";
}

