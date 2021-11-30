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

class NFA;
class DFA : public FA {
public:
  DFA(const States &states, const InputSymbols &inputSymbols,
      const Transitions &transitions, const State &initialState,
      const States &finalStates, bool allowPartial = false);

  DFA(const DFA &dfa);
  virtual ~DFA();

  /**
   * @brief Return True if two DFAs are equivalent.
   *
   * @param other
   * @return true
   * @return false
   */
  bool operator==(const DFA &other) const;

  /**
   * @brief Return True if two DFAs are not equivalent.
   *
   * @param other
   * @return true
   * @return false
   */
  bool operator!=(const DFA &other) const;

  /**
   * @brief Return True if this DFA is a subset of (or equal to) another DFA.
   *
   * @param other
   * @return true
   * @return false
   */
  bool operator<=(const DFA &other) const;

  /**
   * @brief Return True if this DFA is a superset of another DFA.
   *
   * @param other
   * @return true
   * @return false
   */
  bool operator>=(const DFA &other) const;

  /**
   * @brief Return True if this DFA is a strict subset of another DFA.
   *
   * @param other
   * @return true
   * @return false
   */
  bool operator<(const DFA &other) const;

  /**
   * @brief Return True if this DFA is a strict superset of another DFA.
   *
   * @param other
   * @return true
   * @return false
   */
  bool operator>(const DFA &other) const;

  /**
   * @brief Return a DFA that is the difference of this DFA and another DFA.
   *
   * @param other
   * @return DFA
   */
  DFA operator-(const DFA &other) const;

  /**
   * @brief Return True if this DFA is internally consistent.
   *
   * @return true
   * @return false
   */
  bool validate() const override;

  /**
   * @brief Check if the given string is accepted by this DFA.
   *        Yield the current configuration of the DFA at each step.
   *
   * @param input_str
   * @return States
   */
  States readInputStepwise(const InputSymbols &input_str) override;

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
  DFA minify(bool retainNames = true) const;

  /**
   * @brief Takes as input two DFAs M1 and M2 which
   *        accept languages L1 and L2 respectively.
   *        Returns a DFA which accepts the union of L1 and L2.
   *
   * @param other
   * @param retainsName
   * @param minify
   * @return DFA
   */
  DFA unionJoin(const DFA &other, bool retainsName = false,
                bool minify = true) const;

  /**
   * @brief Takes as input two DFAs M1 and M2 which
   *        accept languages L1 and L2 respectively.
   *        Returns a DFA which accepts the intersection of L1 and L2.
   *
   * @param other
   * @param retainsName
   * @param minify
   * @return DFA
   */
  DFA intersection(const DFA &other, bool retainsName = false,
                   bool minify = true) const;

  /**
   * @brief Takes as input two DFAs M1 and M2 which
   *        accept languages L1 and L2 respectively.
   *        Returns a DFA which accepts the difference of L1 and L2.
   *
   * @param other
   * @param retainsName
   * @param minify
   * @return DFA
   */
  DFA difference(const DFA &other, bool retainsName = false,
                 bool minify = true) const;

  /**
   * @brief Takes as input two DFAs M1 and M2 which
   *        accept languages L1 and L2 respectively.
   *        Returns a DFA which accepts the symmetric difference of L1 and L2.
   *
   * @param other
   * @param retainsName
   * @param minify
   * @return DFA
   */
  DFA symmetricDifference(const DFA &other, bool retainsName = false,
                          bool minify = true) const;

  /**
   * @brief Return the complement of this DFA.
   *
   * @return DFA
   */
  DFA complement() const;

  /**
   * @brief Return True if this DFA is a subset of another DFA.
   *
   * @param other
   * @return true
   * @return false
   */
  bool isSubset(const DFA &other) const;

  /**
   * @brief Return True if this DFA is a superset of another DFA.
   *
   * @param other
   * @return true
   * @return false
   */
  bool isSuperset(const DFA &other) const;

  /**
   * @brief Return True if this DFA has no common elements with another DFA.
   *
   * @param other
   * @return true
   * @return false
   */
  bool isDisjoint(const DFA &other) const;

  /**
   * @brief Return True if this DFA is completely empty.
   *
   * @return true
   * @return false
   */
  bool isEmpty() const;

  /**
   * @brief Returns True if the DFA accepts a finite language, False otherwise.
   *
   * @return true
   * @return false
   */
  bool isFinite() const;

  /**
   * @brief Stringify the given set of states as a single state name.
   *
   * @param states
   * @return std::string
   */
  static std::string stringifyStatesUnsorted(const States &states);

  /**
   * @brief Stringify the given set of states as a single state name.
   *
   * @param states
   * @return std::string
   */
  static std::string stringifyStates(const States &states);

  /**
   * @brief Initialize this DFA as one equivalent to the given NFA.
   *
   * @param nfa
   * @return DFA
   */
  static DFA fromNFA(const NFA &nfa);

  /**
   * @brief  Creates the graph associated with this DFA
   *
   * @param path
   */
  void showDiagram(const std::string &path = "") const;

private:
  /**
   * @brief Raise an error if the transition input_symbols are missing.
   *
   * @param start_state
   * @param paths
   */
  void validateTransitionMissingSymbols(const State &start_state,
                                        const Paths &paths) const;

  /**
   * @brief Raise an error if transition input symbols are invalid.
   *
   * @param start_state
   * @param paths
   */
  void validateTransitionInvalidSymbols(const State &start_state,
                                        const Paths &paths) const;

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
  void validateTransitionEndStates(const State &start_state,
                                   const Paths &paths) const;

  /**
   * @brief Raise an error if transitions are missing or invalid.
   *
   * @param start_state
   * @param paths
   */
  void validateTransitions(const State &start_state, const Paths &paths) const;

  /**
   * @brief Follow the transition for the given input symbol on the current
   * state. Raise an error if the transition does not exist.
   *
   * @param current_state
   * @param input_symbol
   * @return State
   */
  State getNextCurrentState(const State &current_state,
                            const InputSymbol &input_symbol) const;

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

  /**
   * @brief Creates a new DFA which is the cross product of DFAs self and other
   *        with an empty set of final states.
   *
   * @param other
   * @return DFA
   */
  DFA crossProduct(const DFA &other) const;

  /**
   * @brief Returns a simple graph representation of the DFA.
   *
   * @return Graph
   */
  Graph makeGraph() const;

  /**
   * @brief Returns the graph G where all edges have been reversed.
   *
   * @param graph
   * @return Graph
   */
  Graph reverseGraph(const Graph &graph) const;

  /**
   * @brief Computes the set of reachable nodes
   *        in the graph G starting at vertex v.
   *
   * @param G
   * @param v
   * @param vis
   */
  void reachableNodes(const Graph &G, const State &v, States &vis) const;

  /**
   * @brief Computes the induced subgraph G[S].
   *
   * @param G
   * @param S
   * @return Graph
   */
  Graph inducedSubgraph(const Graph &G, const States &S) const;

  /**
   * @brief Returns True if the graph G contains a cycle, False otherwise.
   *
   * @param G
   * @return true
   * @return false
   */
  bool hasCycle(const Graph &G) const;

  /**
   * @brief Add NFA states to DFA as it is constructed from NFA.
   *
   * @param nfa
   * @param current_states
   * @param current_state_name
   * @param dfa_states
   * @param dfa_transitions
   * @param dfa_final_states
   */
  static void addNfaStatesFromQueue(const NFA &nfa, const State &currentState,
                                    const State &currentStateName,
                                    States &dfaStates,
                                    Transitions &dfaTransitions,
                                    States &dfaFinalStates);

  /**
   * @brief Enqueue the next set of current states for the generated DFA.
   *
   * @param nfa
   * @param currentStates
   * @param currentStateName
   * @param stateQueue
   * @param dfaTransitions
   */
  static void enqueueNextNfaCurrentStates(const NFA &nfa,
                                          const State &currentState,
                                          const State &currentStateName,
                                          States &stateQueue,
                                          Transitions &dfaTransitions);

  bool allowPartial;
};
} // namespace CXXAUTOMATA

#endif /* CXXAUTOMATA_DFA */
