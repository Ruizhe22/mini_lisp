//
// Created by Ray Chou on 2023/5/7.
//

#ifndef MINI_LISP_EVAL_ENV_H
#define MINI_LISP_EVAL_ENV_H

#include "value.h"
#include "builtins.h"

void addBuiltinProc(std::string fName,std::function<BuiltinFuncType> f);
ValuePtr applyLambda(std::shared_ptr<LambdaValue> func, std::vector<ValuePtr> params,EvalEnv &env);

class EvalEnv : public std::enable_shared_from_this<EvalEnv>{
private:
    void addBuiltinProc(std::string fName,std::function<BuiltinFuncType> f);
    std::shared_ptr<EvalEnv> parent;
public:
    EvalEnv();
    EvalEnv(std::shared_ptr<EvalEnv> );
    EvalEnv(const EvalEnv& env) = default;
    std::unordered_map<std::string,ValuePtr> dictionary;

    ValuePtr eval(ValuePtr expr);
    std::shared_ptr<EvalEnv> getParent() const;
    std::optional<ValuePtr> lookupBinding(const std::string& name) ;
};

#endif  // MINI_LISP_EVAL_ENV_H
