#ifndef ELEMENT_H
#define ELEMENT_H
#include <string>
#include <cstdint>
#include <vector>
#include <raylib.h>

using std::vector, std::string;

struct ElementData
{
    string name;
    string symbol;
    uint8_t atomicNumber;
    float atomicMass;
    float atomicRadius;
    string category;
};

class Element
{
private:
    ElementData elementData;
    Vector3 position;
    Color color;

public:
    static vector<Element> processElements();
    Element();
    string getElectronConfiguration();
    void setName(string name);
    void setSymbol(string symbol);
    void setAtomicNumber(uint8_t atomicNumber);
    void setAtomicMass(float atomicMass);
    void setAtomicRadius(float atomicRadius);
    void setCategory(string category);
    void setPosition(Vector3 position);
    void setColor(Color color);
    string getName() const;
    string getSymbol() const;
    uint8_t getAtomicNumber() const;
    float getAtomicMass() const;
    float getAtomicRadius() const;
    string getCategory() const;
    Vector3 getPosition() const;
    Color getColor() const;
    float getRadius() const;
    void draw();
};

#endif // ELEMENT_H