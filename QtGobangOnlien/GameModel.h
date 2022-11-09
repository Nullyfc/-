#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
using namespace std;
enum GameType
{
    MAN,//人人
    AI  //人机
};


enum GameStatus //游戏状态
{
    PLAYING,
    WIN,
    FAIL
};


//棋盘数据定义
const int BOARD_GRAD_SIZE =15;
const int MARGIN = 30; //棋盘边缘空隙
const int CHESS_RADIUS = 15;  //棋子半径
const int MARK_SIZE = 30;   //预落点大小
const int BLOCK_SIZE = 40;//格子大小
const int POS_OFFSET = BLOCK_SIZE * 0.4;// 模糊距离  20为上限
const int AI_THINK_TIME = 700; //人机延迟

class GameModel
{
public:

    GameModel(){};
    bool playerFlag=true;
public:
    vector<vector<int>> gameMapVec;//地图数据
    vector<vector<int>> scoreMapVec; //点位评分
    int aiRow,aiCol;
    //玩家状态
    GameType gameType;
    GameStatus gameStatus;
    void startGame(GameType type);
    void calculateScore();//计算得分
    void actionByperson(int row,int col);
    void updateGameMap(int row, int col);
    void netupdate(int row, int col, bool yourcolor);
    bool isWin(int row,int col);
    void actionByAI(int &clickRow,int &clickCol); //AI下棋

};


#endif // GAMEMODEL_H
