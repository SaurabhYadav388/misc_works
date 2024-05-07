#pragma once
#include <iostream>
#include "Player.h"
#include <nlohmann/json.hpp>
#include "GlobalDefinitions.h"

using namespace nlohmann;

class Board {

    std::map<std::string, Player*> players;
    std::string room_id;
    std::string turn;
    std::map<std::string, bool> under_check;

    //KEY - Position on the board
    //VALUE - Pair [ player id, chess piece pointer]
    std::map<std::pair<int, int>, std::pair<std::string, Chess_Piece*>> board_map;

public:
    Board(std::string);
    //check if the player's certain piece is at given location
    bool match_piece_at_location(std::string, std::string, std::pair<int,int>);
    //check if players are under check
    bool get_under_check(std::string);
    //update check flag of the players
    void update_under_check(std::string, bool);
    //Update the board variable after move : board_map and piece_map
    void move(json data);

    /*
    * //This function calculates Check Status
    * parameters -
    * "turn" to check which player's king should be looked up for "IN CHECK" condition
    */
    bool calc_check(std::string turn);
    
    //This function calculates the checkmate status
    bool calc_checkmate(std::string turn);
    std::set<std::pair<int,int>> get_valid_moves( std::string, std::string);
    void update_position(std::string, std::string, std::pair<int, int>);
    
    //This function calculates if a certain move can remove the check of player
    bool if_move_remove_check(std::pair<int, int> move,std::string player_id,std::string piece_id);

    void print();
};

