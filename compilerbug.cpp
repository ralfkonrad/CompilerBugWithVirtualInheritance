//
// Created by ralf.eckel on 24.01.2023.
//
#include <iostream>
#include <memory>

class Abstract {
  public:
    Abstract(){};
    Abstract(const Abstract &other) : mA(other.mA){};

    virtual ~Abstract() = default;

    void setA(double inA) { mA = inA; };

    double getA() { return mA; };

    virtual void isAbstract() = 0;

  protected:
    double mA;
};

class Parent : public virtual Abstract {
  public:
    Parent(){};
    Parent(const Parent &other) : Abstract(other){};
};

class Child : public virtual Parent {
  public:
    Child(){};
    Child(const Child &other) : Parent(other){};

    void isAbstract(){};
};

int main() { return 0; }