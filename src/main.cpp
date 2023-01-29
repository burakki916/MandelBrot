#include "Window.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>
class MandelBrot
{
  private:
    Window window;
    sf::Color hsv(int hue, float sat, float val)
    {
        hue %= 360;
        while (hue<0) hue += 360;

        if (sat<0.f) sat = 0.f;
        if (sat>1.f) sat = 1.f;

        if (val<0.f) val = 0.f;
        if (val>1.f) val = 1.f;

        int h = hue / 60;
        float f = float(hue) / 60 - h;
        float p = val*(1.f - sat);
        float q = val*(1.f - sat*f);
        float t = val*(1.f - sat*(1 - f));

        switch (h)
        {
        default:
        case 0:
        case 6: return sf::Color(val * 255, t * 255, p * 255);
        case 1: return sf::Color(q * 255, val * 255, p * 255);
        case 2: return sf::Color(p * 255, val * 255, t * 255);
        case 3: return sf::Color(p * 255, q * 255, val * 255);
        case 4: return sf::Color(t * 255, p * 255, val * 255);
        case 5: return sf::Color(val * 255, p * 255, q * 255);
        }
    }
    float xrange[2] = {-3, 1};
    float yrange[2] = {-1, 1};
    sf::Vector2f centerGraphCords = sf::Vector2f(0,0);
    sf::Vector2u resolution = sf::Vector2u(1920,1080);
    sf::Vector2f screenPointToGraph(sf::Vector2f screenCords)
    {
        sf::Vector2f percentagePosition;
        percentagePosition.x = screenCords.x / resolution.x;
        percentagePosition.y = screenCords.y / resolution.y;

        float xgraphChord = (xrange[1] - xrange[0]) * percentagePosition.x + xrange[0];
        float ygraphChord = (yrange[1] - yrange[0]) * percentagePosition.y + yrange[0];

        return sf::Vector2f(xgraphChord, ygraphChord);
    }
    int iterations = 100;
    int yeahItsProbInfinityTreshHold = 200;
    bool isInMandelBrotSet(sf::Vector2f position)
    {
        sf::Vector2f tempPosition = position;
        for (int i = 0; i < iterations; i++)
        {
            if (magnitudeOfComplexNumber(tempPosition) > yeahItsProbInfinityTreshHold)
            {
                return false;
            }
            tempPosition = mandelBrotFunction(tempPosition, position);
        }
        return true; 
    }
    sf::Color MBSColor(sf::Vector2f position){
        sf::Vector2f tempPosition = position;
        for (int i = 0; i < iterations; i++)
        {
            tempPosition = mandelBrotFunction(tempPosition, position);
        }
        float magnituteOfPoint = magnitudeOfComplexNumber(tempPosition); 
        float hue = colorMapper(magnituteOfPoint); 
        return hsv(hue, 1,1); 
    }
    int colorMapper(float magnitude){
        // F : (0,infinity) -> (0,255) in N 
        float MaxValue = 255; 
        float Smoothing = 0.5; 
        float offset = 0;
        return (MaxValue)/(1+pow(3,-(Smoothing*magnitude-offset)));
        //int returnValue = 255 * pow(sin(magnitude),2);
        //return returnValue;

    }
    sf::Vector2f mandelBrotFunction(sf::Vector2f position, sf::Vector2f original)
    {
        float xValue = position.x * position.x - position.y * position.y;
        xValue += original.x;
        float yValue = 2 * position.x * position.y;
        yValue += original.y;
        return sf::Vector2f(xValue, yValue);
    }
    float magnitudeOfComplexNumber(sf::Vector2f position)
    {
        return sqrt(pow(position.x, 2) + pow(position.y, 2));
    }
    
  public:
    MandelBrot() : window("MandelBrot YUH", sf::Vector2u(1920, 1080))
    {

    }
    void render()
    {
        window.Update(); 
        window.BeginDraw();


        for (int y = 0; y < resolution.y; y++)
        {
            for (int x = 0; x < resolution.x; x++)
            {
                //std::cout << "checking cordinate" << resolution.x << "," << resolution.y << std::endl; 
                //if (isInMandelBrotSet(screenPointToGraph(sf::Vector2f(x, y))))
                {   
                    sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
                    rectangle.setPosition(sf::Vector2f(x, y));
                    rectangle.setFillColor(MBSColor(screenPointToGraph(sf::Vector2f(x, y)))); 
                    window.Draw(rectangle);
                }
            }
        }
                    window.EndDraw(); 
        
    }
};

int main()
{
    MandelBrot mbBaby;
    mbBaby.render();
    std::cin.get();

}
