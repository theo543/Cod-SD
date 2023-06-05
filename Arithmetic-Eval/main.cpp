#include <iostream>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <functional>

struct Token {
    enum Type {
        Number,
        Operator,
        OpenParenthesis,
        CloseParenthesis
    };
    Type type;
    std::string value;
    explicit Token(Type type, std::string value) : type(type), value(std::move(value)) {}
};

std::vector<Token> tokenise(const std::string& expr) {
    std::vector<Token> tokens;
    bool readingNumber = false;
    auto begin = expr.begin();
    auto iter = begin;
    const std::vector<char> whitespace = {' ', '\t', '\n', '\r'};
    const std::vector<char> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    const std::vector<char> operators = {'+', '-', '*', '/', '%'};
    const auto in = [](char c, const std::vector<char>& v) {
        return std::find(v.begin(), v.end(), c) != v.end();
    };
    const auto finishNumber = [&]() {
        if(readingNumber) {
            tokens.emplace_back(Token::Number, std::string(begin, iter));
            readingNumber = false;
        }
    };
    for(; iter != expr.end(); iter++) {
        if(in(*iter, whitespace)) {
            finishNumber();
            begin = iter + 1;
        } else if(in(*iter, digits)) {
            if(!readingNumber) {
                readingNumber = true;
                begin = iter;
            }
        } else if(in(*iter, operators)) {
            finishNumber();
            tokens.emplace_back(Token::Operator, std::string(iter, iter + 1));
            begin = iter + 1;
        } else if(*iter == '(') {
            finishNumber();
            tokens.emplace_back(Token::OpenParenthesis, std::string(iter, iter + 1));
            begin = iter + 1;
        } else if(*iter == ')') {
            finishNumber();
            tokens.emplace_back(Token::CloseParenthesis, std::string(iter, iter + 1));
            begin = iter + 1;
        } else {
            throw std::runtime_error("Unknown character");
        }
    }
    finishNumber();
    return tokens;
}

class SyntaxNode {
    public:
    virtual ~SyntaxNode() = default;
    virtual int evaluate() = 0;
};

class LiteralNode : public SyntaxNode {
    int value;
    public:
    explicit LiteralNode(int value) : value(value) {}
    int evaluate() override {
        return value;
    }
};

class BinaryOperatorNode : public SyntaxNode {
    std::unique_ptr<SyntaxNode> left, right;
    char op;
public:
    BinaryOperatorNode(std::unique_ptr<SyntaxNode> left, std::unique_ptr<SyntaxNode> right, char op) : left(std::move(left)), right(std::move(right)), op(op) {}
    int evaluate() override {
        switch(op) {
            case '+': return left->evaluate() + right->evaluate();
            case '-': return left->evaluate() - right->evaluate();
            case '*': return left->evaluate() * right->evaluate();
            case '/': return left->evaluate() / right->evaluate();
            case '%': return left->evaluate() % right->evaluate();
        }
        throw std::runtime_error("Unknown operator");
    };
};

int main() {
    while(true) {
        try {
            std::string expression;
            std::cout << "Enter an expression: ";
            std::getline(std::cin, expression);
            if (expression.starts_with("exit")) break;
            expression += '\n';
            auto tokens = tokenise(expression);
            auto iter = tokens.begin();
            const std::function<std::unique_ptr<SyntaxNode>()> read_node = [&]() -> std::unique_ptr<SyntaxNode> {
                if (iter->type == Token::Number) {
                    auto node = std::make_unique<LiteralNode>(std::stoi(iter->value));
                    iter++;
                    return node;
                }
                if (iter->type != Token::OpenParenthesis)
                    throw std::runtime_error("Expected number or open parenthesis");
                iter++;
                if (iter->type != Token::Operator) throw std::runtime_error("Expected operator");
                char op = iter->value[0];
                iter++;
                auto left = read_node(), right = read_node();
                if (iter->type != Token::CloseParenthesis) throw std::runtime_error("Expected close parenthesis");
                iter++;
                return std::make_unique<BinaryOperatorNode>(std::move(left), std::move(right), op);
            };
            auto root = read_node();
            std::cout << "Result: " << root->evaluate() << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
            continue;
        }
    }
}
