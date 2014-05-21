#include <QStandardItem>
#include <QDebug>
#include "action.h"
#include "item.h"

namespace WebsiteLocker {

Action::Action() :
    m_item(0), m_actionType(Action::NONE_ACTION)
{}

Action::Action(const Item* const item, ActionType actionType):
    m_item(item), m_actionType(actionType)
{}

Action::~Action()
{}

EditAction::EditAction()
{}

EditAction::EditAction(const Item* const item, ActionType actionType) :
    Action(item, actionType)
{}

void EditAction::print() {
    QString output = "<EDIT> ";
    if (item()->itemType() == Item::WEB_ITEM)
        output += "<WEB>: " + item()->text();
    else if (item()->itemType() == Item::CAT_ITEM)
        output += "<CAT>: " + item()->text();

    output += "  Action Type: " + QString::number(actionType());

    qDebug() << output;
}

RemoveAction::RemoveAction()
{}

RemoveAction::RemoveAction(const Item* const item, ActionType actionType) :
    Action()
{
    setItem(new DataItem(item->id(),item->itemType()));
    setActionType(actionType);
}

RemoveAction::~RemoveAction()
{
    delete m_item;
}

void RemoveAction::print() {
    QString output = "<REMOVE> Id: " + QString::number(item()->id());
    output += "   Item Type: " + QString::number(item()->itemType());
    qDebug() << output;
}


Action* ActionFactory::createAction(const Item* const item, Action::ActionType actionType) {
    Action* action;

    if (actionType == Action::REMOVE_ACTION)
        action = new RemoveAction(item,actionType);
    else
        action = new EditAction(item,actionType);

    return action;
}

}
