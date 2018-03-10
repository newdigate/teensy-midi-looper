#include <vector>
#include <functional>

#ifndef Delegate_h
#define Delegate_h



template<typename ... Args>
class Delegate
{
public:
   Delegate() = default;
   ~Delegate() = default;

   template<typename U>
   Delegate& operator += (const U &func)
   {
        _listeners.push_back(std::function<void(Args...)>(func));
        return *this;
   }

   template<typename Class, typename Method>
   Delegate& operator += (const std::function<void(Args...)> func)
   {
        _listeners.push_back(func);
        return *this;
   }

   void operator() (Args... params)
   {
        for (auto listener : _listeners)
        {
            listener(params...);
        }
   }

   private:
       std::vector<std::function<void(Args...)>> _listeners;
};


#endif
