#ifndef CXXAUTOMATA_FA
#define CXXAUTOMATA_FA

#include "Automaton.hpp"

namespace CXXAUTOMATA {
/**
 * @brief An abstract base class for finite automata.
 *
 */
class FA : public Automaton {
public:
  FA();
  virtual ~FA();
};
} // namespace CXXAUTOMATA

#endif /* CXXAUTOMATA_FA */
