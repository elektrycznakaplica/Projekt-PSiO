#include <game_world_classes.h>


//--------------------class Tile------------------
Tile::Tile(float pos_x,float pos_y,int type)
{
    this->type=type;
    if(type==1){
        sf::Texture *txt_wall;
        txt_wall = new sf::Texture;
        if(!(txt_wall->loadFromFile("wall.png"))){
            std::cerr<<"could not load texture"<<std::endl;
        }
        txt_wall->setRepeated(true);
        tile.setTexture(*txt_wall);
        tile.setOrigin(20.0,20.0);
        tile.setPosition(pos_x,pos_y);
        position = tile.getPosition();
    }
    if(type==0){
        sf::Texture *txt_floor;
        txt_floor = new sf::Texture;
        if(!(txt_floor->loadFromFile("floor.png"))){
            std::cerr<<"could not load texture"<<std::endl;
        }
        txt_floor->setRepeated(true);
        tile.setTexture(*txt_floor);
        tile.setOrigin(20.0,20.0);
        tile.setPosition(pos_x,pos_y);
        position = tile.getPosition();
    }
}

sf::Sprite Tile::getSprite()
{
    return this->tile;
}

int Tile::getType()
{
    return this->type;
}

void Tile::setNode(float g, float h)
{
    this->g=g;
    this->h=h;
}

void Tile::setParent(sf::Vector2f parent_pos)
{
        this->parent_pos = parent_pos;
}

float calculate_distance(Tile start,Tile end)
{
    float dx = abs(start.getSprite().getPosition().x-end.getSprite().getPosition().x);
    float dy = abs(start.getSprite().getPosition().y-end.getSprite().getPosition().y);
    float h = sqrt( dx*dx + dy*dy );
    return h;
}

float calculate_gval(Tile start, Tile current)
{
    float g = start.g + calculate_distance(start,current);
    return g;
}

float Tile::getF()
{
    return this->f;
}

//------------------clas Map-----------------------------------------
Map::Map(std::vector<std::vector<int>> maze)
{
    for(int i=0;i<16;i++){
        std::vector<Tile> row;
        for(int j=0;j<25;j++){
            Tile x(j*40.0+20.0,i*40.0+20.0,maze[i][j]);
            row.emplace_back(x);
            }
        map.emplace_back(row);
        }
}


Map::Map()
{}

void Map::draw_map(sf::RenderWindow &window)
{
    for(auto &x : this->map){
        for(auto &y : x){
          window.draw(y.getSprite());
        }
    }
}

std::vector<sf::Sprite> Map::getWalls()
{
    std::vector<sf::Sprite> walls;
    for(auto &x : this->map){
        for(auto &y : x){
            if(y.getType()==1)
                walls.emplace_back(y.getSprite());
        }
    }
    return walls;
}

sf::Vector2f Map::getTile(sf::Vector2f pos)
{
    sf::Vector2f current={0,0};
    for(int i=0;i<16;i++){
        for(int j=0;j<25;j++){
            sf::FloatRect tile = map[i][j].getSprite().getGlobalBounds();
            if(tile.contains(pos.x,pos.y)){
                current.x = i;
                current.y = j;
            }
        }
    }
    return current;
}

std::vector<std::vector<Tile>> Map::getMap()
{
    return this->map;
}

std::vector<Tile> Map::getSuccesors(Tile parent)
{
    sf::Vector2f parent_cords = getTile(parent.getSprite().getPosition());
    std::vector<Tile> succesors;
    Tile a = map[parent_cords.x][parent_cords.y+1];
    if(a.getType()==0)
        succesors.emplace_back(a);
    Tile b = map[parent_cords.x -1][parent_cords.y+1];
    if(a.getType()==0)
        succesors.emplace_back(b);
    Tile c = map[parent_cords.x -1][parent_cords.y];
    if(a.getType()==0)
        succesors.emplace_back(c);
    Tile d = map[parent_cords.x -1][parent_cords.y -1];
    if(a.getType()==0)
        succesors.emplace_back(d);
    Tile e = map[parent_cords.x][parent_cords.y -1];
    if(a.getType()==0)
        succesors.emplace_back(e);
    Tile f = map[parent_cords.x +1][parent_cords.y -1];
    if(a.getType()==0)
        succesors.emplace_back(f);
    Tile g = map[parent_cords.x +1][parent_cords.y];
    if(a.getType()==0)
        succesors.emplace_back(g);
    Tile h = map[parent_cords.x +1][parent_cords.y +1];
    if(a.getType()==0)
        succesors.emplace_back(h);

    return succesors;
}

//-----------------------------class Colectible------------------------------------------------
Colectible::Colectible(float pos_x,float pos_y)
    {
        sf::Texture *txt_collectible;
        txt_collectible = new sf::Texture;
        if(!(txt_collectible->loadFromFile("wall.png"))){
            std::cerr<<"could not load texture"<<std::endl;
        }
        txt_collectible->setRepeated(true);
        colectible.setPosition(pos_x,pos_y);
        colectible.setTexture(*txt_collectible);
        colectible.scale(0.5,0.5);
    }

bool Colectible::isPicked()
    {
        return this->picked;
    }

void Colectible::pick()
    {
        this->picked=true;
    }

sf::Sprite Colectible::getObject()
    {
        return this->colectible;
    }



//---------------------------class Entity-----------------------------------------------
Entity::Entity(float pos_x, float pos_y)
    {
        sf::Texture *txt_guy;
        txt_guy = new sf::Texture;
        if(!(txt_guy->loadFromFile("guy.png"))){
            std::cerr<<"could not load texture"<<std::endl;
        }
        this->guy.setPosition(pos_x,pos_y);
        this->guy.scale(0.7142,0.7142);
        this->guy.setOrigin(15.0,15.0);
        this->guy.setRotation(90);
        this->guy.setTexture(*txt_guy);
    };

Entity::Entity()
{};

sf::Sprite Entity::getGuy()
    {
        return this->guy;
    }

Character::Character(float pos_x, float pos_y,std::vector<sf::Sprite> shapes)
    : Entity(pos_x,pos_y), walls(shapes)
{}

Character::Character()
{}

void Character::movement(const sf::Time elapsed, sf::RenderWindow &window)
    {
        rotate(window);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            double x = 0;
            double y=-150*elapsed.asSeconds();
            this->guy.move(x,y);
            if(intersection()==true){
                y=-y+0.05;
                this->guy.move(0.0,y);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            double x = 0;
            double y=150*elapsed.asSeconds();
            this->guy.move(x,y);
            if(intersection()==true){
                y=-y-0.05;
                this->guy.move(0.0,y);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            double x =-150*elapsed.asSeconds();
            double y=0;
            this->guy.move(x,y);
            if(intersection()==true){
                x=-x+0.05;
                this->guy.move(x,0.0);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            double x =150*elapsed.asSeconds();
            double y=0;
            this->guy.move(x,y);
            if(intersection()==true){
                x=-x-0.05;
                this->guy.move(x,0.0);
            }
        }
    }

void Character::collect(std::vector<Colectible> &x)
{
    for(Colectible &c : x)
        if((this->guy.getGlobalBounds().intersects(c.getObject().getGlobalBounds()))
                &&(sf::Keyboard::isKeyPressed(sf::Keyboard::E))){
            c.pick();
        }
}

void Character::rotate(sf::RenderWindow & window)
    {
        sf::Vector2f mouse(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
        sf::Vector2f entity(this->guy.getPosition());
        double dx = mouse.x - entity.x;
        double dy = mouse.y - entity.y;
        double angle = (180*atan2(dy,dx))/M_PI;
        this->guy.setRotation(angle);

    }

bool Character::intersection()
    {
        sf::RectangleShape hitbox;
        hitbox.setSize(sf::Vector2f(25,25));
        hitbox.setOrigin(this->guy.getOrigin());
        hitbox.setPosition(this->guy.getPosition());
        bool output = false;
        for(auto &x: this->walls){
            if((hitbox.getGlobalBounds()).intersects(x.getGlobalBounds()))
                output = true;
        }
        return output;
    }

Enemy::Enemy()
{}

Enemy::Enemy(float pos_x, float pos_y,Map m)
    : Entity(pos_x,pos_y), map(m) {};


void Enemy::movement(const sf::Time elapsed, sf::RenderWindow &window, Character target)
{
    sf::Vector2f target_pos = target.getGuy().getPosition();
    sf::Vector2f enemy_pos = this->getGuy().getPosition();
    if(enemy_pos.x>target_pos.x){
        double dx = -20*elapsed.asSeconds();
        double dy = 0;
        this->guy.move(dx,dy);
    }
    if(enemy_pos.x<target_pos.x){
        double dx = 20*elapsed.asSeconds();
        double dy = 0;
        this->guy.move(dx,dy);
    }
    if(enemy_pos.y>target_pos.y){
        double dx = 0;
        double dy = -20*elapsed.asSeconds();
        this->guy.move(dx,dy);
    }
    if(enemy_pos.y<target_pos.y){
        double dx = 0;
        double dy = 20*elapsed.asSeconds();
        this->guy.move(dx,dy);
    }

    rotate(window,target);
}

void Enemy::rotate(sf::RenderWindow & window,Character target)
{
    sf::Vector2f targ(target.getGuy().getPosition().x,target.getGuy().getPosition().y);
    sf::Vector2f entity(this->guy.getPosition());
    double dx = targ.x - entity.x;
    double dy = targ.y - entity.y;
    double angle = (180*atan2(dy,dx))/M_PI;
    this->guy.setRotation(angle);
}

void Enemy::find_path(sf::Vector2f target)
{
    std::list<Tile> open;
    std::list<Tile> closed;
    sf::Vector2f start_pos = this->map.getTile(this->getGuy().getPosition());
    Tile begin = map.getMap()[start_pos.x][start_pos.y];
    sf::Vector2f end_pos = this->map.getTile(target);
    Tile end = map.getMap()[end_pos.x][end_pos.y];

    begin.setNode(0,calculate_distance(begin,end));
    begin.setParent(start_pos);
    open.push_back(begin);

    while(!open.empty())
    {
        auto q = open.begin();
        for( auto i=open.begin();i!=open.end();i++){
            if(q->getF()>i->getF())
                q=i;
        }
        open.erase(q);
        auto succesors = map.getSuccesors(*q);
        for(auto &current : succesors)
        {
            current.setParent(q->getSprite().getPosition());
        }
        for(auto &current : succesors)
        {

        }
    }

}

//----------------------------------class Scene--------------------------------------
Scene::Scene() : window(sf::VideoMode(1000,640),"Lights out")
{
    std::vector<std::vector<int>> map(16,std::vector<int>(25));
    map[0]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    map[1]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1};
    map[2]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1};
    map[3]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1};
    map[4]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1};
    map[5]={1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1};
    map[6]={1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    map[7]={1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1};
    map[8]={1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1};
    map[9]={1,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1};
    map[10]={1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1};
    map[11]={1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1};
    map[12]={1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1};
    map[13]={1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1};
    map[14]={1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1};
    map[15]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    Map m(map);
    this->map=m;

    Colectible col(250.0,250.0);
    Colectible col1(930,500);
    Colectible col2(550,60);
    this->colectibles.emplace_back(col);
    this->colectibles.emplace_back(col1);
    this->colectibles.emplace_back(col2);

    Character guy(430.0,550.0,m.getWalls());
    Enemy en(20.0,20.0,m);
    this->enemy=en;
    this->hero = guy;
}

void Scene::game_loop()
{
    sf::Clock clock;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        sf::Time elapsed = clock.restart();

        this->map.draw_map(window);

        for(auto &col : colectibles){
            if(col.isPicked()==false)
            {
               window.draw(col.getObject());
            }
        }
        enemy.movement(elapsed,window,hero);
        window.draw(enemy.getGuy());

        hero.movement(elapsed,window);
        hero.collect(colectibles);
        window.draw(hero.getGuy());

        window.display();

        auto pos = map.getTile(hero.getGuy().getPosition());
        std::cout<<pos.x<<" "<<pos.y<<std::endl;
    }
}

