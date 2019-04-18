//=========================================================
// This is just the starting point for your final project.
// You are expected to modify and add classes/files as needed.
// The code below is the original code for our first graphics
// project (moving the little green ship). 
//========================================================
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
#include <SFML/Graphics.hpp>
using namespace sf; 
#include "multiMissile.h"
#include "multiBomb.h"

//============================================================
// Aaron Hill
// 4/6/19
// program 7 space invaders
//============================================================

void moveShip(Sprite& ship)
{
	const float DISTANCE = 5.0;

	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		// left arrow is pressed: move our ship left 5 pixels
		// 2nd parm is y direction. We don't want to move up/down, so it's zero.
		ship.move(-DISTANCE, 0);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		// right arrow is pressed: move our ship right 5 pixels
		ship.move(DISTANCE, 0);
	}
}


int main()
{
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "invaders from space");
	// Limit the framerate to 60 frames per second
	window.setFramerateLimit(60);

	Music music;
	if (!music.openFromFile("song.ogg"))
		return 0;

	music.play();

	// load textures from file into memory. This doesn't display anything yet.
	// Notice we do this *before* going into animation loop.
	Texture shipTexture;
	if (!shipTexture.loadFromFile("ship.png"))
	{
		cout << "Unable to load ship texture!" << endl;
		exit(EXIT_FAILURE);
	}
	//Vector2u shipSize = shipTexture.getSize();

	Texture starsTexture;
	if (!starsTexture.loadFromFile("stars.jpg"))
	{
		cout << "Unable to load stars texture!" << endl;
		exit(EXIT_FAILURE);
	}
	Texture alienTexture;
	if (!alienTexture.loadFromFile("alien.jpg"))
	{
		cout << "Unable to load alien texture!" << endl;
		exit(EXIT_FAILURE);
	}
	Texture bombTexture;
	if (!bombTexture.loadFromFile("bomb.png"))
	{
		cout << "Unable to load bomb texture!" << endl;
		exit(EXIT_FAILURE);
	}
	Texture missileTexture;
	if (!missileTexture.loadFromFile("missile.png"))
	{
		cout << "Unable to load missile texture!" << endl;
		exit(EXIT_FAILURE);
	}

	Sprite background;
	background.setTexture(starsTexture);
	//background.setScale(1.5, 1.5);

	Sprite ship;
	ship.setTexture(shipTexture);

	Sprite win;
	Texture winTexture;
	if (!winTexture.loadFromFile("win.png"))
	{
		cout << "Unable to load winner texture!" << endl;
		exit(EXIT_FAILURE);
	}
	win.setTexture(winTexture);

	Sprite lose;
	Texture loseTexture;
	if (!loseTexture.loadFromFile("lose.png"))
	{
		cout << "Unable to load loser texture!" << endl;
		exit(EXIT_FAILURE);
	}
	lose.setTexture(loseTexture);


	multiMissile missiles;
	alienList alienWave;
	multiBomb bombs;

	float shipX = window.getSize().x / 2.0f;
	float shipY = 550;

	ship.setPosition(shipX, shipY);
	alienWave.addAliens(alienTexture, 10, window.getSize());
	string gameState = "playing";


	while (window.isOpen())
	{

		// check all the window's events that were triggered since the last iteration of the loop
		// For now, we just need this so we can click on the window and close it
		Event event;

		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && gameState == "playing")
			{
				if (event.key.code == Keyboard::Space)
				{
					missiles.spawnMissile(missileTexture, ship.getPosition(), 2);
				}
			}
		}

		// draw background first, so everything that's drawn later 
		// will appear on top of background
		window.draw(background);

		// manage gamemode
		if (gameState == "playing")
		{
			moveShip(ship);
			window.draw(ship);

			if (!(alienWave.moveAlienWave(window, 0.3, shipY)))
			{
				gameState = "lose"; // if empty, game win
			}
			// render/move/check collisions for bombs
			if (bombs.bombTimer(300, 50))
			{
				bombs.spawnBomb(bombTexture, alienWave.getBombPos()); // hard code
			}
			if (bombs.moveBombs(window, ship.getGlobalBounds(), 0.7))
			{
				gameState = "lose";
			}
			// render/move/check collisions for missiles
			missiles.moveMissiles(window, alienWave, bombs);

			if ((gameState == "playing") && (alienWave.getWin()))
			{
				gameState = "win"; // change game state if all aliens defeated
			}
		}
		if (gameState == "win")
		{
			window.draw(win);

		}
		if (gameState == "lose")
		{
			window.draw(lose);
		}

		// end the current frame; this makes everything that we have 
		// already "drawn" actually show up on the screen
		window.display();

	} // end body of animation loop

	return 0;
}


