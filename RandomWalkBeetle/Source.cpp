#include <iostream>
#include <time.h>
#include <thread>
#include <fstream>
#include <cmath>
#include "c4smath.h"
using namespace std;

class Tileroom {
public:
    Tileroom(int size) : _roomSize(size)
    {
        _hasVisited = new bool *[size];
        for (int i = 0; i < size; i++)
            _hasVisited[i] = new bool[size];
    }

    ~Tileroom()
    {
        for (int i = 0; i < _roomSize; i++)
            delete _hasVisited[i];
        delete _hasVisited;
    }

    void markTile(int row, int col) 
    { 
        _hasVisited[row][col] = 1; 
    }

    bool visitAll()
    {
        for (int i = 0; i < _roomSize; i++)
        {
            for (int j = 0; j < _roomSize; j++)
            {
                if (_hasVisited[i][j] != 1) 
                { 
                    return false; 
                }
            }
        }
        return true;
    }

    int getRoomSize() { return _roomSize; }
    bool ** getHasVisited() { return _hasVisited; }

private:
    int _roomSize;
    bool ** _hasVisited;
};

class Beetle
{
public:
    Beetle(Tileroom * tileroom) : _tileroom(tileroom)
    {
        _roomSize = tileroom->getRoomSize();

        _rowPosition = rand() % _roomSize;
        _colPosition = rand() % _roomSize;

        _tileroom->markTile(_rowPosition, _colPosition);
    }

    inline void move()
    {
        while (true)
        {
            switch (rand() % 8)
            {
            case 0:
                if (_rowPosition != 0) { _rowPosition--; return; }
            case 1:
                if (_rowPosition != _roomSize - 1) { _rowPosition++; return; } break;
            case 2:
                if (_colPosition != 0) { _colPosition--; return; } break;
            case 3:
                if (_colPosition != _roomSize - 1) { _colPosition++; return; } break;
            case 4:
                if (_rowPosition != 0 && _colPosition != 0) { _rowPosition--; _colPosition--; return; } break;
            case 5:
                if (_rowPosition != 0 && _colPosition != _roomSize - 1) { _rowPosition--; _colPosition++; return; } break;
            case 6:
                if (_rowPosition != _roomSize - 1 && _colPosition != 0) { _rowPosition++; _colPosition--; return; } break;
            case 7:
                if (_rowPosition != _roomSize - 1 && _colPosition != _roomSize - 1) { _rowPosition++; _colPosition++; return; } break;
            default:
                break;
            }
        }
    }

    inline void moveOnce()
    {
        move();
        _tileroom->markTile(_rowPosition, _colPosition);
        movedTime++;
    }

    void visitAllTiles()
    {
        while (_tileroom->visitAll() == false)
        {
            moveOnce();
        }
    }

    Tileroom * getTileroom() { return _tileroom; }
    int getMovedTime() { return movedTime; }

private:
    int _rowPosition;
    int _colPosition;
    Tileroom * _tileroom;
    int _roomSize;
    int movedTime = 0;
};
//
//class BeetleThread : public thread
//{
//public:
//    BeetleThread(Tileroom * tileroom, Beetle * beetle, int * countList, int j) : 
//        thread(&Beetle::visitAllTiles, beetle), _tileroom(tileroom), _beetle(beetle), _countList(countList), _j(j)
//    {
//
//    }
//
//    ~BeetleThread()
//    {
//        _countList[_j] = _beetle->getMovedTime();
//        delete _tileroom;
//        delete _beetle;
//    }
//
//private:
//    Tileroom * _tileroom;
//    Beetle * _beetle;
//    int * _countList;
//    int _j;
//};

void emulateOnce(int i, int j, int * countList)
{
    Tileroom * tileroom = new Tileroom(i);
    Beetle * beetle = new Beetle(tileroom);

    beetle->visitAllTiles();
    /*thread t{ &Beetle::visitAllTiles, beetle };
    t.join();*/
    countList[j] = beetle->getMovedTime();

    delete tileroom;
    delete beetle;

    return;
}

int main()
{
    srand((unsigned int)time(NULL));
    clock_t beginCLK, endCLK;

    int inputStart = 0, inputEnd = 0, inputRepeat = 0;
    cout << "���簢�� �� ���� ������ ���� ����, ���� ����, �ݺ� Ƚ���� �Է����ּ���. : ";
    cin >> inputStart >> inputEnd >> inputRepeat;

    ofstream totalResult("totalResult.csv", ios::out);
    ofstream calcResult("calcResult.csv", ios::out);

    int * countList = new int[inputRepeat];

    beginCLK = clock();

    for (int i = inputStart; i <= inputEnd; i++)
    {
        cout << i << " X " << i << " ���� ��... ";

        for (int j = 0; j < inputRepeat; j++)
        {
            emulateOnce(i, j, countList);
        }

        for (int j = 0; j < inputRepeat; j++)
        {
            totalResult << countList[j] << ",";
        }

        double average = getAverage(countList, inputRepeat);
        double stdDeviation = sqrt(getVariance(countList, inputRepeat));

        totalResult << endl;
        calcResult << i << "," << average << "," << stdDeviation << endl;
        cout << "���� �Ϸ�! ��� : " << average << ", ǥ������ : " << stdDeviation <<  endl;
    }
    delete countList;

    endCLK = clock();
    cout << (endCLK - beginCLK) / (double)CLOCKS_PER_SEC << endl;

    return 0;
}