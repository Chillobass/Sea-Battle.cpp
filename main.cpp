#include <string>
#include <iostream>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Field.h"

namespace seaBattle {


std::vector<std::string> nextCommand() {
    std::string inputLine;
    std::getline(std::cin, inputLine);
    std::istringstream stream(inputLine);
    std::vector<std::string> words;
    std::string word;
    while (stream >> word) {
        words.push_back(word);
    }
    return words;
}

void PrintOk() {
    std::cout << "ok\n";
}

bool checkCreateCommands(Game &game, const std::vector<std::string> &input) {
    if (input[0] == "create") {
        if (game.mode != GAME_MODE::UNDEFINED) {
            std::cout << "Game mode was already defined\n";
        } else if (input.size() == 2) {
            if (input[1] == "master") {
                game.mode = GAME_MODE::SLAVE;
                PrintOk();
            } else if (input[1] == "slave") {
                game.mode = GAME_MODE::MASTER;
                PrintOk();
            } else {
                std::cout << "unknown mode, try again\n";
            }
        } else {
            std::cout << "unknown command, try again\n";
        }
        return true;
    }
    return false;
}

bool checkGetCommands(Field &field, const std::vector<std::string> &input) {
    if (input[0] != "get") {
        return false;
    }

    if(input.size() == 2){
        if (input[1] == "width") {
            std::cout << field.getWidth() << "\n";
        } else if (input[1] == "height") {
            std::cout << field.getHeight() << "\n";
        } else {
            std::cout << "unknown command, try again\n";
        }
        return true;
    }

    if(input.size() == 3 && input[1] == "count"){
        if (input[2] == "1") {
            std::cout << field.getShip1Number() << "\n";
        } else if (input[2] == "2") {
            std::cout << field.getShip2Number() << "\n";
        } else if (input[2] == "3") {
            std::cout << field.getShip3Number() << "\n";
        } else if (input[2] == "4") {
            std::cout << field.getShip4Number() << "\n";
        } else {
            std::cout << "unknown command, try again\n";
        }
        return true;
    }
    return false;
}

bool checkSetCommands(Game &game, Field &field, const std::vector<std::string> &input) {
    if (input[0] != "set" || input.size() < 3 || input[1] == "result") {
        return false;
    }

    if(input[1] == "strategy"){
        if (input[2] == "ordered") {
            game.orderedStrategy = true;
            PrintOk();
        } else if (input[2] == "custom") {
            game.customStrategy = true;
            PrintOk();
        } else {
            std::cout << "Unknown strategy, try again !\n";
        }
        return true;
    }

    if(input[1] == "width") {
        if (game.mode == GAME_MODE::SLAVE) {
            std::cout << "Sorry, you are not Master (your bot is)\n";
        } else if (std::all_of(input[2].begin(), input[2].end(), ::isdigit)) {
            field.setWidth(std::stoll(input[2]));
            PrintOk();
        } else {
            std::cout << "failed\n";
        }
        return true;
    }

    if (input[1] == "height"){
        if (game.mode == GAME_MODE::SLAVE) {
            std::cout << "Sorry, you are not Master (your bot is)\n";
        } else if (std::all_of(input[2].begin(), input[2].end(), ::isdigit)) {
            field.setHeight(std::stoll(input[2]));
            PrintOk();
        } else {
            std::cout << "failed\n";
        }
        return true;
    }

    if(input[1] == "count" && input.size() == 4){
        if (input[2] == "1") {
            if (game.mode == GAME_MODE::SLAVE) {
                std::cout << "Sorry, you are not Master (your bot is)\n";
            } else if (std::all_of(input[3].begin(), input[3].end(), ::isdigit)) {
                field.setShip1Number(std::stoll(input[3]));
                PrintOk();
            } else {
                std::cout << "failed\n";
            }
        } else if (input[2] == "2") {
            if (game.mode == GAME_MODE::SLAVE) {
                std::cout << "Sorry, you are not Master (your bot is)\n";
            } else if (std::all_of(input[3].begin(), input[3].end(), ::isdigit)) {
                field.setShip2Number(std::stoll(input[3]));
                PrintOk();
            } else {
                std::cout << "failed\n";
            }
        } else if (input[2] == "3") {
            if (game.mode == GAME_MODE::SLAVE) {
                std::cout << "Sorry, you are not Master (your bot is)\n";
            } else if (std::all_of(input[3].begin(), input[3].end(), ::isdigit)) {
                field.setShip3Number(std::stoll(input[3]));
                PrintOk();
            } else {
                std::cout << "failed\n";
            }
        } else if (input[2] == "4") {
            if (game.mode == GAME_MODE::SLAVE) {
                std::cout << "Sorry, you are not Master (your bot is)\n";
            } else if (std::all_of(input[3].begin(), input[3].end(), ::isdigit)) {
                field.setShip4Number(std::stoll(input[3]));
                PrintOk();
            } else {
                std::cout << "failed\n";
            }
        } else {
            std::cout << "Unknown ship size, try again\n";
        }
        return true;
    }

    return false;
}

bool processFieldIO(Field &field, const std::vector<std::string> &commands) {
    if (commands[0] == "load") {
        std::ifstream fileInput(commands[1]);
        std::string fileLine;
        std::getline(fileInput, fileLine);
        std::istringstream headerStream(fileLine);
        int height, width;
        headerStream >> width >> height;
        field.setHeight(height);
        field.setWidth(width);
        field.initField();

        uint64_t shipSize;
        uint64_t coordX;
        uint64_t coordY;

        char shipDirection;
        while (std::getline(fileInput, fileLine)) {
            std::istringstream shipStream(fileLine);
            shipStream >> shipSize >> shipDirection >> coordX >> coordY;
            field.placeShip(coordX, coordY, shipSize, shipDirection);
        }
        field.printField();
    } else if (commands[0] == "dump") {
        std::ofstream fileOutput(commands[1]);
        fileOutput << field.getWidth() << " " << field.getHeight() << "\n";

        for (uint64_t row = 0; row < field.getHeight(); row++) {
            for (uint64_t col = 0; col < field.getWidth(); col++) {
                if (field.getPosition(col, row) == Square::Ship) {
                    uint64_t length = 1;
                    char orientation = 'h';

                    while (col + length < field.getWidth() && field.getPosition(col + length, row) == Square::Ship) {
                        orientation = 'h';
                        length++;
                    }

                    if (row + 1 < field.getHeight() && field.getPosition(col, row + 1) == Square::Ship) {
                        orientation = 'v';
                        length = 1;
                        while (row + length < field.getHeight() && field.getPosition(col, row + length) == Square::Ship) {
                            length++;
                        }
                    }

                    if (orientation == 'h') {
                        if(length == 1 && row != 0 && (field.getPosition(col, row - 1) == Square::Ship)){
                            continue;
                        }
                        fileOutput << length << " " << orientation << " " << col << " " << row << "\n";
                        col += length - 1;
                    } else {
                        if (row - 1 >= 0 && (field.getPosition(col, row - 1) == Square::Ship)) {
                            continue;
                        } else {
                            fileOutput << length << " " << orientation << " " << col << " " << row << "\n";
                        }
                    }
                }
            }
        }
        PrintOk();
    } else {
        return false;
    }
    return true;
}


int GameLoop() {
    Game game;
    Field myField;
    Field playersField;

    while (true) {
        auto input = nextCommand();
        if(input.size() == 0){
            std::cout << "Empty input, try again !\n";
            continue;
        }

        if (input[0] == "exit") {
            PrintOk();
            return 0;
        }

        if (input[0] == "ping") {
            std::cout << "pong\n";
            continue;
        }

        if (checkGetCommands(myField, input)) {
            continue;
        }

        if(!game.isStarted) {
            if (checkCreateCommands(game, input)) {
                continue;
            }

            if (checkSetCommands(game, myField, input)) {
                continue;
            }

            if (processFieldIO(myField, input)) {
                continue;
            }
        }

        if (input[0] == "start") {
            if(game.mode == GAME_MODE::UNDEFINED) {
                std::cout << "game mode is not defined\n";
                continue;
            } else if(game.mode == GAME_MODE::MASTER){
                if(myField.getHeight() * myField.getWidth() == 0 || myField.getShip1Number() * myField.getShip2Number() * myField.getShip3Number() * myField.getShip4Number() == 0){
                    std::cout << "field is not ready for the game\n";
                    continue;
                }
                myField.initField();

                playersField.setWidth(myField.getWidth());
                playersField.setHeight(myField.getHeight());
                playersField.initField();

                if(!myField.checkSpaceForShipsEnough()){
                    std::cout << "You should change amount of ships, not enough space\n";
                    continue;
                }
            } else {
                myField.generateField();

                playersField.setWidth(myField.getWidth());
                playersField.setHeight(myField.getHeight());
                playersField.initField();
            }

            if(!game.customStrategy && !game.orderedStrategy){
                game.orderedStrategy = true;
            }

//            myField.addAllShipsRandomly();
            game.isStarted = true;
            PrintOk();
            continue;
        }

        if (input[0] == "stop") {
            game.isStarted = false;
            PrintOk();
            continue;
        }

        if(game.isStarted) {
            if (input[0] == "shot" && input.size() == 3 &&
                    std::all_of(input[1].begin(), input[1].end(), ::isdigit) &&
                    std::all_of(input[2].begin(), input[2].end(), ::isdigit)) {
                int x = std::stoi(input[1]);
                int y = std::stoi(input[2]);
                x--, y--;
                if(myField.getPosition(x, y) == Square::Killed || myField.getPosition(x, y) == Square::Hit){
                    std::cout << "You already shot this square, shot again !\n";
                } else if (myField.getPosition(x, y) == Square::Empty) {
                    std::cout << "You missed, my turn !\n";
                } else {
                    myField.setPosition(x, y, Square::Hit);

                    if(myField.shipWasKilled(x, y)){
                        std::cout << "Killed !\n";
                        if(myField.allShipsKilled()){
                            std::cout << "I have no more ships left, you won !\n";
                            game.finished = true;
                            game.isStarted = false;
                            game.winner = false;
                        } else {
                            std::cout << "I have more ships left, fire again !\n";
                        }
                    } else {
                        std::cout << "Hit !\n";
                        std::cout << "Fire again !\n";
                    }
                }
                continue;
            }

            if (input[0] == "shot" && input.size() == 1) {
                std::pair<int, int> shotResult;
                if (game.orderedStrategy) {
                    shotResult = playersField.orderedShot();
                }

                if (game.customStrategy) {
                    shotResult = playersField.customShot();
                }

                std::cout << shotResult.first + 1 << " " << shotResult.second + 1 << "\n";
                std::vector<std::string> command = nextCommand();
                while (command.size() != 3 || command[0] != "set" || command[1] != "result") {
                    std::cout << "I am waiting for correct result after my shot !\n";
                    command = nextCommand();
                }

                if(command[2] == "miss") {
                    playersField.setPosition(shotResult.first, shotResult.second, Square::Miss);
                    std::cout << "Okay, i missed, now is your turn !\n";
                } else if(command[2] == "hit") {
                    playersField.setPosition(shotResult.first, shotResult.second, Square::Hit);
                    std::cout << "Got you !\n";
                    std::cout << "I fire again !\n";
                } else if(command[2] == "kill") {
                    playersField.setPosition(shotResult.first, shotResult.second, Square::Hit);
                    playersField.markShipAsDestroyed(shotResult.first, shotResult.second);
                    std::cout << "Killed your ship !\n";

                    playersField.oneMoreShipWasKilled();
                    if(playersField.checkGameOver()){
                        std::cout << "I think, you have no more ships left, I won !\n";
                        game.finished = true;
                        game.isStarted = false;
                        game.winner = true;
                    } else {
                        std::cout << "I fire again !\n";
                    }
                } else {
                    std::cout << "Unknown result, try again !\n";
                }
                continue;
            }
        }

        if (input[0] == "finished") {
            if (game.finished) {
                std::cout << "yes\n";
            } else {
                std::cout << "no\n";
            }
        } else if (input[0] == "win") {
            if (game.finished && !game.winner) {
                std::cout << "yes, you won !\n";
            } else {
                std::cout << "no\n";
            }
        } else if (input[0] == "lose") {
            if (game.finished && game.winner) {
                std::cout << "yes, you lost\n";
            } else {
                std::cout << "no\n";
            }
        } else if (input[0] == "printBotField") {
            myField.printField();
        } else if (input[0] == "printUsersField") {
            playersField.printField();
        } else {
            std::cout << "Unknown command, try again !\n";
        }
    }

    return 0;
}
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    return seaBattle::GameLoop();
}
