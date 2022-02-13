#include "FastEngine/C_subscription.hpp"

namespace fge
{

///Subscription
void Subscription::detachAll()
{
    for (auto& data : this->g_subData)
    {
        data.first->detachSilent(this);
    }
    this->g_subData.clear();
}

void Subscription::detachSilent(fge::Subscriber* subscriber)
{
    if (subscriber == nullptr)
    {
        return;
    }

    auto it = this->g_subData.find(subscriber);
    if ( it != this->g_subData.end() )
    {
        this->g_subData.erase(it);
        this->onDetach(subscriber);
    }
}

bool Subscription::detach(fge::Subscriber* subscriber)
{
    if (subscriber == nullptr)
    {
        return true;
    }

    auto it = this->g_subData.find(subscriber);
    if ( it != this->g_subData.end() )
    {
        subscriber->detachSilent(this);
        this->g_subData.erase(it);
        return true;
    }
    return false;
}
fge::Subscription::SubscriberCount Subscription::detachOnce(fge::Subscriber* subscriber)
{
    if (subscriber == nullptr)
    {
        return 1;
    }

    auto it = this->g_subData.find(subscriber);
    if ( it != this->g_subData.end() )
    {
        if ( --it->second == 0)
        {
            subscriber->detachSilent(this);
            this->g_subData.erase(it);
            return 0;
        }

        return it->second;
    }
    return 0;
}

fge::Subscription::SubscriberCount Subscription::attach(fge::Subscriber* subscriber)
{
    if (subscriber == nullptr)
    {
        return 1;
    }

    auto it = this->g_subData.find(subscriber);
    if ( it != this->g_subData.end() )
    {
        return ++it->second;
    }
    else
    {
        this->g_subData[subscriber] = 1;
        subscriber->attachSilent(this);
        return 1;
    }
}

fge::Subscription::SubscriberCount Subscription::getCount(fge::Subscriber* subscriber) const
{
    if (subscriber == nullptr)
    {
        return 0;
    }

    auto it = this->g_subData.find(subscriber);
    if ( it != this->g_subData.cend() )
    {
        return it->second;
    }
    return 0;
}

///Subscriber

void Subscriber::detachAll()
{
    for (auto data : this->g_subData)
    {
        data->detachSilent(this);
    }
    this->g_subData.clear();
}
void Subscriber::detachSilent(fge::Subscription* subscription)
{
    if ( this->g_subData.erase(subscription) )
    {
        this->onDetach(subscription);
    }
}
void Subscriber::detach(fge::Subscription* subscription)
{
    if ( this->g_subData.erase(subscription) )
    {
        subscription->detachSilent(this);
    }
}
void Subscriber::attachSilent(fge::Subscription* subscription)
{
    this->g_subData.insert(subscription);
}

}//end fge
