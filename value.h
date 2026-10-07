#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

class Value {

    struct Node {
        double data = 0;
        double grad = 0;
        std::vector<std::shared_ptr<Node>> prev;
        std::function<void()> backward = []() {};
        std::string op;
    };

    std::shared_ptr<Node> node;

    Value(std::shared_ptr<Node> n) : node(std::move(n)) {}
    
public:
    Value(double data,
          std::vector<std::shared_ptr<Node>> children = {},
          std::string op = "")
        : node(std::make_shared<Node>()) {
        node->data = data;
        node->prev = std::move(children);
        node->op = std::move(op);
    }

    double& data() { return node->data; }
    double data() const { return node->data; }
    double& grad() { return node->grad; }
    double grad() const { return node->grad; }

    Value operator+(const Value& other) const {
        auto out = std::make_shared<Node>();
        out->data = node->data + other.node->data;
        out->prev = {node, other.node};
        out->op = "+";

        auto self_n = node;
        auto other_n = other.node;
        out->backward = [self_n, other_n, out]() {
            self_n->grad += out->grad;
            other_n->grad += out->grad;
        };
        return Value(out);
    }

    Value operator*(const Value& other) const {
        auto out = std::make_shared<Node>();
        out->data = node->data * other.node->data;
        out->prev = {node, other.node};
        out->op = "*";

        auto self_n = node;
        auto other_n = other.node;
        out->backward = [self_n, other_n, out]() {
            self_n->grad += other_n->data * out->grad;
            other_n->grad += self_n->data * out->grad;
        };
        return Value(out);
    }

    Value pow(double other) const {
        auto out = std::make_shared<Node>();
        out->data = std::pow(node->data, other);
        out->prev = {node};
        out->op = "**" + std::to_string(other);

        auto self_n = node;
        out->backward = [self_n, other, out]() {
            self_n->grad += (other * std::pow(self_n->data, other - 1.0)) * out->grad;
        };
        return Value(out);
    }

    Value relu() const {
        auto out = std::make_shared<Node>();
        out->data = node->data < 0 ? 0 : node->data;
        out->prev = {node};
        out->op = "ReLU";

        auto self_n = node;
        out->backward = [self_n, out]() {
            self_n->grad += (out->data > 0) * out->grad;
        };
        return Value(out);
    }

    void backward() {
        std::vector<std::shared_ptr<Node>> topo;
        std::set<Node*> visited;

        std::function<void(const std::shared_ptr<Node>&)> build_topo =
            [&](const std::shared_ptr<Node>& v) {
                if (visited.insert(v.get()).second) {
                    for (const auto& child : v->prev) {
                        build_topo(child);
                    }
                    topo.push_back(v);
                }
            };
        build_topo(node);


      

        node->grad = 1;
        for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
            std::cout << (*it) ->data << ", ";
            (*it)->backward();
        }
        std::cout << "\n";
    }

    Value operator-() const { return *this * Value(-1); }

    Value operator-(const Value& other) const { return *this + (-other); }

    Value operator/(const Value& other) const { return *this * other.pow(-1); }

    Value& operator+=(const Value& other) { return *this = *this + other; }
    Value& operator-=(const Value& other) { return *this = *this - other; }
    Value& operator*=(const Value& other) { return *this = *this * other; }
    Value& operator/=(const Value& other) { return *this = *this / other; }

    friend Value operator+(double lhs, const Value& rhs) { return Value(lhs) + rhs; }
    friend Value operator-(double lhs, const Value& rhs) { return Value(lhs) - rhs; }
    friend Value operator*(double lhs, const Value& rhs) { return Value(lhs) * rhs; }
    friend Value operator/(double lhs, const Value& rhs) { return Value(lhs) / rhs; }

    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        os << "Value(data=" << v.node->data << ", grad=" << v.node->grad << ")";
        return os;
    }
};

