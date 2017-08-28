#ifndef INTERPRETER_RANGE_H
#define INTERPRETER_RANGE_H

#include "../Native/Native.h"
#include "Int.h"

$class(Range)
    int upper, current = -1;

    $method(init, Range)
        self->upper = Int::getValue(args[0]);
        return nullptr;
    }

    $method(next, Range)
        self->current++;
        if (self->current >= self->upper)
            throw Wrap(new StopIteration());
        return New(Int(self->current));
    }

    $method(iter, Range)
        return self;
    }

    static void populate() {
        addMethod("init", init, 1);
        addMethod("next", next);
        addMethod("iter", iter);
    }
};

#endif //INTERPRETER_RANGE_H
