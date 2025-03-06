# Sea-Battle.cpp
Console Sea Battle for 1 player (playing with bot)
supported commands:

| command                      | response       | Description |
| -------                      | -----          | --------                                    |
| ping                         |  pong          |   тестовая команда / test command           |
| exit                         |  ok            |   программа завершается / programm ends          |
| create [master,slave]        |  ok            |   создать игру в режиме master или slave соответственно       |
| start                        |  ok            |   старт игры / game start flag   |
| stop                         |  ok            |   остановка текущей партии / stop this gаme      |
| set width  N                 |  ok/failed     |   установить ширину поля (N положительное, влезает в uint64_t) / set field width (N is positive, fits into uint64_t)    |
| get width                    |  N             |   получить длину поля  (N положительное, влезает в uint64_t) / get the length of the field (N is positive, fits into uint64_t)    |
| set height N                 |  ok/failed     |   установить высоту поля (N положительное, влезает в uint64_t) / set field height (N is positive, fits into uint64_t)    |
| get height                   |  N             |   получить высоту поля  (N положительное, влезает в uint64_t) / get field height (N is positive, fits in uint64_t) |
| set count [1,2,3,4]  N       |  ok/failed     |   установить количество кораблей определенного типа (N положительное, влезает в uint64_t) / set the number of ships of a certain type (N is positive, fits into uint64_t)     |
| get count [1,2,3,4]          |  N             |   получить количество кораблей определенного типа (N положительное, влезает в uint64_t) / get the number of ships of a certain type (N is positive, fits into uint64_t)      |
| set strategy [ordered,custom]|  ok            |   выбрать стратегию для игры / choose a strategy for the game (ordered/custom)     |
| shot X Y                     |  miss/hit/kill |   выстрел по вашим короаблям в координатах (X,Y) (X,Y положительные, влезают в uint64_t) / shoot at your ships at coordinates (X,Y) (X,Y are positive, fit into uint64_t)  | 
| shot                         |  X Y           |   вернуть координаты вашего следующего выстрела, в ответе два числа через пробел  (X,Y положительные, влезают в uint64_t) / return the coordinates of your next shot, the answer is two numbers separated by a space (X,Y are positive, fit into uint64_t)      |
| set result [miss,hit,kill]   |  ok            |   установить результат последнего выстрела программы / set the result of the last shot of the program    |
| finished                     |  yes/no        |   окончена ли текущая партия / Is the current game over    |
| win                          |  yes/no        |   являетесь ли вы победителем / Are you a winner      |
| lose                         |  yes/no        |   являетесь ли вы проигравшим       |
| dump PATH                    |  ok            |   сохранить размер поля и вашу текущую расстановку кораблей в файл / save the field size and your current ship placement to a file    |
| load PATH                    |  ok            |   загрузить размер поля и расстановку кораблей из файла / load field size and ship placement from file
