#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

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
            shape.setOrigin(radius, radius);
            shape.setFillColor(color_);
            shape.setPosition(x, y);
            speed = sf::Vector2f(xspeed, yspeed);
        }
        void checkCollision(Ball& other_ball)
        {
            sf::Vector2f pos1 = shape.getPosition();
            sf::Vector2f pos2 = other_ball.getPosition();

            float distX = pos1.x - pos2.x;
            float distY = pos1.y - pos2.y;
            // Calculate the distance between the two balls
            float distance = sqrtf(powf(pos1.x - pos2.x, 2.0f) + powf(pos1.y - pos2.y, 2.0f));
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

bool CircleCircleCollision(sf::CircleShape& circle1, sf::CircleShape& circle2){
    sf::Vector2f circle1_position = circle1.getPosition();
    sf::Vector2f circle2_position = circle2.getPosition();

    float distance = sqrtf(powf(circle1_position.x - circle2_position.x, 2.0f) + powf(circle1_position.y - circle2_position.y, 2.0f));

    if (distance <= (circle1.getRadius() + circle2.getRadius()))
    {
        return true;
    }
    else return false;
}

void CollisionResponse(sf::CircleShape& circle1, sf::CircleShape& circle2){
    sf::Vector2f circle1_position = circle1.getPosition();
    sf::Vector2f circle2_position = circle2.getPosition();
    
    float distance = sqrtf(powf(circle1_position.x - circle2_position.x, 2.0f) + powf(circle1_position.y - circle2_position.y, 2.0f));

    // division constant
    sf::Vector2f division_constant(2.0f, 2.0f);
    sf::Vector2f midpoint = sf::Vector2f((circle1_position.x + circle2_position.x)/2, (circle1_position.y + circle2_position.y)/2);


    // set the new responsive position for the circle
    circle1_position.x = midpoint.x + circle1.getRadius() * (circle1_position.x - circle2_position.x) / distance;
    circle1_position.y = midpoint.y + circle1.getRadius() * (circle1_position.y - circle2_position.y) / distance;
    circle2_position.x = midpoint.x + circle2.getRadius() * (circle1_position.x - circle2_position.x) / distance;
    circle2_position.y = midpoint.y + circle2.getRadius() * (circle1_position.y - circle2_position.y) / distance;
    
    // set the new center of the circle to be R radius away from p 
}
void reportPositionofCircle(const sf::CircleShape& circle)
{
    sf::Vector2f position = circle.getPosition();
    std::cout << "Position x: " << position.x << "| position y: " << position.y << std::endl;
    sf::Vector2f origin_position = circle.getOrigin();
    std::cout << "Origin Position x: " << origin_position.x << "| Origin position y: " << origin_position.y << std::endl;
}

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
    

    // Collision between circle and circle to implement the bouncing balls program
    sf::CircleShape ball1(100.f);
    ball1.setFillColor(sf::Color::Cyan);
    ball1.setOrigin(100.f, 100.f);
    ball1.setPosition(50.f, 100.f);

    // NOTE: be careful with setting the origin of any shape, including the circle
    // if we are considering the center point to work with collision and other physical interaction
    // here we will need to preset the origin to the center of the circle
    // radius of 50.f so we will set the origin to (50.f, 50.f)
    // notet that the origin is in accordance to the local shape, not the running window
    sf::CircleShape ball2(150.f);
    ball2.setFillColor(sf::Color::Green);
    ball2.setOrigin(150.f, 150.f);
    ball2.setPosition(500.f, 300.f);

    sf::Clock clock;
    // MAIN LOOP
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

        // --------------------------------------------
        // Report position of the circle to check status
        // 
        reportPositionofCircle(ball1);
        // -------------------------------------------
		// IMPLEMENTING Player Movement WITH DIRECTIONAL KEYBOARD
		sf::Vector2f vRequestedPlayerMovement(0.0f, 0.0f);

		// movement input, gather info from keyboard press (up, down, left, right)
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
		{
			vRequestedPlayerMovement += sf::Vector2f(-1.0f, 0.0f);
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
		{
			vRequestedPlayerMovement += sf::Vector2f(0.0f, -1.0f);
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
		{
			vRequestedPlayerMovement += sf::Vector2f(1.0f, 0.0f);
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			vRequestedPlayerMovement += sf::Vector2f(0.0f, 1.0f);
		}
		// define the new position of this object
		ball1.move(vRequestedPlayerMovement*fSpeed);
		
        bool collision_detected = CircleCircleCollision(ball1, ball2);
        if (collision_detected){
            ball2.setFillColor(sf::Color::Red);
        }
        else ball2.setFillColor(sf::Color::Green);

        // Clear the window
        window.clear();
        window.draw(ball2); // draw ball 1 after since we want to prioritize the visibility of our moving avatar
        window.draw(ball1);
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