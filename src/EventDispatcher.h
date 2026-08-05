#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

// variadic template
template<typename... Args>
class EventDispatcher
{

public:

	using EventHandler = std::function<void(Args...)>;

	EventDispatcher() = default;
	~EventDispatcher() {
		listeners.clear();
	}

	size_t registerListener(EventHandler listener) {
		size_t t = token;
		listeners[token++] = listener;
		return t;
	}
	void unregisterListener(size_t t) {
		listeners.erase(t);
	}

	void notifyListeners(Args... args) {
		for (auto& a : listeners) {
			if (a.second) a.second(args...);
		}
	}


private:

	std::unordered_map<std::size_t, EventHandler> listeners;
	std::size_t token = 1;

};
