#ifndef CXXAUTOMATA_DFA
#define CXXAUTOMATA_DFA

#include "FA.hpp"
#include "Typedefs.hpp"
#include <set>

namespace CXXAUTOMATA {
/**
 * @brief A deterministic finite automaton.
 *
 */
class DFA : public FA {
public:
  DFA(const States &states, const InputSymbols &inputSymbols,
      const Transitions &transitions, const State &initialState,
      const States &finalStates, bool allowPartial = false);
  virtual ~DFA();

  /**
             * @brief Return True if two DFAs are equivalent.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator==(const DFA& other) const;

            /**
             * @brief Return True if two DFAs are not equivalent.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(const DFA& other) const;

            /**
             * @brief Return True if this DFA is a subset of (or equal to) another DFA.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator<=(const DFA& other) const;

            /**
             * @brief Return True if this DFA is a superset of another DFA.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator>=(const DFA& other) const;

            /**
             * @brief Return True if this DFA is a strict subset of another DFA.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator<(const DFA& other) const;

            /**
             * @brief Return True if this DFA is a strict superset of another DFA.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator>(const DFA& other) const;

            /**
             * @brief Return a DFA that is the difference of this DFA and another DFA.
             * 
             * @param other 
             * @return DFA 
             */
            DFA operator-(const DFA& other) const;

            /**
             * @brief Return True if this DFA is internally consistent.
             * 
             * @return true 
             * @return false 
             */
            bool validate() const;

            /**
             * @brief Check if the given string is accepted by this DFA.
             *        Yield the current configuration of the DFA at each step.
             * 
             * @param input_str 
             * @return States 
             */
            States readInputStepwise(const InputSymbols& input_str) override;

            /**
             * @brief Create a minimal DFA which accepts the same inputs as this DFA.
             *        First, non-reachable states are removed.
             *        Then, similiar states are merged using Hopcroft's Algorithm.
             *        retain_names: If True, merged states retain names.
             *        If False, new states will be named 0, ..., n-1.
             * 
             * @param retain_names 
             * @return DFA 
             */
            DFA minify(bool retain_names=true) const;

        private:
            /**
             * @brief Raise an error if the transition input_symbols are missing.
             * 
             * @param start_state 
             * @param paths 
             */
            void validateTransitionMissingSymbols(const State& start_state,const Paths& paths) const;

            /**
             * @brief Raise an error if transition input symbols are invalid.
             * 
             * @param start_state 
             * @param paths 
             */
            void validateTransitionInvalidSymbols(const State& start_state,const Paths& paths) const;

            /**
             * @brief Raise an error if transition start states are missing.
             * 
             */
            void validateTransitionStartStates() const;

            /**
             * @brief Raise an error if transition end states are invalid.
             * 
             * @param start_state 
             * @param paths 
             */
            void validateTransitionEndStates(const State& start_state,const Paths& paths) const;

            /**
             * @brief Raise an error if transitions are missing or invalid.
             * 
             * @param start_state 
             * @param paths 
             */
            void validateTransitions(const State& start_state,const Paths& paths) const;            

            /**
             * @brief Follow the transition for the given input symbol on the current state.
             *        Raise an error if the transition does not exist.
             * 
             * @param current_state 
             * @param input_symbol 
             * @return State 
             */
            State getNextCurrentState(const State& current_state, const InputSymbol& input_symbol) const;

            /**
             * @brief Raise an error if the given config indicates rejected
             * input.
             *
             * @param current_state
             */
            void checkForInputRejection(const State &current_state) const;

            /**
             * @brief Remove states which are not reachable from the initial
             * state.
             *
             */
            void removeUnreachableStates();

            /**
             * @brief Compute the states which are reachable from the initial
             * state.
             *
             * @return a set of unreachable states
             */
            std::set<State> computeReachableStates() const;

            void mergeStates(bool retainNames = false);

            bool allowPartial;
};
}

#endif /* CXXAUTOMATA_DFA */
