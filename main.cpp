#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>

using namespace std;

static std::mt19937 gen(12345);

const int MAX_ALLOWED_COMBO = 4;

enum class GameState {
  GAME_STATE_ACTIVE,
  GAME_STATE_WON,
  GAME_STATE_LOST,
  GAME_STATE_ERRORED,
};

enum LogLevel {
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR,
};

int combination_num_1 = 0;
int combination_num_2 = 0;
int combination_num_3 = 0;
int combination_num_4 = 0;

// static const unsigned NUM_OF_DICE = 2;

LogLevel log_level = LogLevel::LOG_LEVEL_INFO;

bool contains(const vector <int> &vec, int value) {
  return find(vec.begin(), vec.end(), value) != vec.end();
}

int dice_roll() {
  // Generate a random number between 1 and 6
  // random_device rd;
  // mt19937 gen(rd());
  uniform_int_distribution<> dis(1, 6);
  return dis(gen);
}

pair<bool,vector<int>> select_numbers_old(vector <bool> &numbers, int n) {
  vector <int> combination;
  for (int i = 8; i >= 0; i--) {
    if (!numbers[i]) {
      if (i + 1 == n) {
        numbers[i] = true;
        combination.push_back(i);
        return {true, combination};
      }
      for (int j = 8; j >= 0; j--) {
        if (!numbers[j] && i != j && i + j + 2 == n) {
          numbers[i] = true;
          numbers[j] = true;
          cout << "Selected: " << i + 1 << " " << j + 1 << endl;
          combination.push_back(i);
          combination.push_back(j);
          return {true, combination};
        }
        for (int k = 8; k >= 0; k--) {
          if (!numbers[k] && i != j && i != k && j != k && i + j + k + 3 == n) {
            numbers[i] = true;
            numbers[j] = true;
            numbers[k] = true;
            cout << "Selected: " << i + 1 << " " << j + 1 << " " << k + 1 << endl;
            combination.push_back(i);
            combination.push_back(j);
            combination.push_back(k);
            return {true, combination};
          }
        }
      }
    }
  }
  return {false, combination};
}

void print_combination(vector <int> combination){
  cout << "Selected: ";
  for (int num: combination) {
    cout << num + 1 << " ";
  }
  cout << endl;
  if (combination.size() == 3) {
    cout << "Wow! You selected 3 numbers!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  }
}

// bool select_combination(vector <bool> &numbers, int target, int start, int depth, vector <int> &combination) {
//   if (target == 0 && depth == 0) {
//     for (int num: combination) {
//       numbers[num] = true;
//     }
//     if (log_level == LogLevel::LOG_LEVEL_DEBUG) {
//       print_combination(combination);
//     }
//     return true;
//   }
//   if (target < 0 || depth < 0) {
//     cout << "Went too far: " << target << " " << depth << endl;
//     return false;
//   }
//   for (int i = start, max = static_cast<int>(numbers.size()); i < max; i++) {
//     if (!numbers[i]) {
//       combination.push_back(i);
//       int new_target = target - i - 1;
//       int new_depth = depth - 1;
//       if (new_depth >= 0 && new_target >= 0) {
//         if (select_combination(numbers, new_target, i + 1, new_depth, combination)) {
//           return true;
//         }
//       }
//       combination.pop_back();
//     }
//   }
//   return false;
// }

bool select_combination(vector <int> &indices, int target, int depth, vector <int> &combination) {
  if (depth > MAX_ALLOWED_COMBO) {
    return false;
  }
  // for (int index: indices) {
  //   cout << index << " ";
  // }
  // Check if target is in indices
  if (contains(indices, target - 1)) {
    combination.push_back(target - 1);
    return true;
  }


  return false;
}

vector<int> filter_indices(const vector<bool> &flags) {
  vector<int> indices;
  for (int i = 0; i < static_cast<int>(flags.size()); ++i) {
    if (!flags[i]) {
      indices.push_back(i);
    }
  }
  return indices;
}

pair<bool, vector <int>> select_numbers(vector <bool> &numbers, int target) {
  vector <int> combination;
  vector <int> indices = filter_indices(numbers);
  if (select_combination(indices, target, 0, combination)) {
    for (int num: combination) {
      cout << "Selected: " << num + 1 << endl;
      numbers[num] = true;
    }
    return {true, combination};
  }
  return {false, combination};
}

bool check_is_complete(vector <bool> numbers) {
  for (int i = 0; i < 9; i++) {
    if (!numbers[i]) {
      return false;
    }
  }
  return true;
}

void print_numbers(vector <bool> numbers) {
  for (int j = 0; j < 9; ++j) {
    cout << (numbers[j] ? "_" : "#");
  }
  cout << endl;
}

void print_outcome(GameState current_game_state, int num_attempts) {
  if (current_game_state == GameState::GAME_STATE_WON) {
    cout << "Bingo! You shut the box in " << num_attempts << " attempts!" << endl;
  }
  else if (current_game_state == GameState::GAME_STATE_LOST) {
    cout << "Game over! You lost after " << num_attempts << " attempts!" << endl;
  }
  else {
    cout << "Game over! You ran out of attempts!" << endl;
  }
}

bool shut_the_box() {
  vector <bool> numbers(9, false);

  GameState current_game_state = GameState::GAME_STATE_ACTIVE;

  int num_attempts;
  for (num_attempts = 0; num_attempts < 100000; num_attempts++) {
    int dice1 = dice_roll();
    int dice2 = dice_roll();
    int dice_combo = dice1 + dice2;
    if (log_level == LogLevel::LOG_LEVEL_DEBUG) cout << "Random number: " << dice_combo << endl;
    auto [hit, combination] = select_numbers(numbers, dice_combo);
    if (hit) {
      if (log_level == LogLevel::LOG_LEVEL_DEBUG) cout << "Hit!" << endl;
      if (combination.size() == 1) {
        combination_num_1++;
      }
      else if (combination.size() == 2) {
        combination_num_2++;
      }
      else if (combination.size() == 3) {
        combination_num_3++;
      }
      else if (combination.size() == 4) {
        combination_num_4++;
        // cout << "Four number nutter!" << endl;
        // cout << dice1 << " " << dice2 << " " << dice_combo << endl;
        // print_numbers(numbers);
        // print_combination(combination);
      }
    }
    else {
      if (log_level == LogLevel::LOG_LEVEL_DEBUG) cout << "Miss!" << endl;
      current_game_state = GameState::GAME_STATE_LOST;
      break;
    }
    if (log_level == LogLevel::LOG_LEVEL_DEBUG) print_numbers(numbers);

    if (check_is_complete(numbers)) {
      current_game_state = GameState::GAME_STATE_WON;
      break;
    }
  }

  if (current_game_state == GameState::GAME_STATE_ACTIVE) {
    current_game_state = GameState::GAME_STATE_ERRORED;
  }

  if (log_level == LogLevel::LOG_LEVEL_DEBUG) {
    print_outcome(current_game_state, num_attempts);
    print_numbers(numbers);
  }

  return current_game_state == GameState::GAME_STATE_WON;
}

void run_simulation(int num_simulations) {
  int succeses {0};
  for (int i = 0; i < num_simulations; i++) {
    if (shut_the_box()) {
      succeses++;
    }
  }

  cout << "Success rate: " << succeses << " / " << num_simulations << " (" << (static_cast<float>(succeses) / num_simulations) * 100 << "%)" << endl;
  cout << "combinations: " << combination_num_1 << " " << combination_num_2 << " " << combination_num_3 << " " << combination_num_4 << endl;
}

void test_decision(vector <bool> numbers, int dice_roll) {
  cout << "Testing decision for: " << dice_roll << endl;
  auto [hit, combination] = select_numbers(numbers, dice_roll);
  print_numbers(numbers);
  if (hit) {
    cout << "Hit!" << endl;
    print_combination(combination);
  }
  else {
    cout << "Miss!" << endl;
  }
}

int main(int argc, char *argv[]) {
  auto start_time = chrono::high_resolution_clock::now();
  cout << "Shut the box!" << endl;

  if (argc > 1 && string(argv[1]) == "debug") {
    log_level = LogLevel::LOG_LEVEL_DEBUG;
  }

  // run_simulation(10);

  test_decision(vector <bool> {0,0,0,0,0,0,0,0,0}, 8);
  test_decision(vector <bool> {0,1,0,1,0,1,0,1,0}, 8);
  test_decision(vector <bool> {0,0,0,0,1,1,1,1,1}, 10);
  test_decision(vector <bool> {1,0,1,0,0,1,1,1,1}, 10);

  auto end_time = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
  cout << "Time taken: " << duration.count() << " microseconds" << endl;
}
