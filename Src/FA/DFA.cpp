#include "DFA.hpp"
#include "Exceptions.hpp"
#include "NFA.hpp"
#include <algorithm>
#include <cassert>
#include <deque>
#include <sstream>
#include <functional>
#include <fstream>

namespace CXXAUTOMATA {
DFA::DFA(const States &state, const InputSymbols &inputSymbols,
         const Transitions &transitions, const State &initialState,
         const States &finalStates, bool allowPartial)
    : FA() {
  this->states = states;
  this->inputSymbols = inputSymbols;
  this->transitions = transitions;
  this->initialState = initialState;
  this->finalStates = finalStates;
  this->allowPartial = allowPartial;
  this->validate();
}

DFA::DFA(const DFA &dfa) {
  this->states = dfa.states;
  this->inputSymbols = dfa.inputSymbols;
  this->transitions = dfa.transitions;
  this->initialState = dfa.initialState;
  this->finalStates = dfa.finalStates;
  this->allowPartial = dfa.allowPartial;
}

DFA::~DFA() {}

bool DFA::operator==(const DFA &other) const {
  return symmetricDifference(other).isEmpty();
}

bool DFA::operator!=(const DFA &other) const { return !(*this == other); }
bool DFA::operator<=(const DFA &other) const { return isSubset(other); }
bool DFA::operator>=(const DFA &other) const { return isSuperset(other); }
bool DFA::operator<(const DFA &other) const {
  return (*this <= other) && (*this != other);
}
bool DFA::operator>(const DFA &other) const {
  return (*this >= other) and (*this != other);
}
DFA DFA::operator-(const DFA &other) const { return difference(other); }

void DFA::validateTransitionMissingSymbols(const State &start_state,
                                           const Paths &paths) const {
  if (allowPartial) {
    return;
  }
  for (auto inputSymbol : inputSymbols) {
    if (paths.find(inputSymbol) == paths.end()) {
      std::stringstream ss;
      ss << "state " << start_state
         << " is missing a transition for input symbol " << inputSymbol;
      throw MissingSymbolException(ss.str());
    }
  }
}

void DFA::validateTransitionInvalidSymbols(const State &start_state,
                                           const Paths &paths) const {
  for (auto path : paths) {
    if (std::find(inputSymbols.begin(), inputSymbols.end(), path.first) ==
        inputSymbols.end()) {
      std::stringstream ss;
      ss << "state " << start_state << " has an invalid transition symbol "
         << path.first;
      throw InvalidSymbolException(ss.str());
    }
  }
}

void DFA::validateTransitionStartStates() const {
  for (auto state : states) {
    if (transitions.find(state) == transitions.end()) {
      std::stringstream ss;
      ss << "transition start state " << state << " is missing";
      throw MissingStateException(ss.str());
    }
  }
}

void DFA::validateTransitionEndStates(const State &start_state,
                                      const Paths &paths) const {
  for (auto path : paths) {
    if (std::find(states.begin(), states.end(), path.second) == states.end()) {
      std::stringstream ss;
      ss << "end state " << path.second << "for transition on " << start_state
         << " is invalid";
      throw InvalidStateException(ss.str());
    }
  }
}

void DFA::validateTransitions(const State &start_state,
                              const Paths &paths) const {
  validateTransitionMissingSymbols(start_state, paths);
  validateTransitionInvalidSymbols(start_state, paths);
  validateTransitionEndStates(start_state, paths);
}

bool DFA::validate() const {
  validateTransitionStartStates();
  for (auto transition : transitions) {
    validateTransitions(transition.first, transition.second);
  }
  validateInitialState();
  validateFinalStates();
  return true;
}

State DFA::getNextCurrentState(const State &current_state,
                               const InputSymbol &input_symbol) const {
  if (transitions.at(current_state).find(input_symbol) ==
      transitions.at(current_state).end()) {
    std::stringstream ss;
    ss << input_symbol << " is not a valid input symbol";
    throw RejectionException(ss.str());
  } else {
    return transitions.at(current_state).at(input_symbol);
  }
}

void DFA::checkForInputRejection(const State &current_state) const {
  if (std::find(finalStates.begin(), finalStates.end(), current_state) !=
      finalStates.end()) {
    std::stringstream ss;
    ss << "the DFA stopped on a non-final state " << current_state;
    throw RejectionException(ss.str());
  }
}

States DFA::readInputStepwise(const InputSymbols &input_str) {
  States stateYield;
  State current_state = initialState;

  stateYield.push_back(current_state);
  for (auto input_symbol : input_str) {
    current_state = getNextCurrentState(current_state, input_symbol);
    stateYield.push_back(current_state);
  }
  checkForInputRejection(current_state);

  return stateYield;
}

DFA DFA::minify(bool retainNames) const {
  DFA newDfa = *this;
  newDfa.removeUnreachableStates();
  newDfa.mergeStates(retainNames);
  return newDfa;
}

void DFA::removeUnreachableStates() {
  auto reachableStates = computeReachableStates();
  States unreachableStates;
  for (auto state : states) {
    if (reachableStates.find(state) == reachableStates.end()) {
      unreachableStates.push_back(state);
    }
  }
  for (auto unreachableState : unreachableStates) {
    states.erase(std::find(states.begin(), states.end(), unreachableState));
    transitions.erase(transitions.find(unreachableState));
    if (std::find(finalStates.begin(), finalStates.end(), unreachableState) !=
        finalStates.end()) {
      finalStates.erase(
          std::find(finalStates.begin(), finalStates.end(), unreachableState));
    }
  }
}
std::set<State> DFA::computeReachableStates() const {
  std::set<State> reachableStates;
  std::deque<State> statesToCheck;
  statesToCheck.push_back(initialState);
  reachableStates.insert(initialState);
  while (!statesToCheck.empty()) {
    auto state = statesToCheck.front();
    statesToCheck.pop_front();
    for (auto transitionForState : transitions.at(state)) {
      if (reachableStates.find(transitionForState.second) ==
          reachableStates.end()) {
        reachableStates.insert(transitionForState.second);
        statesToCheck.push_back(transitionForState.second);
      }
    }
  }
  return reachableStates;
}

void DFA::mergeStates(bool retainNames) {
  std::vector<States> eqClasses_v;
  if (finalStates.size() != 0) {
    eqClasses_v.push_back(finalStates);
  }
  if (finalStates.size() != states.size()) {
    States s;
    std::sort(finalStates.begin(), finalStates.end());
    std::sort(states.begin(), states.end());
    std::set_difference(states.begin(), states.end(), finalStates.begin(),
                        finalStates.end(), std::inserter(s, s.end() - 1));
    eqClasses_v.push_back(s);
  }
  std::set<States> eqClasses(eqClasses_v.begin(), eqClasses_v.end());
  std::set<State> processing(finalStates.begin(), finalStates.end());

  while (!processing.empty()) {
    auto activeState = *processing.begin();
    processing.erase(processing.begin());
    for (auto activeLetter : inputSymbols) {
      States statesThatMoveIntoActiveState;
      for (auto state : states) {
        if (transitions.at(state).find(activeLetter) !=
                transitions.at(state).end() &&
            transitions.at(state).at(activeLetter) == activeState) {
          statesThatMoveIntoActiveState.push_back(
              transitions.at(state).at(activeLetter));
        }
      }
      auto copyEqClasses = eqClasses;
      for (auto checkingSet : copyEqClasses) {
        std::sort(checkingSet.begin(), checkingSet.end());
        std::sort(statesThatMoveIntoActiveState.begin(),
                  statesThatMoveIntoActiveState.end());
        std::vector<State> XIntY;
        auto lsi = std::set_intersection(checkingSet.begin(), checkingSet.end(),
                                         statesThatMoveIntoActiveState.begin(),
                                         statesThatMoveIntoActiveState.end(),
                                         XIntY.begin());
        if (XIntY.size() == 0) {
          continue;
        }
        std::vector<State> XDiffY;
        auto lsd = std::set_difference(checkingSet.begin(), checkingSet.end(),
                                       statesThatMoveIntoActiveState.begin(),
                                       statesThatMoveIntoActiveState.end(),
                                       XDiffY.begin());
        if (XDiffY.size() == 0) {
          continue;
        }
        eqClasses.erase(checkingSet);
        eqClasses.insert(XIntY);
        eqClasses.insert(XDiffY);

        bool found = true;
        for (auto state : checkingSet) {
          if (processing.find(state) == processing.end()) {
            found = false;
            break;
          }
        }
        if (found) {
          for (auto state : checkingSet) {
            processing.erase(state);
          }
          for (auto state : XDiffY) {
            processing.insert(state);
          }
          for (auto state : XIntY) {
            processing.insert(state);
          }

        } else {
          if (XIntY.size() < XDiffY.size()) {
            for (auto state : XIntY) {
              processing.insert(state);
            }
          } else {
            for (auto state : XDiffY) {
              processing.insert(state);
            }
          }
        }
      }
    }
  }

  auto rename = [&](const States &states) {
    if (states.size() == 1) {
      return states.at(0);
    } else {
      return stringifyStates(states);
    }
  };

  std::map<State, std::string> backMap;
  int i = 0;
  for (auto eqClass : eqClasses) {
    std::string name;
    if (retainNames) {
      name = rename(eqClass);
    } else {
      name = std::to_string(i);
    }

    for (auto state : eqClass) {
      backMap.insert(std::pair<State, std::string>(state, name));
    }
    i++;
  }
  auto newInputSymbols = inputSymbols;
  States newStates;
  if (retainNames) {
    for (auto eq : eqClasses) {
      auto state = rename(eq);
      newStates.push_back(state);
    }
  } else {
    for (int i = 0; i < eqClasses.size(); i++) {
      newStates.push_back(std::to_string(i));
    }
  }
  auto newInitialState = backMap.at(initialState);
  States newFinalStates;
  for (auto acc : finalStates) {
    newFinalStates.push_back(backMap.at(acc));
  }

  Transitions newTransitions;
  i = 0;
  for (auto eqClass : eqClasses) {
    std::string name;
    if (retainNames) {
      name = rename(eqClass);
    } else {
      name = std::to_string(i);
    }
    for (auto letter : inputSymbols) {
      newTransitions[name][letter] =
          backMap.at(transitions.at(eqClass.at(0)).at(letter));
    }
    i++;
  }

  states = newStates;
  inputSymbols = newInputSymbols;
  transitions = newTransitions;
  initialState = newInitialState;
  finalStates = newFinalStates;
}

DFA DFA::crossProduct(const DFA &other) const {
  assert(inputSymbols == other.inputSymbols);
  auto states_a = states;
  auto states_b = other.states;
  States newStates;
  for (auto state_a : states_a) {
    for (auto state_b : states_b) {
      States statesToAdd;
      statesToAdd.push_back(state_a);
      statesToAdd.push_back(state_b);
      newStates.push_back(stringifyStatesUnsorted(statesToAdd));
    }
  }

  Transitions newTransitions;
  for (auto transitionIt = transitions.begin();
       transitionIt != transitions.end(); transitionIt++) {
    for (auto otherTransitionIt = other.transitions.begin();
         otherTransitionIt != other.transitions.end(); otherTransitionIt++) {
           States statesToAdd;
            statesToAdd.push_back(transitionIt->first);
            statesToAdd.push_back(otherTransitionIt->first);
      auto new_state = stringifyStatesUnsorted(statesToAdd);
      for (auto symbol : inputSymbols) {
        States statesToAdd;
            statesToAdd.push_back(transitionIt->second.at(symbol));
            statesToAdd.push_back(otherTransitionIt->second.at(symbol));
        newTransitions[new_state][symbol] = stringifyStatesUnsorted(statesToAdd);
      }
    }
  }
  States statesToAdd;
            statesToAdd.push_back(initialState);
            statesToAdd.push_back(other.initialState);
  auto newInitialState =
      stringifyStatesUnsorted(statesToAdd);

  return DFA(newStates, inputSymbols, newTransitions, newInitialState,
             finalStates);
}

DFA DFA::unionJoin(const DFA &other, bool retainsName, bool minify) const {
  auto newDFA = crossProduct(other);
  for (auto stateA : states) {
    for (auto stateB : other.states) {
      if (std::find(finalStates.begin(), finalStates.end(), stateA) !=
              finalStates.end() ||
          std::find(other.finalStates.begin(), other.finalStates.end(),
                    stateB) != other.finalStates.end()) {
                      States statesToAdd;
            statesToAdd.push_back(stateA);
            statesToAdd.push_back(stateB);
        newDFA.finalStates.push_back(stringifyStatesUnsorted(statesToAdd));
      }
    }
  }
  if (minify) {
    return newDFA.minify(retainsName);
  }
  return newDFA;
}

DFA DFA::intersection(const DFA &other, bool retainsName, bool minify) const {
  auto newDFA = crossProduct(other);
  for (auto stateA : states) {
    for (auto stateB : other.states) {
      States statesToAdd;
            statesToAdd.push_back(stateA);
            statesToAdd.push_back(stateB);
      newDFA.finalStates.push_back(stringifyStatesUnsorted(statesToAdd));
    }
  }
  if (minify) {
    return newDFA.minify(retainsName);
  }
  return newDFA;
}

DFA DFA::difference(const DFA &other, bool retainsName, bool minify) const {
  auto newDFA = crossProduct(other);
  for (auto stateA : states) {
    for (auto stateB : other.states) {
      if (std::find(other.finalStates.begin(), other.finalStates.end(),
                    stateB) == other.finalStates.end()) {
                      States statesToAdd;
            statesToAdd.push_back(stateA);
            statesToAdd.push_back(stateB);
        newDFA.finalStates.push_back(stringifyStatesUnsorted(statesToAdd));
      }
    }
  }
  if (minify) {
    return newDFA.minify(retainsName);
  }
  return newDFA;
}

DFA DFA::symmetricDifference(const DFA &other, bool retainsName,
                             bool minify) const {
  auto newDFA = crossProduct(other);
  for (auto stateA : states) {
    for (auto stateB : other.states) {
      if ((std::find(finalStates.begin(), finalStates.end(), stateA) !=
               finalStates.end() &&
           std::find(other.finalStates.begin(), other.finalStates.end(),
                     stateB) == other.finalStates.end()) ||
          (std::find(finalStates.begin(), finalStates.end(), stateA) ==
               finalStates.end() &&
           std::find(other.finalStates.begin(), other.finalStates.end(),
                     stateB) != other.finalStates.end())) {
                       States statesToAdd;
            statesToAdd.push_back(stateA);
            statesToAdd.push_back(stateB);
        newDFA.finalStates.push_back(stringifyStatesUnsorted(statesToAdd));
      }
    }
  }
  if (minify) {
    return newDFA.minify(retainsName);
  }
  return newDFA;
}

DFA DFA::complement() const {
  auto newDFA = *this;
  for (auto state : states) {
    if (std::find(finalStates.begin(), finalStates.end(), state) ==
        finalStates.end()) {
      newDFA.finalStates.push_back(state);
    }
  }
  return newDFA;
}

bool DFA::isSubset(const DFA &other) const {
  return intersection(other) == (*this);
}

bool DFA::isSuperset(const DFA &other) const { return other.isSubset(*this); }

bool DFA::isDisjoint(const DFA &other) const {
  return intersection(other).isEmpty();
}

bool DFA::isEmpty() const { return minify().states.size() == 0; }

Graph DFA::makeGraph() const {
  Graph g;
  for (auto transition : transitions) {
    for (auto path : transition.second) {
      g[transition.first].push_back(path.second);
    }
  }
  return g;
}

Graph DFA::reverseGraph(const Graph &graph) const {
  Graph revG;
  for (auto keyValues : graph) {
    for (auto value : keyValues.second) {
      revG[value].push_back(keyValues.first);
    }
  }
  return revG;
}

void DFA::reachableNodes(const Graph &G, const State &v,
                         States &vis) const {
  if (std::find(vis.begin(), vis.end(), v) == vis.end()) {
    vis.push_back(v);
    for (auto w : G.at(v)) {
      reachableNodes(G, w, vis);
    }
  }
}

Graph DFA::inducedSubgraph(const Graph &G, const States &S) const {
  Graph subG;
  for (auto k : G) {
    if (std::find(S.begin(), S.end(), k.first) != S.end()) {
      for (auto x : k.second) {
        if (std::find(S.begin(), S.end(), x) != S.end()) {
          subG[k.first].push_back(x);
        }
      }
    }
  }
  return subG;
}

bool DFA::hasCycle(const Graph &G) const {
    std::function<bool(const Graph &, const State &, States &,
                                States &)> dfs = [dfs](const Graph &G, const State &at, States &vis,
                                States &stack) -> bool{
    // Helper function which accepts input parameters for the graph, current
    // node, visited set and current stack
    if (std::find(vis.begin(), vis.end(), at) == vis.end()) {
      vis.push_back(at);
      stack.push_back(at);
      for (auto k : G.at(at)) {
        if (std::find(vis.begin(), vis.end(), k) == vis.end() &&
            dfs(G, k, vis, stack)) {
          return true;
        } else if (std::find(stack.begin(), stack.end(), k) != stack.end()) {
          // We have seen this vertex before in the path
          return true;
        }
      }
      stack.erase(std::find(stack.begin(), stack.end(), at));
    }
    return false;
  };
  bool result = false;
  for (auto v : G) {
    States vis, stack;
    result = dfs(G, v.first, vis, stack);
    if (result) {
      break;
    }
  }
  return result;
}

bool DFA::isFinite() const {
  auto g = makeGraph();
  auto revG = reverseGraph(g);

  States accessibleNodes;
  reachableNodes(g, initialState, accessibleNodes);
  States coaccessibleNodes;
  for (auto state : finalStates) {
    reachableNodes(revG, state, coaccessibleNodes);
  }

  States importantNodes;
  std::sort(accessibleNodes.begin(), accessibleNodes.end());
  std::sort(coaccessibleNodes.begin(), coaccessibleNodes.end());

  std::set_intersection(accessibleNodes.begin(), accessibleNodes.end(),
                        coaccessibleNodes.begin(), coaccessibleNodes.end(),
                        back_inserter(importantNodes));

  auto constrainedG = inducedSubgraph(g, importantNodes);

  auto containsCycle = hasCycle(constrainedG);

  return !containsCycle;
}

std::string DFA::stringifyStatesUnsorted(const States &states) {
  std::stringstream ss;
  for (int i = 0; i < states.size(); i++) {
    ss << states[i];
    if (i != states.size() - 1) {
      ss << ",";
    }
  }
  return ss.str();
}

std::string DFA::stringifyStates(const States &states) {
  std::stringstream ss;
  States sortedStates = states;
  std::sort(sortedStates.begin(), sortedStates.end());
  for (int i = 0; i < sortedStates.size(); i++) {
    ss << sortedStates[i];
    if (i != sortedStates.size() - 1) {
      ss << ",";
    }
  }
  return ss.str();
}

void DFA::addNfaStatesFromQueue(const NFA &nfa, const State &currentState,
                                const State &currentStateName,
                                States &dfaStates, Transitions &dfaTransitions,
                                States &dfaFinalStates) {
  dfaStates.push_back(currentStateName);
  dfaTransitions[currentStateName] = Paths();
  if (std::find(nfa.getFinalStates().begin(),nfa.getFinalStates().end(),currentState) != nfa.getFinalStates().end()) {
    dfaFinalStates.push_back(currentStateName);
  }
}

void DFA::enqueueNextNfaCurrentStates(const NFA &nfa, const State &currentState,
                                      const State &currentStateName,
                                      States &stateQueue,
                                      Transitions &dfaTransitions) {
  for (auto inputSymbol : nfa.getInputSymbols()) {
    State nextCurrentState =
        nfa.getNextCurrentState(currentState, inputSymbol);
    dfaTransitions[currentStateName][inputSymbol] =
        stringifyStates({nextCurrentState});
    stateQueue.push_back(nextCurrentState);
  }
}

DFA DFA::fromNFA(const NFA &nfa) {
  States dfaStates;
  InputSymbols dfaInputSymbols = nfa.getInputSymbols();
  Transitions dfaTransitions;
  auto nfaInitialState = nfa.getLambdaClosure(nfa.getInitialState());
  State dfaInitialState = stringifyStates(nfaInitialState);
  States dfaFinalStates;

  States stateQueue;
  for(auto state :nfaInitialState){
    stateQueue.push_back(state);
  }
  
  while (!stateQueue.empty()) {
    State currentState = stateQueue.front();
    stateQueue.erase(stateQueue.begin());
    State currentStateName = stringifyStates({currentState});
    if (std::find(dfaStates.begin(), dfaStates.end(),currentStateName) != dfaStates.end()) {
      // We've been here before and nothing should have changed.
      continue;
    }
    addNfaStatesFromQueue(nfa, currentState, currentStateName, dfaStates,
                          dfaTransitions, dfaFinalStates);
    enqueueNextNfaCurrentStates(nfa, currentState, currentStateName, stateQueue,
                                dfaTransitions);
  }

  return DFA(dfaStates, dfaInputSymbols, dfaTransitions, dfaInitialState,
             dfaFinalStates);
}

void DFA::showDiagram(const std::string &path) const {
  std::ofstream dotFile;
  dotFile.open(path + ".dot");
  dotFile << "digraph DFA {\n";
  dotFile << "rankdir=LR;\n";
  dotFile << "node [shape = circle];\n";
  for (auto state : states) {
    dotFile << state << ";\n";
  }
  for (auto state : states) {
    for (auto inputSymbol : inputSymbols) {
      dotFile << state << " -> " << transitions.at(state).at(inputSymbol)
              << " [label = \"" << inputSymbol << "\"];\n";
    }
  }
  for (auto state : finalStates) {
    dotFile << state << " [shape = doublecircle];\n";
  }
  dotFile << "}\n";
  dotFile.close();
  std::string command = "dot -Tpng " + path + ".dot -o " + path + ".png";
  system(command.c_str());
}

} // namespace CXXAUTOMATA
