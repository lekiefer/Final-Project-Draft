#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#define Hunt_Num 30
#define user_speed 5

using namespace std;
sf::RenderWindow window(sf::VideoMode(1200, 600), "Vincent's User Blip Demo");
sf::CircleShape user_blip(20);//global for ease of moving with keystroke commands

class Hunter: public sf::Drawable, public sf::Transformable
{
    sf::Texture m_tileset;
    double accuracy, speed, radius, damage;
    sf::Color color;

    public:

    void setAccuracy(double);
    void setSpeed(double);
    void setRadius(double);
    void setDamage(double);
    void setFillColor(sf::Color);

    double getAccuracy();
    double getSpeed();
    double getRadius();
    double getDamage();
    sf::Color getFillColor();

    Hunter();
    Hunter(double,double,double,double,sf::Color);

    private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        states.texture = &m_tileset;

        sf::CircleShape shape(radius);
        shape.setFillColor(color);

        target.draw(shape, states);
    }
};

void Hunter::setAccuracy(double a){
    accuracy=a;
};
void Hunter::setSpeed(double a){
    speed=a;
};
void Hunter::setRadius(double a){
    radius=a;
};
void Hunter::setDamage(double a){
    damage=a;
};
void Hunter::setFillColor(sf::Color a){
    color=a;
};
double Hunter::getAccuracy(){
    return accuracy;
};
double Hunter::getSpeed(){
    return speed;
};
double Hunter::getRadius(){
    return radius;
};
double Hunter::getDamage(){
    return damage;
};
sf::Color Hunter::getFillColor(){
    return color;
};
Hunter::Hunter(){
    accuracy=20;
    speed=2.4;
    radius=6;
    damage=.2;
    color=sf::Color::White;
};
Hunter::Hunter(double a,double b,double c,double d,sf::Color e)
{
    accuracy=a;
    speed=b;
    radius=c;
    damage=d;
    color=e;
};

void hunt_collision(Hunter hunters[],double h_move[Hunt_Num][2],vector<string>&run_tog, int tog_counter[])
{
    int i,j,collide_once;
    double dist, rad;
    sf::Vector2f hunt_xy;
    sf::Vector2f hunt_xy_2;

    for (i=0; i<Hunt_Num; i++)
    {
        collide_once=0;
        hunt_xy=hunters[i].getPosition();
        for (j=0; j<Hunt_Num; j++)//finds the distance between two hunters
        {
            if (i==j)
            {
                //skip action, if both loops refer to same hunter
            }
            else if (collide_once==0)//only check the current hunter if no collision has occurred
            {
                hunt_xy_2=hunters[j].getPosition();
                dist=sqrt(((hunt_xy_2.x-hunt_xy.x)*(hunt_xy_2.x-hunt_xy.x)+(hunt_xy_2.y-hunt_xy.y)*(hunt_xy_2.y-hunt_xy.y))+1);
                rad=hunters[i].getRadius()+hunters[j].getRadius();
                if ((dist<=rad) && (hunters[i].getRadius()<=hunters[j].getRadius()))//if the distance between two hunters is too close, reverse the "i" hunter's direction
                {                                                                   //Also, only the smaller hunter reports a collision and will reverse directions (in hunters_move)

                    h_move[i][0]=h_move[i][0]*-1;
                    h_move[i][1]=h_move[i][1]*-1;
                    run_tog[i]="run"; tog_counter[i]=0;
                    collide_once++;//ensures position is reversed only for one collision (don't want it to collide with one, reverse,
                                        //then collide simultaneously with another and reverse back, negating initial reversal).
                }
            }
        }
    }
}

void hunter_hits_user(Hunter hunt_hit,double& user_health)//tracks damage
{
    user_health=user_health-hunt_hit.getDamage();
}

void hunters_move(sf::CircleShape user, Hunter hunters[], double h_move[Hunt_Num][2], int& counter,vector<string>&run_tog,double& health)
{
    double y_dist, x_dist;
    sf::Vector2f user_xy=user.getPosition();
    sf::Vector2f hunt_xy;
    sf::Vector2f hunt_xy_2;
    int hunt_check,dist,i,tog_counter[Hunt_Num];

    if(counter==100)
    {
        counter=0;
    }
    for(i=0; i<Hunt_Num; i++)
    {
        if(counter==0)
        {
            run_tog[i]="chase";
            h_move[i][0]=(rand()%3-1)*hunters[i].getSpeed();
            h_move[i][1]=(rand()%3-1)*hunters[i].getSpeed();
        }

        hunt_xy=hunters[i].getPosition();
        dist=sqrt(((user_xy.x-hunt_xy.x)*(user_xy.x-hunt_xy.x)+(user_xy.y-hunt_xy.y)*(user_xy.y-hunt_xy.y))+1);

        if (((counter%20==0) && run_tog[i]=="chase"))//setting h_move to -1,0,or 1 ==> move negative direction, stay put, or go positive direction
        {                               //counter provides a delay between re-randomizing h_move, to prevent jerky motion
            h_move[i][0]=(rand()%3-1)*hunters[i].getSpeed();
            h_move[i][1]=(rand()%3-1)*hunters[i].getSpeed();
        }
        if(tog_counter[i]==60)
        {
            run_tog[i]="chase";
            tog_counter[i]=0;
        }
        if (dist>900)//sit still, if the distance is too great
        {
            h_move[i][0]=0;
            h_move[i][1]=0;
        }
        hunt_check=((rand()%dist+1)/(hunters[i].getAccuracy()))+1;//establish a number range based on the hunter's distance from user (divided by ten, to shorten range).
        if (hunt_check==1 && run_tog[i]=="chase")
        {
            x_dist=hunt_xy.x-user_xy.x;
            y_dist=hunt_xy.y-user_xy.y;
            if(x_dist>0) //if the hunter is to the right of the user, move it 10 pixels to the left (closer).
                                        //else, if the hunter is to the left, move it 10 pixels to the right (also closer)
            {
                h_move[i][0]=-1*hunters[i].getSpeed();
            }
            else if(x_dist<0)
            {
                h_move[i][0]=hunters[i].getSpeed();
            }
            if(y_dist>0) //if the hunter is above the user, move it 10 pixels down (closer).
                                        //else, if the hunter is below, move it 10 pixels up(also closer)
            {
                h_move[i][1]=-1*hunters[i].getSpeed();
            }
            else if(y_dist<0)
            {
                h_move[i][1]=hunters[i].getSpeed();
            }
        }
        hunt_collision(hunters,h_move,run_tog,tog_counter);//checks for collisions between hunters and then reassigns h_move if they do collide

        if (dist<(hunters[i].getRadius()+user.getRadius()))
        {
            hunter_hits_user(hunters[i],health);
        }

        hunters[i].move(h_move[i][0],h_move[i][1]);
//        dist=sqrt(((user_xy.x-hunt_xy.x)*(user_xy.x-hunt_xy.x)+(user_xy.y-hunt_xy.y)*(user_xy.y-hunt_xy.y))+1);//recalculate distance after movements to see
//         //if hunter has now hit the use
        tog_counter[i]+1;
        cout<<tog_counter[i]<<" ";
        //cout<<tog_counter[i]<<" ";
    }
    counter++; //cout<<counter<<" ";
}

void user_moves(sf::CircleShape& user_blip, int user_x[2], int user_y[2])
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        user_blip.move(user_x[1], 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        user_blip.move(user_x[0], 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        user_blip.move(0, user_y[1]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        user_blip.move(0, user_y[0]);
    }
};

void draw_dynamics(Hunter hunt_array[], sf::CircleShape user_blip, const double user_health)
{
    int i;
    for (i=0;i<Hunt_Num;i++)
    {
        window.draw(hunt_array[i]);

    }

    if(user_health>=0)
    {
        sf::RectangleShape health_bar_wipe(sf::Vector2f(33, 100-user_health));
        health_bar_wipe.setPosition(1102,52);
        health_bar_wipe.setFillColor(sf::Color::White);
        window.draw(health_bar_wipe);
    }

    window.draw(user_blip);
    window.display();
}

void draw_statics()
{

    sf::RectangleShape health_bar(sf::Vector2f(33, 100));
    health_bar.setPosition(1102,52);
    health_bar.setFillColor(sf::Color::Red);

    sf::RectangleShape health_bar_background(sf::Vector2f(37, 104));
    health_bar_background.setFillColor(sf::Color::White);
    health_bar_background.setPosition(1100,50);

    window.draw(health_bar_background);
    window.draw(health_bar);
}

void set_user_blip(sf::CircleShape& blip,int user_x_speed[2],int user_y_speed[2])
{
    blip.setFillColor(sf::Color::Green);
    blip.setOrigin(20,20);
    blip.setPosition(500,500);
    //give user some default speed values
    user_x_speed[0]=user_speed; user_x_speed[1]=user_speed*-1;
    user_y_speed[0]=user_speed; user_y_speed[1]=user_speed*-1;
}

void make_hunters(Hunter hunt_array[Hunt_Num])
{
    hunt_array[0]=Hunter(2000,1.2,10,3,sf::Color::Yellow);
    double rad=hunt_array[0].getRadius();
    hunt_array[0].setOrigin(rad,rad);
    hunt_array[0].setPosition(200,200);

    hunt_array[1]=Hunter(1000,.6,10,.1,sf::Color::Blue);
    rad=hunt_array[1].getRadius();
    hunt_array[1].setOrigin(rad,rad);
    hunt_array[1].setPosition(650,400);


    hunt_array[30]=Hunter();
}

int main()
{
    int user_x_speed[2],user_y_speed[2],counter=0;//give user some default speed values
    Hunter hunt_array[Hunt_Num]; make_hunters(hunt_array);//initialize an array of hunters
    double user_health=100, hunt_move[Hunt_Num][2];
    vector<string>run_toggle; run_toggle.resize(Hunt_Num);
    set_user_blip(user_blip,user_x_speed,user_y_speed);

    sf::Font font;
    if (!font.loadFromFile("sansation.ttf"))
    {
        return EXIT_FAILURE;
    }

    while (user_health>0)
    {
        user_moves(user_blip,user_x_speed,user_y_speed);
        window.clear();
        draw_statics();
        hunters_move(user_blip,hunt_array, hunt_move, counter,run_toggle,user_health);
        draw_dynamics(hunt_array,user_blip,user_health);
    }

    sf::Text text; text.setFont(font); text.setString("GAME OVER.");text.setCharacterSize(100);
    window.clear(sf::Color::Blue);
    window.draw(text);
    window.display();
//return 0;
}
