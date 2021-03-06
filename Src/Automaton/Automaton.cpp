#include "Automaton.hpp"
#include "Exceptions.hpp"
#include <algorithm>
#include <sstream>

namespace CXXAUTOMATA {

Automaton::Automaton() {}

Automaton::~Automaton() {}

bool Automaton::operator==(const Automaton &rhs) const {
  return (states == rhs.states) && (inputSymbols == rhs.inputSymbols) &&
         (transitions == rhs.transitions) &&
         (initialState == rhs.initialState) && (finalStates == rhs.finalStates);
}

Automaton::Automaton(const Automaton &other) {}

State Automaton::readInput(const InputSymbols_v &input_str) {
  auto validation_generator = readInputStepwise(input_str);
  auto last_state = validation_generator.back();
  return last_state;
}

bool Automaton::acceptsInput(const InputSymbols_v &input_str) {
  try {
    readInput(input_str);
    return true;
  } catch (...) {
    return false;
  }
}

void Automaton::validateInitialState() const {
  if (std::find(states.begin(), states.end(), initialState) == states.end()) {
    std::stringstream ss;
    ss << initialState << " is not a valid initial state.";
    throw InvalidStateException(ss.str());
  }
}

void Automaton::validateInitialStateTransitions() const {
  if (transitions.find(initialState) == transitions.end()) {
    std::stringstream ss;
    ss << "initial state " << initialState << " has no transitions defined.";
    throw MissingStateException(ss.str());
  }
}
void Automaton::validateFinalStates() const {
  for (auto state : finalStates) {
    if (std::find(states.begin(), states.end(), state) == states.end()) {
      std::stringstream ss;
      ss << state << " is not a valid final state.";
      throw InvalidStateException(ss.str());
    }
  }
}

const States &Automaton::getStates() const { return states; }
const InputSymbols &Automaton::getInputSymbols() const { return inputSymbols; }
const Transitions &Automaton::getTransitions() const { return transitions; }
const State &Automaton::getInitialState() const { return initialState; }
const States &Automaton::getFinalStates() const { return finalStates; }

} // namespace CXXAUTOMATA