#ifndef CXXAUTOMATA_TYPEDEFS
#define CXXAUTOMATA_TYPEDEFS

#include <string>
#include <vector>
#include <map>

namespace CXXAUTOMATA
{
    typedef std::string State;
    typedef std::vector<State> States;
    typedef std::string InputSymbol;
    typedef std::vector<InputSymbol> InputSymbols;
    typedef std::map<InputSymbol,State> Paths;
    typedef std::map<State, Paths > Transitions;
}

#endif /* CXXAUTOMATA_TYPEDEFS */
