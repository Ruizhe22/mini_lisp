//
// Created by Ray Chou on 2023/5/8.
//

#include "form.h"
#include "eval_env.h"


//暂时只定义变量
ValuePtr defineForm(const std::vector<ValuePtr>& args,EvalEnv& env) {
    if(args.empty()) throw LispError("Malformed define.");

    if (auto name = args[0]->asSymbol()) {
        env.dictionary[*name] = env.eval(args[1]);
        return std::make_shared<NilValue>();
    }
    else if(args[0]->getType()==ValueType::Pair_Value){
        std::optional<std::string> funcName ;
        std::vector<std::string> vPara;

        if(funcName = std::dynamic_pointer_cast<PairValue>(args[0])->getLPtr()->asSymbol()){
            auto vParaPtr = *std::dynamic_pointer_cast<PairValue>(args[0])->getRPtr()->toVector();
            for(auto p:vParaPtr){
                if(p->asSymbol()){
                    vPara.push_back(*p->asSymbol());
                }
                else throw LispError("Malformed define.");
            }
            std::vector<ValuePtr> vBodyPtr = args;
            vBodyPtr.erase(vBodyPtr.begin());
            env.dictionary[*funcName]=std::make_shared<LambdaValue>(env.shared_from_this(),vPara,vBodyPtr);
            return std::make_shared<NilValue>();
        }
        else{
            throw LispError("Malformed define.");
        }

    }
    else{
        LispError("Malformed define.");
    }
}

ValuePtr quoteForm(const std::vector<ValuePtr>& params,EvalEnv& env) {
    if(params.empty()) throw LispError("Malformed quote.");
    return params[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args,EvalEnv& env) {
    if(args.size()!=3) throw LispError("Malformed if.");
    auto value = env.eval(args[0]);
    if(value->getType()==ValueType::Boolean_Value && !std::dynamic_pointer_cast<BooleanValue>(value)->getValue()){
        return env.eval(args[2]);
    }
    else{
        return env.eval(args[1]);
    }

}

ValuePtr andForm(const std::vector<ValuePtr>& args,EvalEnv& env) {
    if(args.empty()) return std::make_shared<BooleanValue>(true);
    ValuePtr tempPtr;
    for(auto& p : args){
        if(( tempPtr = env.eval(p))->getType()==ValueType::Boolean_Value
            && !std::dynamic_pointer_cast<BooleanValue>(tempPtr)->getValue()){
            return std::make_shared<BooleanValue>(false);
        }
    }
    return tempPtr;
}

ValuePtr orForm(const std::vector<ValuePtr>& args,EvalEnv& env) {
    if(args.empty()) return std::make_shared<BooleanValue>(false);
    ValuePtr tempPtr;
    for(auto& p : args){
        if((tempPtr=env.eval(p))->getType()!=ValueType::Boolean_Value
            || std::dynamic_pointer_cast<BooleanValue>(tempPtr)->getValue()){
            return tempPtr;
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args,EvalEnv& env) {
    if(args[0]->getType()==ValueType::Pair_Value){
        std::vector<std::string> vPara;
        auto vParaPtr = *args[0]->toVector();
        for(auto p:vParaPtr){
            if(p->asSymbol()){
                vPara.push_back(*p->asSymbol());
            }
            else throw LispError("Malformed lambda.");
        }
        std::vector<ValuePtr> vBodyPtr = args;
        vBodyPtr.erase(vBodyPtr.begin());
        return std::make_shared<LambdaValue>(env.shared_from_this(),vPara,vBodyPtr);
    } else{
        LispError("Malformed lambda.");
    }
}

bool notVirtual(ValuePtr p,EvalEnv& e);

ValuePtr condForm(const std::vector<ValuePtr>& args,EvalEnv& env){
    for(int i=0;i<args.size();++i){
        //每一个都是一个pair
        std::shared_ptr<PairValue> arg = std::dynamic_pointer_cast<PairValue>(args[i]);
        ValuePtr condition = arg->getLPtr();
        ValuePtr expressionsPair = arg->getRPtr();
        std::vector<ValuePtr> expressions = *expressionsPair->toVector();
        if(condition->asSymbol()&& (*condition->asSymbol()=="else")){
            if(i==args.size()-1){
                ValuePtr tempValue;
                for(auto expr : expressions){
                    tempValue = env.eval(expr);
                }
                return tempValue;
            }
            else throw LispError("else is not the last");
        }
        else if(notVirtual(env.eval(condition),env)){
            ValuePtr tempValue=env.eval(condition);
            for(auto expr : expressions){
                tempValue = env.eval(expr);
            }
            return tempValue;
        }
        else
            continue ;
    }
}

ValuePtr beginForm(const std::vector<ValuePtr>& args,EvalEnv& env){
    ValuePtr tempValue;
    for(auto arg : args){
        tempValue = env.eval(arg);
    }
    return tempValue;
}

//racket测试  (let (x 5) (+ x x))是错误格式 (let ((x 5)) (+ x x))是正确格式
ValuePtr letForm(const std::vector<ValuePtr>& args,EvalEnv& env){
    auto childEnvPtr{std::make_shared<EvalEnv>(env)};
    std::shared_ptr<PairValue> bindingPair = std::dynamic_pointer_cast<PairValue>(args[0]);
    std::vector<ValuePtr> bindings = *bindingPair->toVector();
    for(auto bind : bindings){
        childEnvPtr->dictionary[*std::dynamic_pointer_cast<PairValue>(bind)->getLPtr()->asSymbol()]=env.eval(std::dynamic_pointer_cast<PairValue>(std::dynamic_pointer_cast<PairValue>(bind)->getRPtr())->getLPtr());
    }
    ValuePtr tempValue;
    for(int i=1;i<args.size();++i){
        tempValue =  childEnvPtr->eval(args[i]);
    }
    return tempValue;
}

//递归找pair是unquote并替换
//作为pair的友元访问并修改私有成员
ValuePtr calPair(ValuePtr root,EvalEnv& env){
    if(root->getType() == ValueType::Pair_Value){
        ValuePtr lPtr =  std::dynamic_pointer_cast<PairValue>(root)->lPtr;
        ValuePtr rPtr =  std::dynamic_pointer_cast<PairValue>(root)->rPtr;
        if(lPtr->asSymbol() && (*lPtr->asSymbol()=="unquote")){
            return env.eval(std::dynamic_pointer_cast<PairValue>(rPtr)->lPtr);
        }
        else{
            std::dynamic_pointer_cast<PairValue>(root)->lPtr = calPair(lPtr,env);
            std::dynamic_pointer_cast<PairValue>(root)->rPtr = calPair(rPtr,env);
            return root;
        }
    }
    else return root;
}

ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args,EvalEnv& env){
    auto root = std::dynamic_pointer_cast<PairValue>(args[0]);
    return calPair(root,env);
}


std::unordered_map<std::string, std::function<SpecialFormType>> SPECIAL_FORMS{
    {"define",defineForm},
    {"quote",quoteForm},
    {"if",ifForm},
    {"and",andForm},
    {"or",orForm},
    {"lambda",lambdaForm},
    {"cond",condForm},
    {"begin",beginForm},
    {"let",letForm},
    {"quasiquote",quasiquoteForm}
};

