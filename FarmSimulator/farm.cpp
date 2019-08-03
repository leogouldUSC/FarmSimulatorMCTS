//// Petter Strandmark 2013.
//
//#ifndef FARM_CPP
//#define FARM_CPP
//
//#include <list>
//#include <sstream>
////#include <vector>
//
//#define UNICODE
//#include "farm.hpp"
//
//
//#include "mcts.h"
//
//#endif
//
//using namespace std;
//
//class FarmSimulator
//{
//public:
//    FarmSimulator();
//    void setup();
//    
//    
//    void update();
//    void draw();
//    
//    static const int M = 9;
//    static const int N = 9;
//   //unsigned int time = 0;
//    static const int board_x = 25;
//    static const int board_y = 25;
//    static const int board_width = 40;
//    
//    typedef FarmState<M, N> State;
//    State state;
//    
//    enum PlayerType {HUMAN, COMPUTER};
//    PlayerType player1, player2;
//    MCTS::ComputeOptions player1_options, player2_options;
//    
//    enum GameStatus {WAITING_FOR_USER, COMPUTER_THINKING, GAME_OVER, GAME_ERROR};
//    GameStatus game_status;
//    std::string error_string;
//    
//    // Compute move.
//    std::future<State::Move> computed_move;
//    void next_player();
//    void start_compute_move();
//    void check_for_computed_move();
//    
//};
//
//FarmSimulator::FarmSimulator()
//{
//    
//}
//
//void FarmSimulator::setup()
//{
//    //state = State(board);
//    player1 = COMPUTER;
////    player2 = COMPUTER;
//    
//    player1_options.max_iterations = -1;
//    player1_options.max_time = 5.0;
//    player1_options.verbose = true;
//    
////    player2_options.max_iterations = -1;
////    player2_options.max_time = 1.0;
//    
//    start_compute_move();
//    
//}
//
//void FarmSimulator::start_compute_move()
//{
//    //game_status = WAITING_FOR_USER;
//    //return;
//    
//    game_status = COMPUTER_THINKING;
//    
//    MCTS::ComputeOptions options;
//    if (state.player_to_move == 1) {
//        options = player1_options;
//    }
//    else {
//        options = player2_options;
//    }
//    
//    auto state_copy = state;
//    computed_move =
//    std::async(std::launch::async,
//               [state_copy, options]()
//               {
//                   auto best_move = MCTS::compute_move(state_copy, options);
//                   return best_move;
//                   
//                   //// Single-threaded.
//                   //auto tree = MCTS::compute_tree(state_copy, options, 1241 * std::time(0));
//                   //typedef MCTS::Node<State> Node;
//                   //auto best_child = *std::max_element( tree->children.begin(), tree->children.end(), [](Node* lhs, Node* rhs) { return lhs->visits < rhs->visits; } );
//                   //return best_child->move;
//               });
//}
//
//void FarmSimulator::check_for_computed_move()
//{
//    //return;
//    
//    if (game_status != COMPUTER_THINKING) {
//        return;
//    }
//    
//    auto status = computed_move.wait_for(std::chrono::seconds(0));
//    if (status == std::future_status::ready) {
//        try {
//            auto move = computed_move.get();
//            state.do_move(move);
//            
//            // Are there any more moves possible?
//            if (state.get_moves().empty()) {
//                game_status = GAME_OVER;
//            }
//            else {
//                next_player();
//            }
//            
//        } catch (std::exception& error) {
//            game_status = GAME_ERROR;
//            error_string = error.what();
//        }
//    }
//}
//
//void FarmSimulator::next_player()
//{
////    state.updateBoard();
//    if (state.player_to_move == 1) {
//        if (player1 == HUMAN) {
//            game_status = WAITING_FOR_USER;
//        }
//        else {
//            start_compute_move();
//        }
//    }
//    else if (state.player_to_move == 2) {
//        if (player2 == HUMAN) {
//            game_status = WAITING_FOR_USER;
//        }
//        else {
//            start_compute_move();
//        }
//    }
//}
//
//
//
//void FarmSimulator::update()
//{
//    check_for_computed_move();
//}
//void FarmSimulator::draw()
//{
//    
//    stringstream sout;
//    if (game_status == WAITING_FOR_USER) {
//        sout << "Make your move!" << endl;
//    }
//    else if (game_status == COMPUTER_THINKING) {
//        sout << "Computing..." << endl;
//    }
//    else if (game_status == GAME_OVER) {
//        sout << "Game over!" << endl;
//    }
//    else if (game_status == GAME_ERROR) {
//        sout << "ERROR:\n" << error_string << endl;
//    }
//    
//    sout << "Player 1 (black) score: " << state.get_player_score(1) << endl;
////    state.dump_board();
//    
//    //sout << "Board hash: " << state.previous_board_hash_value << " -> " << state.compute_hash_value() << endl;
//    //for (auto move: state.get_moves()) {
//    //    sout << move << " ";
//    //}
//    
//    
//}
//
////template<unsigned int M, unsigned int N>
////const typename FarmState<M, N>::Move FarmState<M, N>::no_move = std::make_pair(-2, -2);
////
////template<unsigned int M, unsigned int N>
////const typename FarmState<M, N>::Move FarmState<M, N>::pass = std::make_pair(-1, -1);

