#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"


std::map<std::string,BaseRule*> RuleRegister::rules_;
void RuleRegister::addRule(const std::string& name, BaseRule* rule ) {
    rule->setName(name);
    rules_[name] = rule;
}

BaseRule* RuleRegister::getRule(const std::string& name ) {
    if (rules_.find(name) == rules_.end() ) {
        return NULL;
    }
    return rules_[name];
}
