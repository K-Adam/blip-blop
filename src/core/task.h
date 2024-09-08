#pragma once

#include <coroutine>
#include <exception>
#include <utility>

// Frame loops are coroutines: a loop suspends with co_await nextFrame() where it used to block,
// and the main loop resumes it once per frame. That lets the browser drive the game without ASYNCIFY.

namespace task_detail {

// The innermost coroutine that waits for the next frame
inline std::coroutine_handle<> frame_waiter;

struct PromiseBase {
	std::coroutine_handle<> continuation = std::noop_coroutine();

	std::suspend_always initial_suspend() noexcept { return {}; }

	struct FinalAwaiter {
		bool await_ready() noexcept { return false; }
		template <typename P>
		std::coroutine_handle<> await_suspend(std::coroutine_handle<P> h) noexcept {
			return h.promise().continuation;
		}
		void await_resume() noexcept {}
	};
	FinalAwaiter final_suspend() noexcept { return {}; }

	// Release web builds have no exceptions
	void unhandled_exception() { std::terminate(); }
};

template <typename T>
struct Promise : PromiseBase {
	T value{};
	void return_value(T v) { value = std::move(v); }
	T result() { return std::move(value); }
};

template <>
struct Promise<void> : PromiseBase {
	void return_void() {}
	void result() {}
};

}  // namespace task_detail

template <typename T = void>
class [[nodiscard]] Task {
   public:
	struct promise_type : task_detail::Promise<T> {
		Task get_return_object() { return Task(std::coroutine_handle<promise_type>::from_promise(*this)); }
	};

	Task(Task&& o) noexcept : h_(std::exchange(o.h_, {})) {}
	~Task() {
		if (h_) h_.destroy();
	}

	bool done() const { return !h_ || h_.done(); }

	// Makes this task the one the next resumeFrame() runs, used for the root task
	void start() { task_detail::frame_waiter = h_; }

	// Awaiting a task runs it until it finishes, frames included
	bool await_ready() const noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
		h_.promise().continuation = caller;
		return h_;
	}
	T await_resume() { return h_.promise().result(); }

   private:
	explicit Task(std::coroutine_handle<promise_type> h) : h_(h) {}
	std::coroutine_handle<promise_type> h_;
};

struct NextFrame {
	bool await_ready() noexcept { return false; }
	void await_suspend(std::coroutine_handle<> h) noexcept { task_detail::frame_waiter = h; }
	void await_resume() noexcept {}
};

inline NextFrame nextFrame() { return {}; }

// Runs the waiting coroutine until it asks for the next frame or the root task finishes
inline void resumeFrame() {
	if (auto h = std::exchange(task_detail::frame_waiter, {})) h.resume();
}
