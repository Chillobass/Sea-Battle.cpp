#ifndef FIELD_
#define FIELD_

#include <vector>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

namespace seaBattle {

enum Square {
    Empty,
    Ship,
    Hit,
    Killed,
    Miss,
    Bound
};

enum GAME_MODE{
    MASTER,
    SLAVE,
    UNDEFINED
};

struct Game {
    GAME_MODE mode = GAME_MODE::UNDEFINED;
    bool isStarted = false;
    bool orderedStrategy = false;
    bool customStrategy = false;
    bool finished = false;
    bool winner = false;
};

struct Field {
private:
    uint64_t width = 0;
    uint64_t height = 0;
    uint64_t ship1number = 0;
    uint64_t ship2number = 0;
    uint64_t ship3number = 0;
    uint64_t ship4number = 0;
    uint64_t numberOfShipsKilled = 0;
    std::vector<std::vector<Square>> sea;

public:

    uint64_t getWidth() const;

    void setWidth(uint64_t width);

    uint64_t getHeight() const;

    void setHeight(uint64_t height);

    uint64_t getShip1Number() const;

    void setShip1Number(uint64_t ship1Number);

    uint64_t getShip2Number() const;

    void setShip2Number(uint64_t ship2Number);

    uint64_t getShip3Number() const;

    void setShip3Number(uint64_t ship3Number);

    uint64_t getShip4Number() const;

    void setShip4Number(uint64_t ship4Number);

    Field(uint64_t width, uint64_t height)
            : width(width), height(height), sea(height, std::vector<Square>(width, Square::Empty)) {}

    Field() : width(0), height(0) {}

    Square getPosition(int x, int y);

    void setPosition(int x, int y, Square status);

    uint64_t random(uint64_t min, uint64_t max);

    void initField();

    bool checkSpaceForShipsEnough();

    bool tryPlaceAllShips(const std::vector<std::pair<uint64_t, uint64_t>>& ships);

    bool tryPlaceShips(uint64_t count, uint64_t size);

    bool canPlaceShip(uint64_t x, uint64_t y, uint64_t size, char direction);

    bool isAreaClear(uint64_t x, uint64_t y, uint64_t dx, uint64_t dy);

    bool aroundClear(uint64_t x, uint64_t y, uint64_t prev_x, uint64_t prev_y);

    void placeShip(uint64_t x, uint64_t y, uint64_t size, char direction);

    void generateField();

    void printField();

    void markShipAsDestroyed(uint64_t x, uint64_t y);

    bool shipWasKilled(uint64_t x, uint64_t y);

    bool allShipsKilled() const;

    std::pair<int, int> customShot();

    std::pair<int, int> orderedShot();

    bool checkGameOver();

    void oneMoreShipWasKilled();

    bool checkHitShips();

    std::pair<int, int> finishHitShips();

    bool isWithinBounds(int nx, int ny);
};
}

#endif // FIELD_
