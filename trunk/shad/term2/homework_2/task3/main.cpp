#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <array>
#include <unordered_set>
#include <stdexcept>

#define DB(x) std::cerr << #x << ": " << x << std::endl;

const size_t MAX_ALPHABET_SIZE = 26;

class Automaton
{
public:
  enum class StateType
  {
    NONTERMINAL,
    TERMINAL
  };

  explicit Automaton(size_t statesNumber = 0): 
    statesNumber(statesNumber), 
    statesTypes(statesNumber, StateType::NONTERMINAL),
    transitions(statesNumber)
  {
  }

  size_t addState()
  {
    size_t stateIndex = transitions.size();
    transitions.resize(stateIndex + 1);
    statesTypes.push_back(StateType::NONTERMINAL);
    return stateIndex;
  }

  bool statesAreEquivalent(size_t firstState, size_t secondState) const
  {
    std::vector< std::vector<bool> > visited(statesNumber, std::vector<bool> (statesNumber, false));
    return statesAreEquivalent(firstState, secondState, visited);
  }

  static size_t scaleTransitionChar(char transitionChar)
  {
    if (transitionChar < 'a' || transitionChar > 'z')
    {
      throw std::out_of_range("Wrong transition char");
    }
    return transitionChar - 'a';
  }

  size_t getDestination(size_t source, char transitionChar) const
  {
    size_t scaledTransitionChar = scaleTransitionChar(transitionChar);
    return transitions[source][scaledTransitionChar];
  }

  void addTransition(size_t source, size_t destination, char transitionChar)
  {
    size_t scaledTransitionChar = scaleTransitionChar(transitionChar);
    transitions[source][scaledTransitionChar] = destination;
    alphabet.insert(transitionChar);
  }

  void print()
  {
    size_t terminalStatesNumber = 0;
    for (size_t i = 0; i < statesNumber; ++i)
    {
      if (IsTerminalState(i))
      {
        ++terminalStatesNumber;
      }
    }
    std::cout << statesNumber << " " << terminalStatesNumber << " " << alphabet.size() << std::endl;
    for (size_t i = 0; i < statesNumber; ++i)
    {
      if (IsTerminalState(i))
      {
        std::cout << i << " ";
      }
    }
    std::cout << std::endl;
    for (size_t state = 0; state < statesNumber; ++state)
    {
      for (char transitionChar : alphabet)
      {
        size_t destination = getDestination(state, transitionChar);
        std::cout << state << " " << transitionChar << " " << destination << std::endl;
      }
    }
  }

  size_t getStatesNumber() const
  {
    return statesNumber;
  }

  const std::unordered_set<char> getAlphabet() const
  {
    return alphabet;
  }

  StateType getStateType(size_t state) const
  {
    return statesTypes[state];
  }

  void setStateType(size_t state, StateType stateType)
  {
    statesTypes[state] = stateType;
  }

  bool IsTerminalState(size_t state) const
  {
    return (statesTypes[state] == StateType::TERMINAL);
  }

private:

  bool statesAreEquivalent(size_t firstState, size_t secondState,
                            std::vector< std::vector<bool> >& visited) const
  {
    visited[firstState][secondState] = true;
    if (IsTerminalState(firstState) != IsTerminalState(secondState))
    {
      return false;
    }
    for (char transitionChar : alphabet)
    {
      size_t firstDestination = getDestination(firstState, transitionChar);
      size_t secondDestination = getDestination(secondState, transitionChar);
      if (!visited[firstDestination][secondDestination])
      {
        if (!statesAreEquivalent(firstDestination, secondDestination, visited))
        {
          return false;
        }
      }
    }
    return true;
  }

  size_t statesNumber;
  std::unordered_set<char> alphabet;
  std::vector<StateType> statesTypes;
  std::vector< std::array<int, MAX_ALPHABET_SIZE> > transitions;
};

void readAutomaton(Automaton* automaton)
{
  size_t statesNumber, terminalStatesNumber, alphabetSize;
  std::cin >> statesNumber >> terminalStatesNumber >> alphabetSize;
  *automaton = Automaton(statesNumber);
  for (size_t i = 0; i < terminalStatesNumber; ++i)
  {
    size_t terminalStateNumber;
    std::cin >> terminalStateNumber;
    automaton->setStateType(terminalStateNumber, Automaton::StateType::TERMINAL);
  }
  for (size_t i = 0; i < alphabetSize * statesNumber; ++i)
  {
    size_t source, destination;
    char transitionChar;
    std::cin >> source >> transitionChar >> destination;
    automaton->addTransition(source, destination, transitionChar);
  }
}

Automaton mergeAutomata(const Automaton& firstAutomaton, const Automaton& secondAutomaton)
{
  Automaton mergedAutomaton(firstAutomaton.getStatesNumber() + secondAutomaton.getStatesNumber());

  for (size_t state = 0; state < firstAutomaton.getStatesNumber(); ++state)
  {
    if (firstAutomaton.IsTerminalState(state))
    {
      mergedAutomaton.setStateType(state, Automaton::StateType::TERMINAL);
    }
    for (char transitionChar : firstAutomaton.getAlphabet())
    {
      size_t source = state;
      size_t destination = firstAutomaton.getDestination(source, transitionChar);
      mergedAutomaton.addTransition(source, destination, transitionChar);
    }
  }

  for (size_t state = 0; state < secondAutomaton.getStatesNumber(); ++state)
  {
    size_t newState = state + firstAutomaton.getStatesNumber();
    if (secondAutomaton.IsTerminalState(state))
    {
      mergedAutomaton.setStateType(newState, Automaton::StateType::TERMINAL);
    }
    for (char transitionChar : secondAutomaton.getAlphabet())
    {
      size_t destination = secondAutomaton.getDestination(state, transitionChar);
      size_t newDestination = destination + firstAutomaton.getStatesNumber(); 
      mergedAutomaton.addTransition(newState, newDestination, transitionChar);
    }
  }

  return mergedAutomaton;
}

bool checkAutomatonsEquivalence(const Automaton& firstAutomaton, const Automaton& secondAutomaton)
{
  Automaton mergedAutomaton = mergeAutomata(firstAutomaton, secondAutomaton);
  size_t firstAutomatonStartState = 0;
  size_t secondAutomatonStartState = firstAutomaton.getStatesNumber();
  return mergedAutomaton.statesAreEquivalent(firstAutomatonStartState, 
                                              secondAutomatonStartState);
}

void printOutput(bool automataAreEquivalent)
{
  if (automataAreEquivalent)
  {
    std::cout << "EQUIVALENT" << std::endl;
  }
  else
  {
    std::cout << "NOT EQUIVALENT" << std::endl;
  }
}

int main()
{
  Automaton firstAutomaton, secondAutomaton;
  readAutomaton(&firstAutomaton);
  readAutomaton(&secondAutomaton);
  printOutput(checkAutomatonsEquivalence(firstAutomaton, secondAutomaton));
  return 0;
}
