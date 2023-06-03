//
// Created by Ray Chou on 2023/5/7.
//

#ifndef MINI_LISP_VALUE_H
#define MINI_LISP_VALUE_H

#include <string>
#include <functional>
enum class ValueType {
    Boolean_Value,
    Numeric_Value,
    String_Value,
    Nil_Value,
    Symbol_Value,
    Pair_Value,
    Builtin_Proc_Value,
    Lambda_Value
};

class EvalEnv;

class Value;
using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&,EvalEnv& );


class Value {
private:
    ValueType valueType;
public:
    Value(ValueType);
    ValueType getType(){
        return valueType;
    }
    virtual std::string toString() const = 0 ;
    virtual std::optional<std::vector<ValuePtr>> toVector() const;

    virtual std::optional<std::string> asSymbol() const;
    virtual std::optional<double> asNumeric() const;
};

class BooleanValue : public Value{
private:
    bool value;
public:
    BooleanValue(bool);
    std::string toString() const;
    bool getValue() const;
};

class NumericValue : public Value{
private:
    double value;
public:
    NumericValue(double);
    std::string toString() const;
    std::optional<double> asNumeric() const;
};

class StringValue : public Value{
private:
    std::string value;
public:
    StringValue(const std::string&);
    std::string toString() const;
    std::string getValue() const;
};

class NilValue : public Value{
public:
    NilValue();
    std::string toString() const;
    std::optional<std::vector<ValuePtr>> toVector() const;
};


class PairValue : public Value{
    friend ValuePtr calPair(ValuePtr root,EvalEnv& env);
private:
    ValuePtr lPtr;
    ValuePtr rPtr;
    std::string toStringNude() const;
public:
    PairValue(const ValuePtr&,const ValuePtr&);
    std::string toString() const;
    std::optional<std::vector<ValuePtr>> toVector() const;
    ValuePtr getRPtr() const;
    ValuePtr getLPtr() const;
};


class SymbolValue : public Value{
private:
    std::string value;

public:
    SymbolValue(const std::string&);
    std::string toString() const;
    std::optional<std::string> asSymbol() const;
    std::string getValue() const;
};



class BuiltinProcValue : public Value{
private:
    std::function<BuiltinFuncType> func;
public:
    BuiltinProcValue(const std::string& name, std::function<BuiltinFuncType> f): Value(ValueType::Builtin_Proc_Value),func(f){}
    std::string toString() const;
    std::function<BuiltinFuncType> getFunc() const;

};

class LambdaValue : public Value {
private:
    std::shared_ptr<EvalEnv> env;
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    // [...]
public:
    LambdaValue(std::shared_ptr<EvalEnv> e,const std::vector<std::string>& p,const std::vector<ValuePtr>& b);
    std::string toString() const;
    std::shared_ptr<EvalEnv> getEnv() const ;
    std::vector<std::string> getParams() const ;
    std::vector<ValuePtr> getBody() const ;
};

#endif  // MINI_LISP_VALUE_H
