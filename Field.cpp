#include "Field.h"


namespace seaBattle {

uint64_t Field::getWidth() const {
    return width;
}

void Field::setWidth(uint64_t width) {
    Field::width = width;
}

uint64_t Field::getHeight() const {
    return height;
}

void Field::setHeight(uint64_t height) {
    Field::height = height;
}

uint64_t Field::getShip1Number() const {
    return ship1number;
}

void Field::setShip1Number(uint64_t ship1Number) {
    ship1number = ship1Number;
}

uint64_t Field::getShip2Number() const {
    return ship2number;
}

void Field::setShip2Number(uint64_t ship2Number) {
    ship2number = ship2Number;
}

uint64_t Field::getShip3Number() const {
    return ship3number;
}

void Field::setShip3Number(uint64_t ship3Number) {
    ship3number = ship3Number;
}

uint64_t Field::getShip4Number() const {
    return ship4number;
}

void Field::setShip4Number(uint64_t ship4Number) {
    ship4number = ship4Number;
}

void Field::initField() {
    sea.assign(height, std::vector<Square>(width, Square::Empty));
}

bool Field::checkGameOver(){
    return numberOfShipsKilled == (ship1number + ship2number + ship3number + ship4number);
}

void Field::oneMoreShipWasKilled(){
    numberOfShipsKilled++;
}


void Field::generateField() {
    height = random(10, 20);
    width = random(10, 20);
    initField();
    while(true) {
        ship1number = random(1, 5);
        ship2number = random(1, 4);
        ship3number = random(1, 3);
        ship4number = random(1, 2);
        if(checkSpaceForShipsEnough()) break;
    }
}

Square Field::getPosition(int x, int y) {
    return sea[y][x];
}

void Field::setPosition(int x, int y, Square status) {
    sea[y][x] = status;
}

uint64_t Field::random(uint64_t min, uint64_t max) {
    return min + std::rand() % ((max - min) + 1);
}

bool Field::checkSpaceForShipsEnough() {
    return tryPlaceAllShips({{ship4number, 4}, {ship3number, 3}, {ship2number, 2}, {ship1number, 1}});
}

bool Field::tryPlaceAllShips(const std::vector<std::pair<uint64_t, uint64_t>>& ships) {
    for (const auto& [count, size] : ships) {
        if (!tryPlaceShips(count, size)) {
            return false;
        }
    }
    return true;
}

bool Field::tryPlaceShips(uint64_t count, uint64_t size) {
    uint64_t placed = 0;

    for (uint64_t y = 0; y < height && placed < count; y++) {
        for (uint64_t x = 0; x < width && placed < count; x++) {
            if(sea[y][x] != Square::Empty){
                continue;
            }
            if (canPlaceShip(x, y, size, 'h')) {
                placeShip(x, y, size, 'h');
                placed++;
            } else if (canPlaceShip(x, y, size, 'v')) {
                placeShip(x, y, size, 'v');
                placed++;
            }
        }
    }

    return placed == count;
}

bool Field::canPlaceShip(uint64_t x, uint64_t y, uint64_t size, char direction) {
    if (direction == 'h') {
        if (x + size > width) return false;
        return isAreaClear(x, y, size, 0);
    } else if (direction == 'v') {
        if (y + size > height) return false;
        return isAreaClear(x, y, 0, size);
    }
    return false;
}

bool Field::isAreaClear(uint64_t x, uint64_t y, uint64_t dx, uint64_t dy) {
    uint64_t nx = -1;
    uint64_t ny = -1;
    for (uint64_t i = 0; i < std::max(dx, dy); i++) {
        uint64_t prev_x = nx;
        uint64_t prev_y = ny;

        uint64_t nx = x + (dx > 0 ? i : 0);
        uint64_t ny = y + (dy > 0 ? i : 0);

        if (!aroundClear(nx, ny, prev_x, prev_y) || sea[ny][nx] != Square::Empty) {
            return false;
        }
    }
    return true;
}

bool Field::aroundClear(uint64_t x, uint64_t y, uint64_t prev_x, uint64_t prev_y) {
    for (int64_t dy = -1; dy <= 1; dy++) {
        for (int64_t dx = -1; dx <= 1; dx++) {
            int64_t nx = static_cast<int64_t>(x) + dx;
            int64_t ny = static_cast<int64_t>(y) + dy;
            if((nx == prev_x && ny == prev_y) || (dx == 0 && dy == 0)) continue;

            if (nx >= 0 && nx < static_cast<int64_t>(width) && ny >= 0 && ny < static_cast<int64_t>(height)) {
                if (sea[ny][nx] != Square::Empty) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Field::placeShip(uint64_t x, uint64_t y, uint64_t size, char direction) {
    if (direction == 'h') {
        for (uint64_t i = 0; i < size; i++) {
            sea[y][x + i] = Square::Ship;
        }
    } else if (direction == 'v') {
        for (uint64_t i = 0; i < size; i++) {
            sea[y + i][x] = Square::Ship;
        }
    }
}


void Field::printField() {
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (sea[y][x] == Square::Empty) {
                std::cout << '.';
            } else if (sea[y][x] == Square::Ship) {
                std::cout << 'E';
            } else if (sea[y][x] == Square::Hit) {
                std::cout << 'H';
            } else if (sea[y][x] == Square::Miss) {
                std::cout << 'o';
            } else if (sea[y][x] == Square::Killed) {
                std::cout << 'K';
            }
//            else if (sea[y][x] == Square::Bound) {
//                std::cout << 'B';
//            }
        }
        std::cout << '\n';
    }
}

bool Field::isWithinBounds(int nx, int ny) {
    return nx >= 0 && nx < width && ny >= 0 && ny < height;
};

void Field::markShipAsDestroyed(uint64_t x, uint64_t y) {
    bool isHorizontal = false;
    if (isWithinBounds(x + 1, y) && sea[y][x + 1] == Square::Hit) {
        isHorizontal = true;
    } else if (isWithinBounds(x - 1, y) && sea[y][x - 1] == Square::Hit) {
        isHorizontal = true;
    }

    int nx = x, ny = y;
    int dx = isHorizontal ? -1 : 0;
    int dy = isHorizontal ? 0 : -1;
    while (isWithinBounds(nx, ny) && sea[ny][nx] == Square::Hit) {
        sea[ny][nx] = Square::Killed;
        nx += dx;
        ny += dy;
    }

    nx = x - dx, ny = y - dy;
    while (isWithinBounds(nx, ny) && sea[ny][nx] == Square::Hit) {
        sea[ny][nx] = Square::Killed;
        nx -= dx;
        ny -= dy;
    }
}

bool Field::shipWasKilled(uint64_t x, uint64_t y) {
    auto checkDirection = [this](int startX, int startY, int dx, int dy) {
        int nx = startX + dx, ny = startY + dy;
        while (isWithinBounds(nx, ny)) {
            if (sea[ny][nx] == Square::Empty) {
                break;
            }
            if (sea[ny][nx] == Square::Ship) {
                return true;
            }
            nx += dx;
            ny += dy;
        }
        return false;
    };

    if (checkDirection(x, y, -1, 0) || checkDirection(x, y, 1, 0)) {
        return false;
    }

    if (checkDirection(x, y, 0, -1) || checkDirection(x, y, 0, 1)) {
        return false;
    }

    markShipAsDestroyed(x, y);
    return true;
}

bool Field::allShipsKilled() const {
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (sea[y][x] == Square::Ship) {
                return false;
            }
        }
    }
    return true;
}

bool Field::checkHitShips(){
    bool flag = false;
    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            if (sea[y][x] == Square::Hit) {
                flag = true;
                break;
            }
        }
        if(flag) break;
    }
    return flag;
}


std::pair<int, int> Field::finishHitShips() {
    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            if (sea[y][x] == Square::Hit) {
                char direction = 'u';
                if (isWithinBounds(x + 1, y) && sea[y][x + 1] == Square::Hit) {
                    direction = 'h';
                } else if (isWithinBounds(x - 1, y) && sea[y][x - 1] == Square::Hit) {
                    direction = 'h';
                }

                if (isWithinBounds(x, y + 1) && sea[y + 1][x] == Square::Hit) {
                    direction = 'v';
                } else if (isWithinBounds(x, y - 1) && sea[y - 1][x] == Square::Hit) {
                    direction = 'v';
                }

                if (direction == 'h') {
                    if (isWithinBounds(x + 1, y) && sea[y][x + 1] == Square::Empty) {
                        return {x + 1, y};
                    } else if (isWithinBounds(x - 1, y) && sea[y][x - 1] == Square::Empty) {
                        return {x - 1, y};
                    }
                } else if (direction == 'v') {
                    if (isWithinBounds(x, y + 1) && sea[y + 1][x] == Square::Empty) {
                        return {x, y + 1};
                    } else if (isWithinBounds(x, y - 1) && sea[y - 1][x] == Square::Empty) {
                        return {x, y - 1};
                    }
                } else {
                	if (isWithinBounds(x + 1, y) && sea[y][x + 1] == Square::Empty) {
                    	return {x + 1, y};
                	} else if (isWithinBounds(x - 1, y) && sea[y][x - 1] == Square::Empty) {
                    	return {x - 1, y};
                	} else if (isWithinBounds(x, y + 1) && sea[y + 1][x] == Square::Empty) {
                    	return {x, y + 1};
                	} else if (isWithinBounds(x, y - 1) && sea[y - 1][x] == Square::Empty) {
                    	return {x, y - 1};
                	}
                }
            }
        }
    }
    return {0, 0};
}



std::pair<int, int> Field::customShot() {
    if(checkHitShips()){
        return finishHitShips();
    }

    std::vector<std::pair<uint64_t, uint64_t>> validCells;
    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            if (sea[y][x] == Square::Empty) {
                bool nearKilled = false;
                for (int64_t dy = -1; dy <= 1; dy++) {
                    for (int64_t dx = -1; dx <= 1; dx++) {
                        int64_t nx = static_cast<int64_t>(x) + dx;
                        int64_t ny = static_cast<int64_t>(y) + dy;
                        if (isWithinBounds(nx, ny) && sea[ny][nx] == Square::Killed) {
                            nearKilled = true;
                            break;
                        }
                    }
                    if (nearKilled) break;
                }
                if (!nearKilled) {
                    validCells.emplace_back(x, y);
                }
            }
        }
    }

    if (!validCells.empty()) {
        auto [shotX, shotY] = validCells[std::rand() % validCells.size()];
        return {shotX, shotY};
    }
    std::cout << "There is nowhere to shot, the game is probably over\n";
    return {0, 0};
}

std::pair<int, int> Field::orderedShot() {
    static int x = 0;
    static int y = 0;

    if(checkHitShips()){
        return finishHitShips();
    }

    while (y < height) {
        if (sea[y][x] == Square::Empty) {
            std::pair<int, int> result = {x, y};
            if (++x == width) {
                x = 0;
                y++;
            }
            return result;
        }
        if (++x == width) {
            x = 0;
            y++;
        }
    }
    std::cout << "There is nowhere to shot, the game is probably over\n";
    return {0, 0};
}
}
