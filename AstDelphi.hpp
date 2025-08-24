#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Базовый класс для всех узлов AST
class AST {
public:
    virtual ~AST() = default;  // Виртуальный деструктор
    virtual void print(int indent = 0) const = 0;  // Виртуальный метод для печати
};

class UnitNode : public AST
{
    std::vector<std::string> name;
public:
    UnitNode(std::vector<std::string> UnitName) { name = UnitName; };
    void print(int indent) const override {};
    ~UnitNode() override {};

private:
};




// Узел: объявление функции
class FunctionDeclNode : public AST {
    std::string name;
    std::vector<std::string> params;
    std::vector<AST*> body;  // Тело функции (список statement'ов)
public:
    FunctionDeclNode(const std::string& name, const std::vector<std::string>& params)
        : name(name), params(params) {
    }

    ~FunctionDeclNode() override { for (AST* stmt : body) delete stmt; }

    void addStatement(AST* stmt) { body.push_back(stmt); }

    void print(int indent) const override {
        std::cout << std::string(indent, ' ') << "Function: " << name << "(";
        for (size_t i = 0; i < params.size(); ++i) {
            if (i != 0) std::cout << ", ";
            std::cout << params[i];
        }
        std::cout << ")" << std::endl;

        for (const AST* stmt : body) {
            stmt->print(indent + 4);
        }
    }
};

// Узел: объявление переменной
class VarDeclNode : public AST {
    std::string name;
    std::string type;
public:
    VarDeclNode(const std::string& name, const std::string& type)
        : name(name), type(type) {
    }

    void print(int indent) const override {
        std::cout << std::string(indent, ' ') << "Var: " << name << ": " << type << std::endl;
    }
};

// Узел: if-условие
class IfStatementNode : public AST {
    AST* condition;
    AST* thenBlock;
    AST* elseBlock;
public:
    IfStatementNode(AST* cond, AST* then, AST* else_ = nullptr)
        : condition(cond), thenBlock(then), elseBlock(else_) {
    }

    ~IfStatementNode() override {
        delete condition;
        delete thenBlock;
        if (elseBlock) delete elseBlock;
    }

    void print(int indent) const override {
        std::cout << std::string(indent, ' ') << "If:" << std::endl;
        condition->print(indent + 4);
        std::cout << std::string(indent, ' ') << "Then:" << std::endl;
        thenBlock->print(indent + 4);
        if (elseBlock) {
            std::cout << std::string(indent, ' ') << "Else:" << std::endl;
            elseBlock->print(indent + 4);
        }
    }
};