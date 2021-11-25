#include <deque>
#include <algorithm>
#include "DFA.hpp"
#include "Exceptions.hpp"

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

    DFA::~DFA(){}

    bool DFA::operator==(const DFA& other) const{
        return symmetric_difference(other).isempty();
    }

    bool DFA::operator!=(const DFA& other) const{
        return !(*this == other);
    }
            bool DFA::operator<=(const DFA& other) const{
                return issubset(other);
            }
            bool DFA::operator>=(const DFA& other) const{
                return issuperset(other);
            }
            bool DFA::operator<(const DFA& other) const{
                return (*this <= other) && (*this != other);
            }
            bool DFA::operator>(const DFA& other) const{
                return (*this >= other) and (*this != other);
            }
            DFA DFA::operator-(const DFA& other) const{
                return difference(other);
            }

    void  DFA::validateTransitionMissingSymbols(const State& start_state,const Paths& paths) const {
        if (allowPartial){
            return
        }
        for(auto inputSymbol : inputSymbols){
            if(paths.find(inputSymbol) == paths.end()){
                std::stringstream ss;
                ss << "state " << start_state << " is missing a transition for input symbol " << inputSymbol;
                throw MissingSymbolException(ss.str());
            }
        }        
    }

    void DFA::validateTransitionInvalidSymbols(const State& start_state,const Paths& paths) const{
        for (auto path : paths){
            if(inputSymbols.find(path.first) == inputSymbols.end()){
                std::stringstream ss;
                ss << "state " << start_state << " has an invalid transition symbol " << path.first;
                throw InvalidSymbolException(ss.str());
            }
        }
    }

    void DFA::validateTransitionStartStates() const{
        for (auto state : states){
            if(transitions.find(state) == transitions.end()){
                std::stringstream ss;
                ss << "transition start state " << state << " is missing";
                throw MissingTransitionException(ss.str());
            }
        }
    }

    void DFA::validateTransitionEndStates(const State& start_state,const Paths& paths) const{
        for (auto path : paths){
            if(states.find(path.second) == states.end()){
                std::stringstream ss;
                ss << "end state " << path.second << "for transition on " << start_state <<" is invalid";
                throw InvalidStateException(ss.str());
            }
        }
    }

    void DFA::validateTransitions(const State& start_state,const Paths& paths) const{
        validateTransitionMissingSymbols(start_state,paths);
        validateTransitionInvalidSymbols(start_state,paths);
        validateTransitionEndStates(start_state,paths);
    }

    bool DFA::validate() const{
        validateTransitionStartStates();
        for (auto transition : transitions){
            validateTransitions(transition.first,transition.second);
        }
        validateInitialState();
        validateFinalStates();
        return true;
    }


    State DFA::getNextCurrentState(const State& current_state, const InputSymbol& input_symbol) const{
        if (transitions.at(current_state).find(input_symbol) == transitions.at(current_state).end()){
            std::stringstream ss;
            ss << input_symbol << " is not a valid input symbol";
            throw RejectionException(ss.str());
        }else{
            return transitions.at(current_state).at(input_symbol);
        }
    }

    void DFA::checkForInputRejection(const State& current_state) const{
        if (finalStates.find(current_state) != finalStates.end()){
            std::stringstream ss;
            ss << "the DFA stopped on a non-final state " << current_state;
            throw RejectionException(ss.str());
        }
    }

    States DFA::readInputStepwise(const InputSymbols& input_str){
        States stateYield;
        State current_state = initialState;

        stateYield.push_back(current_state);
        for (auto input_symbol : input_str){
            current_state = getNextCurrentState(current_state,input_symbol);
            stateYield.push_back(current_state);
        }
        checkForInputRejection(current_state);

        return stateYield;
    }

    DFA DFA::minify(bool retain_names) const{
        DFA newDfa = *this;
        newDfa.removeUnreachableStates();
        new_dfa.mergeStates(retain_names);
        return new_dfa;
    }

    void DFA::removeUnreachableStates(){
        auto reachableStates = computeReachableStates();
        States unreachableStates;
        for (auto state : states){
            if (reachableStates.find(state) == reachableStates.end()){
                unreachableStates.push_back(state);
            }
        }
        for (auto unreachableState : unreachableStates) {
          states.erase(states.find(unreachableState));
          transitions.erase(transitions.find(unreachableState));
          if (finalStates.find(unreachableState) != finalStates.end()) {
            finalStates.erase(finalStates.find(unreachableState));
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
      States eqClasses;
      if (finalStates.size() != 0) {
        for (auto state : finalStates) {
          eqClasses.push_back(state);
        }
      }
      if (finalStates.size() != states.size()) {
        std::sort(finalStates.begin(), finalStates.end());
        std::sort(states.begin(), states.end());
        std::set_difference(states.begin(), states.end(), finalStates.begin(),
                            finalStates.end(),
                            std::inserter(eqClasses, eqClasses.end() - 1));
      }
    }

    } // namespace CXXAUTOMATA

    def _merge_states(self, retain_names=False):
        eq_classes = []
        if len(self.final_states) != 0:
            eq_classes.append(frozenset(self.final_states))
        if len(self.final_states) != len(self.states):
            eq_classes.append(
                frozenset(set(self.states).difference(self.final_states))
            )
        eq_classes = set(eq_classes)

        processing = set([frozenset(self.final_states)])

        while len(processing) > 0:
            active_state = processing.pop()
            for active_letter in self.input_symbols:
                states_that_move_into_active_state = frozenset(
                    state
                    for state in self.states
                    if self.transitions[state][active_letter] in active_state
                )

                copy_eq_classes = set(eq_classes)

                for checking_set in copy_eq_classes:
                    XintY = checking_set.intersection(
                        states_that_move_into_active_state
                    )
                    if len(XintY) == 0:
                        continue
                    XdiffY = checking_set.difference(
                        states_that_move_into_active_state
                    )
                    if len(XdiffY) == 0:
                        continue
                    eq_classes.remove(checking_set)
                    eq_classes.add(XintY)
                    eq_classes.add(XdiffY)
                    if checking_set in processing:
                        processing.remove(checking_set)
                        processing.add(XintY)
                        processing.add(XdiffY)
                    else:
                        if len(XintY) < len(XdiffY):
                            processing.add(XintY)
                        else:
                            processing.add(XdiffY)

        # now eq_classes are good to go, make them a list for ordering
        eq_classes = list(eq_classes)

        def rename(eq):
            return list(eq)[0] if len(eq) == 1 else DFA._stringify_states(eq)

        # need a backmap to prevent constant calls to index
        back_map = {}
        for i, eq in enumerate(eq_classes):
            name = rename(eq) if retain_names else str(i)
            for state in eq:
                back_map[state] = name

        new_input_symbols = self.input_symbols
        new_states = ({rename(eq) for eq in eq_classes} if retain_names
                      else set(str(i) for i in range(len(eq_classes))))
        new_initial_state = back_map[self.initial_state]
        new_final_states = set([back_map[acc] for acc in self.final_states])
        new_transitions = {}
        for i, eq in enumerate(eq_classes):
            name = rename(eq) if retain_names else str(i)
            new_transitions[name] = {}
            for letter in self.input_symbols:
                new_transitions[name][letter] = back_map[
                    self.transitions[list(eq)[0]][letter]
                ]

        self.states = new_states
        self.input_symbols = new_input_symbols
        self.transitions = new_transitions
        self.initial_state = new_initial_state
        self.final_states = new_final_states

    def _cross_product(self, other):
        """
        Creates a new DFA which is the cross product of DFAs self and other
        with an empty set of final states.
        """
        assert self.input_symbols == other.input_symbols
        states_a = list(self.states)
        states_b = list(other.states)
        new_states = {
            self._stringify_states_unsorted((a, b))
            for a in states_a for b in states_b
        }
        new_transitions = dict()
        for state_a, transitions_a in self.transitions.items():
            for state_b, transitions_b in other.transitions.items():
                new_state = self._stringify_states_unsorted(
                    (state_a, state_b)
                )
                new_transitions[new_state] = dict()
                for symbol in self.input_symbols:
                    new_transitions[new_state][symbol] = (
                        self._stringify_states_unsorted(
                            (transitions_a[symbol], transitions_b[symbol])
                        )
                    )
        new_initial_state = self._stringify_states_unsorted(
            (self.initial_state, other.initial_state)
        )

        return DFA(
            states=new_states,
            input_symbols=self.input_symbols,
            transitions=new_transitions,
            initial_state=new_initial_state,
            final_states=set()
        )

    def union(self, other, *, retain_names=False, minify=True):
        """
        Takes as input two DFAs M1 and M2 which
        accept languages L1 and L2 respectively.
        Returns a DFA which accepts the union of L1 and L2.
        """
        new_dfa = self._cross_product(other)
        for state_a in self.states:
            for state_b in other.states:
                if (state_a in self.final_states or
                        state_b in other.final_states):
                    new_dfa.final_states.add(
                        self._stringify_states_unsorted((state_a, state_b))
                    )
        if minify:
            return new_dfa.minify(retain_names=retain_names)
        return new_dfa

    def intersection(self, other, *, retain_names=False, minify=True):
        """
        Takes as input two DFAs M1 and M2 which
        accept languages L1 and L2 respectively.
        Returns a DFA which accepts the intersection of L1 and L2.
        """
        new_dfa = self._cross_product(other)
        for state_a in self.final_states:
            for state_b in other.final_states:
                new_dfa.final_states.add(
                    self._stringify_states_unsorted((state_a, state_b))
                )
        if minify:
            return new_dfa.minify(retain_names=retain_names)
        return new_dfa

    def difference(self, other, *, retain_names=False, minify=True):
        """
        Takes as input two DFAs M1 and M2 which
        accept languages L1 and L2 respectively.
        Returns a DFA which accepts the difference of L1 and L2.
        """
        new_dfa = self._cross_product(other)
        for state_a in self.final_states:
            for state_b in other.states:
                if state_b not in other.final_states:
                    new_dfa.final_states.add(
                        self._stringify_states_unsorted((state_a, state_b))
                    )
        if minify:
            return new_dfa.minify(retain_names=retain_names)
        return new_dfa

    def symmetric_difference(self, other, *, retain_names=False, minify=True):
        """
        Takes as input two DFAs M1 and M2 which
        accept languages L1 and L2 respectively.
        Returns a DFA which accepts the symmetric difference of L1 and L2.
        """
        new_dfa = self._cross_product(other)
        for state_a in self.states:
            for state_b in other.states:
                if ((state_a in self.final_states and
                        state_b not in other.final_states) or
                    (state_a not in self.final_states and
                        state_b in other.final_states)):
                    new_dfa.final_states.add(
                        self._stringify_states_unsorted((state_a, state_b))
                    )
        if minify:
            return new_dfa.minify(retain_names=retain_names)
        return new_dfa

    def complement(self):
        """Return the complement of this DFA."""
        new_dfa = self.copy()
        new_dfa.final_states = self.states - self.final_states
        return new_dfa

    def issubset(self, other):
        """Return True if this DFA is a subset of another DFA."""
        return self.intersection(other) == self

    def issuperset(self, other):
        """Return True if this DFA is a superset of another DFA."""
        return other.issubset(self)

    def isdisjoint(self, other):
        """Return True if this DFA has no common elements with another DFA."""
        return self.intersection(other).isempty()

    def isempty(self):
        """Return True if this DFA is completely empty."""
        return len(self.minify().final_states) == 0

    def _make_graph(self):
        """
        Returns a simple graph representation of the DFA.
        """
        G = defaultdict(set)
        for k, v in self.transitions.items():
            for c, u in v.items():
                G[k].add(u)
        return G

    def _reverse_graph(self, G):
        """
        Returns the graph G where all edges have been reversed.
        """
        rev_G = defaultdict(set)
        for k, v in G.items():
            for u in v:
                rev_G[u].add(k)
        return rev_G

    def _reachable_nodes(self, G, v, vis):
        """
        Computes the set of reachable nodes
        in the graph G starting at vertex v.
        """
        if v not in vis:
            vis.add(v)
            for u in G[v]:
                self._reachable_nodes(G, u, vis)

    def _induced_subgraph(self, G, S):
        """
        Computes the induced subgraph G[S].
        """
        return {k: {x for x in G[k] if x in S} for k in G if k in S}

    def _has_cycle(self, G):
        """
        Returns True if the graph G contains a cycle, False otherwise.
        """
        def dfs(G, at, vis, stack):
            """
            Helper function which accepts input parameters for
            the graph, current node, visited set and current stack
            """
            if at not in vis:
                vis.add(at)
                stack.add(at)
                for k in G[at]:
                    if k not in vis and dfs(G, k, vis, stack):
                        return True
                    elif k in stack:
                        # We have seen this vertex before in the path
                        return True
                stack.remove(at)
            return False
        vis = set()
        stack = set()
        return any(dfs(G, k, vis, stack) for k in G)

    def isfinite(self):
        """
        Returns True if the DFA accepts a finite language, False otherwise.
        """
        G = self._make_graph()
        rev_G = self._reverse_graph(G)

        accessible_nodes = set()
        self._reachable_nodes(G, self.initial_state, accessible_nodes)
        coaccessible_nodes = set()
        for state in self.final_states:
            self._reachable_nodes(rev_G, state, coaccessible_nodes)

        important_nodes = accessible_nodes.intersection(coaccessible_nodes)

        constrained_G = self._induced_subgraph(G, important_nodes)

        contains_cycle = self._has_cycle(constrained_G)

        return not contains_cycle

    @staticmethod
    def _stringify_states_unsorted(states):
        """Stringify the given set of states as a single state name."""
        return '{{{}}}'.format(','.join(states))

    @staticmethod
    def _stringify_states(states):
        """Stringify the given set of states as a single state name."""
        return '{{{}}}'.format(','.join(sorted(states)))

    @classmethod
    def _add_nfa_states_from_queue(cls, nfa, current_states,
                                   current_state_name, dfa_states,
                                   dfa_transitions, dfa_final_states):
        """Add NFA states to DFA as it is constructed from NFA."""
        dfa_states.add(current_state_name)
        dfa_transitions[current_state_name] = {}
        if (current_states & nfa.final_states):
            dfa_final_states.add(current_state_name)

    @classmethod
    def _enqueue_next_nfa_current_states(cls, nfa, current_states,
                                         current_state_name, state_queue,
                                         dfa_transitions):
        """Enqueue the next set of current states for the generated DFA."""
        for input_symbol in nfa.input_symbols:
            next_current_states = nfa._get_next_current_states(
                current_states, input_symbol)
            dfa_transitions[current_state_name][input_symbol] = (
                cls._stringify_states(next_current_states))
            state_queue.append(next_current_states)

    @classmethod
    def from_nfa(cls, nfa):
        """Initialize this DFA as one equivalent to the given NFA."""
        dfa_states = set()
        dfa_symbols = nfa.input_symbols
        dfa_transitions = {}
        # equivalent DFA states states
        nfa_initial_states = nfa._get_lambda_closure(nfa.initial_state)
        dfa_initial_state = cls._stringify_states(nfa_initial_states)
        dfa_final_states = set()

        state_queue = deque()
        state_queue.append(nfa_initial_states)
        while state_queue:

            current_states = state_queue.popleft()
            current_state_name = cls._stringify_states(current_states)
            if current_state_name in dfa_states:
                # We've been here before and nothing should have changed.
                continue
            cls._add_nfa_states_from_queue(nfa, current_states,
                                           current_state_name, dfa_states,
                                           dfa_transitions, dfa_final_states)
            cls._enqueue_next_nfa_current_states(
                nfa, current_states, current_state_name, state_queue,
                dfa_transitions)

        return cls(
            states=dfa_states, input_symbols=dfa_symbols,
            transitions=dfa_transitions, initial_state=dfa_initial_state,
            final_states=dfa_final_states)

    def show_diagram(self, path=None):
        """
            Creates the graph associated with this DFA
        """
        # Nodes are set of states

        graph = Dot(graph_type='digraph', rankdir='LR')
        nodes = {}
        for state in self.states:
            if state == self.initial_state:
                # color start state with green
                if state in self.final_states:
                    initial_state_node = Node(
                        state,
                        style='filled',
                        peripheries=2,
                        fillcolor='#66cc33')
                else:
                    initial_state_node = Node(
                        state, style='filled', fillcolor='#66cc33')
                nodes[state] = initial_state_node
                graph.add_node(initial_state_node)
            else:
                if state in self.final_states:
                    state_node = Node(state, peripheries=2)
                else:
                    state_node = Node(state)
                nodes[state] = state_node
                graph.add_node(state_node)
        # adding edges
        for from_state, lookup in self.transitions.items():
            for to_label, to_state in lookup.items():
                graph.add_edge(Edge(
                    nodes[from_state],
                    nodes[to_state],
                    label=to_label
                ))
        if path:
            graph.write_png(path)
        return graph