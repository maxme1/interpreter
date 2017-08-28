#ifndef INTERPRETER_BOOL_H
#define INTERPRETER_BOOL_H

$class(Bool)
    bool value;

    explicit Bool(bool value) : value(value) {}

    bool asBool() override {
        return value;
    }

    $method(init, Bool)
        self->value = args[0]->asBool();
        return nullptr;
    }

    $method(str, Bool)
        if (self->value)
            return New(String("True"));
        return New(String("False"));
    }

    static void populate() {
        addMethod("str", str);
        addMethod("init", init, 1);
    }
};

#endif //INTERPRETER_BOOL_H
