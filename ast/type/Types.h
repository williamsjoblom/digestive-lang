//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPES_H
#define DIG_TYPES_H

#include "Type.h"
#include "PrimitiveType.h"

PrimitiveType INT8TYPE = PrimitiveType(PrimitiveKind::INTEGER, 8);
PrimitiveType INT16TYPE = PrimitiveType(PrimitiveKind::INTEGER, 16);
PrimitiveType INT32TYPE = PrimitiveType(PrimitiveKind::INTEGER, 32);
PrimitiveType INT64TYPE = PrimitiveType(PrimitiveKind::INTEGER, 64);

PrimitiveType FLOAT32TYPE = PrimitiveType(PrimitiveKind::REAL, 32);
PrimitiveType FLOAT64TYPE = PrimitiveType(PrimitiveKind::REAL, 64);

#endif //DIG_TYPES_H
