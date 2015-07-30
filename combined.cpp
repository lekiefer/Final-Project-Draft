#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;
using namespace sf;

RenderWindow WINDOW(VideoMode(1000,600), "SFML window");
sf::CircleShape user_blip(20);
sf::CircleShape hunter_blips;

void buildEnvir(vector<CircleShape>& a, vector<RectangleShape>& b, vector<Vector2f>& c, vector<Vector2f>& d);
void drawTree(vector<CircleShape>& a, vector<RectangleShape>& b);
//void checkCollision(vector<Vector2f>& b, vector<Vector2f>& c, int d[], int e[], vector<bool>& rockCollision, vector<bool>& treeCollision);
void checkCollision(vector<Vector2f>& b, vector<Vector2f>& c, int d[], int e[], int& rockDir, int& treeDir);
//class user_blip
//{

//};

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

}


int main()
{
    Clock clock;
    srand(time(NULL));

    //draw environmental obstacles
    vector<CircleShape> rockGroup;       //want lake?
    vector<RectangleShape> treeGroup;
    vector<Vector2f> treeCoords, rockCoords;
    buildEnvir(rockGroup, treeGroup, treeCoords, rockCoords);

    vector<CircleShape> treeTop;
    drawTree(treeTop, treeGroup);

    //vector<bool> rockCollision;
    //vector<bool> treeCollision;
    int rockDir = 0;
    int treeDir = 0;

    //vincent's code
    float hunt_move[2];
    int counter=0, user_x[2], user_y[2], i;

    user_x[0]=2;
    user_x[1]=-2;
    user_y[0]=2;
    user_y[1]=-2;

    user_blip.setFillColor(sf::Color::Black);
    user_blip.setOrigin(20,20);
    user_blip.setPosition(500,500);

    hunter_blips.setFillColor(sf::Color::Red);
    hunter_blips.setOrigin(20,20);
    hunter_blips.setPosition(0,0);
    hunter_blips.setRadius(40);
    //draw background
    /*Texture grass;
    if (!grass.loadFromFile("grass_1.jpg"))
        return EXIT_FAILURE;
    Sprite sprite(grass);
    Vector2f backgroundSize(1000.0f, 1000.0f);
    sprite.setScale(backgroundSize.x/sprite.getLocalBounds().width,
        backgroundSize.y/sprite.getLocalBounds().height);*/

    // Start the game loop
    while (WINDOW.isOpen())
    {
        // Process events
        Event event;
        while (WINDOW.pollEvent(event))
        {
            // Close window: exit
            if (event.type == Event::Closed) {
                WINDOW.close();
            }

            // Escape pressed: exit
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                WINDOW.close();
            }
        }
        /*user_x[0]=2;        //fix this?
        user_x[1]=-2;
        user_y[0]=2;
        user_y[1]=-2;*/
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

        // Clear screen
        WINDOW.clear(Color(10,150,10));
        //WINDOW.clear();
        //WINDOW.draw(sprite);

        WINDOW.draw(user_blip);
        WINDOW.draw(hunter_blips);
        for (int i=0; i<rockGroup.size(); i++)
            WINDOW.draw(rockGroup[i]);
        for (int j=0; j<treeGroup.size(); j++)
            WINDOW.draw(treeGroup[j]);

        for (int k=0; k<treeTop.size(); k++)
            WINDOW.draw(treeTop[k]);

        checkCollision(treeCoords, rockCoords, user_x, user_y, rockDir, treeDir);

        WINDOW.display();
        counter++;
    }
    return EXIT_SUCCESS;
}

void buildEnvir(vector<CircleShape>& rockGroup, vector<RectangleShape>& treeGroup,
vector<Vector2f>& treeCoords, vector<Vector2f>& rockCoords) {
    const double rockRad = 10;
    const double treeWid = 20;
    const double treeHei = 100;
    CircleShape rock(rockRad);
    RectangleShape tree(Vector2f(treeWid,treeHei));
    double ranX, ranY, dist;
    int intersect;
    rock.setFillColor(Color(200,200,200));
    rock.setOrigin(rockRad, rockRad);
    tree.setFillColor(Color(102,51,0));
    tree.setOrigin(treeWid/2,treeHei/2);
    int ranRockNum = rand() % 10 + 1;   //create rand num of rocks between 1-10;
    int ranTreeNum = rand() % 10 + 1;
    for (int i=0; i<ranRockNum; i++) {
        rockGroup.push_back(rock);
        do {                        //set random positions for stones and make sure they don't overlap
            intersect = 0;
            ranX = rand() % 980 + rockRad;
            ranY = rand() % 580 + rockRad;      //change for 1000
            rockGroup[i].setPosition(ranX, ranY);
            for (int j=0; j<i; j++) {
                dist = sqrt((abs(rockGroup[i].getPosition().x-rockGroup[j].getPosition().x)
                    * abs(rockGroup[i].getPosition().x-rockGroup[j].getPosition().x))
                    + (abs(rockGroup[i].getPosition().y-rockGroup[j].getPosition().y)
                    * abs(rockGroup[i].getPosition().y-rockGroup[j].getPosition().y)));
                if (dist <= 20)
                    intersect++;
            }
        } while (intersect>0);
        rockCoords.push_back(rockGroup[i].getPosition());
    }
    for (int i=0; i<ranTreeNum; i++) {
        treeGroup.push_back(tree);
        do {
            intersect = 0;
            ranX = rand() % 980 + 10;
            ranY = rand() % 500 + 50;       //change for 1000
            treeGroup[i].setPosition(ranX, ranY);
            int xdist, ydist;
            for (int j=0; j<i; j++) {
                xdist = abs(treeGroup[i].getPosition().x - treeGroup[j].getPosition().x);
                ydist = abs(treeGroup[i].getPosition().y - treeGroup[j].getPosition().y);
                if (xdist <= 10 && ydist <= 50)
                    intersect++;
            }
        } while (intersect>0);
        treeCoords.push_back(treeGroup[i].getPosition());
    }
}

void drawTree(vector<CircleShape>& treeTop, vector<RectangleShape>& treeGroup) {
    double leafRad = 20;
    Vector2f position;
    CircleShape leaf(leafRad);
    leaf.setFillColor(Color::Green);
    leaf.setOrigin(leafRad, leafRad);
    int counter = 0;
    for (int j=0; j<treeGroup.size(); j++) {
        for (int i=0; i<3; i++) {
            treeTop.push_back(leaf);
        }
        position = treeGroup[j].getPosition();
        treeTop[counter].setPosition(position.x-15,position.y-20);
        treeTop[counter+1].setPosition(position.x+15,position.y-20);
        treeTop[counter+2].setPosition(position.x,position.y-40);
        counter = counter+3;
    }
}

void checkCollision(vector<Vector2f>& treeCoords, vector<Vector2f>& rockCoords,
int user_x[], int user_y[], int& rockDir, int&treeDir) {
//vector<bool>& rockCollision, vector<bool>& treeCollision) {
    Vector2f userPos = user_blip.getPosition();
    double rockDist;

    for (int i=0; i<rockCoords.size(); i++) {       //check for collision with rock
        rockDist = sqrt((abs(rockCoords[i].x-userPos.x)*abs(rockCoords[i].x-userPos.x))
                + (abs(rockCoords[i].y-userPos.y)*abs(rockCoords[i].y-userPos.y)));
        if (rockDist <= 30) {
            if (rockCoords[i].x-userPos.x > 0)
                rockDir = 1;
            else if (rockCoords[i].x-userPos.x < 0)
                rockDir = 2;
            else if (rockCoords[i].y-userPos.y > 0)
                rockDir = 3;
            else if (rockCoords[i].y-userPos.y < 0)
                rockDir = 4;
            else
                rockDir = 0;
            //rockCollision = true;
        }
    }
    for (int j=0; j<treeCoords.size(); j++) {
        if ((abs(userPos.x - treeCoords[j].x) <= 30) && (abs(userPos.y - treeCoords[j].y) <= 60)) {
            /*user_x[0] = 0;
            user_x[1] = 0;
            user_y[0] = 0;
            user_y[1] = 0;*/
            rockDir = 0;
            //treeCollision = true;
        }
    }
}
