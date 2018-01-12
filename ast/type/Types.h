#pragma once

#include "Type.h"
#include "PrimitiveType.h"

/**
 * Untyped type
 * TODO: should this really be an instance of primitive type?
 */
const PrimitiveType VOIDTYPE = PrimitiveType(PrimitiveKind::VOID, 0);


/**
 * Integers: 0, 1, -1, 2, -2...
 */
const PrimitiveType INT16TYPE = PrimitiveType(PrimitiveKind::INTEGER, 2);
const PrimitiveType INT32TYPE = PrimitiveType(PrimitiveKind::INTEGER, 4);
const PrimitiveType INT64TYPE = PrimitiveType(PrimitiveKind::INTEGER, 8);


/**
 * Natural numbers: 0, 1, 2...
 */
const PrimitiveType NAT8TYPE = PrimitiveType(PrimitiveKind::NATURAL, 1);
const PrimitiveType NAT16TYPE = PrimitiveType(PrimitiveKind::NATURAL, 2);
const PrimitiveType NAT32TYPE = PrimitiveType(PrimitiveKind::NATURAL, 4);
const PrimitiveType NAT64TYPE = PrimitiveType(PrimitiveKind::NATURAL, 8);
