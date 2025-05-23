#include <functional>
#include <set>
#include <string>

template< typename Subject, typename StateTag >
class Observer
{
public:
    using OnUpdate = std::function<void(Subject const&,StateTag)>;
    // No virtual destructor necessary

    explicit Observer( OnUpdate onUpdate )
        : onUpdate_{ std::move(onUpdate) }
    {
        // Possibly respond on an invalid/empty std::function instance
    }

    //  Non-virtual update function
    void update( Subject const& subject, StateTag property )
    {
        onUpdate_( subject, property );
    }
private:
    OnUpdate onUpdate_;
};

class Person
{
public:
    enum StateChange
    {
        forenameChanged,
        surnameChanged,
        addressChanged
    };
    using PersonObserver = Observer<Person,StateChange>;
    explicit Person( std::string forename, std::string surname )
        : forename_{ std::move(forename) }
        , surname_{ std::move(surname) }
    {}
    bool attach( PersonObserver* observer );
    bool detach( PersonObserver* observer );
    void notify( StateChange property );
    void forename( std::string newForename );
    void surname ( std::string newSurname );
    void address ( std::string newAddress );
    std::string const& forename() const { return forename_; }
    std::string const& surname () const { return surname_; }
    std::string const& address () const { return address_; }
private:
    std::string forename_;
    std::string surname_;
    std::string address_;
    std::set<PersonObserver*> observers_;
};

bool Person::attach( PersonObserver* observer )
{
    auto [pos,success] = observers_.insert( observer );
    return success;
} bool Person::detach( PersonObserver* observer )
{
    return ( observers_.erase( observer ) > 0U );
}

void Person::notify( StateChange property )
{
    for( auto iter=begin(observers_); iter!=end(observers_); )
    {
        auto const pos = iter++;
        (*pos)->update(*this,property);
    }
}

void Person::forename( std::string newForename )
{
    forename_ = std::move(newForename);
    notify( forenameChanged );
}

void Person::surname( std::string newSurname )
{
    surname_ = std::move(newSurname);
    notify( surnameChanged );
}

void Person::address( std::string newAddress )
{
    address_ = std::move(newAddress);
    notify( addressChanged );
}

void propertyChanged( Person const& person, Person::StateChange property )
{
    if( property == Person::forenameChanged ||
        property == Person::surnameChanged )
    {
        // ... Respond to changed name
    }
}

int main()
{
    using PersonObserver = Observer<Person,Person::StateChange>;
    PersonObserver nameObserver( propertyChanged );
    PersonObserver addressObserver(
        [/*captured state*/]( Person const& person, Person::StateChange property
                             ){
            if( property == Person::addressChanged )
            {
                // ... Respond to changed address
            }
        } );
    Person homer( "Homer" , "Simpson" );
    Person marge( "Marge" , "Simpson" );
    Person monty( "Montgomery", "Burns" );
    // Attaching observers
    homer.attach( &nameObserver );
    marge.attach( &addressObserver );
    monty.attach( &addressObserver );
    // ...
    return EXIT_SUCCESS;
}
