#ifndef CALLCONV_H
#define CALLCONV_H

#include "ast/type/DType.h"

int neededRegisterCount(DType& type);
std::vector<DType> flattenType(DType& type);

#endif
