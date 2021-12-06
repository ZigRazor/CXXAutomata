#include "gtest/gtest.h"

#include "DFA.hpp"
#include "FA.hpp"
#include "NFA.hpp"

using CXXAUTOMATA::DFA;

class FATest : public ::testing::Test {
protected:

    FATest() : 
        Test(), 
        dfa({"q0","q1", "q2"}, 
            {"0", "1"}, 
            {
            {"q0" ,{{"0","q0"},{"1","q1"}}},
            {"q1" ,{{"0","q0"},{"1","q2"}}},
            {"q2" ,{{"0","q2"},{"1","q1"}}}
            },
            "q0",
            {"q1"}
        ){};

    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        /**
        dfa = CXXAUTOMATA::DFA({"q0","q1", "q2"}, 
                                {"0", "1"}, 
                                {
                                {"q0" ,{{"0","q0"},{"1","q1"}}},
                                {"q1" ,{{"0","q2"},{"1","q1"}}},
                                {"q2" ,{{"0","q2"},{"1","q2"}}}
                                },
                                "q0",
                                {"q1"}
                                );
        **/                     
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    

public:
    static void assertIsCopy(const CXXAUTOMATA::FA& first, const CXXAUTOMATA::FA& second){
        // Assert that the first FA is a deep copy of the second.
        ASSERT_EQ(first.getStates(), second.getStates());
        ASSERT_EQ(first.getInputSymbols(), second.getInputSymbols());
        ASSERT_EQ(first.getTransitions(), second.getTransitions());
        ASSERT_EQ(first.getInitialState(), second.getInitialState());
        ASSERT_EQ(first.getFinalStates(), second.getFinalStates());
    }

    DFA dfa;
  // CXXAUTOMATA::NFA nfa;
};

