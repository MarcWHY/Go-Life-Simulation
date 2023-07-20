#include<iostream>
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
