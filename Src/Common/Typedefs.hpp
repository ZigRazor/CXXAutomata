#ifndef CXXAUTOMATA_TYPEDEFS
#define CXXAUTOMATA_TYPEDEFS

#include <map>
#include <string>
#include <set>
#include <vector>

namespace CXXAUTOMATA {
typedef std::string State;
typedef std::vector<State> States_v;
typedef std::set<State> States;
typedef std::string InputSymbol;
typedef std::set<InputSymbol> InputSymbols;
typedef std::vector<InputSymbol> InputSymbols_v;
typedef std::map<InputSymbol, State> Paths;
typedef std::map<State, Paths> Transitions;
typedef std::map<State, States> Graph;
} // namespace CXXAUTOMATA

#endif /* CXXAUTOMATA_TYPEDEFS */
