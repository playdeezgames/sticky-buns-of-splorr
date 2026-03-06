#include "roomstate.h"
#include <format>
#include "rng.h"
constexpr int BOARD_CELL_WIDTH = 3;
constexpr int BOARD_CELL_HEIGHT = 3;
static unsigned char GetBoardCellCharacter(const Location& location)
{
    auto character = location.GetCharacter();
    if(character)
    {
        switch(character->GetCharacterType())
        {
            case CharacterType::FLOGGER:
                return '!';
            case CharacterType::POTION:
                return '&';
            case CharacterType::BUTTHOLE:
                return '*';
            case CharacterType::BISHOP:
                return 'B';
            case CharacterType::PAWN:
                return 'p';
            case CharacterType::STICKY_BUNS:
                return '@';
            case CharacterType::KNIGHT:
                return 'N';
            case CharacterType::BLOCK:
                return 'X';
            default:
                return 0;
        }
    }
    return 0;
}
static FrameBufferCellColor GetBoardCellForegroundColor(const Location& location)
{
    auto character = location.GetCharacter();
    if(character)
    {
        switch(character->GetCharacterType())
        {
            case CharacterType::FLOGGER:
                return FrameBufferCellColor::DARK_GRAY;
            case CharacterType::POTION:
                return FrameBufferCellColor::LIGHT_RED;
            case CharacterType::STICKY_BUNS:
                return FrameBufferCellColor::WHITE;
            case CharacterType::PAWN:
            case CharacterType::BISHOP:
                return FrameBufferCellColor::WHITE;
            case CharacterType::KNIGHT:
                return FrameBufferCellColor::BLACK;
            case CharacterType::BLOCK:
                return FrameBufferCellColor::BLACK;
            case CharacterType::BUTTHOLE:
                return FrameBufferCellColor::BROWN;
            default:
                return FrameBufferCellColor::BLACK;
        }
    }
    return FrameBufferCellColor::BLACK;
}
static FrameBufferCellColor GetBoardCellBackgroundColor(bool light, bool isValid)
{
    if(isValid)
    {
        if(light)
        {
            return FrameBufferCellColor::LIGHT_CYAN;
        }
        else
        {
            return FrameBufferCellColor::CYAN;
        }
    }
    else
    {
        if(light)
        {
            return FrameBufferCellColor::LIGHT_BLUE;
        }
        else
        {
            return FrameBufferCellColor::BLUE;
        }
    }
}
bool RoomState::IsValidMoveDestination(Location location)
{
    for(auto knightMoveType : AllKnightMoveTypes)
    {
        auto neighbor = location.GetNeighbor(knightMoveType);
        if(neighbor)
        {
            auto character = neighbor->GetCharacter();
            if(character)
            {
                if(character->GetCharacterType() == CharacterType::KNIGHT && character->GetStatistic(StatisticType::HEALTH) > 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
void RoomState::DrawBoard()
{
    auto avatar = *_world.GetAvatar();
    auto board = avatar.GetBoard();
    for(size_t column=0;column<board.GetColumns();++column)
    {
        for(size_t row=0;row<board.GetRows();++row)
        {
            auto location = *board.GetLocation(column, row);
            bool isValidMove = false;
            if(
                !location.GetCharacter() || 
                location.GetCharacter()->GetCharacterType() != CharacterType::BLOCK)
            {
                isValidMove = IsValidMoveDestination(location);
            }
            _frameBuffer.Fill(
                column * BOARD_CELL_WIDTH, 
                row * BOARD_CELL_HEIGHT, 
                BOARD_CELL_WIDTH, 
                BOARD_CELL_HEIGHT, 
                0, 
                FrameBufferCellColor::BLACK, 
                GetBoardCellBackgroundColor(location.GetLight(), isValidMove));
            _frameBuffer.SetCell(column * BOARD_CELL_WIDTH + 1, row * BOARD_CELL_HEIGHT + 1, GetBoardCellCharacter(location), GetBoardCellForegroundColor(location), std::nullopt);
        }
    }
}
void RoomState::DrawCursorFrame()
{
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT, 0xc9, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 1, y * BOARD_CELL_HEIGHT, 0xcd, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 1, y * BOARD_CELL_HEIGHT + 2, 0xcd, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT + 1, 0xba, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT + 1, 0xba, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT, 0xbb, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT + 2, 0xbc, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT + 2, 0xc8, std::nullopt, std::nullopt);
}
void RoomState::DrawStats()
{
    auto avatar = *_world.GetAvatar();
    auto board = avatar.GetBoard();
    size_t text_column = board.GetColumns() * BOARD_CELL_WIDTH;
    _frameBuffer.Fill(
        text_column,
        size_t{0},
        _frameBuffer.GetColumns()-text_column,
        _frameBuffer.GetRows()-1,
        0,
        std::nullopt,FrameBufferCellColor::BLACK);
    size_t text_row = 0;
    auto streak = *avatar.GetStatistic(StatisticType::STREAK);
    if(streak > 0)
    {
        _frameBuffer.WriteText(
            text_column, 
            text_row++, 
            std::format(
                "Streak: {}", 
                streak), 
            FrameBufferCellColor::LIGHT_GREEN, 
            std::nullopt);
    }
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "Buns: {}/{}", 
            *avatar.GetStatistic(StatisticType::SUPPLIES),
            avatar.GetStatisticMaximum(StatisticType::SUPPLIES)), 
        FrameBufferCellColor::MAGENTA, 
        std::nullopt);
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "Armour: {}", 
            *avatar.GetStatistic(StatisticType::ARMOUR)), 
        FrameBufferCellColor::YELLOW, 
        std::nullopt);
    auto floggers = *avatar.GetStatistic(StatisticType::FLOGGERS);
    if(floggers>0)
    {
        _frameBuffer.WriteText(
            text_column, 
            text_row++, 
            std::format(
                "Floggers: {}", 
                floggers), 
            FrameBufferCellColor::DARK_GRAY, 
            std::nullopt);
    }
    auto potions = *avatar.GetStatistic(StatisticType::POTIONS);
    if(potions>0)
    {
        _frameBuffer.WriteText(
            text_column, 
            text_row++, 
            std::format(
                "Potions: {}", 
                potions), 
            FrameBufferCellColor::RED,
            std::nullopt);
    }
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "Health: {}/{}", 
            *avatar.GetStatistic(StatisticType::HEALTH),
            avatar.GetStatisticMaximum(StatisticType::HEALTH)), 
        FrameBufferCellColor::LIGHT_RED, 
        std::nullopt);
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "XP: {}/{}", 
            *avatar.GetStatistic(StatisticType::XP),
            *avatar.GetStatistic(StatisticType::XP_GOAL)), 
        FrameBufferCellColor::CYAN, 
        std::nullopt);
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "Level: {}", 
            *avatar.GetStatistic(StatisticType::XP_LEVEL)), 
        FrameBufferCellColor::LIGHT_CYAN, 
        std::nullopt);
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "Jools: {}", 
            *avatar.GetStatistic(StatisticType::JOOLS)), 
        FrameBufferCellColor::LIGHT_GREEN, 
        std::nullopt);
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "Position: {}{}", 
            (char)('A'+x),
            (char)('1'+y)), 
        FrameBufferCellColor::CYAN, 
        std::nullopt);
    auto location= *board.GetLocation(x,y);
    if(IsValidMoveDestination(location))
    {
        _frameBuffer.WriteText(
            text_column, 
            text_row++, 
            std::format(
                "Valid Move"), 
            FrameBufferCellColor::GREEN, 
            std::nullopt);
    }
    auto character = location.GetCharacter();
    if(character)
    {
        switch(character->GetCharacterType())
        {
            case CharacterType::BLOCK:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "Blocked!"), 
                    FrameBufferCellColor::RED, 
                    std::nullopt);
                break;
            case CharacterType::BUTTHOLE:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "Butthole"), 
                    FrameBufferCellColor::BROWN, 
                    std::nullopt);
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "(check it!)"), 
                    FrameBufferCellColor::BROWN, 
                    std::nullopt);
                break;
            case CharacterType::KNIGHT:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "Knight(you)"), 
                    FrameBufferCellColor::DARK_GRAY, 
                    std::nullopt);
                break;
            case CharacterType::STICKY_BUNS:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "+5 Buns"), 
                    FrameBufferCellColor::WHITE, 
                    std::nullopt);
                break;
            case CharacterType::PAWN:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "Pawn(enemy)"), 
                    FrameBufferCellColor::WHITE, 
                    std::nullopt);
                break;
            default:
                //do nothing
                break;
        }
    }
    for(auto message: _world.GetMessages())
    {
        _frameBuffer.WriteText(
            text_column, 
            text_row++, 
            message.GetText().data(), 
            message.GetForeground(), 
            message.GetBackground());
    }
}
void RoomState::DrawStatusBar()
{
    _frameBuffer.WriteText(
        0, 
        _frameBuffer.GetRows() - 1, 
        " WASD:navigate \xb3 SPACE:move \xb3 TAB:menu  ", 
        FrameBufferCellColor::BLACK, 
        FrameBufferCellColor::WHITE);
}
void RoomState::Draw()
{
    DrawBoard();
    DrawCursorFrame();
    DrawStats();
    DrawStatusBar();
}

std::optional<GameState> RoomState::Update()
{
    GameState gameState = GameState::ROOM;
    while(HandleCommand(gameState) && gameState == GameState::ROOM){/* do nothing */}
    return gameState;
}

bool RoomState::HandleCommand(GameState& gameState)
{
    auto command = _commandBuffer.Read();
    if(command)
    {
        switch(*command)
        {
            case CommandType::UP:
                y = (y > 0) ? (y - 1) : (y);
                _world.ClearMessages();
                break;
            case CommandType::DOWN:
                y = (y < World::BOARD_ROWS - 1) ? (y + 1) : (y);
                _world.ClearMessages();
                break;
            case CommandType::LEFT:
                x = (x > 0) ? (x - 1) : (x);
                _world.ClearMessages();
                break;
            case CommandType::RIGHT:
                x = (x < World::BOARD_COLUMNS - 1) ? (x + 1) : (x);
                _world.ClearMessages();
                break;
            case CommandType::GREEN:
                AttemptMove();
                break;
            case CommandType::YELLOW:
                gameState = GameState::GAME_MENU;
                break;
            default:
                //do nothing
                break;
        }
        return true;
    }
    return false;
}
void RoomState::AttemptMove()
{
    auto avatar = *_world.GetAvatar();
    auto location = avatar.GetLocation();
    auto cursorLocation = *location.GetBoard().GetLocation(x,y);
    for(auto knightMoveType : AllKnightMoveTypes)
    {
        auto destination = location.GetNeighbor(knightMoveType);

        if(destination && destination->GetIndex() == cursorLocation.GetIndex())
        {
            _world.ClearMessages();
            auto otherCharacter = destination->GetCharacter();
            if(avatar.GetStatistic(StatisticType::HEALTH) <= 0)
            {
                _world.AddMessage("Yer Dead!",FrameBufferCellColor::RED,FrameBufferCellColor::BLACK);
            }
            else if(otherCharacter && otherCharacter->GetCharacterType() == CharacterType::BLOCK)
            {
                _world.AddMessage("Blocked!",FrameBufferCellColor::BLACK,FrameBufferCellColor::RED);
            }
            else
            {
                Move(location, cursorLocation);
            }
        }
    }
}
void RoomState::ConsumeStickyBuns(Character& character, Character& otherCharacter)
{
    constexpr int SUPPLIES_INCREASE = 5;
    _world.AddMessage(std::format("+{} Buns", SUPPLIES_INCREASE),FrameBufferCellColor::WHITE, FrameBufferCellColor::BLACK);
    character.SetStatistic(StatisticType::SUPPLIES, *character.GetStatistic(StatisticType::SUPPLIES) + SUPPLIES_INCREASE);
    auto board = character.GetBoard();
    _world.SpawnCharacter(board, CharacterType::STICKY_BUNS);
    otherCharacter.Recycle();
}
enum class ButtholeCheckResult 
{
    NOTHING, 
    JOOLS, 
    TRAP, 
    TELEPORT,
    ARMOUR,
    FLOGGER,
    POTION
};
static std::map<ButtholeCheckResult, size_t> buttholeResultGenerator =
{
    {ButtholeCheckResult::NOTHING , size_t{ 10}},
    {ButtholeCheckResult::JOOLS   , size_t{ 40}},
    {ButtholeCheckResult::TRAP    , size_t{ 20}},
    {ButtholeCheckResult::TELEPORT, size_t{ 10}},
    {ButtholeCheckResult::ARMOUR  , size_t{ 40}},
    {ButtholeCheckResult::FLOGGER , size_t{  5}},
    {ButtholeCheckResult::POTION  , size_t{ 10}}
};
void RoomState::CheckButthole(Character& character, Character& otherCharacter)
{
    auto board = character.GetBoard();
    _world.AddMessage("Checking...",FrameBufferCellColor::BROWN, FrameBufferCellColor::BLACK);
    _world.SpawnCharacter(board, CharacterType::BUTTHOLE);
    switch(RNG::FromGenerator(buttholeResultGenerator))
    {
    case ButtholeCheckResult::NOTHING:    
        _world.AddMessage("Nothing!",FrameBufferCellColor::DARK_GRAY, FrameBufferCellColor::BLACK);
        break;
    case ButtholeCheckResult::JOOLS:   
        TriggerJools();     
        break;
    case ButtholeCheckResult::TRAP:  
        TriggerTrap();      
        break;
    case ButtholeCheckResult::TELEPORT:
        TriggerTeleport();
        break;
    case ButtholeCheckResult::ARMOUR:
        TriggerArmour();
        break;
    case ButtholeCheckResult::FLOGGER:
        TriggerFlogger();
        break;
    case ButtholeCheckResult::POTION:
        TriggerPotion();
        break;
    }
    otherCharacter.Recycle();
}
void RoomState::TriggerFlogger()
{
    constexpr int FLOGGER_BONUS = 1;
    _world.AddMessage(
        std::format("+{} Floggers", FLOGGER_BONUS),
        FrameBufferCellColor::DARK_GRAY, 
        FrameBufferCellColor::BLACK);
    _world.GetAvatar()->ChangeStatistic(
        StatisticType::FLOGGERS, 
        FLOGGER_BONUS);
}
void RoomState::TriggerPotion()
{
    constexpr int POTION_BONUS = 1;
    _world.AddMessage(
        std::format("+{} Potion", POTION_BONUS),
        FrameBufferCellColor::RED, 
        FrameBufferCellColor::BLACK);
    _world.GetAvatar()->ChangeStatistic(
        StatisticType::POTIONS, 
        POTION_BONUS);
}
void RoomState::TriggerArmour()
{
    constexpr int ARMOUR_BONUS = 1;
    _world.AddMessage(
        std::format("+{} Armour", ARMOUR_BONUS),
        FrameBufferCellColor::YELLOW, 
        FrameBufferCellColor::BLACK);
    _world.GetAvatar()->ChangeStatistic(
        StatisticType::ARMOUR, 
        ARMOUR_BONUS);
}
void RoomState::TriggerTrap()
{
    constexpr int TRAP_DAMAGE = 1;
    _world.AddMessage("Trap!",FrameBufferCellColor::BLACK, FrameBufferCellColor::RED);
    _world.AddMessage(std::format("-{} Health", TRAP_DAMAGE),FrameBufferCellColor::BLACK, FrameBufferCellColor::RED);
    _world.GetAvatar()->ChangeStatistic(StatisticType::HEALTH, -TRAP_DAMAGE);
}
static std::map<int, size_t> joolsGenerator=
{
    {5,size_t{1}},
    {6,size_t{2}},
    {7,size_t{3}},
    {8,size_t{2}},
    {9,size_t{1}}
};
void RoomState::TriggerJools()
{
    auto jools = RNG::FromGenerator(joolsGenerator);
    _world.AddMessage(std::format("+{} Jools", jools),FrameBufferCellColor::LIGHT_GREEN, FrameBufferCellColor::BLACK);
    _world.GetAvatar()->ChangeStatistic(StatisticType::JOOLS, jools);
}
void RoomState::TriggerTeleport()
{
    _world.AddMessage("Teleport!",FrameBufferCellColor::LIGHT_BLUE, FrameBufferCellColor::BLACK);
    auto avatar = _world.GetAvatar();
    auto board = avatar->GetBoard();
    size_t column;
    size_t row;
    do
    {
        column = RNG::FromRange(size_t{0}, board.GetColumns() - 1);
        row = RNG::FromRange(size_t{0}, board.GetRows() - 1);
    } while (board.GetLocation(column, row)->GetCharacter());
    auto location = board.GetLocation(column, row);
    avatar->GetLocation().SetCharacter(std::nullopt);
    location->SetCharacter(avatar);
    avatar->SetLocation(*location);
}
void RoomState::Move(Location location, Location cursorLocation)
{
    RemoveBlocks();
    auto character = *location.GetCharacter();
    auto supplies = *character.GetStatistic(StatisticType::SUPPLIES);
    if(supplies>0)
    {
        character.SetStatistic(StatisticType::SUPPLIES, supplies - 1);
    }
    else
    {
        character.SetStatistic(StatisticType::HEALTH, *character.GetStatistic(StatisticType::HEALTH) - 1);
    }
    _world.CreateCharacter(CharacterType::BLOCK, location);
    auto otherCharacter = cursorLocation.GetCharacter();
    character.SetLocation(cursorLocation);
    cursorLocation.SetCharacter(character);
    if(otherCharacter)
    {
        character.ChangeStatistic(StatisticType::STREAK, 1);
        switch(otherCharacter->GetCharacterType())
        {
            case CharacterType::STICKY_BUNS:
                ConsumeStickyBuns(character, *otherCharacter);
                break;
            case CharacterType::BUTTHOLE:
                CheckButthole(character, *otherCharacter);
                break;
            case CharacterType::PAWN:
                AttackPawn();
                break;
            case CharacterType::BISHOP:
                AttackBishop();
                break;
            default:
                //do nothing!
                break;
        }
    }
    else
    {
        auto streak = *character.GetStatistic(StatisticType::STREAK);
        if(streak > 1)
        {
            _world.AddMessage("Streak Bonus!", FrameBufferCellColor::CYAN, FrameBufferCellColor::BLACK);
            AddXP(streak - 1);
        }
        character.SetStatistic(StatisticType::STREAK, 0);
    }
}
void RoomState::DamageAvatar(int damage)
{
    auto character = *_world.GetAvatar();
    auto armour = character.GetStatistic(StatisticType::ARMOUR).value_or(0);
    if(damage > 0 && armour > 0)
    {
        auto absorption = std::min(damage, armour);
        character.ChangeStatistic(StatisticType::ARMOUR, -absorption);
        _world.AddMessage(std::format("-{} Armour", absorption), FrameBufferCellColor::BLACK, FrameBufferCellColor::YELLOW);
        damage -= absorption;
    }
    if(damage > 0)
    {
        character.ChangeStatistic(StatisticType::HEALTH, -damage);
        _world.AddMessage(std::format("-{} Health", damage), FrameBufferCellColor::BLACK, FrameBufferCellColor::RED);
    }
}
void RoomState::AttackPawn()
{
    DamageAvatar(1);
    AddXP(1);
    auto board = _world.GetAvatar()->GetBoard();
    if(
        !board.HasCharacterType(CharacterType::PAWN) &&
        !board.HasCharacterType(CharacterType::BISHOP))
    {
        _world.SpawnCharacter(board, CharacterType::BISHOP);
    }
}
void RoomState::AttackBishop()
{
    auto avatar = *_world.GetAvatar();
    if(avatar.GetStatistic(StatisticType::FLOGGERS).value_or(0) > 0)
    {
        avatar.ChangeStatistic(StatisticType::FLOGGERS,-1);
        _world.AddMessage("BISHOP FLOGGED!", FrameBufferCellColor::BLACK, FrameBufferCellColor::DARK_GRAY);
        _world.AddMessage("-1 Flogger", FrameBufferCellColor::DARK_GRAY, FrameBufferCellColor::BLACK);
        AddXP(5);
    }
    else
    {
        DamageAvatar(3);
        AddXP(3);
    }
    auto board = avatar.GetBoard();
    if(
        !board.HasCharacterType(CharacterType::PAWN) &&
        !board.HasCharacterType(CharacterType::BISHOP))
    {
        _world.SpawnCharacter(board, CharacterType::PAWN);
        _world.SpawnCharacter(board, CharacterType::PAWN);
        _world.SpawnCharacter(board, CharacterType::PAWN);
        _world.SpawnCharacter(board, CharacterType::PAWN);
        _world.SpawnCharacter(board, CharacterType::PAWN);
        _world.SpawnCharacter(board, CharacterType::PAWN);
        _world.SpawnCharacter(board, CharacterType::PAWN);
        _world.SpawnCharacter(board, CharacterType::PAWN);
    }
}
void RoomState::AddXP(int xp)
{
    auto avatar = *_world.GetAvatar();
    avatar.ChangeStatistic(StatisticType::XP, xp);
    _world.AddMessage(std::format("+{} XP", xp), FrameBufferCellColor::CYAN, FrameBufferCellColor::BLACK);
    while(*avatar.GetStatistic(StatisticType::XP_GOAL) <= *avatar.GetStatistic(StatisticType::XP))
    {
        _world.AddMessage("+1 XP Level", FrameBufferCellColor::CYAN, FrameBufferCellColor::BLACK);
        _world.AddMessage("+1 Max HP", FrameBufferCellColor::LIGHT_RED, FrameBufferCellColor::BLACK);
        _world.AddMessage("+1 HP", FrameBufferCellColor::RED, FrameBufferCellColor::BLACK);
        auto xpGoal = *avatar.GetStatistic(StatisticType::XP_GOAL);
        avatar.ChangeStatistic(StatisticType::XP_LEVEL, 1);
        avatar.ChangeStatistic(StatisticType::XP, -xpGoal);
        avatar.ChangeStatistic(StatisticType::XP_GOAL, xpGoal);
        avatar.SetStatisticMaximum(StatisticType::HEALTH, avatar.GetStatisticMaximum(StatisticType::HEALTH) + 1);
        avatar.ChangeStatistic(StatisticType::HEALTH, 1);
    }
}
void RoomState::RemoveBlocks()
{
    auto board = _world.GetAvatar()->GetLocation().GetBoard();
    for(size_t column = 0; column < board.GetColumns(); ++column)
    {
        for(size_t row = 0; row < board.GetRows(); ++row)
        {
            auto location = *board.GetLocation(column, row);
            auto character = location.GetCharacter();
            if(character && character->GetCharacterType() == CharacterType::BLOCK)
            {
                location.SetCharacter(std::nullopt);
                character->Recycle();
            }
        }
    }
}
