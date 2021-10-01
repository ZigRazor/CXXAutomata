#include "Automaton.hpp"

namespace CXXAUTOMATA{

    Automaton::Automaton(){

    }

    Automaton::~Automaton(){

    }

    bool Automaton::operator==(const Automaton &rhs) const{

    }

    Automaton::Automaton(const Automaton& other){

    }

    void Automaton::readInput(const std::string& input_str){
        /*
        validation_generator = self.read_input_stepwise(input_str)
        for config in validation_generator:
            pass
        return config
        */
    }

    bool Automaton::acceptsInput(const std::string& input_str){
        try{
            readInput(input_str);
            return true;
        }catch(...){
            return false;
        }
        
    }

    void Automaton::validateInitialState(){
        /*
        if self.initial_state not in self.states:
            raise exceptions.InvalidStateError(
                '{} is not a valid initial state'.format(self.initial_state))
        */
    }
    void Automaton::validateInitialStateTransitions(){
        /*
        if self.initial_state not in self.transitions:
            raise exceptions.MissingStateError(
                'initial state {} has no transitions defined'.format(
                    self.initial_state))
        */
    }
    void Automaton::validateFinalStates(){
        /*
        invalid_states = self.final_states - self.states
        if invalid_states:
            raise exceptions.InvalidStateError(
                'final states are not valid ({})'.format(
                    ', '.join(str(state) for state in invalid_states)))
        */
    }


} //CXXAUTOMATA