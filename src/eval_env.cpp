//
// Created by Ray Chou on 2023/5/7.
//

#include "./eval_env.h"
#include "./error.h"
#include  "./form.h"


void EvalEnv::addBuiltinProc(std::string fName,std::function<BuiltinFuncType> f){
    dictionary[fName] = std::make_shared<BuiltinProcValue>(fName,f);
}
//
ValuePtr applyLambda(std::shared_ptr<LambdaValue> func, std::vector<ValuePtr> params,EvalEnv &env){
    std::vector<ValuePtr> args;
    std::ranges::transform(params,
                           std::back_inserter(args),
                           [&](ValuePtr v) { return env.eval(v); });

    auto lambdaEnv = std::make_shared<EvalEnv>(func->getEnv());

    if(args.size()!=func->getParams().size()) throw LispError("lambda apply params error");

    for(int i=0;i<args.size();i++){
        lambdaEnv->dictionary[func->getParams()[i]]=args[i];
    }

    ValuePtr result;
    for(auto vPtr:func->getBody()){
        result=lambdaEnv->eval(vPtr);
    }
    return result;
}

EvalEnv::EvalEnv():parent(nullptr){
        addBuiltinProc("+",_add);
        addBuiltinProc("-",_minus);
        addBuiltinProc("print",_print);
        addBuiltinProc("*",_multiply);
        addBuiltinProc(">",_greater);
        addBuiltinProc("apply",_apply);
        addBuiltinProc("eval",_eval);
        addBuiltinProc("display",_display);
        addBuiltinProc("displayln",_displayln);
        addBuiltinProc("error",_error);
        addBuiltinProc("newline",_newline);
        addBuiltinProc("exit",_exit);
        addBuiltinProc("atom?",_atom);
        addBuiltinProc("boolean?",_boolean);
        addBuiltinProc("integer?",_integer);
        addBuiltinProc("list?",_is_list);
        addBuiltinProc("number?",_number);
        addBuiltinProc("null?",_null);
        addBuiltinProc("pair?",_pair);
        addBuiltinProc("procedure?",_procedure);
        addBuiltinProc("string?",_string);

        addBuiltinProc("/",_divide);
        addBuiltinProc("abs",_abs);
        addBuiltinProc("expt",_expt);
        addBuiltinProc("quotient",_quotient);
        addBuiltinProc("modulo",_modulo);
        addBuiltinProc("remainder",_remainder);


        addBuiltinProc("eq?",_eq);
        addBuiltinProc("symbol?",_symbol);
        addBuiltinProc("equal?",_equal);
        addBuiltinProc("not",_not);
        addBuiltinProc("=",_equal_symbol);
        addBuiltinProc("<",_less);
        addBuiltinProc(">=",_greater_eq);
        addBuiltinProc("<=",_less_eq);
        addBuiltinProc("odd?",_odd);
        addBuiltinProc("even?",_even);
        addBuiltinProc("zero?",_zero);

        ValuePtr _append(const std::vector<ValuePtr>& params,EvalEnv& env);
        ValuePtr _car(const std::vector<ValuePtr>& params,EvalEnv& env);
        ValuePtr _cdr(const std::vector<ValuePtr>& params,EvalEnv& env);
        ValuePtr _cons(const std::vector<ValuePtr>& params,EvalEnv& env);
        ValuePtr _length(const std::vector<ValuePtr>& params,EvalEnv& env);
        ValuePtr _list(const std::vector<ValuePtr>& params,EvalEnv& env);
        ValuePtr _map(const std::vector<ValuePtr>& params,EvalEnv& env);
        ValuePtr _filter(const std::vector<ValuePtr>& params,EvalEnv& env);
        ValuePtr _reduce(const std::vector<ValuePtr>& params,EvalEnv& env);
        addBuiltinProc("append",_append);
        addBuiltinProc("car",_car);
        addBuiltinProc("cdr",_cdr);
        addBuiltinProc("cons",_cons);
        addBuiltinProc("length",_length);
        addBuiltinProc("list",_list);
        addBuiltinProc("map",_map);
        addBuiltinProc("filter",_filter);
        addBuiltinProc("reduce",_reduce);

        //append all
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->getType() == ValueType::Boolean_Value ||
        expr->getType() == ValueType::Numeric_Value ||
        expr->getType() == ValueType::String_Value ||
        expr->getType() == ValueType::Builtin_Proc_Value||
        expr->getType() == ValueType::Lambda_Value) {
        return expr;
    }

    else if (expr->getType() == ValueType::Nil_Value) {
        throw LispError("Evaluating nil is prohibited.");
    }
    //Pair
    else if(expr->getType() == ValueType::Pair_Value){
        auto exprPair = dynamic_pointer_cast<PairValue>(expr);
        std::vector<ValuePtr> v = *exprPair->toVector();
        using namespace std::literals; // 使用 s 后缀

        //第一个元素为符号
        if(v[0]->getType() == ValueType::Symbol_Value){
            auto firstSymbol = v[0]->asSymbol();
            auto findSymbol=this->lookupBinding(*firstSymbol);
            //特殊形式
            if (SPECIAL_FORMS.find(*firstSymbol) != SPECIAL_FORMS.end()) {
                return SPECIAL_FORMS[*firstSymbol](*exprPair->getRPtr()->toVector(),*this);
            }
            //dictionary中可以找到
            else if (findSymbol) {
                //内置过程
                if((*findSymbol)->getType() == ValueType::Builtin_Proc_Value){
                    std::function<BuiltinFuncType> proc = dynamic_pointer_cast<BuiltinProcValue>(*findSymbol)->getFunc();
                    std::vector<ValuePtr> vCdr = std::move(*(exprPair->getRPtr()->toVector()));
                    //与下面调用库函数等价
                    for(auto& eCdr:vCdr){
                        eCdr = this->eval(eCdr);
                    }
                    return proc(vCdr,*this);
                }
                //lambda
                else if((*findSymbol)->getType() == ValueType::Lambda_Value){
                    auto func = dynamic_pointer_cast<LambdaValue>(*findSymbol);
                    std::vector<ValuePtr> vCdr = std::move(*(exprPair->getRPtr()->toVector()));
                    return applyLambda(func,vCdr,*this);
                }
                else{
                    return expr;
                }

            }

            else{
                return expr;
            }
        }
        //第一个元素还是pair
        else if(v[0]->getType() == ValueType::Pair_Value){
            return eval(std::make_shared<PairValue>(
                eval(exprPair->getLPtr()),
                exprPair->getRPtr())
                        );
        }
        else if(v[0]->getType() == ValueType::Builtin_Proc_Value){
            std::function<BuiltinFuncType> proc = dynamic_pointer_cast<BuiltinProcValue>(v[0])->getFunc();
            std::vector<ValuePtr> vCdr = std::move(*(exprPair->getRPtr()->toVector()));
            //与下面调用库函数等价
            for(auto& eCdr:vCdr){
                eCdr = eval(eCdr);
            }
            return proc(vCdr,*this);
        }
        else if(v[0]->getType() == ValueType::Lambda_Value){
            auto func = dynamic_pointer_cast<LambdaValue>(v[0]);
            std::vector<ValuePtr> vCdr = std::move(*(exprPair->getRPtr()->toVector()));
            return applyLambda(func,vCdr,*this);
        }
        //racket就报错，我也报错
        else return expr;

    }//pair结束
    //单个符号
    else if(expr->getType() == ValueType::Symbol_Value){
        if (auto name = expr->asSymbol()) {
            if (this->lookupBinding(*name)) {
                    return *this->lookupBinding(*name);
            } else {
                throw LispError("Variable " + *name + " not defined.");
            }
        }
    }
    else{
        throw LispError("Unimplemented");
    }
}
EvalEnv::EvalEnv(std::shared_ptr<EvalEnv>e):parent(e) {}

std::shared_ptr<EvalEnv> EvalEnv::getParent() const{
    return parent;
}

std::optional<ValuePtr> EvalEnv::lookupBinding(const std::string& name){
    if(this->dictionary.find(name)!=this->dictionary.end()) return this->dictionary[name];

    std::shared_ptr<EvalEnv> pEnv=this->parent;
    while(pEnv!=nullptr){
        if(pEnv->dictionary.find(name)!=pEnv->dictionary.end()) return pEnv->dictionary[name];
        pEnv=pEnv->parent;
    }
    return std::nullopt;
}
