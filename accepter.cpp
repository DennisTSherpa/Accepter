/*
 * Accepter
 * Informs whether or not an input string belongs to one of the three 
 * regular langauges encoded within.
 * Author: Dennis Sherpa - sherpad@bc.edu
 */

#include <iostream>

template <typename State>
struct PairHash {
    std::size_t operator()(const std::pair<State, char>& p) const {
        return std::hash<unsigned int>()(static_cast<unsigned int>(p.first))
            ^ (std::hash<int>()(p.second) << 1);
    }
};

struct DFA1 {
    inline static const char * kRegEx = "abbc*b*b|dcaa";
    enum State { kStart, kA, kB, kSecondB, kUnboundC, kUnboundB, kBD, kC,
                 kSecondA, kEnd };
    std::unordered_map<std::pair<State, char>, State, PairHash<State>>
        transition = {
            {{State::kStart, 'a'}, State::kA},
            {{State::kA, 'b'}, State::kB},
            {{State::kB, 'b'}, State::kSecondB},
            {{State::kSecondB, 'c'}, State::kUnboundC},
            {{State::kSecondB, 'b'}, State::kUnboundB},
            {{State::kSecondB, 'd'}, State::kBD},
            {{State::kUnboundC, 'c'}, State::kUnboundC},
            {{State::kUnboundC, 'b'}, State::kUnboundB},
            {{State::kUnboundC, 'd'}, State::kBD},
            {{State::kUnboundB, 'd'}, State::kBD},
            {{State::kUnboundB, 'c'}, State::kC},
            {{State::kUnboundB, 'b'}, State::kUnboundB},
            {{State::kBD, 'c'}, State::kC},
            {{State::kC, 'a'}, State::kSecondA},
            {{State::kSecondA, 'a'}, State::kEnd}
        };
    bool is_final(State state) {
        return state == kEnd;
    }
};

struct DFA2 {
    inline static const char * kRegEx = "Ilove(love)*BC";
    enum State { kStart, kI, kL, kO, kV, kE, kB, kEnd };
    std::unordered_map<std::pair<State, char>, State, PairHash<State>>
        transition = {
            {{State::kStart, 'I'}, State::kI},
            {{State::kI, 'l'}, State::kL},
            {{State::kL, 'o'}, State::kO},
            {{State::kO, 'v'}, State::kV},
            {{State::kV, 'e'}, State::kE},
            {{State::kE, 'l'}, State::kL},
            {{State::kE, 'B'}, State::kB},
            {{State::kB, 'C'}, State::kEnd}
        };
    bool is_final(State state) {
        return state == kEnd;
    }
};

struct DFA3 {
    inline static const char * kRegEx = "I love (love )*BC!";
    enum State { kStart, kI, kSpace, kL, kO, kV, kE, kSecondSpace,
                 kB, kC, kEnd };
    std::unordered_map<std::pair<State, char>, State, PairHash<State>>
        transition = {
            {{State::kStart, 'I'}, State::kI},
            {{State::kI, ' '}, State::kSpace},
            {{State::kSpace, 'l'}, State::kL},
            {{State::kL, 'o'}, State::kO},
            {{State::kO, 'v'}, State::kV},
            {{State::kV, 'e'}, State::kE},
            {{State::kE, ' '}, State::kSecondSpace},
            {{State::kSecondSpace, 'l'}, State::kL},
            {{State::kSecondSpace, 'B'}, State::kB},
            {{State::kB, 'C'}, State::kC},
            {{State::kC, '!'}, State::kEnd}
        };
    bool is_final(State state) {
        return state == kEnd;
    }
};

class Accepter {
    public:
        virtual bool accept(std::string input) = 0;
        virtual const char * get_reg_ex() = 0;
        virtual ~Accepter() {}
};

template <typename DFA>
class AccepterImpl : public Accepter {
    public: 
        bool accept(std::string input) override {
            state = DFA::State::kStart;
            for (char c : input) {
                if (!dfa.transition.count({state, c})) {
                    return false;
                }
                state = dfa.transition[{state, c}];
            }
            return dfa.is_final(state);
        }
        const char * get_reg_ex() override {
            return dfa.kRegEx;
        }
    private:
        DFA dfa;
        typename DFA::State state;
};

int test() {
    const char * regexes[] = { DFA1::kRegEx, DFA2::kRegEx, DFA3::kRegEx };
    std::cout << "Please choose one of the following regular expressions:" << std::endl;
    for (size_t i = 0; i < sizeof(regexes)/sizeof(regexes[0]); ++i) {
        std::cout << "\t" << (i+1) << ". " << regexes[i] << std::endl;
    }
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);
    Accepter * accepter;
    if (input == "1") {
        accepter = new AccepterImpl<DFA1>;
    } else if (input == "2") {
        accepter = new AccepterImpl<DFA2>;
    } else if (input == "3") {
        accepter = new AccepterImpl<DFA3>;
    } else {
        std::cout << "Selection must be an integer between 1 and 3, inclusive." << std::endl;
        return EXIT_SUCCESS;
    }
    std::cout << "Now please enter a string to see whether it is accepted within the language you selected." << std::endl;
    std::cout << "Enter $ to quit." << std::endl;
    std::cout << "Your chosen regular expression is as follows:" << std::endl;
    std::cout << accepter->get_reg_ex() << std::endl;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "$") {
            break;
        }
        if (!accepter->accept(input)) {
            std::cout << "Input not accepted." << std::endl;
        }
        else {
            std::cout << "Input accepted." << std::endl;
        }
    }
    return EXIT_SUCCESS;
}

int main() {
    return test();
}
