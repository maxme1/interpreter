#include <utility>

#include "Array.h"

ObjPtr Array::iter(ObjPtr _self, const std::vector<ObjPtr> &args, API *api) {
    auto self = Array::cast(std::move(_self), true);
    return New(IndexIterator(self));
}