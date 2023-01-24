//
// Created by ralf.eckel on 24.01.2023.
//

#include <exception>
#include <iostream>
#include <memory>

class Abstract {
  public:
    explicit Abstract() : a_(0) {}
    Abstract(const Abstract&) {}
    Abstract& operator=(const Abstract&) { return *this; }
    virtual ~Abstract() = default;

    void increaseA() { a_++; }
    int a() { return a_; }

  private:
    int a_;
};

class Parent : public virtual Abstract {
  public:
    explicit Parent(int p) : p_(p) {}

    int p() { return p_; }

  private:
    int p_;
};

class Child : public Parent {
  public:
    explicit Child(int p) : Parent(p) {}
    explicit Child(const std::shared_ptr<Parent>& parent) : Child(makeChild(parent)) {}

  private:
    static Child makeChild(const std::shared_ptr<Parent>& parent) {
        auto child = std::dynamic_pointer_cast<Child>(parent);
        if (child == nullptr) {
            throw std::exception("No child given");
        }
        return Child(child->p());
    }
};

int main() {
    auto child = std::make_shared<Child>(42);
    auto copiedChild = Child(child);
}
