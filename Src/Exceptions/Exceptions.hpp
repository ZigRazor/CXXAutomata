#ifndef CXXAUTOMATA_EXCEPTIONS
#define CXXAUTOMATA_EXCEPTIONS

#include <exception>
#include <iostream>

/**
 * @brief Exception classes shared by all automata.
 *
 */

namespace CXXAUTOMATA {
/**
 * @brief The base class for all automaton-related errors.
 *
 */
class AutomatonException : public std::exception {
public:
  /**
   * @brief Construct a new Automaton Exception object
   *
   * @param message
   */
  AutomatonException(const std::string &message);
  /**
   * @brief Destroy the Automaton Exception object
   *
   */
  virtual ~AutomatonException() throw();
  /**
   * @brief Get the Message object
   *
   * @return const char* the message of the exception
   */
  virtual const char *what() const throw();

private:
  /**
   * @brief  The message of the exception
   *
   */
  std::string message;
};

/**
 * @brief A state is not a valid state for this automaton.
 *
 */
class InvalidStateException : public AutomatonException {
public:
  /**
   * @brief Construct a new Invalid State Exception object
   *
   * @param message
   */
  InvalidStateException(const std::string &message);
  /**
   * @brief Destroy the Invalid State Exception object
   *
   */
  virtual ~InvalidStateException() throw();
};

/**
 * @brief A symbol is not a valid symbol for this automaton.
 *
 */
class InvalidSymbolException : public AutomatonException {
public:
  /**
   * @brief Construct a new Invalid Symbol Exception object
   *
   * @param message
   */
  InvalidSymbolException(const std::string &message);
  /**
   * @brief Destroy the Invalid Symbol Exception object
   *
   */
  virtual ~InvalidSymbolException() throw();
};

/**
 * @brief A state is missing from the automaton definition.
 *
 */
class MissingStateException : public AutomatonException {
public:
  /**
   * @brief Construct a new Missing State Exception object
   *
   * @param message
   */
  MissingStateException(const std::string &message);
  /**
   * @brief Destroy the Missing State Exception object
   *
   */
  virtual ~MissingStateException() throw();
};

/**
 * @brief A symbol is missing from the automaton definition.
 *
 */
class MissingSymbolException : public AutomatonException {
public:
  /**
   * @brief Construct a new Missing Symbol Exception object
   *
   * @param message
   */
  MissingSymbolException(const std::string &message);
  /**
   * @brief Destroy the Missing Symbol Exception object
   *
   */
  virtual ~MissingSymbolException() throw();
};

/**
 * @brief The initial state fails to meet some required condition.
 *
 */
class InitialStateException : public AutomatonException {
public:
  /**
   * @brief Construct a new Initial State Exception object
   *
   * @param message
   */
  InitialStateException(const std::string &message);
  /**
   * @brief Destroy the Initial State Exception object
   *
   */
  virtual ~InitialStateException() throw();
};

/**
 * @brief A final state fails to meet some required condition.
 *
 */
class FinalStateException : public AutomatonException {
public:
  /**
   * @brief Construct a new Final State Exception object
   *
   * @param message
   */
  FinalStateException(const std::string &message);
  /**
   * @brief Destroy the Final State Exception object
   *
   */
  virtual ~FinalStateException() throw();
};

/**
 * @brief The input was rejected by the automaton.
 *
 */
class RejectionException : public AutomatonException {
public:
  /**
   * @brief Construct a new Rejection Exception object
   *
   * @param message
   */
  RejectionException(const std::string &message);
  /**
   * @brief Destroy the Rejection Exception object
   *
   */
  virtual ~RejectionException() throw();
};

class NotImplementedException : public AutomatonException {
public:
  /**
   * @brief Construct a new NotImplemented Exception object
   *
   * @param message
   */
  NotImplementedException(const std::string &message);
  /**
   * @brief Destroy the Rejection Exception object
   *
   */
  virtual ~NotImplementedException() throw();
};

} // namespace CXXAUTOMATA

#endif /* CXXAUTOMATA_EXCEPTIONS */
