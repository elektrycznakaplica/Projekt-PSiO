#include <game_world_classes.h>

//--------------------class Tile------------------
Tile::Tile()
{}
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
        if(!(txt_floor->loadFromFile("floor.jpg"))){
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
    this->f=g+h;
}

void Tile::setParent(Tile* parent_pos)
{
        this->parent_pos = parent_pos;
}

Tile* Tile::getParent()
{
    return parent_pos;
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

float Tile::getG()
{
    return this->g;
}

void Tile::setVisited(bool state)
{
    this->visited = state;
}

bool Tile::wasVisited()
{
    return this->visited;
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

std::vector<Tile*> Map::getSuccesors(Tile parent)
{
    sf::Vector2f parent_cords = getTile(parent.getSprite().getPosition());
    std::vector<Tile*> succesors;
    Tile* a = &map[parent_cords.x][parent_cords.y+1];
    if(a->getType()==0)
        succesors.emplace_back(a);
    Tile* a1 = &map[parent_cords.x-1][parent_cords.y+1];
    if(a1->getType()==0)
        succesors.emplace_back(a1);
    Tile* a2 = &map[parent_cords.x-1][parent_cords.y];
    if(a2->getType()==0)
        succesors.emplace_back(a2);
    Tile* a3 = &map[parent_cords.x-1][parent_cords.y-1];
    if(a3->getType()==0)
        succesors.emplace_back(a3);
    Tile* a4 = &map[parent_cords.x-1][parent_cords.y];
    if(a4->getType()==0)
        succesors.emplace_back(a4);
    Tile* a5 = &map[parent_cords.x+1][parent_cords.y-1];
    if(a5->getType()==0)
        succesors.emplace_back(a5);
    Tile* a6 = &map[parent_cords.x+1][parent_cords.y];
    if(a6->getType()==0)
        succesors.emplace_back(a6);
    Tile* a7 = &map[parent_cords.x+1][parent_cords.y+1];
    if(a7->getType()==0)
        succesors.emplace_back(a7);

    return succesors;
}

//-----------------------------class Colectible------------------------------------------------
Colectible::Colectible(float pos_x,float pos_y)
    {
        sf::Texture *txt_collectible;
        txt_collectible = new sf::Texture;
        if(!(txt_collectible->loadFromFile("collectible.png"))){
            std::cerr<<"could not load texture"<<std::endl;
        }
        txt_collectible->setRepeated(true);
        colectible.setPosition(pos_x,pos_y);
        colectible.setTexture(*txt_collectible);
        colectible.scale(0.8,0.8);
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
        this->guy.setPosition(pos_x,pos_y);
    };

Entity::Entity()
{};

sf::Sprite Entity::getGuy()
    {
        return this->guy;
    }

Character::Character(float pos_x, float pos_y,std::vector<sf::Sprite> shapes)
    : Entity(pos_x,pos_y), walls(shapes)
{
    sf::Texture *txt_guy;
    txt_guy = new sf::Texture;
    if(!(txt_guy->loadFromFile("guy.png"))){
        std::cerr<<"could not load texture"<<std::endl;
    }
    this->guy.scale(0.7142,0.7142);
    this->guy.setOrigin(15.0,15.0);
    this->guy.setRotation(90);
    this->guy.setTexture(*txt_guy);
}

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
    : Entity(pos_x,pos_y), map(m)
{
    sf::Texture *txt_guy;
    txt_guy = new sf::Texture;
    if(!(txt_guy->loadFromFile("enemy.png"))){
        std::cerr<<"could not load texture"<<std::endl;
    }
    this->guy.scale(0.7142,0.7142);
    this->guy.setOrigin(15.0,15.0);
    this->guy.setRotation(90);
    this->guy.setTexture(*txt_guy);
};


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

std::vector<sf::Vector2f> Enemy::find_path(sf::Vector2f target, Map map)
{
    std::list<Tile*> *open = new std::list<Tile*>;

    sf::Vector2f start_pos = map.getTile(this->getGuy().getPosition());
    Tile begining = map.getMap()[start_pos.x][start_pos.y];
    sf::Vector2f end_pos = map.getTile(target);
    Tile end = map.getMap()[end_pos.x][end_pos.y];
    Tile* fin = new Tile;

    begining.setNode(0,calculate_distance(begining,end));
    begining.setParent(&begining);
    open->push_back(&begining);

    while(!open->empty())
    {

        open->sort([](Tile* x,Tile* y){return x->getF()<y->getF();});
        while(!open->empty()&&open->front()->wasVisited()){
            open->pop_front();
        }
        if(open->empty())
            break;

        Tile* current = open->front();
        current->setVisited(true);

        std::vector<Tile*> *succesors = new std::vector<Tile*>;
        *succesors = map.getSuccesors(*current);

        for(auto &neighbour : *succesors)
        {

            double g_temp = current->getG() + calculate_distance(*current,*neighbour);
            if(g_temp<neighbour->getG())
            {
                 neighbour->setParent(current);
                 neighbour->setNode(g_temp,calculate_distance(*neighbour,end));
                 if(!neighbour->wasVisited()){
                     open->push_back(neighbour);
                 }
                 if(current->getSprite().getPosition()==end.getSprite().getPosition()){
                     fin=current;
                     break;
                 }
            }
        }
    }
    Tile* x = fin;
    std::vector<sf::Vector2f> path;
    while(x!=x->getParent())
    {
        auto pos=x->getSprite().getPosition();
        path.emplace_back(pos);
        x=x->getParent();
    }
    path.emplace_back(x->getSprite().getPosition());
    std::reverse(path.begin(),path.end());

    return path;
}

void Enemy::follow(sf::Time elapsed,std::vector<sf::Vector2f> path)
{
    this->move(elapsed,path[path_nr]);
    if((abs(path[path_nr].x-this->getGuy().getPosition().x)<1)&&
       (abs(path[path_nr].y-this->getGuy().getPosition().y)<1))
        path_nr++;
}

void Enemy::move(sf::Time elapsed,sf::Vector2f dest)
{
    sf::Vector2f pos = this->guy.getPosition();
    double dx = dest.x - pos.x;
    double dy = dest.y - pos.y;
    double angle = (180*atan2(dy,dx))/M_PI;
    this->guy.setRotation(angle);
    double v_x = cos(angle)*80;
    double v_y = sin(angle)*80;
    this->guy.move(v_x*elapsed.asSeconds(),v_y*elapsed.asSeconds());
}

Window::Window() : window(sf::VideoMode(1000,640),"Lights out")
{}

Button::Button(double pos_x,double pos_y, unsigned int size, std::string txt)
{
    sf::Font *font;
    font = new sf::Font;
    font->loadFromFile("font.ttf");
    if(!font->loadFromFile("font.ttf")){
        std::cout<<"could not load a font";
    }
    this->text.setFont(*font);
    this->text.setPosition(pos_x,pos_y);
    this->text.setCharacterSize(size);
    this->text.setString(txt);
    this->text.setFillColor(sf::Color::Black);
}

void Button::click(sf::Event event)
{
        sf::FloatRect bounds = this->text.getGlobalBounds();
        if((event.mouseButton.x>=bounds.left)&&(event.mouseButton.x<=bounds.left+bounds.width)
                &&(event.mouseButton.y>=bounds.top)&&(event.mouseButton.y<=bounds.top+bounds.height)){
            if(event.type== sf::Event::MouseButtonPressed){
                this->text.setFillColor(sf::Color::Red);
                this->clicked=true;
            }
            if(event.type == sf::Event::MouseButtonReleased){
                this->text.setFillColor(sf::Color::Black);
            }
        }
}

void Button::draw(sf::RenderWindow* window)
{
    window->draw(this->text);
}

bool Button::was_clicked()
{
    return this->clicked;
}

Menu::Menu() : Window()
{
    Button title(235,0,150,"Lights Out");
    menu.emplace_back(title);

    Button play(445,150,75,"play");
    menu.emplace_back(play);

    Button instructions(335,220,75,"instructions");
    menu.emplace_back(instructions);

    Button exit(445,290,75,"exit");
    menu.emplace_back(exit);

    sf::Texture *txt;
    txt = new sf::Texture;
    if(!(txt->loadFromFile("flashlight.png"))){
        std::cerr<<"could not load texture"<<std::endl;
    }
     this->flashlight.setTexture(*txt);
     this->flashlight.setPosition(490,600);
}

int Menu::loop()
{
    sf::RenderTexture mask;
    mask.create(1000,640);

    sf::ConvexShape light;
    light.setPointCount(3);
    light.setPoint(0,sf::Vector2f(40,0));
    light.setPoint(1,sf::Vector2f(960,0));
    light.setPoint(2,sf::Vector2f(500,600));
    light.setFillColor(sf::Color(255,255,255,1));

    sf::Sprite spr;
    spr.setPosition(0,0);
    spr.setTexture(mask.getTexture(),true);

    bool light_on = false;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed){
                for(auto i=menu.begin()+1;i<menu.end();i++){
                    i->click(event);
                }
                sf::FloatRect bounds = this->flashlight.getGlobalBounds();
                if((event.mouseButton.x>=bounds.left)&&(event.mouseButton.x<=bounds.left+bounds.width)
                        &&(event.mouseButton.y>=bounds.top)&&(event.mouseButton.y<=bounds.top+bounds.height)){
                    light_on=!light_on;
                }
            }
            if(event.type == sf::Event::MouseButtonReleased){
                for(auto i=menu.begin()+1;i<menu.end();i++){
                    i->click(event);
                }
            }

        }
        mask.clear(sf::Color::Black);
        if(light_on==true){
            mask.draw(light,sf::BlendNone);
        }
        mask.display();

        spr.setTexture(mask.getTexture(),true);

        window.clear(sf::Color::White);
        for(auto &option : menu){
            option.draw(&window);
        }
        window.draw(spr);
        window.draw(flashlight);
        window.display();

        if(menu[Play].was_clicked()==true){
            window.close();
            return 1;
        }
        if(menu[Exit].was_clicked()==true){
            window.close();
            return 0;
        }
    }
}

//----------------------------------class Scene--------------------------------------
Scene::Scene() : Window()
{
//    score.setFont(font);
//    score.setPosition(60,60);
//    score.setString("0/5");

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
    Colectible col1(920,500);
    Colectible col2(550,60);
    Colectible col3(70,560);
    Colectible col4(570,560);
    this->colectibles.emplace_back(col);
    this->colectibles.emplace_back(col1);
    this->colectibles.emplace_back(col2);
    this->colectibles.emplace_back(col3);
    this->colectibles.emplace_back(col4);

    Character guy(430.0,550.0,m.getWalls());
    Enemy en(60.0,60.0,m);
    Enemy en1(560.0,560.0,m);
    Enemy en2(800.0,70.0,m);
    this->enemies.emplace_back(en);
    this->enemies.emplace_back(en1);
    this->enemies.emplace_back(en2);
    this->hero = guy;
}

int Scene::loop()
{
    sf::Clock clock;

    std::vector<std::vector<sf::Vector2f>> paths;
    for(auto i = enemies.begin();i<enemies.end();i++){
        paths.emplace_back(i->find_path(hero.getGuy().getPosition(),map));
    }
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
        timer+=elapsed.asSeconds();
        this->map.draw_map(window);

        for(auto i=colectibles.begin();i<colectibles.end();i++){
            window.draw(i->getObject());
            if(i->isPicked()==true)
            {
               colectibles.erase(i);
               colectible_count++;
               std::string sc= std::to_string(colectible_count) + "/5";
               score.setString(sc);
            }
        }
        for(auto i = 0;i<enemies.size();i++){
            window.draw(enemies[i].getGuy());
            enemies[i].follow(elapsed,paths[i]);
        }

        hero.movement(elapsed,window);
        hero.collect(colectibles);
        window.draw(hero.getGuy());

        window.draw(score);

        window.display();

        if(colectible_count==5){
            window.close();
            std::cout<<"WIN";
        }

    }

//    for(auto i = x.begin() ;i<x.end();i++)
//    {
//        std::cout<<i->x<<" "<<i->y<<std::endl;
//    }
//    auto pos = map.getTile(hero.getGuy().getPosition());
//    std::cout<<pos.x<<" "<<pos.y<<" "<<map.getMap()[pos.x][pos.y].getType()<<std::endl;
    return 1;
}

