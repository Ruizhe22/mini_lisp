//
// Created by Ray Chou on 2023/5/8.
//

#ifndef MINI_LISP_BUILTINS_H
#define MINI_LISP_BUILTINS_H

#include "value.h"
#include "eval_env.h"
#include <vector>

ValuePtr _add(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _minus(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _multiply(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _divide(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _abs(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _expt(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _quotient(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _modulo(const std::vector<ValuePtr>& params,EvalEnv& env) ;
ValuePtr _remainder(const std::vector<ValuePtr>& params,EvalEnv& env);



ValuePtr _greater(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _eq(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _equal(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _not(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _equal_symbol(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _less(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _greater_eq(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _less_eq(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _odd(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _even(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _zero(const std::vector<ValuePtr>& params,EvalEnv& env);



ValuePtr _apply(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _eval(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _print(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _display(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _displayln(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _error(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _newline(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _exit(const std::vector<ValuePtr>& params,EvalEnv& env);

ValuePtr _atom(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _boolean(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _integer(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _is_list(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _number(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _null(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _pair(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _procedure(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _string(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _symbol(const std::vector<ValuePtr>& params,EvalEnv& env);


//ValuePtr _(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _append(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _car(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _cdr(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _cons(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _length(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _list(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _map(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _filter(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr _reduce(const std::vector<ValuePtr>& params,EvalEnv& env);


#endif  // MINI_LISP_BUILTINS_H
