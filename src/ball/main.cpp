#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

class Ball: public sf::CircleShape
{
    public:
        sf::CircleShape shape;
        sf::Vector2f speed;
        Ball(){
            // set random color

            // set prefixed size and shape (Or Random)

            // set Position

            // set Speed
        }
        Ball(float radius, float x, float y, float xspeed, float yspeed, sf::Color color_)
        {
            shape.setRadius(radius);
            shape.setFillColor(color_);
            shape.setPosition(x, y);
            speed = sf::Vector2f(xspeed, yspeed);
        }
        void checkCollision(Ball& other_ball)
        {
            sf::Vector2f pos1 = shape.getPosition();
            sf::Vector2f pos2 = other_ball.getPosition();

            // Calculate the distance between the two balls
            float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2));
            float radiusSum = shape.getRadius() + other_ball.shape.getRadius();

            // If balls collide, reverse their velocities
            if (distance < radiusSum) 
            {
                // Swap velocities upon collision
                std::swap(speed, other_ball.speed);
            }

        }
        void move(const sf::RenderWindow& window)
        {
            // General understanding:
            // 1. Move the ball first with the current
            // 2. Check for any collision with the window then update the speed
            // ## Enhancement: add bouncier, randomized trajectory, which can make it feels realistic
            shape.move(speed);

            // get the ball current position
            sf::Vector2f position = shape.getPosition();
            float ball_radius = shape.getRadius();
            float windowWidth = window.getSize().x;
            float windowHeight =  window.getSize().y;

            // Check for collision with the window edges and bounce (reverse the speed polarity)
            // One thing to note in here is that the origin of this sprite(or the shape) is on the upper left corner
            // Sometimes, we want to put the origin, which can be interpreted as the center point, in the center, duh obviously
            // just be careful when you want to configure the origin or something like that, but this works nonetheless
            if (position.x <= 0 || position.x + ball_radius * 2 >= windowWidth) speed.x = -speed.x; 
            if (position.y <= 0 || position.y + ball_radius * 2 >= windowHeight) speed.y = -speed.y;  
        }      
};

int main() 
{
    // Create a window with 800x600 resolution
    sf::RenderWindow window(sf::VideoMode(1280, 720), "!!Bouncing Ball(s)!!");

    // Create multiple balls
    std::vector<Ball> balls = {
        Ball(20.f, 100.f, 100.f, 0.3f, 0.4f, sf::Color::Red),
        Ball(30.f, 300.f, 200.f, -0.5f, 0.3f, sf::Color::Blue),
        Ball(25.f, 500.f, 300.f, 0.6f, -0.4f, sf::Color::Green),
        Ball(15.f, 150.f, 400.f, -0.4f, 0.6f, sf::Color::Yellow),
        Ball(45.f, 35.f, 12.f, 0.5f, -1.3f, sf::Color::Magenta)
    };


    sf::Clock clock;
    // main loop of the sfml screen
    while (window.isOpen()) 
    {
        // Process events
        sf::Time timeSinceLastFrame = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		float fSpeed = 100.0f * timeSinceLastFrame.asSeconds(); // speed constant to compensate for the slow movement rate when movign the player

        // Update the position of each ball
        for (int i = 0; i < balls.size(); i++) 
        {
            balls[i].move(window);
            // Check for collision with every other ball
            for (int j = i + 1; j < balls.size(); j++) 
            {
                balls[i].checkCollision(balls[j]);
            }
        }
        // Clear the window
        window.clear();
        // Draw all the balls
        for (const auto& ball : balls) 
        {
            window.draw(ball.shape);
        }
        // Display the updated content
        window.display();
    }
    return 0;
}

/*
NOTE FOR DEVELOPMENT
- Understand the ball-ball collision
    - Euclidean distance formula, other collision theories

- Enhancements
    - Collision Response: Currently, this example simply swaps the velocities for a basic collision response. 
    For more accurate physics, you could calculate the collision normal and adjust the velocity components 
    accordingly.
    - Frame-rate Independence: You could add a time delta (sf::Clock) to ensure the ball's movement 
    is consistent across different hardware.
    - Random Ball Generation: Instead of hardcoding ball positions and velocities, 
    you could generate them randomly.
    - RGB Color Changing Shifting/Fading? Like RGB Light stuff on each of the ball
    - Using mouse click to generate more balls
        - Need to know how to manage all this data structure of the ball
*/