#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SAT (Separated Axis Theorem) - Collision Detection!");
    window.setFramerateLimit(60);
	
	sf::RectangleShape rectangle(sf::Vector2f(200.f, 200.f));
    rectangle.setFillColor(sf::Color::White);
	// IMPORTANT: set the origin of the moving character to the center so when we use movement input to rotate, 
	// it will rotate around the center point of that geometry. 
	// Else it will default to the top left corner
	rectangle.setOrigin(100.f, 100.f);
	rectangle.setPosition(100.f, 100.f);

	// another stationary rectangle to observe the collision
	sf::RectangleShape rectangle_1(sf::Vector2f(200, 200));
	rectangle_1.setFillColor(sf::Color::Green);
	rectangle_1.setPosition(sf::Vector2f(500, 400));

	sf::Clock clock;
    while (window.isOpen())
    {	
		sf::Time timeSinceLastFrame = clock.restart(); // get the time since the last frame being produced
        sf::Event event;

		// simple event to check if the 'closed' button is hit
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		// -------------------------------------------
		// IMPLEMENTING MOUSE POSITION
		sf::Vector2i vMousePosition = sf::Mouse::getPosition(window);
		// std::cout << "x: " << vMousePosition.x << "| y: " << vMousePosition.y << std::endl;
		
		// -------------------------------------------
		// IMPLEMENTING Player Movement WITH DIRECTIONAL KEYBOARD
		sf::Vector2f vRequestedPlayerMovement(0.0f, 0.0f);
		float fSpeed = 150.0f * timeSinceLastFrame.asSeconds(); // speed constant to compensate for the slow movement rate when movign the player
		
		// movement input, gather info from keyboard press (up, down, left, right)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			rectangle.move(-1.0f * fSpeed, 0.0f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			rectangle.move(0.0f, -1.0f * fSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			rectangle.move(1.0f * fSpeed, 0.0f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			rectangle.move(0.0f, 1.0f*fSpeed);
		}

		// ROTATION for the Player
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			rectangle.rotate(-1.0f * fSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			rectangle.rotate(1.0f * fSpeed);
		}

		// -------------------------------------------------------
		// Separated Axis Theorem
		/*
		sf::Vector2f movingBoxPosition = rectangle.getPosition();
		sf::Vector2f stationBoxPosition = rectangle_1.getPosition();
		sf::Vector2f movingBoxSize = rectangle.getSize();
		sf::Vector2f stationBoxSize = rectangle_1.getSize();
		*/
		sf::FloatRect bounds1 = rectangle.getGlobalBounds();
		sf::FloatRect bounds2 = rectangle_1.getGlobalBounds();
		/*
		if(player1.x < player2.x + player2.width &&
			player1.x + player1.width > player2.x &&
			player1.y < player2.y + player2.height &&
			player1.y + player1.height > player2.y)
		{ cout << "Collision Detection!!!";
		}
		*/
		// Check for collision (intersection) between the two bounding boxes
        if (bounds1.intersects(bounds2)) {
            rectangle_1.setFillColor(sf::Color::Red);  // Change color to red on collision
		} 
		else{
			rectangle_1.setFillColor(sf::Color::Green);
		}

		// basic reset frame and animate(draw) the sprites/avatars/objects etc.
        window.clear();
		window.draw(rectangle_1);
		window.draw(rectangle); // NOTE: order of drawing will matter if collision, so the objects/sprites will overlap
		// make sure which objects to be prioritized or emerged that you want to be concentrated on
        window.display();
    }

    return 0;
}
/*
------------
SUMMARY:
This basic AABB collision detection works for non-rotated rectangular shapes. 
If you're working with rotated objects, you might need to consider other collision methods 
like the Separating Axis Theorem (SAT).
------------
*/