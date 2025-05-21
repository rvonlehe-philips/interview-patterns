#include <memory>
#include <vector>

template< typename T >
class DrawStrategy
{
public:
    virtual ~DrawStrategy() = default;
    virtual void draw( T const& ) const = 0;
};

class Shape
{
public:
    virtual ~Shape() = default;
    virtual void draw( /*some arguments*/ ) const = 0;
    // ... Potentially other functions, e.g. a 'serialize()' member function
};

class Circle : public Shape
{
public:
    explicit Circle( double radius, std::unique_ptr<DrawStrategy<Circle>> drawer
                    )
        : radius_( radius )
        , drawer_( std::move(drawer) )
    {
        /* Checking that the given radius is valid and that
the given 'std::unique_ptr' is not a nullptr */
    }

    void draw( /*some arguments*/ ) const override
    {
        drawer_->draw( *this /*some more arguments*/ );
    }

    double radius() const { return radius_; }
private:
    double radius_;
    std::unique_ptr<DrawStrategy<Circle>> drawer_;
};

class Square : public Shape
{
public:
    explicit Square( double side, std::unique_ptr<DrawStrategy<Square>> drawer )
        : side_( side )
        , drawer_( std::move(drawer) )
    {
        /* Checking that the given side length is valid and that
the given std::unique_ptr instance is not nullptr */
    }

    void draw( /*some arguments*/ ) const override
    {
        drawer_->draw( *this /*some more arguments*/ );
    }

    double side() const { return side_; }

private:
    double side_;
    std::unique_ptr<DrawStrategy<Square>> drawer_;
};

class OpenGLCircleStrategy : public DrawStrategy<Circle>
{
    // ...
    virtual void draw(const Circle&) const override { /* todo */ }
};

class OpenGLSquareStrategy : public DrawStrategy<Square>
{
    // ...
    virtual void draw(const Square&) const override { /* todo */ }
};


int main()
{
    using Shapes = std::vector<std::unique_ptr<Shape>>;
    Shapes shapes{};

    shapes.emplace_back(
        std::make_unique<Circle>(
            2.3, std::make_unique<OpenGLCircleStrategy>(/*...red...*/) ) );
    shapes.emplace_back(
        std::make_unique<Square>(
            1.2, std::make_unique<OpenGLSquareStrategy>(/*...green...*/) ) );
    shapes.emplace_back(
        std::make_unique<Circle>(
            4.1, std::make_unique<OpenGLCircleStrategy>(/*...blue...*/) ) );

    // Drawing all shapes
    for( auto const& shape : shapes )
    {
        shape->draw( /*some arguments*/ );
    }

    return EXIT_SUCCESS;
}
