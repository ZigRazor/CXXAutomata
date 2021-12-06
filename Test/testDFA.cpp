#include "Exceptions.hpp"
#include "Typedefs.hpp"
#include "testFA.cpp"
#include "gtest/gtest.h"

class DFATest : public FATest {};

using namespace CXXAUTOMATA;

TEST_F(DFATest, testInitDFA) {
  // Should copy DFA if passed into DFA constructor.
  auto new_dfa = dfa;
  DFATest::assertIsCopy(new_dfa, dfa);
}

TEST_F(DFATest, test_init_dfa_missing_formal_params) {
  // Should raise an error if formal DFA parameters are missing.

  EXPECT_THROW(DFA({"q0", "q1"}, {"0", "1"}, {}, "q0", {"q1"}),
               MissingStateException);
}

TEST_F(DFATest, test_dfa_equal) {
  // Should correctly determine if two DFAs are equal.
  auto new_dfa = dfa;
  // DFATest::assertIsCopy(new_dfa, dfa);
  ASSERT_TRUE(dfa == new_dfa);
}

TEST_F(DFATest, test_dfa_not_equal) {
  // Should correctly determine if two DFAs are not equal.
  auto new_dfa = DFA({"q0", "q1", "q2"}, {"0", "1"},
                     {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                      {"q1", {{"0", "q2"}, {"1", "q1"}}},
                      {"q2", {{"0", "q2"}, {"1", "q2"}}}},
                     "q0", {"q1", "q2"}
                     // Added state "q2" to make sure it is not equal to dfa.
  );
  ASSERT_TRUE(dfa != new_dfa);
}

TEST_F(DFATest, test_validate_missing_state) {
  // Should raise error if a state has no transitions defined.
  EXPECT_THROW(DFA({"q0", "q1", "q2"}, {"0", "1"},
                   {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                    {"q2", {{"0", "q2"}, {"1", "q2"}}}},
                   "q0", {"q1"}),
               MissingStateException);
}

TEST_F(DFATest, test_validate_missing_symbol) {
  // Should raise error if a symbol transition is missing.

  EXPECT_THROW(DFA({"q0", "q1", "q2"}, {"0", "1"},
                   {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                    {"q1", {{"0", "q2"}}},
                    {"q2", {{"0", "q2"}, {"1", "q2"}}}},
                   "q0", {"q1"}),
               MissingSymbolException);
}

TEST_F(DFATest, test_validate_invalid_symbol) {
  // Should raise error if a transition references an invalid symbol.
  EXPECT_THROW(DFA({"q0", "q1", "q2"}, {"0", "1"},
                   {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                    {"q1", {{"0", "q2"}, {"1", "q1"}, {"2", "q2"}}},
                    {"q2", {{"0", "q2"}, {"1", "q2"}}}},
                   "q0", {"q1"}),
               InvalidSymbolException);
}

TEST_F(DFATest, test_validate_invalid_state) {
  // Should raise error if a transition references an invalid state.
  EXPECT_THROW(DFA({"q0", "q1", "q2"}, {"0", "1"},
                   {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                    {"q1", {{"0", "q2"}, {"1", "q3"}}},
                    {"q2", {{"0", "q2"}, {"1", "q2"}}}},
                   "q0", {"q1"}),
               InvalidStateException);
}

TEST_F(DFATest, test_validate_invalid_initial_state) {
  // Should raise error if the initial state is invalid.
  EXPECT_THROW(DFA({"q0", "q1", "q2"}, {"0", "1"},
                   {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                    {"q1", {{"0", "q2"}, {"1", "q1"}}},
                    {"q2", {{"0", "q2"}, {"1", "q2"}}}},
                   "q3", {"q1"}),
               InvalidStateException);
}

TEST_F(DFATest, test_validate_invalid_final_state) {
  // Should raise error if the final state is invalid.
  EXPECT_THROW(DFA({"q0", "q1", "q2"}, {"0", "1"},
                   {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                    {"q1", {{"0", "q2"}, {"1", "q1"}}},
                    {"q2", {{"0", "q2"}, {"1", "q2"}}}},
                   "q0", {"q3"}),
               InvalidStateException);
}

TEST_F(DFATest, test_read_input_accepted) {
  // Should return correct state if acceptable DFA input is given.
  InputSymbols_v inputSymbols = {"0", "1", "1", "1"};
  ASSERT_EQ(dfa.readInput(inputSymbols), "q1");
}

TEST_F(DFATest, test_read_input_rejection) {
  // Should raise error if the stop state is not a final state.
  InputSymbols_v inputSymbols = {"0", "1", "0"};
  EXPECT_THROW(dfa.readInput(inputSymbols), RejectionException);
}

TEST_F(DFATest, test_read_input_rejection_invalid_symbol) {
  // Should raise error if an invalid symbol is read.
  InputSymbols_v inputSymbols = {"0", "1", "1", "1", "2"};
  EXPECT_THROW(dfa.readInput(inputSymbols), RejectionException);
}

TEST_F(DFATest, test_accepts_input_true) {
  // Should return True if DFA input is accepted.
  InputSymbols_v inputSymbols = {"0", "1", "1", "1"};
  ASSERT_TRUE(dfa.acceptsInput(inputSymbols));
}

TEST_F(DFATest, test_accepts_input_false) {
  // Should return False if DFA input is rejected.
  InputSymbols_v inputSymbols = {"0", "1", "0"};
  ASSERT_FALSE(dfa.acceptsInput(inputSymbols));
}

TEST_F(DFATest, test_read_input_step) {
  // Should return validation generator if step flag is supplied.
  InputSymbols_v inputSymbols = {"0", "1", "1", "1"};
  auto validation_generator = dfa.readInputStepwise(inputSymbols);
  States_v statesToCompare = {"q0", "q0", "q1", "q2", "q1"};
  ASSERT_EQ(validation_generator, statesToCompare);
}

TEST_F(DFATest, test_equivalence_not_equal) {
  // Should not be equal.
  /*
   * This DFA accepts all words which do not contain two
   * consecutive occurrences of 1
   */
  auto no_consecutive_11_dfa = DFA({"q0", "q1", "q2"}, {"0", "1"},
                                   {
                                       {"q0", {{"0", "q0"}, {"1", "q1"}}},
                                       {"q1", {{"0", "q0"}, {"1", "q2"}}},
                                       {"q2", {{"0", "q2"}, {"1", "q2"}}},
                                   },
                                   "q0", {"q0", "q1"});
  /*
   * This DFA accepts all words which contain either zero
   * or one occurrence of 1
   */
  auto zero_or_one_1_dfa = DFA({"q0", "q1", "q2"}, {"0", "1"},
                               {
                                   {"q0", {{"0", "q0"}, {"1", "q1"}}},
                                   {"q1", {{"0", "q1"}, {"1", "q2"}}},
                                   {"q2", {{"0", "q2"}, {"1", "q2"}}},
                               },
                               "q0", {"q0", "q1"});

  ASSERT_TRUE(no_consecutive_11_dfa != zero_or_one_1_dfa);
}

TEST_F(DFATest, test_equivalence_minify) {
  // Should be equivalent after minify.
  auto no_consecutive_11_dfa = DFA({"q0", "q1", "q2", "q3"}, {"0", "1"},
                                   {
                                       {"q0", {{"0", "q3"}, {"1", "q1"}}},
                                       {"q1", {{"0", "q0"}, {"1", "q2"}}},
                                       {"q2", {{"0", "q2"}, {"1", "q2"}}},
                                       {"q3", {{"0", "q0"}, {"1", "q1"}}},
                                   },
                                   "q0", {"q0", "q1", "q3"});
  auto minimal_dfa = no_consecutive_11_dfa.minify();
  ASSERT_EQ(no_consecutive_11_dfa, minimal_dfa);
}

TEST_F(DFATest, test_equivalence_two_non_minimal) {
  // Should be equivalent even though they are non minimal.
  auto no_consecutive_11_dfa = DFA({"q0", "q1", "q2", "q3"}, {"0", "1"},
                                   {
                                       {"q0", {{"0", "q3"}, {"1", "q1"}}},
                                       {"q1", {{"0", "q0"}, {"1", "q2"}}},
                                       {"q2", {{"0", "q2"}, {"1", "q2"}}},
                                       {"q3", {{"0", "q0"}, {"1", "q1"}}},
                                   },
                                   "q0", {"q0", "q1", "q3"});

  auto other_dfa = DFA({"q0", "q1", "q2", "q3"}, {"0", "1"},
                       {
                           {"q0", {{"0", "q0"}, {"1", "q1"}}},
                           {"q1", {{"0", "q0"}, {"1", "q2"}}},
                           {"q2", {{"0", "q3"}, {"1", "q2"}}},
                           {"q3", {{"0", "q3"}, {"1", "q2"}}},
                       },
                       "q0", {"q0", "q1"});
  ASSERT_EQ(no_consecutive_11_dfa, other_dfa);
}

TEST_F(DFATest, test_complement) {
  auto no_consecutive_11_dfa = DFA({"q0", "q1", "q2", "q3"}, {"0", "1"},
                                   {
                                       {"q0", {{"0", "q3"}, {"1", "q1"}}},
                                       {"q1", {{"0", "q0"}, {"1", "q2"}}},
                                       {"q2", {{"0", "q2"}, {"1", "q2"}}},
                                       {"q3", {{"0", "q0"}, {"1", "q1"}}},
                                   },
                                   "q0", {"q0", "q1", "q3"});
  auto complement_dfa = no_consecutive_11_dfa.complement();
  ASSERT_EQ(complement_dfa.getStates(), no_consecutive_11_dfa.getStates());
  ASSERT_EQ(complement_dfa.getInputSymbols(),
            no_consecutive_11_dfa.getInputSymbols());
  ASSERT_EQ(complement_dfa.getTransitions(),
            no_consecutive_11_dfa.getTransitions());
  ASSERT_EQ(complement_dfa.getInitialState(),
            no_consecutive_11_dfa.getInitialState());
  States finalStateComp = {"q2"};
  ASSERT_EQ(complement_dfa.getFinalStates(), finalStateComp);
}

TEST_F(DFATest, test_union) {
  /*
   * This DFA accepts all words which contain at least four
   * occurrences of 1
   */
  auto A = DFA({"q0", "q1", "q2", "q3", "q4"}, {"0", "1"},
               {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                {"q1", {{"0", "q1"}, {"1", "q2"}}},
                {"q2", {{"0", "q2"}, {"1", "q3"}}},
                {"q3", {{"0", "q3"}, {"1", "q4"}}},
                {"q4", {{"0", "q4"}, {"1", "q4"}}}},
               "q0", {"q4"});
  /*
   * This DFA accepts all words which do not contain two
   * consecutive occurrences of 1
   */
  auto B = DFA({"p0", "p1", "p2"}, {"0", "1"},
               {{"p0", {{"0", "p0"}, {"1", "p1"}}},
                {"p1", {{"0", "p0"}, {"1", "p2"}}},
                {"p2", {{"0", "p2"}, {"1", "p2"}}}},
               "p0", {"p0", "p1"});
  auto new_dfa = A.unionJoin(B, true, false);
  States expected_states = {"q0,p0", "q0,p1", "q0,p2", "q1,p0", "q1,p1",
                            "q1,p2", "q2,p0", "q2,p1", "q2,p2", "q3,p0",
                            "q3,p1", "q3,p2", "q4,p0", "q4,p1", "q4,p2"};
  ASSERT_EQ(new_dfa.getStates(), expected_states);
  InputSymbols expected_input_symbols = {"0", "1"};
  ASSERT_EQ(new_dfa.getInputSymbols(), expected_input_symbols);
  Transitions expected_transition = {
      {"q0,p0", {{"0", "q0,p0"}, {"1", "q1,p1"}}},
      {"q0,p1", {{"0", "q0,p0"}, {"1", "q1,p2"}}},
      {"q0,p2", {{"0", "q0,p2"}, {"1", "q1,p2"}}},
      {"q1,p0", {{"0", "q1,p0"}, {"1", "q2,p1"}}},
      {"q1,p1", {{"0", "q1,p0"}, {"1", "q2,p2"}}},
      {"q1,p2", {{"0", "q1,p2"}, {"1", "q2,p2"}}},
      {"q2,p0", {{"0", "q2,p0"}, {"1", "q3,p1"}}},
      {"q2,p1", {{"0", "q2,p0"}, {"1", "q3,p2"}}},
      {"q2,p2", {{"0", "q2,p2"}, {"1", "q3,p2"}}},
      {"q3,p0", {{"0", "q3,p0"}, {"1", "q4,p1"}}},
      {"q3,p1", {{"0", "q3,p0"}, {"1", "q4,p2"}}},
      {"q3,p2", {{"0", "q3,p2"}, {"1", "q4,p2"}}},
      {"q4,p0", {{"0", "q4,p0"}, {"1", "q4,p1"}}},
      {"q4,p1", {{"0", "q4,p0"}, {"1", "q4,p2"}}},
      {"q4,p2", {{"0", "q4,p2"}, {"1", "q4,p2"}}}};
  ASSERT_EQ(new_dfa.getTransitions(), expected_transition);
  ASSERT_EQ(new_dfa.getInitialState(), "q0,p0");
  States expected_final_states = {"q0,p0", "q0,p1", "q1,p0", "q1,p1",
                                  "q2,p0", "q2,p1", "q3,p0", "q3,p1",
                                  "q4,p0", "q4,p1", "q4,p2"};
  ASSERT_EQ(new_dfa.getFinalStates(), expected_final_states);
}

TEST_F(DFATest, test_intersection) {
  /*
   * This DFA accepts all words which contain at least four
   * occurrences of 1
   */
  auto A = DFA({"q0", "q1", "q2", "q3", "q4"}, {"0", "1"},
               {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                {"q1", {{"0", "q1"}, {"1", "q2"}}},
                {"q2", {{"0", "q2"}, {"1", "q3"}}},
                {"q3", {{"0", "q3"}, {"1", "q4"}}},
                {"q4", {{"0", "q4"}, {"1", "q4"}}}},
               "q0", {"q4"});
  /*
   * This DFA accepts all words which do not contain two
   * consecutive occurrences of 1
   */
  auto B = DFA({"p0", "p1", "p2"}, {"0", "1"},
               {{"p0", {{"0", "p0"}, {"1", "p1"}}},
                {"p1", {{"0", "p0"}, {"1", "p2"}}},
                {"p2", {{"0", "p2"}, {"1", "p2"}}}},
               "p0", {"p0", "p1"});
  auto new_dfa = A.intersection(B, true, false);
  States expected_states = {"q0,p0", "q0,p1", "q0,p2", "q1,p0", "q1,p1",
                            "q1,p2", "q2,p0", "q2,p1", "q2,p2", "q3,p0",
                            "q3,p1", "q3,p2", "q4,p0", "q4,p1", "q4,p2"};
  ASSERT_EQ(new_dfa.getStates(), expected_states);
  InputSymbols expected_input_symbols = {"0", "1"};
  ASSERT_EQ(new_dfa.getInputSymbols(), expected_input_symbols);
  Transitions expected_transition = {
      {"q0,p0", {{"0", "q0,p0"}, {"1", "q1,p1"}}},
      {"q0,p1", {{"0", "q0,p0"}, {"1", "q1,p2"}}},
      {"q0,p2", {{"0", "q0,p2"}, {"1", "q1,p2"}}},
      {"q1,p0", {{"0", "q1,p0"}, {"1", "q2,p1"}}},
      {"q1,p1", {{"0", "q1,p0"}, {"1", "q2,p2"}}},
      {"q1,p2", {{"0", "q1,p2"}, {"1", "q2,p2"}}},
      {"q2,p0", {{"0", "q2,p0"}, {"1", "q3,p1"}}},
      {"q2,p1", {{"0", "q2,p0"}, {"1", "q3,p2"}}},
      {"q2,p2", {{"0", "q2,p2"}, {"1", "q3,p2"}}},
      {"q3,p0", {{"0", "q3,p0"}, {"1", "q4,p1"}}},
      {"q3,p1", {{"0", "q3,p0"}, {"1", "q4,p2"}}},
      {"q3,p2", {{"0", "q3,p2"}, {"1", "q4,p2"}}},
      {"q4,p0", {{"0", "q4,p0"}, {"1", "q4,p1"}}},
      {"q4,p1", {{"0", "q4,p0"}, {"1", "q4,p2"}}},
      {"q4,p2", {{"0", "q4,p2"}, {"1", "q4,p2"}}}};
  ASSERT_EQ(new_dfa.getTransitions(), expected_transition);
  ASSERT_EQ(new_dfa.getInitialState(), "q0,p0");
  States expected_final_states = {"q4,p0", "q4,p1"};
  ASSERT_EQ(new_dfa.getFinalStates(), expected_final_states);
}

TEST_F(DFATest, test_difference) {
  /*
   * This DFA accepts all words which contain at least four
   * occurrences of 1
   */
  auto A = DFA({"q0", "q1", "q2", "q3", "q4"}, {"0", "1"},
               {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                {"q1", {{"0", "q1"}, {"1", "q2"}}},
                {"q2", {{"0", "q2"}, {"1", "q3"}}},
                {"q3", {{"0", "q3"}, {"1", "q4"}}},
                {"q4", {{"0", "q4"}, {"1", "q4"}}}},
               "q0", {"q4"});
  /*
   * This DFA accepts all words which do not contain two
   * consecutive occurrences of 1
   */
  auto B = DFA({"p0", "p1", "p2"}, {"0", "1"},
               {{"p0", {{"0", "p0"}, {"1", "p1"}}},
                {"p1", {{"0", "p0"}, {"1", "p2"}}},
                {"p2", {{"0", "p2"}, {"1", "p2"}}}},
               "p0", {"p0", "p1"});
  auto new_dfa = A.difference(B, true, false);
  States expected_states = {"q0,p0", "q0,p1", "q0,p2", "q1,p0", "q1,p1",
                            "q1,p2", "q2,p0", "q2,p1", "q2,p2", "q3,p0",
                            "q3,p1", "q3,p2", "q4,p0", "q4,p1", "q4,p2"};
  ASSERT_EQ(new_dfa.getStates(), expected_states);
  InputSymbols expected_input_symbols = {"0", "1"};
  ASSERT_EQ(new_dfa.getInputSymbols(), expected_input_symbols);
  Transitions expected_transition = {
      {"q0,p0", {{"0", "q0,p0"}, {"1", "q1,p1"}}},
      {"q0,p1", {{"0", "q0,p0"}, {"1", "q1,p2"}}},
      {"q0,p2", {{"0", "q0,p2"}, {"1", "q1,p2"}}},
      {"q1,p0", {{"0", "q1,p0"}, {"1", "q2,p1"}}},
      {"q1,p1", {{"0", "q1,p0"}, {"1", "q2,p2"}}},
      {"q1,p2", {{"0", "q1,p2"}, {"1", "q2,p2"}}},
      {"q2,p0", {{"0", "q2,p0"}, {"1", "q3,p1"}}},
      {"q2,p1", {{"0", "q2,p0"}, {"1", "q3,p2"}}},
      {"q2,p2", {{"0", "q2,p2"}, {"1", "q3,p2"}}},
      {"q3,p0", {{"0", "q3,p0"}, {"1", "q4,p1"}}},
      {"q3,p1", {{"0", "q3,p0"}, {"1", "q4,p2"}}},
      {"q3,p2", {{"0", "q3,p2"}, {"1", "q4,p2"}}},
      {"q4,p0", {{"0", "q4,p0"}, {"1", "q4,p1"}}},
      {"q4,p1", {{"0", "q4,p0"}, {"1", "q4,p2"}}},
      {"q4,p2", {{"0", "q4,p2"}, {"1", "q4,p2"}}}};
  ASSERT_EQ(new_dfa.getTransitions(), expected_transition);
  ASSERT_EQ(new_dfa.getInitialState(), "q0,p0");
  States expected_final_states = {"q4,p2"};
  ASSERT_EQ(new_dfa.getFinalStates(), expected_final_states);
}

TEST_F(DFATest, test_symmetric_difference) {
  /*
   * This DFA accepts all words which contain at least four
   * occurrences of 1
   */
  auto A = DFA({"q0", "q1", "q2", "q3", "q4"}, {"0", "1"},
               {{"q0", {{"0", "q0"}, {"1", "q1"}}},
                {"q1", {{"0", "q1"}, {"1", "q2"}}},
                {"q2", {{"0", "q2"}, {"1", "q3"}}},
                {"q3", {{"0", "q3"}, {"1", "q4"}}},
                {"q4", {{"0", "q4"}, {"1", "q4"}}}},
               "q0", {"q4"});
  /*
   * This DFA accepts all words which do not contain two
   * consecutive occurrences of 1
   */
  auto B = DFA({"p0", "p1", "p2"}, {"0", "1"},
               {{"p0", {{"0", "p0"}, {"1", "p1"}}},
                {"p1", {{"0", "p0"}, {"1", "p2"}}},
                {"p2", {{"0", "p2"}, {"1", "p2"}}}},
               "p0", {"p0", "p1"});
  auto new_dfa = A.symmetricDifference(B, true, false);
  States expected_states = {"q0,p0", "q0,p1", "q0,p2", "q1,p0", "q1,p1",
                            "q1,p2", "q2,p0", "q2,p1", "q2,p2", "q3,p0",
                            "q3,p1", "q3,p2", "q4,p0", "q4,p1", "q4,p2"};
  ASSERT_EQ(new_dfa.getStates(), expected_states);
  InputSymbols expected_input_symbols = {"0", "1"};
  ASSERT_EQ(new_dfa.getInputSymbols(), expected_input_symbols);
  Transitions expected_transition = {
      {"q0,p0", {{"0", "q0,p0"}, {"1", "q1,p1"}}},
      {"q0,p1", {{"0", "q0,p0"}, {"1", "q1,p2"}}},
      {"q0,p2", {{"0", "q0,p2"}, {"1", "q1,p2"}}},
      {"q1,p0", {{"0", "q1,p0"}, {"1", "q2,p1"}}},
      {"q1,p1", {{"0", "q1,p0"}, {"1", "q2,p2"}}},
      {"q1,p2", {{"0", "q1,p2"}, {"1", "q2,p2"}}},
      {"q2,p0", {{"0", "q2,p0"}, {"1", "q3,p1"}}},
      {"q2,p1", {{"0", "q2,p0"}, {"1", "q3,p2"}}},
      {"q2,p2", {{"0", "q2,p2"}, {"1", "q3,p2"}}},
      {"q3,p0", {{"0", "q3,p0"}, {"1", "q4,p1"}}},
      {"q3,p1", {{"0", "q3,p0"}, {"1", "q4,p2"}}},
      {"q3,p2", {{"0", "q3,p2"}, {"1", "q4,p2"}}},
      {"q4,p0", {{"0", "q4,p0"}, {"1", "q4,p1"}}},
      {"q4,p1", {{"0", "q4,p0"}, {"1", "q4,p2"}}},
      {"q4,p2", {{"0", "q4,p2"}, {"1", "q4,p2"}}}};
  ASSERT_EQ(new_dfa.getTransitions(), expected_transition);
  ASSERT_EQ(new_dfa.getInitialState(), "q0,p0");
  States expected_final_states = {"q0,p0", "q0,p1", "q1,p0", "q1,p1", "q2,p0",
                                  "q2,p1", "q3,p0", "q3,p1", "q4,p2"};
  ASSERT_EQ(new_dfa.getFinalStates(), expected_final_states);
}