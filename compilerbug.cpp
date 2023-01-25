#include <exception>
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
        auto child = std::dynamic_pointer_cast<ChildWithNoProblem>(parent);
        if (child == nullptr) {
            throw std::exception("No child given");
        }
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
        auto child = std::dynamic_pointer_cast<ChildWithProblem>(parent);
        if (child == nullptr) {
            throw std::exception("No child given");
        }
        return {};
    }
};

int main() {
    auto childWithNoProblem = std::make_shared<ChildWithNoProblem>();
    auto copiedChildWithNoProblem = ChildWithNoProblem(childWithNoProblem);

    auto childWithProblem = std::make_shared<ChildWithProblem>();
    auto copiedChildWithProblem = ChildWithProblem(childWithProblem);
}
