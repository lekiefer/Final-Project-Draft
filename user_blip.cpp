#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;

sf::RenderWindow window(sf::VideoMode(1000, 1000), "Vincent's User Blip Demo");
sf::CircleShape user_blip(20);
sf::CircleShape hunter_blips;

class user_blip
{


};

void hunt_progress(sf::CircleShape user, sf::CircleShape& hunter, float h_move[], int& counter)
{
    int dist,i;
    float y_dist, x_dist;
    sf::Vector2f user_xy=user.getPosition();
    int hunt_check, hunt_action;

    //for (i=0; i<3; i++)
    //{
        sf::Vector2f hunt_xy=hunter.getPosition();
        dist=sqrt((user_xy.x-hunt_xy.x)*(user_xy.x-hunt_xy.x)+(user_xy.y-hunt_xy.y)*(user_xy.y-hunt_xy.y));
        x_dist=hunt_xy.x-user_xy.x;
        y_dist=hunt_xy.y-user_xy.y;

    //setting h_move to -1,0,or 1 ==> move negative direction, stay put, or go positive direction
        if (counter==30)//counter provides a delay between re-randomizing h_move, to prevent jerky motion
        {
            h_move[0]=(rand()%3-1);
            h_move[1]=(rand()%3-1);
        }

        if (dist>900)
        {
            h_move[0]=0;
            h_move[1]=0;
        }

        hunt_check=(rand()%dist+1)/2;//establish a number range based on the hunter's distance from user (divided by ten, to shorten range).
        if (hunt_check==1)
        {
            if(x_dist>0) //if the hunter is to the right of the user, move it 10 pixels to the left (closer).
                                        //else, if the hunter is to the left, move it 10 pixels to the right (also closer)
            {
                h_move[0]=-1;
            }
            else if(x_dist<0)
            {
                h_move[0]=1;
            }
            if(y_dist>0) //if the hunter is above the user, move it 10 pixels down (closer).
                                        //else, if the hunter is below, move it 10 pixels up(also closer)
            {
                h_move[1]=-1;
            }
            else if(y_dist<0)
            {
                h_move[1]=1;
            }
        }

        if (counter==2)
        {
            hunter.move(h_move[0],h_move[1]);
            counter=0;
        }
        //window.draw(hunter[i]);
    //}
}
int main()
{
    float hunt_move[2];
    int counter=0, user_x[2], user_y[2], i;

    user_x[0]=2;
    user_x[1]=-2;
    user_y[0]=2;
    user_y[1]=-2;


    user_blip.setFillColor(sf::Color::Green);
    user_blip.setOrigin(20,20);
    user_blip.setPosition(500,500);
    //window.draw(user_blip);

    //for (i=0; i<3; i++)
    //{
        hunter_blips.setFillColor(sf::Color::Red);
        hunter_blips.setOrigin(20,20);
        hunter_blips.setPosition(0,0);
        hunter_blips.setRadius(40);
    //}

    //window.draw(hunter_blip);

    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            //window.clear();
            user_blip.move(user_x[1], 0);
            //window.draw(user_blip);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            //window.clear();
            user_blip.move(user_x[0], 0);
            //window.draw(user_blip);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            //window.clear();
            user_blip.move(0, user_y[1]);
            //window.draw(user_blip);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            //window.clear();
            user_blip.move(0, user_y[0]);
            //window.draw(user_blip);
        }

        hunt_progress(user_blip,hunter_blips, hunt_move, counter);

        window.clear();
        window.draw(user_blip);
        window.draw(hunter_blips);
        window.display();

        counter++;
    }
return 0;
}
