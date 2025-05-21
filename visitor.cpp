#include <variant>
#include <vector>

struct Point
{
    double x;
    double y;
};

class Square
{
public:
    explicit Square( double side )
        : side_( side )
    {
        /* Checking that the given side length is valid */
    }

    double side () const { return side_; }
    Point center() const { return center_; }

private:
    double side_;
    Point center_{};
};

class Circle
{
public:
    explicit Circle( double radius )
        : radius_( radius )
    {
        /* Checking that the given radius is valid */
    }

    double radius() const { return radius_; }
    Point center() const { return center_; }

private:
    double radius_;
    Point center_{};
};

struct Draw
{
    void operator()( Circle const& c ) const
    { /* ... Implementing the logic for drawing a circle ... */ }

    void operator()( Square const& s ) const
    { /* ... Implementing the logic for drawing a square ... */ }
};

using Shape = std::variant<Circle,Square>;
using Shapes = std::vector<Shape>;

void drawAllShapes( Shapes const& shapes )
{
    for( auto const& shape : shapes )
    {
        std::visit( Draw{}, shape );
    }
}

int main()
{
    Shapes shapes;
    shapes.emplace_back( Circle{ 2.3 } );
    shapes.emplace_back( Square{ 1.2 } );
    shapes.emplace_back( Circle{ 4.1 } );
    drawAllShapes( shapes );
    return EXIT_SUCCESS;
}
