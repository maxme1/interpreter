#include "NativeObject.h"
#include "../Exception.h"

void thr(std::string name) {
    throw Interpreter::ExceptionWrapper(new ValueError("Could not convert object to " + name));
}
