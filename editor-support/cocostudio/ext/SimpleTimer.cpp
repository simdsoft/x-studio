#include "SimpleTimer.h"
#include "object_pool.h"
#include "ref_ptr.h"
#include "cocos2d.h"

USING_NS_CC;

#define DEFINE_REFERENCE_CLASS \
	private: \
	    unsigned  int referenceCount_; \
	public: \
		void retain() { ++referenceCount_; } \
		void release(){ --referenceCount_; if (referenceCount_ == 0) delete this; } \
    private:

namespace simple_timer {

    struct TimerObject
    {
        TimerObject()
        {
            referenceCount_ = 1;
        }
        vcallback_t callback;
        DEFINE_OBJECT_POOL_ALLOCATION(TimerObject, 128)
        DEFINE_REFERENCE_CLASS
    };

    TIMER_ID loop(int n, float interval, vcallback_t callback)
    {
        if (n > 0 && interval > 0) {
            purelib::gc::ref_ptr<TimerObject> timerObj(new TimerObject());
            timerObj->callback = std::move(callback);

            std::string key = StringUtils::format("SIMPLE_TIMER_%p", timerObj.get());
            Director::getInstance()->getScheduler()->schedule([timerObj](float /*dt*/) { // lambda expression hold the reference of timerObj automatically.
                timerObj->callback();
            }, timerObj.get(), interval, n - 1, 0, false, key);

            return timerObj.get();
        }
        return nullptr;
    }

    TIMER_ID delay(float delay, vcallback_t callback)
    {
        if (delay > 0) {
            purelib::gc::ref_ptr<TimerObject> timerObj(new TimerObject());
            timerObj->callback = std::move(callback);

            std::string key = StringUtils::format("SIMPLE_TIMER_%p", timerObj.get());
            Director::getInstance()->getScheduler()->schedule([timerObj](float /*dt*/) { // lambda expression hold the reference of timerObj automatically.
                timerObj->callback();
            }, timerObj.get(), 0, 0, delay, false, key);

            return timerObj.get();
        }
        return nullptr;
    }

    void kill(TIMER_ID timerId)
    {
        std::string key = StringUtils::format("SIMPLE_TIMER_%p", timerId);
        Director::getInstance()->getScheduler()->unschedule(key, timerId);
    }
}
