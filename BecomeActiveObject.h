#pragma once
#ifndef _BECOME_ACTIVE_OBJECT_H_
#define _BECOME_ACTIVE_OBJECT_H_
	//
	// This version of the ActiveObject illustrates more advanced
	// features such as calling public methods with references,
	// calling methods with return values, and accessing the object's
	// internal state.
	//
#include <thread>
#include <queue>
#include <mutex>
#include <future>
#include <iostream>

namespace VEActiveObject
{
	typedef std::function<void()> Operation;

	class DispatchQueue
	{
		std::mutex qlock;
		std::queue<Operation> ops_queue;
		std::condition_variable empty;
	public:
		void put(Operation op)
		{
			std::lock_guard<std::mutex> guard(qlock);
			ops_queue.push(op);
			empty.notify_one();
		}

		Operation take()
		{
			std::unique_lock<std::mutex> lock(qlock);
			empty.wait(lock, [&] { return !ops_queue.empty(); });

			Operation op = ops_queue.front();
			ops_queue.pop();
			return op;
		}
	};

	class BecomeActiveObject
	{
	private:
		double val;
		DispatchQueue dispatchQueue;
		std::atomic<bool> done;
		std::thread runnable;
	public:
		BecomeActiveObject() : val(0), done(false)
		{
			runnable = std::thread([=] { run(); });
		}
		~BecomeActiveObject()
		{
			// Schedule a No-Op runnable to flush the dispatch queue
			dispatchQueue.put([&]() { done = true; });
			runnable.join();
		}

		double getVal() { return val; }

		void run()
		{
			while (!done)
			{
				dispatchQueue.take()();
			}
		}

		// This method returns a value, so it is blocking on the future result
		int doSomething()
		{
			std::promise<int> return_val;
			auto runnable = [&]()
			{
				int ret = 999;
				//test sleep : mmp
				printf("sleep a bit\n");
				std::this_thread::sleep_for(std::chrono::seconds(2));
				printf("woke up\n");
				return_val.set_value(ret);
			};

			dispatchQueue.put(runnable);
			return return_val.get_future().get();
		}

		void Send(std::function<void()> fnc)
		{
			dispatchQueue.put(fnc);
		}

		// This method accesses the object's internal state from within the closure
		// Because the access to the ActiveObject is serialized, we can safely access 
		// the object's internal state.
		void doSomethingElse()
		{
			dispatchQueue.put(([this]()
			{
				this->val = 2.0;
			}
			));
		}

		// This method takes two params which we want to reference in the closure
		void doSomethingWithParams(int a, int b)
		{
			// This lambda function code will execute later from the context of a different thread, 
			// but the integers {a, b} are bound now.
			// This is a beautiful way to write clear code
			dispatchQueue.put(([a, b]()
			{
				std::cout << "this is the internal implementation of doSomethingWithParams(";
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << a << "," << b << ")\n";
			}
			));
		}

		// This method takes two reference parameters so it must execute blocking
		void doSomethingWithReferenceParams(int &a, int &b)
		{
			std::promise<void> return_val;
			// This lambda function code will execute later from the context of a different thread, 
			// but the integers {a, b} are bound now.
			// This is a beautiful way to write clear code
			dispatchQueue.put(([&a, &b, &return_val]()
			{
				std::this_thread::sleep_for(std::chrono::seconds(2));
				std::cout << "this is the internal implementation of doSomethingWithReferenceParams(";
				std::cout << a << "," << b << ")\n";
				a = 1234;
				b = 5678;
				return_val.set_value();
			}
			));

			return_val.get_future().get();
		}
	};

}

#endif // !_BECOME_ACTIVE_OBJECT_H_
