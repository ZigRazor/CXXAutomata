#ifndef CXXAUTOMATA_NFA
#define CXXAUTOMATA_NFA
#pragma once
#include "FA.hpp"

namespace CXXAUTOMATA {
class NFA : public FA {

private:
public:
  NFA();
  ~NFA();

  State getNextCurrentState(const State &current_state,
                               const InputSymbol &input_symbol) const;

  States getLambdaClosure(const State &current_state) const;

private:
  
};
}
#endif // CXXAUTOMATA_NFA	