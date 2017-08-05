#include "NativeObject.h"

Object *BaseNative::not_equal(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::less_or_equal(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::greater_or_equal(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::less(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::greater(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::equal(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::divide(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::multiply(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::unary_subtract() { throw Exception("Operator not defined"); }

Object *BaseNative::subtract(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::unary_add() { throw Exception("Operator not defined"); }

Object *BaseNative::add(Object *other) { throw Exception("Operator not defined"); }

Object *BaseNative::getItem(Object *other) { throw Exception("Operator not defined"); }