//
// Created by Ray Chou on 2023/5/8.
//

#include "builtins.h"
#include "error.h"
#include "eval_env.h"

#include <iostream>
#include <algorithm>

//builtin

ValuePtr _add(const std::vector<ValuePtr>& params,EvalEnv& env) {
    double result = 0;

    if(params.empty()) return std::make_shared<NumericValue>(result);

    for (auto i : params) {
        i=env.eval(i);
        if (!i->asNumeric()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += *i->asNumeric();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr _minus(const std::vector<ValuePtr>& params,EvalEnv& env) {
    double result = 0;

    if(params.empty()) return std::make_shared<NumericValue>(result);

    if(params.size()==1) result=-*env.eval(params[0])->asNumeric();
    else if(params.size()==2) result= *env.eval(params[0])->asNumeric() - *params[1]->asNumeric();
    else throw LispError("Minus params error.");
    return std::make_shared<NumericValue>(result);
}


ValuePtr _multiply(const std::vector<ValuePtr>& params,EvalEnv& env){
    double result = 1;
    if(params.empty()) return std::make_shared<NumericValue>(result);

    for (auto i : params) {
        i=env.eval(i);
        if (!i->asNumeric()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result *= *i->asNumeric();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr _divide(const std::vector<ValuePtr>& params,EvalEnv& env) {
    double result = 0;

    if(params.empty()) return std::make_shared<NumericValue>(result);

    if(params.size()==1) result=1/ *env.eval(params[0])->asNumeric();
    else if(params.size()==2) result= *env.eval(params[0])->asNumeric() / *params[1]->asNumeric();
    else throw LispError("Divide params error.");
    return std::make_shared<NumericValue>(result);
}

ValuePtr _abs(const std::vector<ValuePtr>& params,EvalEnv& env) {
    if(params.size()!=1) throw LispError("_abs Params Error");
    if(env.eval(params[0])->getType()==ValueType::Numeric_Value){
        double x = *env.eval(params[0])->asNumeric();
        if(x>=0) return std::make_shared<NumericValue>(x);
        else return std::make_shared<NumericValue>(-x);
    }
    else throw LispError("_abs Params Error");
}

ValuePtr _expt(const std::vector<ValuePtr>& params,EvalEnv& env) {
    if(params.size()!=2) throw LispError("_expt Params Error");
    std::optional<double> x = env.eval(params[0])->asNumeric();
    std::optional<double> y = env.eval(params[1])->asNumeric();
    if(x&&y){
        return std::make_shared<NumericValue>(pow(*x,*y));
    }
    else throw LispError("_expt Params Error");
}

ValuePtr _quotient(const std::vector<ValuePtr>& params,EvalEnv& env) {
    if(params.size()!=2) throw LispError("_quotient Params Error");
    std::optional<double> x = env.eval(params[0])->asNumeric();
    std::optional<double> y = env.eval(params[1])->asNumeric();
    if(x&&y&&*y){
        return std::make_shared<NumericValue>(static_cast<int>(*x / *y));
    }
    else throw LispError("_quotient Params Error");
}

ValuePtr _modulo(const std::vector<ValuePtr>& params,EvalEnv& env) {
    if(params.size()!=2) throw LispError("_modulo Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    std::optional<double> yp = env.eval(params[1])->asNumeric();
    int q;
    if(xp&&yp&&*yp){
        int x = *xp;
        int y = *yp;

        if(*xp!=x || *yp!=y){
            throw LispError("_modulo Params Error");
        }

        if(y>0){
            for(q=0;q<y;q++){
                if(abs(q-x)%y==0) return std::make_shared<NumericValue>(q);
            }
            throw LispError("_modulo No Result");
        }
        else{
            for(q=0;q>y;q--){
                if(abs(q-x)%y==0) return std::make_shared<NumericValue>(q);
            }
            throw LispError("_modulo No Result");
        }
    }
    else throw LispError("_modulo Params Error");
}

ValuePtr _remainder(const std::vector<ValuePtr>& params,EvalEnv& env) {
    if(params.size()!=2) throw LispError("_remainder Params Error");
    std::optional<double> x = env.eval(params[0])->asNumeric();
    std::optional<double> y = env.eval(params[1])->asNumeric();
    if(x&&y&&*y){
        return std::make_shared<NumericValue>(*x- *y * static_cast<int>(*x / *y));
    }
    else throw LispError("_remainder Params Error");
}



ValuePtr _greater(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2) throw LispError("_greater Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    std::optional<double> yp = env.eval(params[1])->asNumeric();
    if(xp&&yp){
        double x = *xp;
        double y = *yp;
        return std::make_shared<BooleanValue>(x>y);
    }
    else throw LispError("_greater Params Error");
}

ValuePtr _eq(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2) throw LispError("_eq Params Error");
    auto xp = params[0];
    auto yp = params[1];
    if(xp->getType()==ValueType::Nil_Value||yp->getType()==ValueType::Nil_Value) {
        if(xp->getType() != yp->getType()) return std::make_shared<BooleanValue>(false);
        else return std::make_shared<BooleanValue>(true);
    }
    if(xp->getType() != yp->getType()) return std::make_shared<BooleanValue>(false);
    if(xp->getType()==ValueType::Numeric_Value ||
        xp->getType()==ValueType::Boolean_Value||
        xp->getType()==ValueType::Symbol_Value){

        return std::make_shared<BooleanValue>(xp->toString()==yp->toString());

    }else{
        return std::make_shared<BooleanValue>(xp.get()==yp.get());
    }
}

ValuePtr _equal(const std::vector<ValuePtr>& params,EvalEnv& env)
{
    if(params.size()!=2) throw LispError("_equal Params Error");
    auto xp = params[0];
    auto yp = params[1];
    if(xp->getType()==ValueType::Nil_Value||yp->getType()==ValueType::Nil_Value) {
        if(xp->getType() != yp->getType()) return std::make_shared<BooleanValue>(false);
        else return std::make_shared<BooleanValue>(true);
    }
    if(xp->getType() != yp->getType()) return std::make_shared<BooleanValue>(false);
    return std::make_shared<BooleanValue>(xp->toString()==yp->toString());
}

ValuePtr _not(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_not Params Error");
    if(params[0]->getType()==ValueType::Nil_Value) return  std::make_shared<BooleanValue>(false);
    auto xp = params[0];
    if(xp->getType() == ValueType::Boolean_Value &&
        std::dynamic_pointer_cast<BooleanValue>(xp)->getValue()==false) return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}

ValuePtr _equal_symbol(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2) throw LispError("_equal_symbol Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    std::optional<double> yp = env.eval(params[1])->asNumeric();
    if(xp&&yp){
        double x = *xp;
        double y = *yp;
        return std::make_shared<BooleanValue>(x==y);
    }
    else throw LispError("_equal_symbol Params Error");
}

ValuePtr _less(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2) throw LispError("_less Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    std::optional<double> yp = env.eval(params[1])->asNumeric();
    if(xp&&yp){
        double x = *xp;
        double y = *yp;
        return std::make_shared<BooleanValue>(x<y);
    }
    else throw LispError("_less Params Error");
}

ValuePtr _greater_eq(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2) throw LispError("_greater_eq Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    std::optional<double> yp = env.eval(params[1])->asNumeric();
    if(xp&&yp){
        double x = *xp;
        double y = *yp;
        return std::make_shared<BooleanValue>(x>=y);
    }
    else throw LispError("_greater_eq Params Error");
}

ValuePtr _less_eq(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2) throw LispError("_less_eq Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    std::optional<double> yp = env.eval(params[1])->asNumeric();
    if(xp&&yp){
        double x = *xp;
        double y = *yp;
        return std::make_shared<BooleanValue>(x<=y);
    }
    else throw LispError("_less_eq Params Error");
}

ValuePtr _odd(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_odd Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    if(xp){
        int x = *xp;
        if(x == *xp && x%2!=0) return std::make_shared<BooleanValue>(true);
        else return std::make_shared<BooleanValue>(false);
    }
    else throw LispError("_odd Params Error");
}

ValuePtr _even(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_even Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    if(xp){
        int x = *xp;
        if(x == *xp && x%2==0) return std::make_shared<BooleanValue>(true);
        else return std::make_shared<BooleanValue>(false);
    }
    else throw LispError("_even Params Error");
}

ValuePtr _zero(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_zero Params Error");
    std::optional<double> xp = env.eval(params[0])->asNumeric();
    if(xp){
        return std::make_shared<BooleanValue>(!static_cast<bool>(*xp));
    }
    else throw LispError("_zero Params Error");
}



ValuePtr _apply(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(env.eval(params[0])->getType() == ValueType::Builtin_Proc_Value){
        std::function<BuiltinFuncType> func = std::dynamic_pointer_cast<BuiltinProcValue>(env.eval(params[0]))->getFunc();
        std::vector<ValuePtr> args = *params[1]->toVector();
        return func(args,env);
    }
    else if(env.eval(params[0])->getType() == ValueType::Lambda_Value){
        auto func = dynamic_pointer_cast<LambdaValue>(env.eval(params[0]));
        std::vector<ValuePtr> args = *params[1]->toVector();
        return applyLambda(func,args,env);
    }
    else throw LispError("Not Function");

}

ValuePtr _eval(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_eval Params Error");

    return env.eval(params[0]);
}


ValuePtr _print(const std::vector<ValuePtr>& params,EvalEnv& env){
    //std::cout<<"'";
    for(auto& p : params){
        std::cout<<p->toString()<<std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr _display(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_display Params Error");

    if(params[0]->getType()==ValueType::String_Value){
        std::string s = std::dynamic_pointer_cast<StringValue>(params[0])->getValue();
        std::cout<<s<<std::endl;
    }
    else std::cout<< params[0]->toString() << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr _newline(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.empty()){
        std::cout<<std::endl;
        return std::make_shared<NilValue>();
    }
    else throw LispError("_newline Params Error");
}

ValuePtr _displayln(const std::vector<ValuePtr>& params,EvalEnv& env){
    _display(params,env);
    return _newline(std::vector<ValuePtr>(),env);
}

ValuePtr _error(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) {
        if(params.empty()) throw LispError("");
        else throw LispError("_error Params Error");
    }
    if(env.eval(params[0])->getType()==ValueType::String_Value){
        std::string s = std::dynamic_pointer_cast<StringValue>(env.eval(params[0]))->getValue();
        std::cout<<s<<std::endl;
    }
    else if(env.eval(params[0])->getType()==ValueType::Symbol_Value){
        std::string s = std::dynamic_pointer_cast<SymbolValue>(env.eval(params[0]))->getValue();
        std::cout<<s<<std::endl;
    }
    else throw LispError("_error Params Error");
}

ValuePtr _exit(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.empty()) exit(-1);
    else if(params.size()==1 && env.eval(params[0])->asNumeric()){
        exit(*env.eval(params[0])->asNumeric());
    }
    else throw LispError("_exit Params Error");
}

ValuePtr _atom(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_atom Params Error");
    auto temp = params[0];
    if(temp->getType() == ValueType::Nil_Value ||
        temp->getType() == ValueType::Numeric_Value ||
        temp->getType() == ValueType::Symbol_Value ||
        temp->getType() == ValueType::String_Value ||
        temp->getType() == ValueType::Boolean_Value )
        return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}
ValuePtr _boolean(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_boolean Params Error");
    if(params[0]->getType()==ValueType::Nil_Value)  return std::make_shared<BooleanValue>(false);
    if(env.eval(params[0])->getType()==ValueType::Boolean_Value) return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}
ValuePtr _integer(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_integer Params Error");
    if(params[0]->getType()==ValueType::Nil_Value)  return std::make_shared<BooleanValue>(false);
    if(env.eval(params[0])->getType()==ValueType::Numeric_Value &&
        static_cast<int>(*env.eval(params[0])->asNumeric())==(*env.eval(params[0])->asNumeric()))
            return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}

ValuePtr _is_list(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_is_list Params Error");
    if(params[0]->getType() == ValueType::Nil_Value) return std::make_shared<BooleanValue>(true);
    if(params[0]->getType() != ValueType::Pair_Value) return std::make_shared<BooleanValue>(false);
    ValuePtr rPtrTemp = std::dynamic_pointer_cast<PairValue>(params[0])->getRPtr();
    ValuePtr lPtrTemp = std::dynamic_pointer_cast<PairValue>(params[0])->getLPtr();;
    while(rPtrTemp->getType() == ValueType::Pair_Value){
            auto rTemp = static_cast<PairValue&>(*rPtrTemp);
            rPtrTemp=rTemp.getRPtr();
    }
    if(rPtrTemp->getType() == ValueType::Nil_Value){
            return std::make_shared<BooleanValue>(true);
    }
    else{
            return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr _number(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_number Params Error");
    if(params[0]->getType()==ValueType::Nil_Value)  return std::make_shared<BooleanValue>(false);
    if(env.eval(params[0])->getType()==ValueType::Numeric_Value) return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}

ValuePtr _null(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_null Params Error");
    if(params[0]->getType()==ValueType::Nil_Value) return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}

ValuePtr _pair(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_pair Params Error");
    if(params[0]->getType()==ValueType::Pair_Value) return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}
ValuePtr _procedure(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_procedure Params Error");
    if((params[0])->getType()==ValueType::Builtin_Proc_Value || (params[0])->getType()==ValueType::Lambda_Value) return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}
ValuePtr _string(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_string Params Error");
    if(params[0]->getType()==ValueType::Nil_Value)  return std::make_shared<BooleanValue>(false);
    if(env.eval(params[0])->getType()==ValueType::String_Value) return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}
ValuePtr _symbol(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_symbol Params Error");
    if(params[0]->getType()==ValueType::Symbol_Value) return std::make_shared<BooleanValue>(true);
    else return std::make_shared<BooleanValue>(false);
}

//辅助函数
bool isList(ValuePtr param){
    if(param->getType() == ValueType::Nil_Value) return true;
    ValuePtr rPtrTemp = std::dynamic_pointer_cast<PairValue>(param)->getRPtr();
    ValuePtr lPtrTemp = std::dynamic_pointer_cast<PairValue>(param)->getLPtr();;
    while(rPtrTemp->getType() == ValueType::Pair_Value){
            auto rTemp = static_cast<PairValue&>(*rPtrTemp);
            rPtrTemp=rTemp.getRPtr();
    }
    if(rPtrTemp->getType() == ValueType::Nil_Value){
            return true;
    }
    else{
            return false;
    }
}



ValuePtr _append(const std::vector<ValuePtr>& params,EvalEnv& env){
    //判断是不是列表
    std::vector<ValuePtr> result;
    for(auto param:params){
        if(!isList(param)) throw LispError("_append not a list");
        auto listVector = *param->toVector();
        result.insert(result.end(),listVector.begin(),listVector.end());
    }
    return _list(result,env);
}

ValuePtr _car(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_car Params Error");
    if(params[0]->getType()==ValueType::Pair_Value){
        return std::dynamic_pointer_cast<PairValue>(params[0])->getLPtr();
    }else return params[0];
}

ValuePtr _cdr(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1) throw LispError("_cdr Params Error 1");
    if(params[0]->getType()==ValueType::Pair_Value){
        return std::dynamic_pointer_cast<PairValue>(params[0])->getRPtr();
    }else return std::make_shared<NilValue>();
}

ValuePtr _cons(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2) throw LispError("_cons Params Error");
    return std::make_shared<PairValue>(params[0],params[1]);
}

ValuePtr _length(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=1&&!isList(params[0])) throw LispError("_length Params Error");
    else return std::make_shared<NumericValue>(params[0]->toVector()->size());
}

ValuePtr _list(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.empty()) return std::make_shared<NilValue>();
    std::vector<ValuePtr> args = params;
    args.erase(args.begin());
    if(!args.empty()) return std::make_shared<PairValue>(params[0],_list(args,env));
    else return std::make_shared<PairValue>(params[0],std::make_shared<NilValue>());
}

ValuePtr applyLambda(std::shared_ptr<LambdaValue> func, std::vector<ValuePtr> params,EvalEnv &env);

ValuePtr _map(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2 ||
        !(env.eval(params[0])->getType() ==ValueType::Builtin_Proc_Value ||
          env.eval(params[0])->getType() ==ValueType::Lambda_Value)||
        !isList(params[1])
        ) throw LispError("_map Params Error");

    auto args = *params[1]->toVector();
    decltype(args) resultVector;

    if(env.eval(params[0])->getType() ==ValueType::Builtin_Proc_Value){
        auto func = std::dynamic_pointer_cast<BuiltinProcValue>(env.eval(params[0]))->getFunc();
        for(auto arg:args){
            std::vector<ValuePtr> argVector{arg};
            resultVector.push_back(func(argVector,env));
        }
        return _list(resultVector,env);
    }
    else if(env.eval(params[0])->getType() ==ValueType::Lambda_Value){
        auto func = std::dynamic_pointer_cast<LambdaValue>(env.eval(params[0]));
        for(auto arg:args){
            std::vector<ValuePtr> argVector{arg};
            resultVector.push_back(applyLambda(func,argVector,env));
        }
        return _list(resultVector,env);
    }
}
//辅助函数
bool notVirtual(ValuePtr p,EvalEnv& e){
    if(e.eval(p)->getType()==ValueType::Boolean_Value &&
        !std::dynamic_pointer_cast<BooleanValue>(e.eval(p))->getValue())
        return false;
    else return true;
}

ValuePtr _filter(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2 ||
        !(env.eval(params[0])->getType() ==ValueType::Builtin_Proc_Value ||
          env.eval(params[0])->getType() ==ValueType::Lambda_Value)||
        !isList(params[1])
            ) throw LispError("_filter Params Error");

    auto args = *params[1]->toVector();
    decltype(args) resultVector;

    if(env.eval(params[0])->getType() ==ValueType::Builtin_Proc_Value){
        auto func = std::dynamic_pointer_cast<BuiltinProcValue>(env.eval(params[0]))->getFunc();
        for(auto arg:args){
            std::vector<ValuePtr> argVector{arg};
            if(notVirtual(func(argVector,env),env))
                resultVector.push_back(arg);
        }
        return _list(resultVector,env);
    }
    else if(env.eval(params[0])->getType() ==ValueType::Lambda_Value){
        auto func = std::dynamic_pointer_cast<LambdaValue>(env.eval(params[0]));
        for(auto arg:args){
            std::vector<ValuePtr> argVector{arg};
            if(notVirtual(applyLambda(func,argVector,env),env))
                resultVector.push_back(arg);
        }
        return _list(resultVector,env);
    }
}

ValuePtr _reduce(const std::vector<ValuePtr>& params,EvalEnv& env){
    if(params.size()!=2 || !isList(params[1])) throw LispError("_reduce Params Error");
    std::shared_ptr<PairValue> list = std::dynamic_pointer_cast<PairValue>(params[1]);
    if(list->toVector()->size() == 1) return list->getLPtr();
    else{
        if(env.eval(params[0])->getType() ==ValueType::Builtin_Proc_Value){
            auto func = std::dynamic_pointer_cast<BuiltinProcValue>(env.eval(params[0]))->getFunc();
            return func(std::vector<ValuePtr>{list->getLPtr(),
                                              _reduce(std::vector<ValuePtr>{params[0],list->getRPtr()},env) },env);
        }
        else if(env.eval(params[0])->getType() ==ValueType::Lambda_Value){
            auto func = std::dynamic_pointer_cast<LambdaValue>(env.eval(params[0]));
            return applyLambda(
                func,
                std::vector<ValuePtr>{list->getLPtr(),
                                      _reduce(std::vector<ValuePtr>{params[0],list->getRPtr()},
                                              env)
                },
                env);
        }
    }
}





