#ifndef ACTION_H
#define ACTION_H

class QStandardItem;

#include "websitelocker.h"
using namespace WebsiteLocker;

namespace WebsiteLocker {
/**
 * @brief Abstract action that happen to the tree model.
 *
 * Every change in the tree model is represented as a specific action. Every action containe the data needed to represent
 * the actual event in the tree. Action is categories as RemoveAction and EditAction.
 * \see RemoveAction, EditAction.
 */
class Action
{
public:
    /**
     * @brief Actions have different type based on the real event happend on the tree model
     *
     */
    enum ActionType {
        NONE_ACTION, /**< represent non action. used internally for create dump action */
        INSERT_ACTION, /**< represent Insert action */
        UPDATE_ACTION, /**< represent update action */
        REMOVE_ACTION /**< represent remove action */
    };

/**
 * @brief construct an empty action.
 *
 * \todo remove the emtpy ctor from base and derived class.
 */
    Action();
/**
 * @brief Construct an action object.
 *
 * This is class is an abstract, so comiler will compline if you try to create object of this class.
 *
 * @param item is pointer to the tree item model
 * @param actionType is the type of the action.
 */
    Action(const Item* const item, ActionType actionType);
    /**
     * @brief virtual destructor function
     *
     */
    virtual ~Action() = 0;
    /**
     * @brief virtual print function used to dump the action content to the console screen
     *
     */
    virtual void print() = 0;
    /**
     * @brief Change the action type
     *
     * @param actionType
     */
    void setActionType(ActionType actionType) { m_actionType = actionType; }
    /**
     * @brief return the action type
     *
     * @return ActionType
     */
    ActionType actionType() const { return m_actionType; }
    /**
     * @brief Change the item in the action
     *
     * @param item
     */
    void setItem(const Item* item) { m_item = item; }
    /**
     * @brief Return item in the action
     *
     * @return const Item *
     */
    const Item* item() const { return m_item; }

protected:
    const Item* m_item; /**< pointer to the item */
    ActionType m_actionType; /**< type of the action */
};


/**
 * @brief
 *
 */
class EditAction : public Action
{
public:
/**
 * @brief
 *
 */
    EditAction();
/**
 * @brief
 *
 * @param item
 * @param actionType
 */
    EditAction(const Item* const item, ActionType actionType);
    /**
     * @brief
     *
     */
    void print();
};


/**
 * @brief Containe remove action data getted from the remove event on the tree model.
 *
 * This class containe all the information needed to represent remove event in the tree model.
 */
class RemoveAction : public Action
{
public:
/**
 * @brief Empty construction of remove action
 *
 * Create an empty remove action object
 */
    RemoveAction();
/**
 * @brief Construct remove action object
 *
 * Create a full object of a remove event happend to the tree model
 *
 * @param item is a pointer to the removed item.
 * @param actionType is the type of the action and its default value is Action::REMOVE_ACTION.
 */
    RemoveAction(const Item* const item, ActionType actionType = Action::REMOVE_ACTION);
    /**
     * @brief
     *
     */
    ~RemoveAction();
    /**
     * @brief dump the action content to console.
     *
     * this member function print all the data inside the remove action to the
     * console and it use a qdebug function.
     *
     * \todo Move the output functionality to other class.
     *
     * Example Usage:
     * @code
     *  action->print(); // print to the console output.
     * @endcode
     */
    void print();
};

/**
 * @brief
 *
 */
class ActionFactory
{
public:
    /**
     * @brief
     *
     * @param item
     * @param actionType
     */
    static Action* createAction(const Item* const item, Action::ActionType actionType);
};

}

#endif // ACTION_H
