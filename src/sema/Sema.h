#ifndef SEMA_H
#define SEMA_H

#include "parser/AST.h"


class Sema {
public:
    bool semantic(AST *st);
};


#endif