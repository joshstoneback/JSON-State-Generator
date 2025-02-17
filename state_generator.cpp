/*
    Below is an example JSON configuration of a Klondike solitiare starting position.

    {
    "tableau piles": [
        ["KH"],
        ["10d", "7S"],
        ["8h", "7c", "6H"],
        ["6c", "4s", "3d", "2S"],
        ["8d", "qd", "10c", "8c", "9D"],
        ["as", "kd", "qc", "jc", "10s", "9H"],
        ["ks", "qh", "js", "10h", "9c", "8s", "7H"]
    ],
    "stock": ["6s", "kc", "jh", "5d", "3s", "9s", "4c", "ad", "7d", "5H", "2H", "4D", "3H", "2C", "ah", "ac"],
    "waste": [],
    "foundations": [[], [], [], []]
    }

    This program generates a user-input number of possible states of a solitaire game given conditions placed on 
    the initially visible cards, signified by lowercase letters.

*/

#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <filesystem>
#include <cctype>

// generate an ordered deck of cards
std::vector <std::string> generate_deck(){
    std::vector<std::string> deck;
    const std::string suits = "hdsc";
    const std::string values = "a23456789jqk";

    for (char suit : suits) {
        for (char value : values) {
            deck.push_back("\"" + std::string(1, value) + suit + "\"");
        }
        deck.push_back("\"10" + std::string(1, suit) + "\"");
    }

    return deck;
}

// shuffle a full deck of cards
std::vector <std::string> shuffle_deck(){
    std::vector<std::string> deck = generate_deck();
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    return deck;
}

// shuffle any input cards
std::vector <std::string> shuffle_cards(std::vector <std::string> cards){
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
    return cards;
}

// generate a random state of the game
std::string random_state(){
    std::vector <std::string> deck = shuffle_deck();
    std::string state = "{\n";
    state += "\"tableau piles\": [";
    for(int i = 0; i < 7; i++){
        state += "[";
        for(int j = 0; j < i + 1; j++){
            if(j == i){
                std::string card = deck.back();
                deck.pop_back();
                card[1] = std::toupper(card[1]);
                card[2] = std::toupper(card[2]);
                if (card.size() > 3) { // Account for 10
                    card[3] = std::toupper(card[3]);
                }
                state += card;
            } else {
                state += deck.back() + ", ";
                deck.pop_back();
            }
        }
        state += "]";
        if(i != 6){
            state += ", ";
        }
    }
    state += "],\n";
    state += "\"stock\": [";
    for(int i = 0; i < 24; i++){
        state += deck.back();
        deck.pop_back();
        if(i != 23){
            state += ", ";
        }
    }
    state += "]\n";
    state += "}";
    return state;
}

// generate a state of the game with a specific set of cards in the tableau
std::string state_from_set_tableau(std::vector<std::string> cards){
    std::vector<std::string> deck = shuffle_deck();
    for (std::string card : cards) {
        deck.erase(std::remove(deck.begin(), deck.end(), card), deck.end());
    }
    std::string state = "{\n";
    state += "\"tableau piles\": [";
    for (int i = 0; i < 7; i++) {
        state += "[";
        for (int j = 0; j < i + 1; j++) {
            if (j == i) {
                std::string card = "";
                if (!cards.empty()) {
                    card = cards.back();
                    cards.pop_back();
                } else {
                    std::cerr << "Error: Attempted to access an empty cards vector." << std::endl;
                    return "";
                }
                card[1] = std::toupper(card[1]);
                card[2] = std::toupper(card[2]);
                if (card.size() > 3) { // Account for 10
                    card[3] = std::toupper(card[3]);
                }
                state += card;
            } else {
                state += deck.back() + ", ";
                deck.pop_back();
            }
        }
        state += "]";
        if (i != 6) {
            state += ", ";
        }
    }
    
    state += "],\n";
    state += "\"stock\": [";
    for(int i = 0; i < 24; i++){
        state += deck.back();
        deck.pop_back();
        if(i != 23){
            state += ", ";
        }
    }
    state += "]\n";
    state += "}";
    return state;
}

// generate states with exactly n of a specific rank into a file
void n_of_one_rank_to_folder(int n, std::string foldername, std::string rank, int num_states){
    if (!std::filesystem::exists(foldername)) {
        std::filesystem::create_directory(foldername);
    }
    for(int i = 0; i < num_states; i++){
        std::vector<std::string> deck = shuffle_deck();
        std::vector<std::string> cards;
        for (int j = 0; j < 52; ++j) {
            if (cards.size() == n) { // Happens before the deck is searched to account for the case where n = 0
                break;
            }
            std::string card = deck[j];
            if (std::tolower(card[1]) == char(std::tolower(rank[0])) || (card[2] == 0 && rank == "10")) {
                cards.push_back(card);
            }
        }
        for (int j = 0; j < 7 - n; ++j) {
            if (std::tolower(deck.back()[1]) == char(std::tolower(rank[0])) || (deck.back()[2] == 0 && rank == "10")) {
                deck.pop_back();
                j--;
                continue;
            } else {
                cards.push_back(deck.back());
                deck.pop_back();
            }
            
        }
        cards = shuffle_cards(cards);

        std::string game_state = state_from_set_tableau(cards);
        std::string filename = foldername + "/game" + std::to_string(i + 1) + ".json";

        std::ofstream file;
        if (!file) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            continue;
        }
        file.open(filename);
        file << game_state << std::endl;
        file.close();
    }
}

// generate random states into a folder
void random_to_folder(std::string foldername, int num_states){
    if (!std::filesystem::exists(foldername)) {
        std::filesystem::create_directory(foldername);
    }
    for(int i = 0; i < num_states; i++){
        std::string game_state = random_state();
        std::string filename = foldername + "/game" + std::to_string(i + 1) + ".json";

        std::ofstream file;
        if (!file) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            continue;
        }
        file.open(filename);
        file << game_state << std::endl;
        file.close();
    }
}

// generate states with no duplicate ranks into a folder
void no_duplicates_to_folder(std::string foldername, int num_states){
    if (!std::filesystem::exists(foldername)) {
        std::filesystem::create_directory(foldername);
    }
    for(int i = 0; i < num_states; i++){
        std::vector<std::string> deck = shuffle_deck();
        std::vector<std::string> cards;
        std::vector<char> ranks;

        // Generate 7 cards of different ranks
        for (int j = 0; j < 52; ++j) {
            std::string card = deck[j];
            char rank = std::tolower(card[1]);
            std::cout << rank << std::endl;
            if (std::find(ranks.begin(), ranks.end(), rank) == ranks.end()) {
                cards.push_back(card);
                ranks.push_back(rank);
            }
            if (cards.size() >= 7) {
                break;
            }
        }
        cards = shuffle_cards(cards);

        std::string game_state = state_from_set_tableau(cards);
        std::string filename = foldername + "/game" + std::to_string(i + 1) + ".json";

        std::ofstream file;
        if (!file) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            continue;
        }
        file.open(filename);
        file << game_state << std::endl;
        file.close();
    }
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    // random_to_folder("random1000", 1000);
    // n_of_one_rank_to_folder(4, "all_aces", "A", 1000);
    // no_duplicates_to_folder("no_duplicates", 1000);
    n_of_one_rank_to_folder(4, "new_all_8s", "8", 1000);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}