#include <QDebug>
#include "actionqueue.h"
#include "item.h"

namespace WebsiteLocker {

ActionQueue::ActionQueue() :
    m_optimizer(new ActionQueueOptimizer)
{
    m_optimizer->setActionQueue(this);
}


ActionQueue::~ActionQueue()
{
    clear();
    delete m_optimizer;
}

void ActionQueue::clear() {
    qDeleteAll(m_queue.begin(),m_queue.end());
    m_queue.clear();
}

void ActionQueue::addAction(Action* action) {
    m_optimizer->addAction(action);
}

int ActionQueue::indexOf(Action *action) {
    for (int i=0; i<m_queue.count(); ++i) {
        if (m_queue.at(i) == action)
            return i;
    }

    return -1; // Error!
}

void ActionQueue::printQueue() const {
    qDebug() << "####################################";
    qDebug() << "             ACTION LIST            ";
    qDebug() << "####################################";
    QList<Action*>::const_iterator cItor;
    for (cItor=m_queue.constBegin(); cItor!=m_queue.constEnd(); ++cItor) {
        (*cItor)->print();
    }
    qDebug() << "####################################";
}

Action * ActionQueue::findAction(Action *action)  {
    if (action->actionType() == Action::INSERT_ACTION)
        return NULL;

    if (action->actionType() == Action::REMOVE_ACTION)
        return findRemoveAction(dynamic_cast<RemoveAction*>(action));
    else
        return findEditAction(dynamic_cast<EditAction*>(action));
}

Action* ActionQueue::findRemoveAction(RemoveAction *action)  {
    // search the queue
    QList<Action*>::iterator cItor;
    for (cItor=m_queue.begin(); cItor!=m_queue.end(); ++cItor) {
        if (EditAction* currentAction = dynamic_cast<EditAction*>(*cItor)) {
            if ((currentAction->item()->id() == action->item()->id()) && (currentAction->item()->itemType() == action->item()->itemType()))
                return currentAction;
        }
    }

    return NULL; //ERORR!
}

Action* ActionQueue::findEditAction(EditAction* action)  {
    // search the queue
    QList<Action*>::iterator cItor;
    for (cItor=m_queue.begin(); cItor!=m_queue.end(); ++cItor) {
        if (EditAction* currentAction = dynamic_cast<EditAction*>(*cItor)) {
            if (currentAction->item() == action->item())
                return currentAction;
        }
    }

    return NULL; //ERORR!
}

const QList<Action *> & ActionQueue::actionQueue() const {
    return m_queue;
}

}
