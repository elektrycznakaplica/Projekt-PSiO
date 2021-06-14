#ifndef GAME_WORLD_CLASSES_H
#define GAME_WORLD_CLASSES_H
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include <iostream>
#include<math.h>
#include <algorithm>
#include <list>
#include <algorithm>


class Tile
{
private:
    sf::Sprite tile;
    sf::Vector2f position;
    int type;
    bool visited=false;
    float g=3000;
    float f=0;
    Tile* parent_pos;
public:
    Tile();
    Tile(float pos_x,float pos_y,int type);
    sf::Sprite getSprite();
    int getType();
    void setNode(float g,float h);
    void setParent(Tile* parent_pos);
    Tile* getParent();
    friend float calculate_gval(Tile start,Tile current);
    friend float calculate_distance(Tile start,Tile end);
    float getF();
    float getG();
    bool wasVisited();
    void setVisited(bool state);
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
    std::vector<Tile*> getSuccesors(Tile parent);
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
    int path_nr=0;
public:
    Enemy();
    Enemy(float pos_x, float pos_y,Map m);
    void movement(const sf::Time elapsed, sf::RenderWindow &window,Character target);
    void follow(sf::Time elapsed, std::vector<sf::Vector2f> path);
    void move(sf::Time elapsed,sf::Vector2f dest);
    std::vector<sf::Vector2f> find_path(sf::Vector2f t,Map m);
    void rotate(sf::RenderWindow & window,Character target);
};

class Window
{
protected:
    sf::RenderWindow window;
public:
    Window();
    virtual int loop()=0;
};

class Button
{
private:
    sf::Text text;
    bool clicked = false;
public:
    Button(double pos_x,double pos_y,unsigned int size, std::string txt);
    void click(sf::Event);
    bool was_clicked();
    void draw(sf::RenderWindow* window);
};

class Menu : public Window
{
private:
    enum Option {Title,Play,Instructions,Exit};
    std::vector<Button> menu;
    sf::Sprite flashlight;
public:
    Menu();
    int loop();

};

class Scene : public Window
{
private:
    Character hero;
    std::vector<Enemy> enemies;
    Map map;
    std::vector<Colectible> colectibles;
    sf::Text score;
    double timer=0;
    int colectible_count = 0;
public:
    Scene();
    int loop();
};


#endif // GAME_WORLD_CLASSES_H
