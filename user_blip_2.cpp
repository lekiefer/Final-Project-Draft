#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;
sf::RenderWindow window(sf::VideoMode(1000, 1000), "Vincent's User Blip Demo");
sf::CircleShape user_blip(20);//global for ease of moving

class Hunter: public sf::Drawable, public sf::Transformable
{
    sf::Texture m_tileset;
    double accuracy, speed, radius;
    sf::Color color;

    public:

    void setAccuracy(double);
    void setSpeed(double);
    void setRadius(double);
    void setFillColor(sf::Color);

    double getAccuracy();
    double getSpeed();
    double getRadius();
    sf::Color getFillColor();

    Hunter();
    Hunter(double,double,double,sf::Color);

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
sf::Color Hunter::getFillColor(){
    return color;
};
Hunter::Hunter(){
    accuracy=10;
    speed=1;
    radius=30;
    color=sf::Color::Red;
};
Hunter::Hunter(double a,double b,double c,sf::Color d)
{
    accuracy=a;
    speed=b;
    radius=c;
    color=d;
};

void hunt_progress(sf::CircleShape user, Hunter hunters[], double h_move[3][2], int& counter)
{
    int dist,i,j;
    double y_dist, x_dist,rad;
    sf::Vector2f user_xy=user.getPosition();
    sf::Vector2f hunt_xy;
    sf::Vector2f hunt_xy_2;
    int hunt_check;

    for(i=0; i<3; i++)
    {
        hunt_xy=hunters[i].getPosition();
        dist=sqrt(((user_xy.x-hunt_xy.x)*(user_xy.x-hunt_xy.x)+(user_xy.y-hunt_xy.y)*(user_xy.y-hunt_xy.y))+1);
        x_dist=hunt_xy.x-user_xy.x;
        y_dist=hunt_xy.y-user_xy.y;

        if (counter==0 || counter==30)//setting h_move to -1,0,or 1 ==> move negative direction, stay put, or go positive direction
        {                               //counter provides a delay between re-randomizing h_move, to prevent jerky motion
            h_move[i][0]=(rand()%3-1)*hunters[i].getSpeed();
            h_move[i][1]=(rand()%3-1)*hunters[i].getSpeed();
        }
        if (counter==60)//reset counter, so that the move direction is forced to change
        {
            counter=0;
        }

        if (dist>900)//sit still, if the distance is too great
        {
            h_move[i][0]=0;
            h_move[i][1]=0;
        }

        hunt_check=((rand()%dist+1)/(hunters[i].getAccuracy()))+1;//establish a number range based on the hunter's distance from user (divided by ten, to shorten range).
        if (hunt_check==1)
        {
            if(x_dist>0) //if the hunter is to the right of the user, move it 10 pixels to the left (closer).
                                        //else, if the hunter is to the left, move it 10 pixels to the right (also closer)
            {
                h_move[i][0]=-1*hunters[i].getSpeed();
            }
            else if(x_dist<0)
            {
                h_move[i][0]=1*hunters[i].getSpeed();
            }
            if(y_dist>0) //if the hunter is above the user, move it 10 pixels down (closer).
                                        //else, if the hunter is below, move it 10 pixels up(also closer)
            {
                h_move[i][1]=-1*hunters[i].getSpeed();
            }
            else if(y_dist<0)
            {
                h_move[i][1]=1*hunters[i].getSpeed();
            }
        }
        hunters[i].move(h_move[i][0],h_move[i][1]);
        window.draw(hunters[i]);
    }
}

//void hunt_collision_check(Hunter hunters[],hunt_move[3][2],int& hunt_ind_1,int& hunt_ind_2)
//{
//    sf::Vector2f hunt_xy;
//    sf::Vector2f hunt_xy_2;
//    bool check;

//    for (i=0; i<3; i++)
//    {
//        hunt_xy=hunters[i].getPosition();
//        for (j=0; j<3; j++)//finds the distance between two hunters
//        {
//            hunt_xy_2=hunters[j].getPosition();
//            dist=sqrt(((hunt_xy_2.x-hunt_xy.x)*(hunt_xy_2.x-hunt_xy.x)+(hunt_xy_2.y-hunt_xy.y)*(hunt_xy_2.y-hunt_xy.y))+1);
//            rad=hunters[i].getRadius()+hunters[j].getRadius();
//            if (i==j)
//            {
//                dist=100000;
//            }
//            if (dist<=rad)//if the distance between two hunters is too close, reverse the "i" hunter's direction
//            {
//                check=1;
//                hunt_ind_1=i;
//                hunt_ind_2=i
//            }
//        }
//    }
//}

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

void set_user_blip(sf::CircleShape& blip,int user_x_speed[2],int user_y_speed[2])
{
    blip.setFillColor(sf::Color::Green);
    blip.setOrigin(20,20);
    blip.setPosition(500,500);
    //give user some default speed values
    user_x_speed[0]=5; user_x_speed[1]=-5;
    user_y_speed[0]=5; user_y_speed[1]=-5;
}

void make_hunters(Hunter hunt_array[3])
{
    double rad=hunt_array[0].getRadius();
    hunt_array[0]=Hunter(2,1.8,13.5,sf::Color::Yellow);
    hunt_array[0].setOrigin(rad,rad);
    hunt_array[0].setPosition(200,200);

    rad=hunt_array[1].getRadius();
    hunt_array[1]=Hunter(1000,.5,60,sf::Color::Blue);
    hunt_array[1].setOrigin(rad,rad);
    hunt_array[1].setPosition(100,0);

    rad=hunt_array[2].getRadius();
    hunt_array[2]=Hunter(6,2.4,6,sf::Color::White);
    hunt_array[2].setOrigin(rad,rad);
    hunt_array[2].setPosition(650,800);
}

int main()
{
    int user_x_speed[2], user_y_speed[2];//give user some default speed values
    Hunter hunt_array[3];
    make_hunters(hunt_array);
    double hunt_move[3][2];
    int counter=0;

    set_user_blip(user_blip,user_x_speed,user_y_speed);

    while (window.isOpen())
    {
        user_moves(user_blip,user_x_speed,user_y_speed);
        window.clear();
        hunt_progress(user_blip,hunt_array, hunt_move, counter);
//        hunt_collision_check(hunt_array,hunt_move);
        window.draw(user_blip);
        window.display();

        counter++;
    }
return 0;
}
