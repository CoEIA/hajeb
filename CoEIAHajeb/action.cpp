/*
* Copyright (C) 2014 - Center of Excellence in Information Assurance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
