
//IMPORTANT: This program is only compatible with C++23 dialect


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "map"
#include "stack"
enum InputStates{
    NUMBER_OF_VARIABLES_IN_INPUT_ALPHABET,
    NUMBER_OF_VARIABLES_IN_STACK_ALPHABET,
    NUMBER_OF_STATES,
    NUMBER_OF_GOAL_STATES,
    STATES,
    START_STATE,
    GOAL_STATES,
    VARIABLES_IN_INPUT_ALPHABET,
    VARIABLES_IN_STACK_ALPHABET,
    INITIAL_STACK_SYMBOL,
    TRANSITIONS,
    STRINGS
};



struct PdaOutput{
    bool accepted;
    std::vector<std::string> path;
    PdaOutput(bool accepted, auto& path) {
        this->accepted = accepted;
        this->path = path;
        this->path = path;
    }
};

struct InputParse{
    int number_of_states = 0;
    int number_of_variables_in_input_alphabet = 0;
    int number_of_variables_in_stack_alphabet = 0;
    int number_of_goal_states = 0;
    int state_start = 0;
    int state_length = 0;
    InputStates inputState = InputStates();
    std::vector<std::string> states;
    std::string start_state;
    char initial_stack_symbol;
    std::vector<std::string> goal_states;
    std::vector<char> variables_in_input_alphabet;
    std::vector<char> variables_in_stack_alphabet;
    std::map<std::pair<std::pair<std::string, char>, char>,std::vector<std::pair<std::string, char>>> transitions ;
    std::vector<std::string> strings;
    std::ifstream inputFile;
    std::string transition_state_from;
    char transition_character;
    char pop_character;
    char push_character;
    std::string transition_state_to;

    InputParse(auto &inputFile_parameter){
        inputState = InputStates();
        states = std::vector<std::string>();
        goal_states = std::vector<std::string>();
        variables_in_input_alphabet = std::vector<char>();
        transitions = std::map<std::pair<std::pair<std::string, char>, char>,std::vector<std::pair<std::string, char>>>();
        strings = std::vector<std::string>();
        inputFile = std::move(inputFile_parameter);

    }
    int parseInput(int &number_of_states_parameter, int &number_of_variables_in_input_alphabet_parameter, int &number_of_variables_in_stack_alphabet_parameter, int &number_of_goal_states_parameter, auto& states_parameter, std::string& start_state_parameter, auto& goal_states_parameter, auto& variables_in_input_alphabet_parameter, auto& variables_in_stack_alphabet_parameter, char& initial_stack_symbol_parameter, auto& transitions_parameter, auto& strings_parameter ) {
        std::string line;
        int num_spaces = 0;
        inputState = NUMBER_OF_VARIABLES_IN_INPUT_ALPHABET;
        while (std::getline(inputFile, line)) {
            switch (inputState) {
                case NUMBER_OF_STATES:
                    number_of_states = std::stoi(line);
                    inputState = STATES;
                    break;
                case NUMBER_OF_VARIABLES_IN_INPUT_ALPHABET:
                    number_of_variables_in_input_alphabet = std::stoi(line);
                    inputState = NUMBER_OF_VARIABLES_IN_STACK_ALPHABET;
                    break;
                case NUMBER_OF_VARIABLES_IN_STACK_ALPHABET:
                    number_of_variables_in_stack_alphabet = std::stoi(line);
                    inputState = NUMBER_OF_GOAL_STATES;
                    break;
                case NUMBER_OF_GOAL_STATES:
                    number_of_goal_states = std::stoi(line);
                    inputState = NUMBER_OF_STATES;
                    break;
                case STATES:
                    state_start = 0;
                    state_length = 0;
                    for (auto i = 0; i <= line.length(); i++) {
                        if (i == line.length() || line[i] == ' ') {
                            states.push_back(line.substr(state_start, state_length));
                            state_start = i + 1;
                            state_length = 0;
                        } else {
                            state_length++;
                        }
                    }
                    inputState = START_STATE;
                    break;
                case START_STATE:
                    start_state = line;
                    inputState = GOAL_STATES;
                    break;
                case GOAL_STATES:
                    state_start = 0;
                    state_length = 0;
                    for (auto i = 0; i <= line.length(); i++) {
                        if (i == line.length() || line[i] == ' ') {
                            goal_states.push_back(line.substr(state_start, state_length));
                            state_start = i + 1;
                            state_length = 0;
                        } else {
                            state_length++;
                        }
                    }
                    inputState = VARIABLES_IN_STACK_ALPHABET;
                    break;
                case VARIABLES_IN_INPUT_ALPHABET:
                    for(auto i = 0; i < line.length(); i++) {
                        if (line[i] != ' ') {
                            variables_in_input_alphabet.push_back(line[i]);
                        }
                    }
                    inputState = TRANSITIONS;
                    break;
                case VARIABLES_IN_STACK_ALPHABET:
                    for(auto i = 0; i < line.length(); i++) {
                        if (line[i] != ' ') {
                            variables_in_stack_alphabet.push_back(line[i]);
                        }
                    }
                    inputState = INITIAL_STACK_SYMBOL;
                    break;
                case INITIAL_STACK_SYMBOL:
                    initial_stack_symbol = line[0];
                    inputState = VARIABLES_IN_INPUT_ALPHABET;
                    break;
                case TRANSITIONS:
                    state_start = 0;
                    state_length = 0;
                    num_spaces = 0;
                    if (line == "END_OF_TRANSITIONS") {
                        inputState = STRINGS;
                        break;
                    }
                    for (auto i = 0; i <= line.length(); i++) {
                        if (i == line.length()) {
                            transition_state_to = line.substr(state_start, state_length);
                            state_start = i + 1;
                            state_length = 0;
                            auto transition_tuple = std::pair(std::pair(transition_state_from, transition_character), pop_character);
                            transitions[transition_tuple].push_back(std::pair(transition_state_to, push_character));
                        } else if (line[i] == ' ' && num_spaces == 0) {
                            transition_state_from = line.substr(state_start, state_length);
                            state_length = 0;
                            num_spaces++;
                        } else if (line[i] == ' ' && num_spaces == 1) {
                            transition_character = line[i-1];
                            state_start = i + 1;
                            state_length = 0;
                            num_spaces++;
                        } else if (line[i] == ' ' && num_spaces == 2) {
                            pop_character = line[i-1];
                            state_start = i + 1;
                            state_length = 0;
                            num_spaces++;
                        }  else if (line[i] == ' ' && num_spaces == 3) {
                            push_character = line[i-1];
                            state_start = i + 1;
                            state_length = 0;
                        } else {
                            state_length++;
                        }
                    }
                    break;
                case STRINGS:
                    std::string current_string = "";
                    for (auto i = 0; i < line.length(); i++) {
                        current_string+=line[i];
                        state_start = i + 1;
                        state_length = 0;
                    }

                    strings.push_back(current_string);
                    break;
            }
        }
        number_of_states_parameter = number_of_states;
        goal_states_parameter = std::move(goal_states);
        number_of_variables_in_input_alphabet_parameter = number_of_variables_in_input_alphabet;
        number_of_variables_in_stack_alphabet_parameter = number_of_variables_in_stack_alphabet;
        number_of_goal_states_parameter = number_of_goal_states;
        states_parameter = std::move(states);
        start_state_parameter = start_state;
        strings_parameter = std::move(strings);
        transitions_parameter = std::move(transitions);
        variables_in_input_alphabet_parameter = std::move(variables_in_input_alphabet);
        variables_in_stack_alphabet_parameter = std::move(variables_in_stack_alphabet);
        initial_stack_symbol_parameter = initial_stack_symbol;
        return 0;
    }
};

struct PdaEvaluater{
    int number_of_states = 0;
    int number_of_variables_in_input_alphabet = 0;
    int number_of_variables_in_stack_alphabet = 0;
    int number_of_goal_states = 0;
    std::vector<std::string> states;
    std::string start_state;
    char initial_stack_symbol;
    std::vector<std::string> goal_states;
    std::vector<char> variables_in_input_alphabet;
    std::vector<char> variables_in_stack_alphabet;
    std::map<std::pair<std::pair<std::string, char>, char>,std::vector<std::pair<std::string, char>>> transitions ;
    std::vector<std::string> strings;
    std::vector<std::vector<std::string>> allPaths;

    PdaEvaluater(int number_of_states_parameter, int number_of_variables_in_input_alphabet_parameter, int number_of_variables_in_stack_alphabet_parameter, int number_of_goal_states_parameter, auto& states_parameter, std::string start_state_parameter, auto& goal_states_parameter, auto& variables_in_input_alphabet_parameter, auto& variables_in_stack_alphabet_parameter, char initial_stack_symbol_parameter, auto& transitions_parameter, auto& strings_parameter){
        this->number_of_states = number_of_states_parameter;
        this->number_of_variables_in_input_alphabet = number_of_variables_in_input_alphabet_parameter;
        this->number_of_variables_in_stack_alphabet = number_of_variables_in_stack_alphabet_parameter;
        this->states = states_parameter;
        this->goal_states = goal_states_parameter;
        this->number_of_goal_states = number_of_goal_states_parameter;
        this->states = states_parameter;
        this->start_state = start_state_parameter;
        this->goal_states = goal_states_parameter;
        this->variables_in_input_alphabet = variables_in_input_alphabet_parameter;
        this->variables_in_stack_alphabet = variables_in_stack_alphabet_parameter;
        this->initial_stack_symbol = initial_stack_symbol_parameter;
        this->transitions = transitions_parameter;
        this->strings = strings_parameter;

    }

    bool checkPDA(){
        if (number_of_states != states.size()){
            std::cout << "The provided number of states is inconsistent with the given states." << std::endl;
            exit(EXIT_FAILURE);
        }

        //Input Variable Number Check

        if (number_of_variables_in_input_alphabet != variables_in_input_alphabet.size()){
            std::cout << "The provided number of input variables is inconsistent with the given input variables." << std::endl;
            exit(EXIT_FAILURE);
        }

        //Stack Variable Number Check

        if (number_of_variables_in_stack_alphabet != variables_in_stack_alphabet.size()){
            std::cout << "The provided number of stack variables is inconsistent with the given stack variables." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (number_of_goal_states != goal_states.size()){
            std::cout << "The provided number of goal states is inconsistent with the given goal states." << std::endl;
            exit(EXIT_FAILURE);
        }

        bool start_state_found = false;
        for (const auto& state: states) {
            if (start_state == state) {
                start_state_found = true;
                break;
            }
        }
        if(!start_state_found){
            std::cout << "Start state is not among the states of the PDA" << std::endl;
            exit(EXIT_FAILURE);
        }


        bool initial_stack_symbol_found = false;
        for (const auto& var: variables_in_stack_alphabet) {
            if (initial_stack_symbol == var) {
                initial_stack_symbol_found = true;
                break;
            }
        }
        if(!initial_stack_symbol_found){
            std::cout << "Initial stack symbol is not among the stack variables" << std::endl;
            exit(EXIT_FAILURE);
        }

        bool goal_state_found = false;
        for (auto &goalState: goal_states) {
            goal_state_found = false;
            for (auto &state: states) {
                if (goalState == state) {
                    goal_state_found = true;
                    break;
                }
            }
            if (!goal_state_found) {
                std::cout << "At least one of the final states is not among the states of the PDA." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        bool character_found;

        for (const auto& transition: transitions){
            bool from_state_found = false;
            bool transition_character_found = false;
            bool pop_character_found = false;

            auto from_state = transition.first.first.first;
            auto transition_character = transition.first.first.second;
            auto pop_character = transition.first.second;

            for(const auto& state: states){
                if (from_state == state){
                    from_state_found = true;
                    break;
                }
            }
            if (!from_state_found) {
                std::cout << "From State is not among the states of the PDA in at least one transition" << std::endl;
                exit(EXIT_FAILURE);
            }

            for(const auto& var: variables_in_input_alphabet){
                if (transition_character == var || transition_character == 'e'){
                    transition_character_found = true;
                    break;
                }
            }
            if (!transition_character_found) {
                std::cout << "Transition character is not among the input variables in at least one transition" << std::endl;
                exit(EXIT_FAILURE);
            }

            for(const auto& var: variables_in_stack_alphabet){
                if (pop_character == var || pop_character == 'e'){
                    pop_character_found = true;
                    break;
                }
            }
            if (!pop_character_found) {
                std::cout << "Pop character is not among the input variables in at least one transition" << std::endl;
                exit(EXIT_FAILURE);
            }

            for (const auto& branch: transition.second){

                bool dest_state_found = false;
                bool push_character_found = false;

                auto dest_state = branch.first;
                auto push_character = branch.second;

                for(const auto& state: states){
                    if (dest_state == state){
                        dest_state_found = true;
                        break;
                    }
                }
                if (!dest_state_found) {
                    std::cout << "Destination State is not among the states of the PDA in at least one transition" << std::endl;
                    exit(EXIT_FAILURE);
                }

                for(const auto& var: variables_in_stack_alphabet){
                    if (push_character == var || push_character == 'e'){
                        push_character_found = true;
                        break;
                    }
                }
                if (!push_character_found) {
                    std::cout << "Push character is not among the input variables in at least one transition" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        for (auto& string: strings) {
            for (auto &character: string) {
                character_found = false;
                for (const auto &variable: variables_in_input_alphabet) {
                    if (character == variable) {
                        character_found = true;
                        break;
                    }
                }
                if (!character_found) {
                    std::cout << "Invalid character in one of the strings." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        return true;
    }

    void evaluate(){
        checkPDA();
        auto initialStack = std::stack<char>();
        auto pathStart = std::vector<std::string>{start_state};
        if(initial_stack_symbol != 'e'){
            initialStack.push(initial_stack_symbol);
        }
        std::ofstream outputFile;
        outputFile.open("C:/Users/emirm/CLionProjects/CS410_HW2/output.txt");
        for(int i = 0; i < strings.size(); i++){
            allPaths = std::vector<std::vector<std::string>>();
            auto result = findRoute(start_state, 0, i, initialStack, pathStart);
            if(result.accepted){
                for(const auto& state: result.path){
                    outputFile << state << " ";
                }
            }
            else{
                for(const auto& state: allPaths[0]){
                    outputFile << state << " ";
                }
            }
            outputFile << "(route taken)";
            outputFile << std::endl;
            if(result.accepted){
                outputFile << "Accepted" << std::endl;
            }
            else{
                outputFile << "Rejected" << std::endl;
            }
            outputFile << "********" << std::endl;
        }
        outputFile.close();
    }

    PdaOutput findRoute(const std::string &currentState, int characterIndex, int stringIndex, std::stack<char>& currentStack, std::vector<std::string>& path){
        if(characterIndex == strings[stringIndex].size()){ // Check if the string is empty and we
            for(const auto& state: goal_states){
                if(currentState == state){
                    return PdaOutput(true, path);
                }
            }
            for(const auto& var: variables_in_stack_alphabet){
                auto transition_parameters = std::pair(std::pair(currentState, 'e'), var); //Check epsilon (as input variable) transitions
                if (!transitions[transition_parameters].empty()){
                    if(currentStack.top() == var || var == 'e') {
                        auto currentStackCopy = currentStack;
                        if (var != 'e'){
                            currentStackCopy.pop();
                        }
                        for (const auto &branch: transitions[transition_parameters]) {

                            auto currentStackCopy_2 = currentStackCopy;
                            if(branch.second != 'e') { //Push element to stack if the element is not epsilon
                                currentStackCopy_2.push(branch.second);
                            }
                            auto pathCopy = path;
                            pathCopy.push_back(branch.first);
                            auto result = findRoute(branch.first, characterIndex, stringIndex, currentStackCopy_2, pathCopy);
                            if (result.accepted) {
                                return result;
                            }
                        }
                    }
                }
            }
        }

        char current_character = strings[stringIndex][characterIndex];
        for(const auto& element: transitions){ //Iterate through every transition
            std::string state = element.first.first.first;
            if(state == currentState){
                char pop_character = element.first.second;
                auto transition_parameters = std::pair(std::pair(currentState, current_character), pop_character); //Check transitions with the current input variable
                if (!transitions[transition_parameters].empty()){
                    if(currentStack.top() == pop_character || pop_character == 'e') {
                        auto currentStackCopy = currentStack;
                        if (pop_character != 'e'){ // If the pop character is not epsilon, we pop the
                            currentStackCopy.pop();
                        }
                        for (const auto &branch: transitions[transition_parameters]) {
                            auto currentStackCopy_2 = currentStackCopy;
                            if(branch.second != 'e') {
                                currentStackCopy_2.push(branch.second);
                            }
                            auto pathCopy = path;
                            pathCopy.push_back(branch.first);
                            auto result = findRoute(branch.first, characterIndex + 1, stringIndex, currentStackCopy_2, pathCopy);
                            if (result.accepted) {
                                return result;
                            }
                        }
                    }
                }
                transition_parameters = std::pair(std::pair(currentState, 'e'), pop_character); //Check epsilon (as input variable) transitions
                if (!transitions[transition_parameters].empty()){
                    if(currentStack.top() == pop_character || pop_character == 'e') {
                        auto currentStackCopy = currentStack;
                        if (pop_character != 'e'){
                            currentStackCopy.pop();
                        }
                        for (const auto &branch: transitions[transition_parameters]) {
                            auto currentStackCopy_2 = currentStack;
                            if(branch.second != 'e') { //Push element to stack if the element is not epsilon
                                currentStackCopy_2.push(branch.second);
                            }
                            auto pathCopy = path;
                            pathCopy.push_back(branch.first);
                            auto result = findRoute(branch.first, characterIndex, stringIndex, currentStackCopy_2, pathCopy);
                            if (result.accepted) {
                                return result;
                            }
                        }
                    }
                }
            }
        }
        allPaths.push_back(path);
        return PdaOutput(false, path);
    }
};

int main() {
    int number_of_states = 0;
    int number_of_variables_in_input_alphabet = 0;
    int number_of_variables_in_stack_alphabet = 0;
    int number_of_goal_states = 0;
    std::vector<std::string> states;
    std::vector<std::string> goal_states;
    std::string start_state;
    std::vector<char> variables_in_input_alphabet;
    std::vector<char> variables_in_stack_alphabet;
    char initial_stack_symbol;
    std::map<std::pair<std::pair<std::string, char>, char>,std::vector<std::pair<std::string,char>>> transitions ;
    std::vector<std::string> strings;
    std::string file_loc;
    std::cout << "Please enter the loaction of the input file:";
    std::cin >> file_loc;
    std::ifstream inputFile(file_loc);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }
    auto inputParser = InputParse{inputFile};
    inputParser.parseInput(number_of_states, number_of_variables_in_input_alphabet, number_of_variables_in_stack_alphabet, number_of_goal_states, states, start_state, goal_states, variables_in_input_alphabet, variables_in_stack_alphabet, initial_stack_symbol, transitions, strings );
    /*std::cout << "Num States: "  << number_of_states << std::endl;
    std::cout << "Num Variables in Input Alphabet: " << number_of_variables_in_input_alphabet << std::endl;
    std::cout << "Num Variables in Stack Alphabet: " << number_of_variables_in_stack_alphabet << std::endl;
    std::cout << "Num Goal States: " << number_of_goal_states << std::endl;

    std::cout << "States: " << std::endl;
    for (auto state: states) {
        std::cout << state << std::endl;
    }
    std::cout << "Start State: " << start_state << std::endl;
    std::cout << "Goal States: " << std::endl;
    for (auto state: goal_states) {
        std::cout << state << std::endl;
    }
    std::cout << "Variables in Input Alphabet" << std::endl;
    for (auto variable: variables_in_input_alphabet) {
        std::cout << variable << std::endl;
    }
    std::cout << "Variables in Stack Alphabet" << std::endl;
    for (auto variable: variables_in_stack_alphabet) {
        std::cout << variable << std::endl;
    }
    std::cout << "Initial Stack Symbol: " << initial_stack_symbol << std::endl;
    std::cout << "Transitions: " << std::endl;

    for(auto element: transitions){
        std::cout << "From State: " << element.first.first.first << std::endl;
        std::cout << "With Input Character: " << element.first.first.second << std::endl;
        std::cout << "Pop Character: " << element.first.second << std::endl;
        std::cout << "Push Character: " << element.second[0].second << std::endl;
        std::cout << "To State: " << element.second[0].first << std::endl;
        std::cout << "************" << std::endl;
    }
    std::cout << "Strings: " << std::endl;
    for (auto string: strings) {
        std::cout << string << std::endl;
    }
    {};*/
    auto eval = PdaEvaluater{number_of_states, number_of_variables_in_input_alphabet, number_of_variables_in_stack_alphabet, number_of_goal_states, states, start_state, goal_states, variables_in_input_alphabet, variables_in_stack_alphabet, initial_stack_symbol, transitions, strings};
    //evaluatePDA(number_of_states, number_of_variables_in_input_alphabet, number_of_variables_in_stack_alphabet, states, goal_states, variables_in_input_alphabet, transitions, strings);
    eval.evaluate();
    std::cout << "Successfully written the results on the file: output.txt" << std::endl;
    return 0;
}

