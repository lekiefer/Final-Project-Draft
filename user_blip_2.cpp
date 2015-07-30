#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#define Hunt_Num 20

using namespace std;
sf::RenderWindow window(sf::VideoMode(1400, 1600), "Vincent's User Blip Demo");
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

bool hunt_collision_check(Hunter hunters[],int& hunt_ind_1,int& hunt_ind_2)
{
    int i,j;
    double dist, rad;
    sf::Vector2f hunt_xy;
    sf::Vector2f hunt_xy_2;
    bool check;

    check=0;
    for (i=0; i<Hunt_Num; i++)
    {
        hunt_xy=hunters[i].getPosition();
        for (j=0; j<Hunt_Num; j++)//finds the distance between two hunters
        {
            if (i==j)
            {
                //skip action, if it's referring to same hunter
            }
            else
            {
                hunt_xy_2=hunters[j].getPosition();
                dist=sqrt(((hunt_xy_2.x-hunt_xy.x)*(hunt_xy_2.x-hunt_xy.x)+(hunt_xy_2.y-hunt_xy.y)*(hunt_xy_2.y-hunt_xy.y))+1);
                rad=hunters[i].getRadius()+hunters[j].getRadius();
                if (dist<=rad)//if the distance between two hunters is too close, reverse the "i" hunter's direction
                {
                    check=1;
                    hunt_ind_1=i;
                    hunt_ind_2=j;
                }
            }
        }
    }
    return check;
}

void hunters_move(sf::CircleShape user, Hunter hunters[], double h_move[Hunt_Num][2], int& counter,vector<string>&run_tog, int tog_counter[])
{
    int dist,i;
    double y_dist, x_dist;
    sf::Vector2f user_xy=user.getPosition();
    sf::Vector2f hunt_xy;
    sf::Vector2f hunt_xy_2;
    int hunt_check,hunt_ind_1,hunt_ind_2;
    bool check;
    if(counter==100)
    {
        counter=0;
    }
    for(i=0; i<Hunt_Num; i++)
    {
        if(counter==0)
        {
            run_tog[i]="chase";
            h_move[i][0]=(rand()%4-1)*hunters[i].getSpeed();
            h_move[i][1]=(rand()%4-1)*hunters[i].getSpeed();
        }

        hunt_xy=hunters[i].getPosition();
        dist=sqrt(((user_xy.x-hunt_xy.x)*(user_xy.x-hunt_xy.x)+(user_xy.y-hunt_xy.y)*(user_xy.y-hunt_xy.y))+1);

        if ((counter%20==0) && run_tog[i]=="chase")//setting h_move to -1,0,or 1 ==> move negative direction, stay put, or go positive direction
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
        check=hunt_collision_check(hunters,hunt_ind_1,hunt_ind_2);
        if (check==true)
        {
            h_move[hunt_ind_1][0]=h_move[hunt_ind_1][0]*-1;
            h_move[hunt_ind_1][1]=h_move[hunt_ind_1][1]*-1;
            h_move[hunt_ind_2][0]=h_move[hunt_ind_2][0]*-1;
            h_move[hunt_ind_2][1]=h_move[hunt_ind_2][1]*-1;
            run_tog[hunt_ind_1]="run"; tog_counter[hunt_ind_1]=0;
            run_tog[hunt_ind_2]="run"; tog_counter[hunt_ind_2]=0;
        }
        hunters[i].move(h_move[i][0],h_move[i][1]);
    }
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

void draw_all_blips(Hunter hunt_array[], sf::CircleShape user_blip)
{
    int i;
    for (i=0;i<Hunt_Num;i++)
    {
        window.draw(hunt_array[i]);
    }
    window.draw(user_blip);
    window.display();
}

void set_user_blip(sf::CircleShape& blip,int user_x_speed[2],int user_y_speed[2])
{
    blip.setFillColor(sf::Color::Green);
    blip.setOrigin(20,20);
    blip.setPosition(500,500);
    //give user some default speed values
    user_x_speed[0]=5; user_x_speed[1]=-5;
    user_y_speed[0]=5; user_y_speed[1]=-5;
}

void make_hunters(Hunter hunt_array[Hunt_Num])
{
    double rad=hunt_array[0].getRadius();
    hunt_array[0]=Hunter(2000,.9,25,sf::Color::Yellow);
    hunt_array[0].setOrigin(rad,rad);
    hunt_array[0].setPosition(200,200);

    rad=hunt_array[1].getRadius();
    hunt_array[1]=Hunter(50,2.4,6,sf::Color::White);
    hunt_array[1].setOrigin(rad,rad);
    hunt_array[1].setPosition(250,800);

    rad=hunt_array[2].getRadius();
    hunt_array[2]=Hunter(50,2.4,6,sf::Color::White);
    hunt_array[2].setOrigin(rad,rad);
    hunt_array[2].setPosition(650,800);

    rad=hunt_array[3].getRadius();
    hunt_array[3]=Hunter(50,2.4,6,sf::Color::White);
    hunt_array[3].setOrigin(rad,rad);
    hunt_array[3].setPosition(20,700);

    rad=hunt_array[4].getRadius();
    hunt_array[4]=Hunter(50,2.4,6,sf::Color::White);
    hunt_array[4].setOrigin(rad,rad);
    hunt_array[4].setPosition(20,40);

        rad=hunt_array[5].getRadius();
    hunt_array[5]=Hunter(50,2.4,6,sf::Color::White);
    hunt_array[5].setOrigin(rad,rad);
    hunt_array[5].setPosition(20,400);

        rad=hunt_array[6].getRadius();
    hunt_array[6]=Hunter(50,2.4,6,sf::Color::White);
    hunt_array[6].setOrigin(rad,rad);
    hunt_array[6].setPosition(200,40);

    hunt_array[20]=Hunter(50,2.4,6,sf::Color::White);
}

int main()
{
    int user_x_speed[2], user_y_speed[2];//give user some default speed values
    Hunter hunt_array[Hunt_Num];
    make_hunters(hunt_array);
    double hunt_move[Hunt_Num][2];
    int counter=0,tog_counter[Hunt_Num];
    vector<string>run_toggle;
    run_toggle.resize(Hunt_Num);

    set_user_blip(user_blip,user_x_speed,user_y_speed);

    while (window.isOpen())
    {
        user_moves(user_blip,user_x_speed,user_y_speed);
        window.clear();
        hunters_move(user_blip,hunt_array, hunt_move, counter,run_toggle,tog_counter);
        draw_all_blips(hunt_array,user_blip);
        counter++; tog_counter+1;
    }
return 0;
}
