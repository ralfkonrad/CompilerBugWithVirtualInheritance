#include <iostream>
#include <memory>

class Abstract {
  public:
    virtual ~Abstract() = default;
};

class Parent : public virtual Abstract {};

class ChildWithNoProblem : public Parent {
  public:
    ChildWithNoProblem() = default;
    explicit ChildWithNoProblem(const std::shared_ptr<Parent>& parent)
    : ChildWithNoProblem(*makeChild(parent)) {}

  private:
    static std::shared_ptr<ChildWithNoProblem> makeChild(const std::shared_ptr<Parent>& parent) {
        std::cout << "\t Casting ChildWithNoProblem..." << std::endl;
        auto child = std::dynamic_pointer_cast<ChildWithNoProblem>(parent);
        std::cout << "\t Done..." << std::endl;
        return std::make_shared<ChildWithNoProblem>();
    }
};

class ChildWithProblem : public Parent {
  public:
    ChildWithProblem() = default;
    explicit ChildWithProblem(const std::shared_ptr<Parent>& parent)
    : ChildWithProblem(makeChild(parent)) {}

  private:
    static ChildWithProblem makeChild(const std::shared_ptr<Parent>& parent) {
        std::cout << "\t Casting ChildWithProblem..." << std::endl;
        auto child = std::dynamic_pointer_cast<ChildWithProblem>(parent);
        std::cout << "\t Done..." << std::endl;
        return {};
    }
};

int main() {
    try {
        std::cout << "Running ChildWithNoProblem..." << std::endl;
        auto childWithNoProblem = std::make_shared<ChildWithNoProblem>();
        auto copiedChildWithNoProblem = ChildWithNoProblem(childWithNoProblem);

        std::cout << "Running ChildWithProblem..." << std::endl;
        auto childWithProblem = std::make_shared<ChildWithProblem>();
        auto copiedChildWithProblem = ChildWithProblem(childWithProblem);
    } catch (...) {
        std::cout << "We have an error..." << std::endl;
    }
    std::cout << "We never get here..." << std::endl;
}
