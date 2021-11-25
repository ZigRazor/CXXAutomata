#include "Automaton.hpp"
#include "Exceptions/Exceptions.hpp"
#include <stringstream>

namespace CXXAUTOMATA{

    Automaton::Automaton(){

    }

    Automaton::~Automaton(){

    }

    bool Automaton::operator==(const Automaton &rhs) const{

    }

    Automaton::Automaton(const Automaton& other){

    }

    State Automaton::readInput(const InputSymbols& input_str){
        auto validation_generator = readInputStepwise(input_str)
        auto last_state = validation_generator.back();
        return last_state;
    }

    bool Automaton::acceptsInput(const InputSymbols& input_str){
        try{
            readInput(input_str);
            return true;
        }catch(...){
            return false;
        }
        
    }

    void Automaton::validateInitialState(){        
        if (states.find(initialState) == states.end()) const{
            std::stringstream ss;
            ss << initialState << " is not a valid initial state.";
            throw InvalidStateException(ss.str());
        }
    }

    void Automaton::validateInitialStateTransitions() const {
        if (transitions.find(initialState) == transitions.end()){
            std::stringstream ss;
            ss << "initial state " << initialState << " has no transitions defined.";
            throw MissingStateException(ss.str());
        }
        
    }
    void Automaton::validateFinalStates() const{
        for (auto state : finalStates){
            if (states.find(state) == states.end()){
                std::stringstream ss;
                ss << state << " is not a valid final state.";
                throw InvalidStateException(ss.str());
            }
        }
    }


} //CXXAUTOMATA