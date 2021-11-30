#ifndef CXXAUTOMATA_TYPEDEFS
#define CXXAUTOMATA_TYPEDEFS

#include <map>
#include <string>
#include <vector>

namespace CXXAUTOMATA {
typedef std::string State;
typedef std::vector<State> States;
typedef std::string InputSymbol;
typedef std::vector<InputSymbol> InputSymbols;
typedef std::map<InputSymbol, State> Paths;
typedef std::map<State, Paths> Transitions;
typedef std::map<State, States> Graph;
} // namespace CXXAUTOMATA

#endif /* CXXAUTOMATA_TYPEDEFS */
