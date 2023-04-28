#include <stdinc.hpp>

#include "loader/component_loader.hpp"
#include "scheduler.hpp"

#include <utils/concurrency.hpp>
#include <utils/hook.hpp>

namespace scheduler
{
	namespace
	{
		utils::hook::detour server_frame_hook;
		bool kill_thread = false;
		
		struct task
		{
			std::function<bool()> handler{};
			std::chrono::milliseconds interval{};
			std::chrono::high_resolution_clock::time_point last_call{};
			bool update_after_call;
		};

		using task_list = std::vector<task>;

		class task_pipeline
		{
		public:
			void add(task&& task)
			{
				new_callbacks_.access([&task](task_list& tasks)
				{
					tasks.emplace_back(std::move(task));
				});
			}

			void execute()
			{
				callbacks_.access([&](task_list& tasks)
				{
					this->merge_callbacks();

					for (auto i = tasks.begin(); i != tasks.end();)
					{
						const auto now = std::chrono::high_resolution_clock::now();
						const auto diff = now - i->last_call;

						if (diff < i->interval)
						{
							++i;
							continue;
						}

						if (!(i->update_after_call))
						{
							i->last_call = now;
						}

						const auto res = i->handler();

						if (i->update_after_call)
						{
							i->last_call = now;
						}

						if (res == cond_end)
						{
							i = tasks.erase(i);
						}
						else
						{
							++i;
						}
					}
				});
			}

		private:
			utils::concurrency::container<task_list> new_callbacks_;
			utils::concurrency::container<task_list, std::recursive_mutex> callbacks_;

			void merge_callbacks()
			{
				callbacks_.access([&](task_list& tasks)
				{
					new_callbacks_.access([&](task_list& new_tasks)
					{
						tasks.insert(tasks.end(), std::move_iterator<task_list::iterator>(new_tasks.begin()), std::move_iterator<task_list::iterator>(new_tasks.end()));
						new_tasks = {};
					});
				});
			}
		};

		std::thread thread;
		task_pipeline pipelines[pipeline::count];

		void execute(const pipeline type)
		{
			assert(type >= 0 && type < pipeline::count);
			pipelines[type].execute();
		}

		void execute_server()
		{
			execute(pipeline::server);
		}

		void server_frame_stub()
		{
			execute(pipeline::server);
			server_frame_hook.invoke<void>();
		}
	}

	void schedule(const std::function<bool()>& callback, const pipeline type,
		const std::chrono::milliseconds delay, bool update_after_call)
	{
		assert(type >= 0 && type < pipeline::count);

		task task;
		task.handler = callback;
		task.interval = delay;
		task.last_call = std::chrono::high_resolution_clock::now();
		task.update_after_call = update_after_call;

		pipelines[type].add(std::move(task));
	}

	void loop(const std::function<void()>& callback, const pipeline type,
		const std::chrono::milliseconds delay, bool update_after_call)
	{
		schedule([callback]()
		{
			callback();
			return cond_continue;
		}, type, delay, update_after_call);
	}

	void once(const std::function<void()>& callback, const pipeline type,
		const std::chrono::milliseconds delay, bool update_after_call)
	{
		schedule([callback]()
		{
			callback();
			return cond_end;
		}, type, delay, update_after_call);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			thread = std::thread([]()
			{
				while (!kill_thread)
				{
					execute(pipeline::async);
					std::this_thread::sleep_for(10ms);
				}
			});

			server_frame_hook.create(0x43E340, server_frame_stub);
		}

		void pre_destroy() override
		{
			kill_thread = true;

			if (thread.joinable())
			{
				thread.join();
			}
		}
	};
}

REGISTER_COMPONENT(scheduler::component)
