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
#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include <QList>
#include "action.h"
#include "actionqueueoptimizer.h"

namespace WebsiteLocker {

/**
 * @brief
 *
 */
class ActionQueue
{
public:
/**
 * @brief
 *
 */
    ActionQueue();
    /**
     * @brief
     *
     */
    ~ActionQueue();
    /**
     * @brief
     *
     * @param action
     */
    void addAction(Action* action);
    /**
     * @brief
     *
     */
    void printQueue() const;
    /**
     * @brief
     *
     */
    void clear();
    /**
     * @brief
     *
     */
    const QList<Action*>& actionQueue() const;
    friend class ActionQueueOptimizer;
private:

    // Private cctor and assignment operator
    ActionQueue(const ActionQueue& rhs);
    ActionQueue& operator=(const ActionQueue& rhs);

    /**
     * @brief
     *
     * @param action
     */
    Action* findAction(Action* action) ;

    /**
     * @brief find RemoveAction in the action queue.
     *
     * @bug remove action buggy code
     * @param action
     * @return Action *
     */

    Action* findRemoveAction(RemoveAction* action) ;
    /**
     * @brief
     *
     * @param action
     */
    Action* findEditAction(EditAction* action) ;
    /**
     * @brief
     *
     * @param action
     */
    int indexOf(Action* action);
    QList<Action*> m_queue; /**< TODO */
    ActionQueueOptimizer* m_optimizer; /**< TODO */
};

}

#endif // ACTIONQUEUE_H
