#include "thing.h"
#include <iostream>
#include <ranges>
#include <format>
#include <algorithm>
#include <string>
constexpr std::string QUIT = "quit";
constexpr std::string RESET = "reset";
constexpr std::string UUR = "^^>";
constexpr std::string UUL = "^^<";
constexpr std::string DDR = "vv>";
constexpr std::string DDL = "vv<";
constexpr std::string RRU = ">>^";
constexpr std::string RRD = ">>v";
constexpr std::string LLU = "<<^";
constexpr std::string LLD = "<<v";
constexpr std::string_view HORIZONTAL_SEPARATOR = "-+-+-+-+-+-+-+-+-+";

void Thing::Initialize()
{
    board.Initialize();
}
void Thing::DrawBoard() const
{
    std::cout << " |";
    for(const auto& column: std::views::iota((size_t)0, board.BOARD_WIDTH))
    {
        std::cout << (char)('A'+column) << "|";
    }
    std::cout << std::endl;
    std::cout << HORIZONTAL_SEPARATOR << std::endl;

    char rowchar = '1';
    for(const auto& row: std::views::iota((size_t)0, board.BOARD_HEIGHT))
    {
        std::cout << std::format("{}|", rowchar);
        for(const auto& column: std::views::iota((size_t)0,board.BOARD_WIDTH))
        {
            const auto& token = board.GetCell(column, row).GetToken();
            if(token)
            {
                switch(token->GetTokenType())
                {
                    case TokenType::KNIGHT:
                        std::cout << "N";
                    break;
                    case TokenType::BLOCK:
                        std::cout << "X";
                    break;
                    case TokenType::STICKY_BUNS:
                        std::cout << "@";
                    break;
                    case TokenType::CHESST:
                        std::cout << "c";
                    break;
                    default:
                        std::cout << "?";
                    break;
                }
            }
            else
            {
                std::cout << ".";
            }
            std::cout << "|";
        }
        std::cout << std::endl;
        std::cout << HORIZONTAL_SEPARATOR << std::endl;
        ++rowchar;
    }
}
void Thing::ShowStats() const
{
    std::cout << std::format("Moves: {}", board.GetMoves()) << std::endl;
    std::cout << std::format("Supplies: {}/{}", board.GetSupplies(), board.GetMaximumSupplies()) << std::endl;
    std::cout << std::format("Health: {}/{}", board.GetHealth(), board.GetMaximumHealth()) << std::endl;
    std::cout << std::format("Jools: {}", board.GetJools()) << std::endl;
}
void Thing::Run()
{
    bool done = false;
    while(!done)
    {
        DrawBoard();
        ShowStats();

        std::string line;
        std::getline(std::cin, line);
        std::transform(
            line.begin(), 
            line.end(), 
            line.begin(), 
            [](char ch)
            {
                return std::tolower(ch);
            });

        done = ProcessCommand(line);
    }
}
void Thing::CleanUp()
{
    board.CleanUp();
}
bool Thing::ProcessCommand(const std::string& command)
{
    if(QUIT == command)
    {
        return true;
    }
    else if(command.size()==3 && command[0] == '?' && command[1] >= 'a' && command[1] <= 'h' && command[2] >= '1' && command[2] <= '8')
    {
        Inspect(command[1]-'a', command[2]-'1');
    }
    else if(RESET == command)
    {
        board.Initialize();
    }
    else if(UUR == command)
    {
        board.Move(1,-2);
    }
    else if(UUL == command)
    {
        board.Move(-1,-2);
    }
    else if(DDR == command)
    {
        board.Move(1,2);
    }
    else if(DDL == command)
    {
        board.Move(-1,2);
    }
    else if(RRU == command)
    {
        board.Move(2,-1);
    }
    else if(RRD == command)
    {
        board.Move(2,1);
    }
    else if(LLU == command)
    {
        board.Move(-2,-1);
    }
    else if(LLD == command)
    {
        board.Move(-2,1);
    }
    else
    {
        std::cout << "Dunno what yer on about!" << std::endl;
    }
    return false;
}
void Thing::Inspect(int column, int row) const
{
    auto cell = board.GetCell(column, row);
    auto token = cell.GetToken();
    if(token)
    {
        switch(token->GetTokenType())
        {
            case TokenType::BLOCK:
                std::cout << "Its a blocked square. You cannot move there." << std::endl;
            break;
            case TokenType::STICKY_BUNS:
                std::cout << "It is sticky buns! (how they got sticky? don't ask!) It gives you supplies." << std::endl;
            break;
            case TokenType::KNIGHT:
                std::cout << "It is the knight. That's you." << std::endl;
            break;
            case TokenType::CHESST:
                std::cout << "It is a chesst. With two 's' (see what we did there? #ThanksRoikku)." << std::endl;
            break;
            default:
                std::cout << "I don't know what that is, either." << std::endl;
            break;
        }
    }
    else
    {
        std::cout << "There is nothing there." << std::endl;
    }   
}