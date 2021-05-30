#ifndef GAME_WORLD_CLASSES_H
#define GAME_WORLD_CLASSES_H
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include <iostream>
#include<math.h>
#include <algorithm>
#include <list>


class Tile
{
private:
    sf::Sprite tile;
    sf::Vector2f position;
    int type;
    float g=0;
    float h=0;
    float f=0;
    sf::Vector2f parent_pos={0,0};
public:
    Tile(float pos_x,float pos_y,int type);
    sf::Sprite getSprite();
    int getType();
    void setNode(float g,float h);
    void setParent(sf::Vector2f parent_pos);
    friend float calculate_gval(Tile start,Tile current);
    friend float calculate_distance(Tile start,Tile end);
    float getF();
};



class Map
{
private:
    std::vector<std::vector<Tile>> map;
public:
    Map(std::vector<std::vector<int>> maze);
    Map();
    void draw_map(sf::RenderWindow &window);
    std::vector<sf::Sprite> getWalls();
    sf::Vector2f getTile(sf::Vector2f pos);
    std::vector<Tile> getSuccesors(Tile parent);
    std::vector<std::vector<Tile>> getMap();
};



class Colectible
{
private:
    sf::Sprite colectible;
    bool picked=false;
public:
    Colectible(float pos_x,float pos_y);
    Colectible();
    bool isPicked();
    void pick();
    sf::Sprite getObject();
};



class Entity
{
protected:
    sf::Sprite guy;
public:
    Entity(float pos_x, float pos_y);
    Entity();
    //virtual void movement(const sf::Time elapsed, sf::RenderWindow &window)=0;
    //virtual void rotate(sf::RenderWindow & window)=0;
    sf::Sprite getGuy();
};

class Character : public Entity
{
private:
    std::vector<sf::Sprite> walls;
public:
    Character();
    Character(float pos_x, float pos_y,std::vector<sf::Sprite> shapes);
    void movement(const sf::Time elapsed, sf::RenderWindow &window);
    void rotate(sf::RenderWindow & window);
    void collect(std::vector<Colectible> &x);
    bool intersection();
};

class Enemy : public Entity
{
private:
    Map map;
public:
    Enemy();
    Enemy(float pos_x, float pos_y,Map m);
    void movement(const sf::Time elapsed, sf::RenderWindow &window,Character target);
    void find_path(sf::Vector2f);
    void rotate(sf::RenderWindow & window,Character target);
};

class Scene
{
private:
    sf::RenderWindow window;
    Character hero;
    Enemy enemy;
    Map map;
    std::vector<Colectible> colectibles;
public:
    Scene();
    void game_loop();
};


#endif // GAME_WORLD_CLASSES_H
