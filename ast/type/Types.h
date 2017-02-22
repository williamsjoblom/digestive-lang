//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPES_H
#define DIG_TYPES_H

#include "Type.h"
#include "PrimitiveType.h"

const Type int8Type = PrimitiveType(PrimitiveKind::INTEGER, 8);
const Type int16Type = PrimitiveType(PrimitiveKind::INTEGER, 16);
const Type int32Type = PrimitiveType(PrimitiveKind::INTEGER, 32);
const Type int64Type = PrimitiveType(PrimitiveKind::INTEGER, 64);

const Type float32Type = PrimitiveType(PrimitiveKind::REAL, 32);
const Type float64Type = PrimitiveType(PrimitiveKind::REAL, 64);

#endif //DIG_TYPES_H
