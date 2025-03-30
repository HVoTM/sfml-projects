#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

std::vector<sf::Vector2f> getRectangleVertices(const sf::RectangleShape& rect) // Note, "&" to reference to the original object, so any change will be saved on the original object
{
	std::vector<sf::Vector2f> vertices(4); // initialize an array as the return argument

	// Get the Size of the rectangle
	sf::Vector2f size = rect.getSize();

	// Get the corners of the rectangle, called as vertices
	// this is an unrotated rectangle relative to the origins

	sf::Vector2f topLeft(0.f, 0.f);
    sf::Vector2f topRight(size.x, 0.f);
    sf::Vector2f bottomRight(size.x, size.y);
    sf::Vector2f bottomLeft(0.f, size.y);
	
	// Get the rectangle's transform (includes position and rotation)
    sf::Transform transform = rect.getTransform();

    // Apply the transform to the corners to get their world positions
    // from the current position for each corner of the rectangle
    // we will transform to the established unrotated corners above to get the world-based coordination
    vertices[0] = transform.transformPoint(topLeft);
    vertices[1] = transform.transformPoint(topRight);
    vertices[2] = transform.transformPoint(bottomRight);
    vertices[3] = transform.transformPoint(bottomLeft);

    return vertices;
}

std::vector<sf::Vector2f> getAxesToTest(const std::vector<sf::Vector2f>& vertices) {
    std::vector<sf::Vector2f> axes; // initialize an object to return

    for (size_t i = 0; i < vertices.size(); ++i) {
        // Get current and next vertex
        sf::Vector2f p1 = vertices[i];
        sf::Vector2f p2 = vertices[(i + 1) % vertices.size()];

        // Get the edge vector
        sf::Vector2f edge = p2 - p1;

        // Get the perpendicular vector (normal) -> which is the separating axis to test for overlap
        // I was confused if Vector2f has a normal function
        // I am just dumb, this is an object declaration with name 'normal'
        sf::Vector2f normal(-edge.y, edge.x);

        // Normalize the normal vectors by divding x and y component by the magnitude of the vector
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        normal /= length; // here is where we can use operator function on Vector2f, line 218 in Vector2.hpp 
        // finally store in the initialized array of vector2f
        axes.push_back(normal);
    }
    return axes;
}

sf::Vector2f projectRectangleOnAxis(const std::vector<sf::Vector2f>& vertices, const sf::Vector2f& axis) {
    float min = (vertices[0].x * axis.x + vertices[0].y * axis.y);
    float max = min;

    for (size_t i = 1; i < vertices.size(); ++i) {
        float projection = (vertices[i].x * axis.x + vertices[i].y * axis.y);
        if (projection < min) min = projection;
        if (projection > max) max = projection;
    }

    return sf::Vector2f(min, max);  // Return the min and max projection
}

bool overlapOnAxis(const sf::Vector2f& proj1, const sf::Vector2f& proj2) {
    return !(proj1.y < proj2.x || proj2.y < proj1.x);
}

// this function will only check if there is collision and return a boolean value 
bool checkSATCollision(const sf::RectangleShape &rect1, const sf::RectangleShape &rect2) {
    // Get the vertices of both rectangles
    std::vector<sf::Vector2f> vertices1 = getRectangleVertices(rect1);
    std::vector<sf::Vector2f> vertices2 = getRectangleVertices(rect2);

    // Get the axes to test from both rectangles
    std::vector<sf::Vector2f> axes1 = getAxesToTest(vertices1);
    std::vector<sf::Vector2f> axes2 = getAxesToTest(vertices2);

    // Test all axes from the first rectangle
    for (const auto& axis : axes1) { // this is C++ for-each loop
        sf::Vector2f proj1 = projectRectangleOnAxis(vertices1, axis);
        sf::Vector2f proj2 = projectRectangleOnAxis(vertices2, axis);

        if (!overlapOnAxis(proj1, proj2)) {
            return false;  // Separation found, no collision
        }
    }

    // Test all axes from the second rectangle
    for (const auto& axis : axes2) {
        sf::Vector2f proj1 = projectRectangleOnAxis(vertices1, axis);
        sf::Vector2f proj2 = projectRectangleOnAxis(vertices2, axis);

        if (!overlapOnAxis(proj1, proj2)) {
            return false;  // Separation found, no collision
        }
    }

    return true;  // No separation found, collision detected
}

float computeOverlap(const sf::Vector2f& proj1, const sf::Vector2f& proj2){
    return std::min(proj1.y, proj2.y) - std::max(proj1.x, proj2.x);
}

sf::Vector2f computeMTV(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2){
    // Get the vertices of both rectangles
    std::vector<sf::Vector2f> vertices1 = getRectangleVertices(rect1);
    std::vector<sf::Vector2f> vertices2 = getRectangleVertices(rect2);

    // Get the axes to test from both rectangles
    std::vector<sf::Vector2f> axes1 = getAxesToTest(vertices1);
    std::vector<sf::Vector2f> axes2 = getAxesToTest(vertices2);

    float smallestOverlap = std::numeric_limits<float>::infinity();
    sf::Vector2f mtvAxis;
    
    // Test all axes from the first rectangle
    for (const auto& axis : axes1) {
        sf::Vector2f proj1 = projectRectangleOnAxis(vertices1, axis);
        sf::Vector2f proj2 = projectRectangleOnAxis(vertices2, axis);

        if (!overlapOnAxis(proj1, proj2)) {
            return sf::Vector2f(0.0f, 0.0f);  // Separation found, no collision
        }
        

        // Compute the overlap, remember, the overlap is the minimum depth of collision
        float overlap = computeOverlap(proj1, proj2);
        if (overlap < smallestOverlap) {
            smallestOverlap = overlap;
            mtvAxis = axis;
        }
    }

    for (const auto& axis : axes2){
        sf::Vector2f proj1 = projectRectangleOnAxis(vertices1, axis);
        sf::Vector2f proj2 = projectRectangleOnAxis(vertices2, axis);

        if (!overlapOnAxis(proj1, proj2)) {
            return sf::Vector2f(0.f, 0.f);  // No collision
        }

        // Compute the overlap
        float overlap = computeOverlap(proj1, proj2);
        if (overlap < smallestOverlap) {
            smallestOverlap = overlap;
            mtvAxis = axis;
        }
    }
    // Return the MTV by multiplying the smallest overlap with the MTV axis
    return mtvAxis * smallestOverlap;
}

// Once we have found the MTV, we can decide how we split the translation between the two blocks
// decide if you want to move two object or one, NOTE: maybe also consider the elasticity in collision, but probably to complex to use in this case
void resolveCollision(sf::RectangleShape& rect1, sf::RectangleShape& rect2) {
    sf::Vector2f mtv = computeMTV(rect1, rect2);

    if (mtv != sf::Vector2f(0.f, 0.f)) {
        // Move each rectangle by half the MTV in opposite directions
        rect1.move(-mtv * 0.5f);
        rect2.move(mtv * 0.0f);
    }
}

// To make the physics realistic, use this optional helper function to adjust the velocities of the object after resolving the collision
// Reverse or Dampen the velocities along the axis of collision
void adjustVelocities(sf::Vector2f& velocity1, sf::Vector2f& velocity2, const sf::Vector2f& mtvAxis) {
    // Calculate the velocity components along the MTV axis for each rectangle
    float velocity1AlongMTV = velocity1.x * mtvAxis.x + velocity1.y * mtvAxis.y;
    float velocity2AlongMTV = velocity2.x * mtvAxis.x + velocity2.y * mtvAxis.y;

    // Reverse the velocity along the MTV axis to simulate a bounce effect
    if (velocity1AlongMTV > 0) {
        velocity1 -= mtvAxis * velocity1AlongMTV;
    }
    if (velocity2AlongMTV < 0) {
        velocity2 -= mtvAxis * velocity2AlongMTV;
    }
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SAT (Separated Axis Theorem) - Collision Detection!");
    window.setFramerateLimit(144);
	
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


	// ---- CONVEX SHAPE -----
	// WE WILL GET TO POLYGON COLLISION LATER IF NEEDED, LOL
	sf::ConvexShape polygon1;
	polygon1.setPointCount(4);
    polygon1.setPoint(0, sf::Vector2f(100.f, 100.f));
    polygon1.setPoint(1, sf::Vector2f(200.f, 100.f));
    polygon1.setPoint(2, sf::Vector2f(200.f, 200.f));
    polygon1.setPoint(3, sf::Vector2f(100.f, 200.f));
	polygon1.setOrigin(150.f, 150.f); polygon1.setPosition(150.f, 150.f);
    polygon1.setFillColor(sf::Color::Green);
	//--------------------------
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
		float fSpeed = 100.0f * timeSinceLastFrame.asSeconds(); // speed constant to compensate for the slow movement rate when movign the player
		
        sf::Vector2f AvatarVelocity;
        sf::Vector2f TargetVelocity(0.0f, 0.0f); // we will preset this to 0.0 for now

		// movement input, gather info from keyboard press (up, down, left, right)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			// rectangle.move(-1.0f * fSpeed, 0.0f);
            AvatarVelocity = sf::Vector2f(-1.0f * fSpeed, 0.0f);

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			// rectangle.move(0.0f, -1.0f * fSpeed);
            AvatarVelocity = sf::Vector2f(0.0f, -1.0f * fSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			// rectangle.move(1.0f * fSpeed, 0.0f);
            AvatarVelocity = sf::Vector2f(1.0f * fSpeed, 0.0f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			// rectangle.move(0.0f, 1.0f*fSpeed);
            AvatarVelocity = sf::Vector2f(0.0f, 1.0f *fSpeed);
		}
        // get the received input to move the rectangle 
        rectangle.move(AvatarVelocity);

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
		// Check for collision using SAT
        /*
        if (checkSATCollision(rectangle, rectangle_1)) {
            rectangle_1.setFillColor(sf::Color::Red);  // Collision detected
        } else {
            rectangle_1.setFillColor(sf::Color::Green);  // No collision
        } 
        */
        resolveCollision(rectangle, rectangle_1);
        // adjust the velocities, we will have the target block stationary for the sake of simplicity
        // if you want, just use an available game engine to resolve collision
        adjustVelocities(AvatarVelocity, TargetVelocity, computeMTV(rectangle, rectangle_1));

		// basic reset frame and animate(draw) the sprites/avatars/objects etc.
        window.clear();
		window.draw(rectangle_1);
		window.draw(rectangle); // NOTE: order of drawing will matter if collision, so the objects/sprites will overlap
		// make sure which objects to be prioritized or emerged that you want to be concentrated on
        window.display();

        // Reset Velocities, note: potential bug
        AvatarVelocity = sf::Vector2f(0.0f, 0.0f);
    }
    return 0;
}