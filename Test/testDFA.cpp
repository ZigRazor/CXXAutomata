#include "gtest/gtest.h"
#include "Typedefs.hpp"
#include "testFA.cpp"
#include "Exceptions.hpp"

class DFATest : public FATest{};

using namespace CXXAUTOMATA;

TEST_F(DFATest, testInitDFA)
{
    //Should copy DFA if passed into DFA constructor.
    auto new_dfa = dfa;
    DFATest::assertIsCopy(new_dfa, dfa);
}

TEST_F(DFATest, test_init_dfa_missing_formal_params){
        // Should raise an error if formal DFA parameters are missing.
        
        EXPECT_THROW(DFA(
                {"q0", "q1"},
                {"0", "1"},
                {},
                "q0",
                {"q1"}
            ), MissingStateException);
}

TEST_F(DFATest, test_dfa_equal){
        //Should correctly determine if two DFAs are equal. 
        auto new_dfa = dfa;
        //DFATest::assertIsCopy(new_dfa, dfa);
        ASSERT_TRUE(dfa == new_dfa);            
}

TEST_F(DFATest,test_dfa_not_equal){
        // Should correctly determine if two DFAs are not equal.
        auto new_dfa = DFA({"q0","q1", "q2"}, 
            {"0", "1"}, 
            {
            {"q0" ,{{"0","q0"},{"1","q1"}}},
            {"q1" ,{{"0","q2"},{"1","q1"}}},
            {"q2" ,{{"0","q2"},{"1","q2"}}}
            },
            "q0",
            {"q1","q2"} //Added state "q2" to make sure it is not equal to dfa.
        );
        ASSERT_TRUE(dfa != new_dfa);  
}

TEST_F(DFATest, test_validate_missing_state){
        // Should raise error if a state has no transitions defined. 
        EXPECT_THROW(DFA({"q0","q1", "q2"}, 
            {"0", "1"}, 
            {
            {"q0" ,{{"0","q0"},{"1","q1"}}},
            {"q2" ,{{"0","q2"},{"1","q2"}}}
            },
            "q0",
            {"q1"}
        ), MissingStateException);
}

TEST_F(DFATest,test_validate_missing_symbol){
        // Should raise error if a symbol transition is missing. 
        
        EXPECT_THROW(DFA({"q0","q1", "q2"}, 
            {"0", "1"}, 
            {
            {"q0" ,{{"0","q0"},{"1","q1"}}},
            {"q1" ,{{"0","q2"}}},
            {"q2" ,{{"0","q2"},{"1","q2"}}}
            },
            "q0",
            {"q1"}
        ), MissingSymbolException);

}

TEST_F(DFATest,test_validate_invalid_symbol){
        // Should raise error if a transition references an invalid symbol.
        EXPECT_THROW(DFA({"q0","q1", "q2"}, 
            {"0", "1"}, 
            {
            {"q0" ,{{"0","q0"},{"1","q1"}}},
            {"q1" ,{{"0","q2"},{"1","q1"},{"2","q2"}}},
            {"q2" ,{{"0","q2"},{"1","q2"}}}
            },
            "q0",
            {"q1"}
        ), InvalidSymbolException);
}

TEST_F(DFATest,test_validate_invalid_state){
        // Should raise error if a transition references an invalid state.
        EXPECT_THROW(DFA({"q0","q1", "q2"}, 
            {"0", "1"}, 
            {
            {"q0" ,{{"0","q0"},{"1","q1"}}},
            {"q1" ,{{"0","q2"},{"1","q3"}}},
            {"q2" ,{{"0","q2"},{"1","q2"}}}
            },
            "q0",
            {"q1"}
        ), InvalidStateException);
}

TEST_F(DFATest,test_validate_invalid_initial_state){
        // Should raise error if the initial state is invalid.
        EXPECT_THROW(DFA({"q0","q1", "q2"}, 
            {"0", "1"}, 
            {
            {"q0" ,{{"0","q0"},{"1","q1"}}},
            {"q1" ,{{"0","q2"},{"1","q1"}}},
            {"q2" ,{{"0","q2"},{"1","q2"}}}
            },
            "q3",
            {"q1"}
        ), InvalidStateException);
}

TEST_F(DFATest,test_validate_invalid_final_state){
        // Should raise error if the final state is invalid.
        EXPECT_THROW(DFA({"q0","q1", "q2"}, 
            {"0", "1"}, 
            {
            {"q0" ,{{"0","q0"},{"1","q1"}}},
            {"q1" ,{{"0","q2"},{"1","q1"}}},
            {"q2" ,{{"0","q2"},{"1","q2"}}}
            },
            "q0",
            {"q3"}
        ), InvalidStateException);
}

TEST_F(DFATest,test_read_input_accepted){
        // Should return correct state if acceptable DFA input is given.
        InputSymbols_v inputSymbols = {"0", "1","1","1"};
        ASSERT_EQ(dfa.readInput(inputSymbols), "q1");
}

TEST_F(DFATest,test_read_input_rejection){
        // Should raise error if the stop state is not a final state.
        InputSymbols_v inputSymbols = {"0", "1","0"};
        EXPECT_THROW(dfa.readInput(inputSymbols), RejectionException);
}

TEST_F(DFATest,test_read_input_rejection_invalid_symbol){
        // Should raise error if an invalid symbol is read.
        InputSymbols_v inputSymbols = {"0", "1","1","1","2"};
        EXPECT_THROW(dfa.readInput(inputSymbols), RejectionException);
}

TEST_F(DFATest,test_accepts_input_true){
        // Should return True if DFA input is accepted.
        InputSymbols_v inputSymbols = {"0", "1","1","1"};
        ASSERT_TRUE(dfa.acceptsInput(inputSymbols));
}

TEST_F(DFATest,test_accepts_input_false){
        // Should return False if DFA input is rejected.
        InputSymbols_v inputSymbols = {"0", "1","0"};
        ASSERT_FALSE(dfa.acceptsInput(inputSymbols));
}

TEST_F(DFATest,test_read_input_step){
        // Should return validation generator if step flag is supplied.
        InputSymbols_v inputSymbols = {"0", "1","1","1"};
        auto validation_generator = dfa.readInputStepwise(inputSymbols);
        States_v statesToCompare = {"q0", "q0", "q1", "q2", "q1"};
        ASSERT_EQ(validation_generator,statesToCompare);
}

TEST_F(DFATest,test_equivalence_not_equal){
        // Should not be equal.
        /* 
        * This DFA accepts all words which do not contain two
        * consecutive occurrences of 1
        */
        auto no_consecutive_11_dfa = DFA(
           {"q0", "q1", "q2"},
           {"0", "1"},
           { {"q0", {{"0","q0"},{"1","q1"}}},
             {"q1", {{"0","q0"},{"1","q2"}}},
             {"q2", {{"0","q2"},{"1","q2"}}},
            },
            "q0",
            {"q0", "q1"}
        );
        /*
        * This DFA accepts all words which contain either zero
        * or one occurrence of 1
        */
        auto zero_or_one_1_dfa = DFA(
           {"q0", "q1", "q2"},
           {"0", "1"},
           { {"q0", {{"0","q0"},{"1","q1"}}},
             {"q1", {{"0","q1"},{"1","q2"}}},
             {"q2", {{"0","q2"},{"1","q2"}}},
            },
            "q0",
            {"q0", "q1"}
        );
        
        ASSERT_TRUE(no_consecutive_11_dfa != zero_or_one_1_dfa);
}

TEST_F(DFATest,test_equivalence_minify){
        // Should be equivalent after minify.
        auto no_consecutive_11_dfa = DFA(
            {"q0", "q1", "q2","q3"},
           {"0", "1"},
           { {"q0", {{"0","q3"},{"1","q1"}}},
             {"q1", {{"0","q0"},{"1","q2"}}},
             {"q2", {{"0","q2"},{"1","q2"}}},
             {"q3", {{"0","q0"},{"1","q1"}}},
            },
            "q0",
            {"q0", "q1","q3"}
        );
        auto minimal_dfa = no_consecutive_11_dfa.minify();
        ASSERT_EQ(no_consecutive_11_dfa, minimal_dfa);
}