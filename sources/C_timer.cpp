#include "FastEngine/C_timer.hpp"

namespace fge
{

Timer::Timer(const fge::Timer& timer) :
    g_lifeTimePoint( timer.g_lifeTimePoint ),

    g_stepDuration( timer.g_stepDuration ),
    g_goalDuration( timer.g_goalDuration ),

    g_isPaused( timer.g_isPaused ),
    g_name( timer.g_name )
{

}
Timer::Timer(fge::Timer&& timer) :
    g_lifeTimePoint( std::move(timer.g_lifeTimePoint) ),

    g_stepDuration( std::move(timer.g_stepDuration) ),
    g_goalDuration( std::move(timer.g_goalDuration) ),

    g_isPaused( std::move(timer.g_isPaused) ),
    g_name( std::move(timer.g_name) )
{

}

Timer::Timer(const std::chrono::milliseconds& goal) :
    g_lifeTimePoint( std::chrono::steady_clock::now() ),

    g_stepDuration(0),
    g_goalDuration(goal),

    g_isPaused(false),
    g_name()
{

}
Timer::Timer(const std::chrono::milliseconds& goal, bool paused) :
    g_lifeTimePoint( std::chrono::steady_clock::now() ),

    g_stepDuration(0),
    g_goalDuration(goal),

    g_isPaused(paused),
    g_name()
{

}
Timer::Timer(const std::chrono::milliseconds& goal, const std::string& name, bool paused) :
    g_lifeTimePoint( std::chrono::steady_clock::now() ),

    g_stepDuration(0),
    g_goalDuration(goal),

    g_isPaused(paused),
    g_name(name)
{

}

void Timer::setName(const std::string& name)
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    this->g_name = name;
}
const std::string& Timer::getName() const
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    return this->g_name;
}

void Timer::setGoalDuration(const std::chrono::milliseconds& t)
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    this->g_goalDuration = t;
}
void Timer::addToGoal(const std::chrono::milliseconds& t)
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    this->g_goalDuration += t;
}
void Timer::subToGoal(const std::chrono::milliseconds& t)
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    this->g_goalDuration -= t;
}

void Timer::setStepDuration(const std::chrono::milliseconds& t)
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    if (!this->g_isPaused)
    {
        this->g_stepDuration = t;
    }
}
void Timer::addToStep(const std::chrono::milliseconds& t)
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    if (!this->g_isPaused)
    {
        this->g_stepDuration += t;
    }
}
void Timer::subToStep(const std::chrono::milliseconds& t)
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    if (!this->g_isPaused)
    {
        this->g_stepDuration -= t;
    }
}

const std::chrono::steady_clock::time_point& Timer::getLifeTimePoint() const
{
    return this->g_lifeTimePoint;
}
std::chrono::milliseconds Timer::getLifeDuration() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->g_lifeTimePoint);
}

const std::chrono::milliseconds& Timer::getStepDuration() const
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    return this->g_stepDuration;
}
const std::chrono::milliseconds& Timer::getGoalDuration() const
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    return this->g_goalDuration;
}

std::chrono::milliseconds Timer::getTimeLeft() const
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    return this->g_goalDuration - this->g_stepDuration;
}

bool Timer::goalReached() const
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    return (this->g_goalDuration - this->g_stepDuration).count() <= 0;
}
void Timer::restart()
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    this->g_stepDuration = std::chrono::milliseconds(0);
}

void Timer::pause()
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    this->g_isPaused = true;
}
void Timer::resume()
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    this->g_isPaused = false;
}
bool Timer::isPaused() const
{
    std::lock_guard<std::mutex> lck(this->g_mutex);
    return this->g_isPaused;
}

std::mutex& Timer::getMutex()
{
    return this->g_mutex;
}

}//end fge
