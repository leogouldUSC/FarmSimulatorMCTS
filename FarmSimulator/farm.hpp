//
//  farm.hpp
//  FarmSimulator
//
//  Created by Leo Gould on 8/1/19.
//  Copyright © 2019 Leo Gould. All rights reserved.
//

// Petter Strandmark 2013
// petter.strandmark@gmail.com

#ifndef FARM_H
#define FARM_H

#include <algorithm>
#include <iostream>
#include <utility>
#include <map>
#include "mcts.h"
#include <list>
#include <sstream>

#endif


template<unsigned int M, unsigned int N>
class FarmState
{
public:
    
    // Mutable because is_move_possible temporary modifies
    // the board.
    mutable unsigned char board[M][N];
    unsigned int score;
    unsigned int time;
    std::map<int, int> plantedCrops;
    unsigned int previous_board_hash_value;
    std::set<unsigned int> all_hash_values;
    
    
public:
    static const unsigned char empty = 0;
    static const unsigned char corn = 1;
    static const unsigned char bean = 2;
    static const unsigned char grownCorn = 4;
    static const unsigned char grownBean = 5;

    static const unsigned char pick = 3;
    static const unsigned char player1 = 1;
    static const unsigned char player2 = 2;
    
    //first int represents the location, second represents the
    typedef std::pair<int, int> Move;
    mutable int depth;
    int player_to_move;
//    typedef int Move;
    static const Move no_move;
    static const Move pass;
    
    static int ij_to_ind(int i, int j)
    {
        attest(i >= 0 && j >= 0 && i < M && j < N);
        return N*i + j;
    }
    
    static std::pair<int, int> ind_to_ij(int ind)
    {
        attest(ind >= 0 && ind < M * N);
        return std::make_pair(ind / N, ind % N);
    }
    
    
    FarmState():
        player_to_move(1),
        previous_board_hash_value(0),
        depth(0),
        score(0),
        time(0)
    {
        all_hash_values.insert(compute_hash_value());
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                board[i][j] =  empty;
            }
        }
    }
    
    FarmState(char board[M][N+1]):
        player_to_move(1),
        previous_board_hash_value(0),
        depth(0)
    {
        FarmState<M, N> state;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == '1') {
                    set_pos(i, j, 1);
                }
                else if (board[i][j] == '2') {
                    set_pos(i, j, 2);
                }
                else if (board[i][j] == '4') {
                    set_pos(i, j, 4);
                }
                else if (board[i][j] == '5') {
                    set_pos(i, j, 5);
                }
                else {
                    set_pos(i, j, empty);
                }
            }}
    }
    
    virtual ~FarmState() { }
    
    virtual unsigned char get_pos(int i, int j) const
    {
        attest(ij_to_ind(i, j) >= 0);
        return board[i][j];
    }
    
    virtual void set_pos(int i, int j, unsigned char player)
    {
        attest(ij_to_ind(i, j) >= 0);
        board[i][j] = player;
    }
    
    virtual unsigned int compute_hash_value() const
    {
        unsigned int value = 0;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                value = 65537 * value + board[i][j];
            }}
        return value;
    }
    
    virtual bool is_move_possible(const int i, const int j, const int k) const
    {
        if (0 <= i && i < M && 0 <= j && j < N) {
            if (board[i][j] != empty) {
                //if the crop is fully grown and the move is harvest return true
                if((board[i][j] == grownCorn || board[i][j] == grownBean) && k == 3){
                    return true;
                }
                return false;
            }
            if(k == 1 || k == 2) {
                return true;
            }
        }
        else {
            // Not a valid position.
            return false;
        }
        return false;
    }
    
    virtual void do_move(Move move)
    {
        depth++;
        
        int opponent = 1;
        

        int i,j;
        if(move.first != -1 && move.second !=-1){
            std::tie(i, j) = ind_to_ij(move.first);
        
            attest(is_move_possible(i, j, move.second));
        
            if(move.second == corn){
                plantedCrops[move.first] = 0;
                board[i][j] = corn;
            }
            if(move.second == bean){
                plantedCrops[move.first] = 0;
                board[i][j] = bean;
            }
            if(move.second == pick && board[i][j] == grownCorn){
                board[i][j] = empty;
                score += 1;
            }
            if(move.second == pick && board[i][j] == grownBean){
                board[i][j] = empty;
                score += 2;
            }
        
        }
        
        // We save the hash values before all captures as this is way easier
        // to check.
        previous_board_hash_value = compute_hash_value();
        all_hash_values.insert(previous_board_hash_value);
        
    
        // Next player
        player_to_move = opponent;
    }
    
    template<typename RandomEngine>
    void do_random_move(RandomEngine* engine)
    {
        auto moves = get_moves();
        attest(! moves.empty());
        std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
        auto move = moves[move_ind(*engine)];
        do_move(move);
    }
    
    virtual bool has_moves() const
    {
        // TODO: make faster.
        return ! get_moves().empty();
    }
    
    
    virtual std::vector<Move> get_moves() const
    {
        std::vector<Move> moves;
        if (depth > 1000) {
            attest(false);
            return moves;
        }
        //loop through whole board and return any empty square as having the moves
        //plant bean(2) or plant corn(3)
        //any square with a fully grown bean/corn should return the move to harvest it 4
        
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == empty) {
                    moves.push_back(std::make_pair(ij_to_ind(i, j), 1));
                    moves.push_back(std::make_pair(ij_to_ind(i, j), 2));
                }
                if(board[i][j] == grownCorn || board[i][j] == grownBean){
                    moves.push_back(std::make_pair(ij_to_ind(i, j), 3));
                }
            }}
        if (moves.empty()) {
            moves.push_back(pass);
        }
        
        return moves;
    }
    
    virtual int get_player_score(int current_player_to_move) const{
        return score;
    }
    virtual int get_result(int current_player_to_move) const
    {
        return score;
    }
    virtual void updateBoard(){
        ++time;
        for(auto a : plantedCrops){
            int day = a.second;
            plantedCrops[a.first] = ++day;
            if(day == 2){
//                plantedCrops.erase(a.first);
                int i, j;
                std::tie(i, j) = ind_to_ij(a.first);
                if(board[i][j] == corn){
                    board[i][j] = 4;
                }
                if(board[i][j] == bean){
                    board[i][j] = 5;
                }
            }
        }
        dump_board();
    }
    virtual void dump_board() const
    {
        std::cout << "static const int M = " << M << ";" << std::endl;
        std::cout << "static const int N = " << N << ";" << std::endl;
        std::cout << "char board[M][N+1] = {" << std::endl;
        for (int i = 0; i < M; ++i) {
            std::cout << "\t\"";
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == empty) std::cout << ".";
                else std::cout << int(board[i][j]);
            }
            std::cout << "\"," << std::endl;
        }
        std::cout << "};" << std::endl;
    }
};


template<unsigned int M, unsigned int N>
const typename FarmState<M, N>::Move FarmState<M, N>::no_move = std::make_pair(-2, -2);

template<unsigned int M, unsigned int N>
const typename FarmState<M, N>::Move FarmState<M, N>::pass = std::make_pair(-1, -1);


class FarmSimulator
{
public:
    FarmSimulator();
    void setup();
    
    
    void update();
    void draw();
    
    static const int M = 1;
    static const int N = 1;
    //unsigned int time = 0;
    static const int board_x = 25;
    static const int board_y = 25;
    static const int board_width = 40;
    
    typedef FarmState<M, N> State;
    State state;
    
    enum PlayerType {HUMAN, COMPUTER};
    PlayerType player1, player2;
    MCTS::ComputeOptions player1_options, player2_options;
    
    enum GameStatus {WAITING_FOR_USER, COMPUTER_THINKING, GAME_OVER, GAME_ERROR};
    GameStatus game_status;
    std::string error_string;
    
    // Compute move.
    std::future<State::Move> computed_move;
    void next_player();
    void start_compute_move();
    void check_for_computed_move();
    
};

FarmSimulator::FarmSimulator()
{
    
}

void FarmSimulator::setup()
{
    //state = State(board);
    player1 = COMPUTER;
    //    player2 = COMPUTER;
    
    player1_options.max_iterations = -1;
    player1_options.max_time = 5.0;
    player1_options.verbose = true;
    
    //    player2_options.max_iterations = -1;
    //    player2_options.max_time = 1.0;
    
    start_compute_move();
    
}

void FarmSimulator::start_compute_move()
{
    //game_status = WAITING_FOR_USER;
    //return;
    
    game_status = COMPUTER_THINKING;
    
    MCTS::ComputeOptions options;
    if (state.player_to_move == 1) {
        options = player1_options;
    }
    else {
        options = player2_options;
    }
    
    auto state_copy = state;
    computed_move =
    std::async(std::launch::async,
               [state_copy, options]()
               {
                   auto best_move = MCTS::compute_move(state_copy, options);
                   return best_move;
                   
                   //// Single-threaded.
//                   auto tree = MCTS::compute_tree(state_copy, options, 1241 * std::time(0));
//                   typedef MCTS::Node<State> Node;
//                   auto best_child = *std::max_element( tree->children.begin(), tree->children.end(), [](Node* lhs, Node* rhs) { return lhs->visits < rhs->visits; } );
//                   return best_child->move;
               });
//    auto best_move = MCTS::compute_move(state_copy, options);

    try {
        auto move = computed_move.get();
        state.do_move(move);
        
        // Are there any more moves possible?
        std::cout << state.time << std::endl;
//        if (state.get_moves().empty() && state.time == 91) {
        if (state.time == 91) {
            game_status = GAME_OVER;
        }
        else {
            next_player();
        }
        
    } catch (std::exception& error) {
        game_status = GAME_ERROR;
        error_string = error.what();
    }
}

void FarmSimulator::check_for_computed_move()
{
    //return;
    
    if (game_status != COMPUTER_THINKING) {
        return;
    }
    
    auto status = computed_move.wait_for(std::chrono::seconds(10));
    if (status == std::future_status::ready) {
        try {
            auto move = computed_move.get();
            state.do_move(move);
            
            // Are there any more moves possible?
            if (state.get_moves().empty()) {
                game_status = GAME_OVER;
            }
            else {
                next_player();
            }
            
        } catch (std::exception& error) {
            game_status = GAME_ERROR;
            error_string = error.what();
        }
    }
}

void FarmSimulator::next_player()
{
    state.updateBoard();
    if (state.player_to_move == 1) {
        if (player1 == HUMAN) {
            game_status = WAITING_FOR_USER;
        }
        else {
            start_compute_move();
        }
    }
    else if (state.player_to_move == 2) {
        if (player2 == HUMAN) {
            game_status = WAITING_FOR_USER;
        }
        else {
            start_compute_move();
        }
    }
}



void FarmSimulator::update()
{
    check_for_computed_move();
}
void FarmSimulator::draw()
{
    
    std::stringstream sout;
    if (game_status == WAITING_FOR_USER) {
        sout << "Make your move!" << std::endl;
    }
    else if (game_status == COMPUTER_THINKING) {
        sout << "Computing..." << std::endl;
    }
    else if (game_status == GAME_OVER) {
        sout << "Game over!" << std::endl;
    }
    else if (game_status == GAME_ERROR) {
        sout << "ERROR:\n" << error_string << std::endl;
    }
    
    std::cout << "Player 1 (black) score: " << state.get_player_score(1) << std::endl;
    state.dump_board();
    
    //sout << "Board hash: " << state.previous_board_hash_value << " -> " << state.compute_hash_value() << endl;
    //for (auto move: state.get_moves()) {
    //    sout << move << " ";
    //}
    
    
}
