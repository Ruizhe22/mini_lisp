//
// Created by Ray Chou on 2023/5/7.
//

#include "value.h"
#include "eval_env.h"

#include <sstream>
#include <iostream>

Value::Value(ValueType v):valueType(v){}

std::optional<std::vector<ValuePtr>> Value::toVector() const {
    return std::nullopt;
}

std::optional<std::vector<ValuePtr>> NilValue::toVector() const{
    return std::vector<ValuePtr>{};
}

std::optional<std::vector<ValuePtr>> PairValue::toVector() const {
    std::vector<ValuePtr> vPtr;
    ValuePtr rPtrTemp = rPtr;
    ValuePtr lPtrTemp = lPtr;
    while(rPtrTemp->getType() == ValueType::Pair_Value){
        auto rTemp = static_cast<PairValue&>(*rPtrTemp);
        vPtr.push_back(lPtrTemp);
        lPtrTemp=rTemp.lPtr;
        rPtrTemp=rTemp.rPtr;
    }
    if(rPtrTemp->getType() == ValueType::Nil_Value){
        vPtr.push_back(lPtrTemp);
        return vPtr;
    }
    else{
        vPtr.push_back(lPtrTemp);
        vPtr.push_back(rPtrTemp);
        return vPtr;
    }

}

std::optional<std::string> Value::asSymbol() const {
    return std::nullopt;
}

std::optional<std::string> SymbolValue::asSymbol() const {
    return value;
}

std::optional<double> Value::asNumeric() const {
    return std::nullopt;
}
std::optional<double> NumericValue::asNumeric() const {
    return value;
}



BooleanValue::BooleanValue(bool v):Value(ValueType::Boolean_Value),value(v){}
std::string BooleanValue::toString() const {
    return value?"#t":"#f";
}
bool BooleanValue::getValue() const{
    return value;
}

NumericValue::NumericValue(double v):Value(ValueType::Numeric_Value),value(v){}
std::string NumericValue::toString() const {
    std::ostringstream oss;
    oss<<value;
    return oss.str();
}

StringValue::StringValue(const std::string& v):Value(ValueType::String_Value),value(v){}
std::string StringValue::toString() const {
    std::string  s= value;
    unsigned pos=0;
    while((pos = s.find_first_of('\"',pos)) != -1){
        s = s.replace(pos, std::strlen("\""),"\\\"");
        pos+=2;
    }
    return "\""+s+"\"";
}

NilValue::NilValue():Value(ValueType::Nil_Value){}
std::string NilValue::toString() const {
    return "()";
}

SymbolValue::SymbolValue(const std::string& v):Value(ValueType::Symbol_Value),value(v){}
std::string SymbolValue::toString() const {
    return value;
}

PairValue::PairValue(const ValuePtr& lp,const ValuePtr& rp):Value(ValueType::Pair_Value),lPtr(lp),rPtr(rp){}

std::string PairValue::toString() const {
    if(typeid(*rPtr)==typeid(PairValue)){
        auto& rPair = static_cast<const PairValue&>(*rPtr);
        return "(" + lPtr->toString() + rPair.toStringNude() +")" ;
    }
    else if(typeid(*rPtr)==typeid(NilValue)){
        return "(" + lPtr->toString() +")" ;
    }
    else{
        return "(" + lPtr->toString() + " . " + rPtr->toString() +")" ;
    }

}

ValuePtr PairValue::getRPtr() const {
    return rPtr;
}
ValuePtr PairValue::getLPtr() const {
    return lPtr;
}

std::string PairValue::toStringNude() const{
    if(typeid(*rPtr)==typeid(PairValue)){
        auto& rPair = static_cast<const PairValue&>(*rPtr) ;
        return " "+lPtr->toString() + rPair.toStringNude() ;
    }
    else if(typeid(*rPtr)==typeid(NilValue)){
        return " "+lPtr->toString() ;
    }
    else{
        return " "+lPtr->toString() + " . " + rPtr->toString() ;
    }
}

std::string BuiltinProcValue::toString() const{
    return "#<procedure:+>";
}

std::string LambdaValue::toString() const{
    return "#<procedure:+>";
}

std::function<ValuePtr(const std::vector<ValuePtr>&,EvalEnv& )> BuiltinProcValue::getFunc() const{
    return func;
}

std::ostream& operator<<(std::ostream& os,const Value& vObj){
    os << vObj.toString();
    return os;
}

LambdaValue::LambdaValue(std::shared_ptr<EvalEnv> e,const std::vector<std::string>& p,const std::vector<ValuePtr>& b)
    : Value(ValueType::Lambda_Value),env(std::make_shared<EvalEnv>(e)),params(p),body(b){}

std::vector<std::string> LambdaValue::getParams() const{
    return params;
}
std::vector<ValuePtr> LambdaValue::getBody() const{
    return body;
}
std::shared_ptr<EvalEnv> LambdaValue::getEnv() const {
    return env;
}

std::string StringValue::getValue() const{
    return value;
}

std::string SymbolValue::getValue() const{
    return value;
}