/*
** EPITECH PROJECT, 2018
** api
** File description:
** Pipeline.hpp
*/

#pragma once

#include <type_traits>
#include <unordered_map>
#include <exception>
#include <array>
#include "ThreadPool.hpp"

namespace zany {

class Pipeline {
public:

	class Hooks {
	private:
		template<auto ...Ds>
		struct HookMetaIterate {};
		
		template<auto D>
		struct HookMetaIterate<D> {
			template<typename Hdl>
			static inline void	callSet(Hdl &&);

			static inline constexpr std::size_t	count()
				{ return 1; }

			static inline constexpr decltype(D)	next(decltype(D) hook)
				{ return HookMetaIterate<D>::getHook(); }

			static inline constexpr decltype(D)	getHook()
				{ return D; }
		};
		template<auto D, auto ...Ds>
		struct HookMetaIterate<D, Ds...> {
			template<typename ...Args>
			static inline void	callSet(Args &&...);

			static inline constexpr std::size_t	count()
				{ return HookMetaIterate<Ds...>::count() + 1; }
			
			static inline constexpr decltype(D)	next(decltype(D) hook)
				{ return (hook == D)
					? HookMetaIterate<Ds...>::getHook()
					: HookMetaIterate<Ds...>::next(hook); }

			static inline constexpr decltype(D)	getHook()
				{ return D; }
		};
	public:
#		define HOOKSFACTORY(...) \
			enum Decl { \
				__BEGIN__, \
				__VA_ARGS__, \
				__END__ \
			}; \
			template<typename ...Args> \
			static inline void forEach(Args &&...args) \
				{ HookMetaIterate<__VA_ARGS__>::callSet(args...); } \
			static inline constexpr std::size_t	count() \
				{ return HookMetaIterate<__VA_ARGS__>::count(); } \
			static inline constexpr Decl	next(Decl hook) \
				{ return HookMetaIterate<__BEGIN__, __VA_ARGS__, __END__>::next(hook); }
		
		HOOKSFACTORY(
			BEFORE_HANDLE_REQUEST,
			ON_HANDLE_REQUEST,
			ON_DATA_AVAILABLE,
			ON_DATA_END,
			BEFORE_HANDLE_RESPONSE,
			ON_HANDLE_RESPONSE,
			BEFORE_CLOSE_SOCKET
		)
#		undef HOOKSMAKER
	};

	enum class Priority: std::uint8_t {
		HIGH,
		MEDIUM,
		LOW,
	};
	enum class Rights: std::uint8_t {
		READ_ONLY,
		READ_WRITE
	};

	class Instance;

	class Set {
	public:
		struct ID {
			Set				*set;
			std::uint16_t	PRSignValue;
			std::uint64_t	uniqueId;
		};

		Set(Pipeline *parent): _parent(*parent) {}

		template<Rights R>
		struct _FunctionTypeSelector {};

		template<Priority P = Priority::LOW, Rights R = Rights::READ_WRITE>
		inline ID	addTask(typename _FunctionTypeSelector<R>::type const &fct);
		inline void	execute(Instance &pipeline);
		inline void	removeTask(ID id);

		/* maybe useless ... */
		inline auto	&getMutex() { return _mtx; }
		inline auto	&getParent() { return _parent; }
	private:
		std::uint64_t	_genId()
			{ static std::uint64_t nextid = 0; return ++nextid; }

		std::map<
			std::uint16_t,
			std::unordered_map<
				std::uint64_t,
				std::shared_ptr<void>
			>>		_handlers;
		std::mutex	_mtx;
		Pipeline	&_parent;
	};

	Pipeline(): _pool(nullptr) {
		Hooks::forEach([this] (Hooks::Decl hook) {
			_sets.emplace(std::make_pair(hook, std::make_unique<Set>(this)));
		});
	}

	class Instance {
	public:
		Instance() {}
	private:

	};

	inline void	linkThreadPool(ThreadPool &pool) { _pool = &pool; }
	inline auto &getThreadPool() { return *_pool; }

	template<Hooks::Decl H>
	inline Set	&getHookSet();
	inline Set	&getHookSet(Hooks::Decl hook);
	template<Hooks::Decl H>
	inline void	executeHook(Instance &instance)
		{ getHookSet<H>().execute(instance); }
private:
	ThreadPool								*_pool;
	std::unordered_map<Hooks::Decl, std::unique_ptr<Set>>	_sets;
};


}

#include "Pipeline.ipp"