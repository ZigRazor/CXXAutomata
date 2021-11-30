#ifndef CXXAUTOMATA_AUTOMATON
#define CXXAUTOMATA_AUTOMATON

#include "Typedefs.hpp"
#include <string>

namespace CXXAUTOMATA {

/**
 * @brief An abstract base class for all automata, including Turing machines.
 *
 */
class Automaton {
public:
  /**
   * @brief Construct a new Automaton object
   *
   */
  Automaton();
  /**
   * @brief Destroy the Automaton object
   *
   */
  virtual ~Automaton();
  /**
   * @brief Validate the Automaton
   *
   * @return true if this automaton is internally consistent
   * @return false otherwise
   */
  virtual bool validate() const = 0;
  /**
   * @brief Return a generator that yields each step while reading input.
   *
   * @param input_str
   * @return States
   */
  virtual States readInputStepwise(const InputSymbols &input_str) = 0;
  /**
  * @brief Check if the given string is accepted by this automaton.
  Return the automaton's final configuration if this string is valid.
  *
  * @param input_str
  * @return States
  */
  virtual State readInput(const InputSymbols &input_str);
  /**
   * @brief validate input to the automaton
   *
   * @param input_str
   * @return true if this automaton accepts the given input.
   * @return false otherwise
   */
  virtual bool acceptsInput(const InputSymbols &input_str);
  /**
   * @brief Copy Construct a new Automaton object
   *
   * @param other
   */
  Automaton(const Automaton &other);
  /**
   * @brief
   *
   * @param rhs
   * @return true if equal
   * @return false otherwise
   */
  virtual bool operator==(const Automaton &rhs) const;

  const States& getStates() const;
  const InputSymbols& getInputSymbols() const;
  const Transitions& getTransitions() const;
  const State& getInitialState() const ;
  const States& getFinalStates() const;

protected:
  /**
   * @brief Raise an error if the initial state is invalid.
   *
   */
  virtual void validateInitialState() const;
  /**
   * @brief Raise an error if the initial state has no transitions defined.
   *
   */
  virtual void validateInitialStateTransitions() const;
  /**
   * @brief Raise an error if any final states are invalid.
   *
   */
  virtual void validateFinalStates() const;

  States states;
  InputSymbols inputSymbols;
  Transitions transitions;
  State initialState;
  States finalStates;
};
} // namespace CXXAUTOMATA

#endif // CXXAUTOMATA_AUTOMATON
