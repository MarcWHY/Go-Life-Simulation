// Dev:       波
// Date:      2023/07/20
// Function:  文字冒险游戏主程序

#include<iostream>
#include<string>
using namespace std;

class Player
{
public:
  Player(string name, int health, int elo, int wealth) {
    this->name = name;
    this->health = health;
    this->strength = elo;
    this->agility = wealth;
  }
private:
  string name;
  int health;
  int elo;
  int wealth;
};

int main()
{
  return 0;
}
