#pragma once


#include<thread>
#include<chrono>
#include<functional>

class Timer
{
	std::thread Thread;
	bool Alive = false;
	long CallNumber = -1L;
	long repeat_count = -1L;
	std::chrono::milliseconds interval = std::chrono::milliseconds(0);
	std::function<void(void)> funct = nullptr;
	void SleepAndRun() {
		std::this_thread::sleep_for(interval);
		if (Alive) {
			Function()();
		}
	}
	void ThreadFunc() {
		//std::cout << "this is from threadFunc " << std::endl;
		if (CallNumber == Infinite) 
			while (Alive)
				SleepAndRun();
		else
			while (repeat_count--)
				SleepAndRun();
	}

	void ThreadFunc2() {
		if (CallNumber == Infinite)
			while (Alive)
				SleepAndRun();
		else
			while (repeat_count--)
				SleepAndRun();
	}
public:
	static const long Infinite = -1L;
	Timer() {}
	Timer(const std::function<void(void)>& f) :funct(f) {}
	Timer(const std::function<void(void)>& f, const unsigned long& i, const long repeat = Timer::Infinite) 
		:funct(f), interval(std::chrono::milliseconds(i)), CallNumber(repeat) {}

	bool IsAlive() const{
		return Alive;
	}

	void Start(bool Async = true) {
		if (IsAlive())
			return;
		Alive = true;
		repeat_count = CallNumber;
		if (Async) {
			Thread = std::thread(&Timer::ThreadFunc, this);
			//Thread.join();
			//std::function<void(Timer*)> f(& Timer::ThreadFunc);
		}
		else
			ThreadFunc();
	}
	void Stop() {
		Alive = false;
		Thread.join();
	}
	void RepeatCount(const long r) {
		if (!IsAlive())
			CallNumber = r;
	}
	void SetFunction(std::function<void(void)>& f) {
		funct = f;
	}
	long GetLeftCount() const{
		return repeat_count;
	}
	long RepeatCount() const{
		return CallNumber;
	}
	void SetInterval(long unsigned int& i) {
		if (!IsAlive())
			interval = std::chrono::milliseconds(i);
	}
	unsigned long Interval() const{
		return interval.count();
	}

	const std::function<void(void)>& Function() const{
		return funct;
	}

};
