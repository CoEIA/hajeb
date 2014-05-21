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
