#ifndef CXXAUTOMATA_AUTOMATON
#define CXXAUTOMATA_AUTOMATON

#include <string>

namespace CXXAUTOMATA
{

    /**
    * @brief An abstract base class for all automata, including Turing machines.
    * 
    */
    class Automaton
    {
    public:
        /**
        * @brief Construct a new Automaton object
        * 
        */
        Automaton();
        /**
        * @brief Destroy the Automaton object
        * 
        */
        virtual ~Automaton();
        /**
        * @brief Validate the Automaton
        * 
        * @return true if this automaton is internally consistent
        * @return false otherwise
        */
        virtual bool validate() = 0;
        /**
        * @brief Return a generator that yields each step while reading input.
        * 
        * @param input_str 
        */
        virtual void readInputStepwise(const std::string& input_str) = 0;
        /**
        * @brief Check if the given string is accepted by this automaton.
        Return the automaton's final configuration if this string is valid.
        * 
        * @param input_str 
        */
        virtual void readInput(const std::string& input_str);
        /**
         * @brief validate input to the automaton
         * 
         * @param input_str 
         * @return true if this automaton accepts the given input.
         * @return false otherwise
         */
        virtual bool acceptsInput(const std::string& input_str);
        /**
        * @brief Copy Construct a new Automaton object
        * 
        * @param other 
        */
        Automaton(const Automaton &other);
        /**
        * @brief 
        * 
        * @param rhs 
        * @return true if equal
        * @return false otherwise
        */
        virtual bool operator==(const Automaton &rhs) const;

    private:
        /**
        * @brief Raise an error if the initial state is invalid.
        * 
        */
        virtual void validateInitialState();
        /**
        * @brief Raise an error if the initial state has no transitions defined.
        * 
        */
        virtual void validateInitialStateTransitions();
        /**
        * @brief Raise an error if any final states are invalid.
        * 
        */
        virtual void validateFinalStates();
    };
} //CXXAUTOMATA

#endif //CXXAUTOMATA_AUTOMATON
