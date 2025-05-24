#include "EventHandler.h"

//void EventHandler::HandleEvent(const sf::Event& event, Window& window)
//{
    //if (event.is<sf::Event::Closed>())
    //{
    //    window.Close();
    //}

    //if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    //    
    //    switch (keyPressed->code) 
    //    {
    //        case sf::Keyboard::Key::Escape:
    //            window.Close();
    //            break;
    //        case sf::Keyboard::Key::Space:
    //            //Jump
    //            break;
    //        case sf::Keyboard::Key::A:
    //            //Move Left Requested on game
    //            break;
    //        case sf::Keyboard::Key::D:
    //            //Move Right Requested on game
    //            break;
    //        default:
    //            break;
    //    }
    //}
    ////// On Text Entered invokes subscribers
    ////if (const sf::Event::TextEntered* textEntered = event.getIf<sf::Event::TextEntered>())
    ////{
    ////    //std::cout << "OnTextEntered Event Invoked..." << std::endl;

    ////    onTextEntered.Invoke(textEntered->unicode);
    ////}
    //if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    //{
    //    switch (mousePressed->button)
    //    {
    //        case sf::Mouse::Button::Left:
    //            //std::cout << "Left mouse button pressed at: " << mousePressed->position.x << ", " << mousePressed->position.y << "onClick Event invoked" << std::endl;
    //            //onClick.Invoke(sf::Vector2f(mousePressed->position.x, mousePressed->position.y));

    //            break;
    //        default:
    //            break;
    //    }
    //}
//}
