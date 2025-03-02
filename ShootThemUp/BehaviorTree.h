#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>

enum class NodeStatus {
    SUCCESS,
    FAILURE,
    RUNNING
};

class BehaviorNode {
public:
    virtual ~BehaviorNode() = default;
    virtual NodeStatus execute() = 0;
    virtual std::string getName() const = 0;
};

class Selector : public BehaviorNode {
private:
    std::vector<std::shared_ptr<BehaviorNode>> mChildren;
    size_t mCurrentChild = 0;

public:
    Selector(std::vector<std::shared_ptr<BehaviorNode>> children)
        : mChildren(std::move(children)) {
    }

    NodeStatus execute() override {
        if (mChildren.empty()) {
            return NodeStatus::FAILURE;
        }

        while (mCurrentChild < mChildren.size()) {
            NodeStatus status = mChildren[mCurrentChild]->execute();

            if (status == NodeStatus::RUNNING) {
                return NodeStatus::RUNNING;
            }

            if (status == NodeStatus::SUCCESS) {
                mCurrentChild = 0;
                return NodeStatus::SUCCESS;
            }

            mCurrentChild++;
        }

        mCurrentChild = 0;
        return NodeStatus::FAILURE;
    }

    std::string getName() const override {
        return "Selector";
    }
};

class Sequence : public BehaviorNode {
private:
    std::vector<std::shared_ptr<BehaviorNode>> mChildren;
    size_t mCurrentChild = 0;

public:
    Sequence(std::vector<std::shared_ptr<BehaviorNode>> children)
        : mChildren(std::move(children)) {
    }

    NodeStatus execute() override {
        if (mChildren.empty()) {
            return NodeStatus::SUCCESS;
        }

        while (mCurrentChild < mChildren.size()) {
            NodeStatus status = mChildren[mCurrentChild]->execute();

            if (status == NodeStatus::RUNNING) {
                return NodeStatus::RUNNING;
            }

            if (status == NodeStatus::FAILURE) {
                mCurrentChild = 0;
                return NodeStatus::FAILURE;
            }

            mCurrentChild++;
        }

        mCurrentChild = 0;
        return NodeStatus::SUCCESS;
    }

    std::string getName() const override {
        return "Sequence";
    }
};

class Action : public BehaviorNode {
private:
    std::function<NodeStatus()> mAction;
    std::string mName;

public:
    Action(std::function<NodeStatus()> action, std::string name = "Action")
        : mAction(std::move(action)), mName(std::move(name)) {
    }

    NodeStatus execute() override {
        return mAction();
    }

    std::string getName() const override {
        return mName;
    }
};

class Condition : public BehaviorNode {
private:
    std::function<bool()> mCondition;
    std::string mName;

public:
    Condition(std::function<bool()> condition, std::string name = "Condition")
        : mCondition(std::move(condition)), mName(std::move(name)) {
    }

    NodeStatus execute() override {
        return mCondition() ? NodeStatus::SUCCESS : NodeStatus::FAILURE;
    }

    std::string getName() const override {
        return mName;
    }
};