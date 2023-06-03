//
// Created by Ray Chou on 2023/5/8.
//

#ifndef MINI_LISP_FORM_H
#define MINI_LISP_FORM_H

#include "value.h"
#include "error.h"
#include "eval_env.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <string>

using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
extern std::unordered_map<std::string, std::function<SpecialFormType>> SPECIAL_FORMS;

#endif  // MINI_LISP_FORM_H
