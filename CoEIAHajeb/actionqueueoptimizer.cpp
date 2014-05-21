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
#include "actionqueueoptimizer.h"
#include "actionqueue.h"
#include <QDebug>

namespace WebsiteLocker {


ActionQueueOptimizer::ActionQueueOptimizer()
{
}

void ActionQueueOptimizer::setActionQueue(ActionQueue *queue) {
    actionQueue = queue;
}

void ActionQueueOptimizer::addAction(Action *action) {
    // find similar action
    // if no similar and the current is REMOVE|INSERT|UPDATE, then add current action to the list.
    // if there is a similar action and the current action is REMOVE_ACTION, then drop the similar action from the list.
    // if there is a similar action and the current action is UPDATE, then don't add anything (ignore current).

    // Find similar action in the queue
    Action* similarAction = actionQueue->findAction(action);
    if (similarAction == NULL) {
        appendAction(action);
        return ;
    }

    int index = actionQueue->indexOf(similarAction);
    if (index == -1)
        return ;

    // if current action is REMOVE_ACTION and the similar is insert, then drop the similar action
    if (action->actionType() == Action::REMOVE_ACTION && similarAction->actionType() == Action::INSERT_ACTION) {
        qDebug() << "REMOVE THE FOLLOWING ACTION FROM THE LIST";
        similarAction->print();
        removeAction(index);
    } else if (action->actionType() == Action::REMOVE_ACTION && similarAction->actionType() == Action::UPDATE_ACTION) {
        qDebug() << "REMOVE THE FOLLOWING ACTION FROM THE LIST";
        similarAction->print();
        removeAction(index);
        appendAction(action);
    }

    // This is will be an UPDATE_ACTION, simply ignore it.
}

void ActionQueueOptimizer::removeAction(int index) {
//    delete m_queue->actionList[index];
    actionQueue->m_queue.removeAt(index);
}

void ActionQueueOptimizer::appendAction(Action* action) {
    actionQueue->m_queue.append(action);
//    action->print();

    // print the list
    actionQueue->printQueue();
}

}


