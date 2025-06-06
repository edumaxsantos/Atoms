#include "element.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using std::vector;

struct Orbital
{
    string name;
    int capacity;
};

const vector<Orbital> ORBITALS = {
    {"1s", 2}, {"2s", 2}, {"2p", 6}, {"3s", 2}, {"3p", 6}, {"4s", 2}, {"3d", 10}, {"4p", 6}, {"5s", 2}, {"4d", 10}, {"5p", 6}, {"6s", 2}, {"4f", 14}, {"5d", 10}, {"6p", 6}, {"7s", 2}, {"5f", 14}, {"6d", 10}, {"7p", 6}};

string Element::getElectronConfiguration()
{
    string config;
    int remaining = this->getAtomicNumber();

    for (const auto &orb : ORBITALS)
    {
        if (remaining == 0)
            break;

        int electrons = std::min(remaining, orb.capacity);
        config += orb.name + std::to_string(electrons) + " ";
        remaining -= electrons;
    }

    return config;
}

vector<Element> Element::processElements()
{

    auto elements = vector<Element>();
    std::cout << "Processing elements" << std::endl;

    std::ifstream file("elements.json");
    if (!file)
    {
        std::cerr << "Failed to open file.\n";
        return elements;
    }

    nlohmann::json jsonData;

    file >> jsonData;

    for (const auto &elementJson : jsonData["elements"])
    {
        auto element = Element();
        element.setName(elementJson["name"]);
        element.setSymbol(elementJson["symbol"]);
        element.setAtomicNumber(elementJson["atomic_number"].get<uint8_t>());
        element.setAtomicMass(elementJson["atomic_mass"].get<float>());
        element.setAtomicRadius(elementJson["atomic_radius"].get<float>());
        element.setCategory(elementJson["category"]);
        elements.push_back(element);
    }

    std::cout << "Elements processed. Found: " << elements.size() << std::endl;

    return elements;
}

void Element::draw()
{
    DrawSphere(this->position, this->getRadius(), this->color);
}

Element::Element()
{
}

void Element::setName(string name)
{
    this->elementData.name = name;
}

void Element::setSymbol(string symbol)
{
    this->elementData.symbol = symbol;
}

void Element::setAtomicNumber(uint8_t atomicNumber)
{
    this->elementData.atomicNumber = atomicNumber;
}

void Element::setAtomicMass(float atomicMass)
{
    this->elementData.atomicMass = atomicMass;
}

void Element::setAtomicRadius(float atomicRadius)
{
    this->elementData.atomicRadius = atomicRadius;
}

void Element::setCategory(string category)
{
    this->elementData.category = category;
}

void Element::setPosition(Vector3 position)
{
    this->position = position;
}

void Element::setColor(Color color)
{
    this->color = color;
}

string Element::getName() const 
{
    return this->elementData.name;
}

string Element::getSymbol() const
{
    return this->elementData.symbol;
}

uint8_t Element::getAtomicNumber() const
{
    return this->elementData.atomicNumber;
}

float Element::getAtomicMass() const
{
    return this->elementData.atomicMass;
}

float Element::getAtomicRadius() const
{
    return this->elementData.atomicRadius;
}

string Element::getCategory() const
{
    return this->elementData.category;
}

Vector3 Element::getPosition() const
{
    return this->position;
}

Color Element::getColor() const
{
    return this->color;
}

float Element::getRadius() const
{
  return this->elementData.atomicRadius / 100;
}
