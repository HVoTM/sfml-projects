#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "AABB - Collision Detection!");
    sf::RectangleShape rectangle(sf::Vector2f(200, 200));
    rectangle.setFillColor(sf::Color::Red);
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
		float fSpeed = 150.0f; // speed constant to compensate for the slow movement rate when movign the player
		
		// movement input, gather info from keyboard press (up, down, left, right)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			vRequestedPlayerMovement += sf::Vector2f(-1.0f, 0.0f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			vRequestedPlayerMovement += sf::Vector2f(0.0f, -1.0f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			vRequestedPlayerMovement += sf::Vector2f(1.0f, 0.0f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			vRequestedPlayerMovement += sf::Vector2f(0.0f, 1.0f);
		}
		// ROTATIO
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			vRequestedPlayerMovement += sf::Vector2f(0.0f, 1.0f);
		}
		// define the new position of this object
		rectangle.move(vRequestedPlayerMovement*timeSinceLastFrame.asSeconds()*fSpeed);
		
		// -------------------------------------------------------
		// AABB COLLISION DETECTION
		sf::Vector2f movingBoxPosition = rectangle.getPosition();
		sf::Vector2f stationBoxPosition = rectangle_1.getPosition();
		/*
		if(player1.x < player2.x + player2.width &&
			player1.x + player1.width > player2.x &&
			player1.y < player2.y + player2.height &&
			player1.y + player1.height > player2.y)
		{ cout << "Collision Detection!!!";
		}
		*/
		// basic reset frame and animate(draw) the sprites/avatars/objects etc.
        window.clear();
		window.draw(rectangle_1);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}