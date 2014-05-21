#ifndef ACTIONQUEUEOPTIMIZER_H
#define ACTIONQUEUEOPTIMIZER_H


#include "websitelocker.h"
using namespace WebsiteLocker;

namespace WebsiteLocker {

class ActionQueueOptimizer
{
public:
    ActionQueueOptimizer();
    void setActionQueue(ActionQueue* queue);
    void addAction(Action* action);
private:
    ActionQueueOptimizer(const ActionQueueOptimizer& rhs);
    ActionQueueOptimizer& operator=(const ActionQueueOptimizer& rhs);

    ActionQueue* actionQueue;
    void removeAction(int index);
    void appendAction(Action* action);
};

}

#endif // ACTIONQUEUEOPTIMIZER_H
