#include "Board.h"


Board::Board(std::string room_id)
{
    players[ID::PLAYER1] = new Player(ID::PLAYER1);
    players[ID::PLAYER2] = new Player(ID::PLAYER2);

    //Set the turn to player 1 for starters
    turn = ID::PLAYER1;

    //Set under check status of players
    under_check[ID::PLAYER1] = false;
    under_check[ID::PLAYER2] = false;

    //Set Room id for the current Game
    room_id = room_id;

    /*
    * KEY - Position on the board
    * VALUE - Pair [ player id, piece pointer]
    */

    //player one piece on board_map
    board_map[{1, 1}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::ROOK1] };     board_map[{1, 2}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::KNIGHT1] };
    board_map[{1, 3}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::BISHOP1] };   board_map[{1, 4}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::QUEEN] };
    board_map[{1, 5}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::KING] };      board_map[{1, 6}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::BISHOP2] };
    board_map[{1, 7}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::KNIGHT2] };   board_map[{1, 8}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::ROOK2] };
    board_map[{2, 1}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::PAWN1] };     board_map[{2, 2}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::PAWN2] };
    board_map[{2, 3}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::PAWN3] };     board_map[{2, 4}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::PAWN4] };
    board_map[{2, 5}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::PAWN5] };     board_map[{2, 6}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::PAWN6] };
    board_map[{2, 7}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::PAWN7] };     board_map[{2, 8}] = { ID::PLAYER1,players[ID::PLAYER1]->piece_map[ID::PAWN8] };

    //player 2 piece on board_map                                                                                         
    board_map[{8, 1}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::ROOK1] };     board_map[{8, 2}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::KNIGHT1] };
    board_map[{8, 3}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::BISHOP1] };   board_map[{8, 4}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::QUEEN] };
    board_map[{8, 5}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::KING] };      board_map[{8, 6}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::BISHOP2] };
    board_map[{8, 7}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::KNIGHT2]};   board_map[{8, 8}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::ROOK2] };
    board_map[{7, 1}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::PAWN1] };     board_map[{7, 2}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::PAWN2] };
    board_map[{7, 3}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::PAWN3] };     board_map[{7, 4}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::PAWN4] };
    board_map[{7, 5}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::PAWN5] };     board_map[{7, 6}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::PAWN6] };
    board_map[{7, 7}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::PAWN7] };     board_map[{7, 8}] = { ID::PLAYER2,players[ID::PLAYER2]->piece_map[ID::PAWN7] };
}


//This function calculates Check Status
// parameter - 
// "turn" to check whose king should be checked
// this turn is different from the turn data member - this one is passed as a parameter
bool Board::calc_check(std::string turn)
{
    std::string checked_player_id = turn;
    std::string opponent_id = ID::PLAYER1 == turn ? ID::PLAYER2 : ID::PLAYER1;

    std::pair<int, int> king_position = players[checked_player_id]->piece_map[ID::KING]->get_position();

    //use copy of piece_map : due to two use cases

    for (auto opp_piece : players[opponent_id]->piece_map)
    {
        //king cannot check another king
        if (opp_piece.first == ID::KING)
        {
            continue;
        }
        std::set<std::pair<int, int>> attack_positions = opp_piece.second->get_possible_moves(board_map, opponent_id);
        if (attack_positions.find(king_position) != attack_positions.end())
        {
            return true;//tell that the king is in check
        }
    }

    return false;
}

bool Board::calc_checkmate(std::string turn)
{
    bool king_under_check = this->calc_check(turn);
    
    //if not in check also not in checkmate
    if (king_under_check == false)
        return false;   

    //if the king can escape check
    std::set<std::pair<int, int>> king_escape_moves = this->get_valid_moves( turn, ID::KING);
    if (king_escape_moves.empty())
    {
        //if no escape move for king: is checkmate
        return true;
    }
    
    //if escape moves not empty: not checkmate
    return false;
}

void Board::move(json data) {
    //extracting pos data 
    int x = data["position"]["x"];
    int y = data["position"]["y"];

    std::pair<int, int> newPosition = std::make_pair(x, y);

    //update board_map
    board_map.erase(players[data["player_id"]]->get_position(data["piece_id"]));
    board_map[newPosition] = { data["player_id"], players[data["player_id"]]->piece_map[data["piece_id"]] };

    //update piece position
    update_position(data["player_id"], data["piece_id"], newPosition);
 
}

void Board::update_position(std::string player_id, std::string piece_id, std::pair<int,int> position)
{
    players[player_id]->piece_map[piece_id]->set_position(position);
}

std::set<std::pair<int,int>> Board::get_valid_moves( std::string player_id, std::string piece_id)
{
    return players[player_id]->piece_map[piece_id]->get_possible_moves(board_map, player_id);
}

bool Board::match_piece_at_location(std::string player_id, std::string piece_id, std::pair<int,int> position)
{
    if(board_map.find(position) == board_map.end())
    {
        return false;
    }
    else
    {
        if(((board_map[position].second)->get_id() == piece_id) && board_map[position].first == player_id)
        {
            return true;
        }
    }
    return false;
}
bool Board::get_under_check(std::string player_id)
{
    return under_check[player_id];
}
void Board::update_under_check(std::string player_id, bool is_checked)
{
    under_check[player_id] = is_checked;
}

bool Board::if_move_remove_check(std::pair<int, int> move, std::string player_id, std::string piece_id)
{
    std::pair<int, int> new_pos = move;
    //save curr(old) position of piece
    std::pair<int, int> old_pos = this->players[player_id]->get_position(piece_id);
    //save if anything existed in new_Pos 
    bool flag = false;
    std::string saved_piece_player_id = "";
    Chess_Piece* saved_piece_ptr = nullptr;
    if (board_map.find(new_pos)!=board_map.end())
    {
        flag = true;
        saved_piece_player_id = board_map[new_pos].first;
        saved_piece_ptr = board_map[new_pos].second;
    }
    //temporarily move the piece
    board_map.erase(old_pos);
    board_map[new_pos] = { player_id, players[player_id]->piece_map[piece_id] };
    update_position(player_id, piece_id, new_pos);

    bool is_check_removed = ! this->calc_check(player_id);

    //undo the temp. move
    board_map.erase(new_pos);
    if (flag == true)
    {
        board_map[new_pos].first = saved_piece_player_id;
        board_map[new_pos].second = saved_piece_ptr;
    }
    board_map[old_pos]= { player_id, players[player_id]->piece_map[piece_id] };
    update_position(player_id, piece_id, old_pos);

    return is_check_removed;
}
void Board::print()
{
    std::cout << "HELLO" << std::endl;
    //std::cout << players[ID::PLAYER1]->get_position(ID::PAWN1).first << " " << players[ID::PLAYER1]->get_position(ID::PAWN1).second;
    std::cout << "game room id:" << room_id;
}