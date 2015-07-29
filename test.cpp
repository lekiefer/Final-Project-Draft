#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;
using namespace sf;

RenderWindow WINDOW(VideoMode(1000, 1000), "SFML window");
const double ROCKRAD = 10;

void buildenvir(vector<CircleShape>& a, vector<RectangleShape>& b);

int main()
{
    Clock clock;
    srand(time(NULL));

    //draw environmental obstacles
    vector<CircleShape> rockGroup;
    vector<RectangleShape> treeGroup;
    buildenvir(rockGroup, treeGroup);

    Texture grass;
    if (!grass.loadFromFile("grass_texture_four.png"))
        return EXIT_FAILURE;
    Sprite sprite(grass);
    Vector2f backgroundSize(1000.0f, 1000.0f);
    sprite.setScale(backgroundSize.x/sprite.getLocalBounds().width,
        backgroundSize.y/sprite.getLocalBounds().height);
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

        // Clear screen
        WINDOW.clear(Color(10,200,10));
        WINDOW.clear();
        WINDOW.draw(sprite);

        for (int i=0; i<rockGroup.size(); i++)
            WINDOW.draw(rockGroup[i]);
        for (int j=0; j<treeGroup.size(); j++)
            WINDOW.draw(treeGroup[j]);
        // Update the window

        WINDOW.display();
    }
    return EXIT_SUCCESS;
}

void buildenvir(vector<CircleShape>& rockGroup, vector<RectangleShape>& treeGroup) {
    CircleShape rock(ROCKRAD);
    RectangleShape tree(Vector2f(20,100));
    double ranX, ranY, dist;
    int intersect;
    rock.setFillColor(Color(200,200,200));
    rock.setOrigin(ROCKRAD, ROCKRAD);
    tree.setFillColor(Color(102,51,0));
    tree.setOrigin(10,50);
    int ranRockNum = rand() % 10 + 1;   //create rand num of rocks between 1-10;
    int ranTreeNum = rand() % 10 + 1;
    for (int i=0; i<ranRockNum; i++) {
        rockGroup.push_back(rock);
        do {                        //set random positions for stones and make sure they don't overlap
            intersect = 0;
            ranX = rand() % (980) + ROCKRAD;
            ranY = rand() % (480) + ROCKRAD;
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
    }
    for (int i=0; i<ranTreeNum; i++) {
        treeGroup.push_back(tree);
        do {                        //set random positions for stones and make sure they don't overlap
            intersect = 0;
            ranX = rand() % (980) + 10;
            ranY = rand() % (400) + 50;
            treeGroup[i].setPosition(ranX, ranY);
            for (int j=0; j<i; j++) {
                dist = sqrt((abs(treeGroup[i].getPosition().x-treeGroup[j].getPosition().x)
                    * abs(treeGroup[i].getPosition().x-treeGroup[j].getPosition().x))
                    + (abs(treeGroup[i].getPosition().y-treeGroup[j].getPosition().y)
                    * abs(treeGroup[i].getPosition().y-treeGroup[j].getPosition().y)));
                if (dist <= 20)
                    intersect++;
            }
        } while (intersect>0);
    }
}
