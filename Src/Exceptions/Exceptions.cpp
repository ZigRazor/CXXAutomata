#include "Exceptions.hpp"

namespace CXXAUTOMATA {
AutomatonException::AutomatonException(const std::string &message)
    : message(message) {}

AutomatonException::~AutomatonException() throw() {}

const char *AutomatonException::what() const throw() { return message.c_str(); }

InvalidStateException::InvalidStateException(const std::string &message)
    : AutomatonException(message) {}

InvalidStateException::~InvalidStateException() throw() {}

InvalidSymbolException::InvalidSymbolException(const std::string &message)
    : AutomatonException(message) {}

InvalidSymbolException::~InvalidSymbolException() throw() {}

MissingStateException::MissingStateException(const std::string &message)
    : AutomatonException(message) {}

MissingStateException::~MissingStateException() throw() {}

MissingSymbolException::MissingSymbolException(const std::string &message)
    : AutomatonException(message) {}

MissingSymbolException::~MissingSymbolException() throw() {}

InitialStateException::InitialStateException(const std::string &message)
    : AutomatonException(message) {}

InitialStateException::~InitialStateException() throw() {}

FinalStateException::FinalStateException(const std::string &message)
    : AutomatonException(message) {}

FinalStateException::~FinalStateException() throw() {}

RejectionException::RejectionException(const std::string &message)
    : AutomatonException(message) {}

RejectionException::~RejectionException() throw() {}

NotImplementedException::NotImplementedException(const std::string &message)
    : AutomatonException(message) {}

NotImplementedException::~NotImplementedException() throw() {}

} // namespace CXXAUTOMATA