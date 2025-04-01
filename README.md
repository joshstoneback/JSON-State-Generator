# Klondike Solitaire JSON State Generator

This C++ program generates JSON-formatted initial states of Klondike Solitaire games, tailored for use with a  **[modified version of Solvitaire](https://github.com/joshstoneback/Modified-Solvitaire)**, a solitaire solver. The original version of Solvitaire can be found **[here](https://github.com/thecharlieblake/Solvitaire)**. This state generator allows users to create random or condition-specific game setups to test solver performance or run statistical analyses.

---

## 🧩 JSON Output Format

Each generated JSON file defines a valid initial solitaire state. 

**Example:**
```json
{
  "tableau piles": [
    ["AC"],
    ["6d", "6H"],
    ["jd", "qs", "6S"],
    ["3d", "2s", "ks", "10D"],
    ["9c", "kd", "9h", "qd", "5S"],
    ["9s", "4h", "7c", "qc", "5c", "7H"],
    ["10c", "10s", "8d", "5d", "9d", "4c", "AS"]
  ],
  "stock": ["2h", "5h", "8c", "3s", "js", "7d", "jh", "7s", "kh", "2c", "10h", "jc", "8h", "6c", "2d", "kc", "ad", "3h", "ah", "4s", "qh", "3c", "4d", "8s"]
}
```

- Uppercase card codes (e.g. "KH") indicate face-up cards in the tableau.

- Lowercase card codes (e.g. "10d") are face-down cards.

## ⚙️ Features
This tool can generate game states with specific constraints, such as:

- ✅ Random states

- ♠️ N cards of a specific rank (e.g. 4 Aces)

- ♦️ N and M cards of two specific ranks (e.g. 4 Queens and 3 Tens)

- ♥️ All visible cards of one color (e.g. all red)

- ♣️ No duplicate ranks

## 🚀 How to Use

1. **Compile the program**  
   ```bash
   g++ -std=c++17 -O2 solitaire_generator.cpp -o solitaire_generator
   ```

2. **Modify `main()`**  
   Inside the `main()` function, uncomment or edit one or more of the generation functions based on what dataset you'd like to create.

   **Example:**
   ```cpp
   random_to_folder("random1000", 1000);
   n_of_one_rank_to_folder(4, "all_aces", "A", 1000);
   no_duplicates_to_folder("no_duplicates", 1000);
   ```
   Each function will create a folder and write JSON files (`game1.json`, `game2.json`, ...) inside.

3. **Run the program**  
   ```bash
   ./solitaire_generator
   ```
   It will generate the files and print how long it took.


## 📦 Generation Functions

| Function | Description |
|----------|-------------|
| `random_to_folder(folder, num_states)` | Generates `num_states` random game states. |
| `n_of_one_rank_to_folder(n, folder, rank, num_states)` | Generates states with exactly `n` face-up tableau cards of a given rank (e.g. "A" for Aces, "9" for Nines). |
| `n_m_of_two_ranks_to_folder(folder, n, m, rank1, rank2, num_states)` | Generates states with `n` cards of `rank1` and `m` cards of `rank2` in the tableau. |
| `all_one_color_to_folder(folder, color, num_states)` | Generates states where all 7 face-up tableau cards are either "red" (hearts/diamonds) or "black" (spades/clubs). |
| `no_duplicates_to_folder(folder, num_states)` | Generates states where the 7 face-up tableau cards are all different ranks. |


## 🛠 Internal Structure

| Function | Purpose |
|----------|---------|
| `generate_deck()` / `shuffle_deck()` | Manage card creation and shuffling. |
| `state_from_set_tableau()` | Builds a game state with specific visible cards. |
| `shuffle_cards()` | Shuffles selected cards for varied output. |
| `*_to_folder()` methods | Generate and write game states under different conditions. |

