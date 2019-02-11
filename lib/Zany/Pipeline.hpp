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
#include "./Property.hpp"
#include "./ThreadPool.hpp"
#include "./HttpRequest.hpp"

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

	/*
	** Un Set est un ensemble d'handlers pour un hook
	** il tri les handlers en fonction de leur priorité.
	** Ce n'est pas a vous de les creer, Pipeline::getHookSet() le permet.
	*/ 
	class Set {
	public:
		struct ID {
			Set				*set;
			std::uint16_t	PRSignValue;
			std::uint64_t	uniqueId;
		};

		Set(Pipeline *parent): _parent(*parent) {}
		Set(Set const &other) = delete;
		Set(Set &&other) = default;
		Set &operator=(Set const &other) = delete;


		template<Rights R>
		struct _FunctionTypeSelector {};

		/*
		** Ajoute un event handler au Set
		*/
		template<Priority P = Priority::LOW, Rights R = Rights::READ_WRITE>
		inline ID	addTask(typename _FunctionTypeSelector<R>::type const &fct);

		/*
		** Execute les handlers dans le bon ordre
		*/
		inline void	execute(Instance &pipeline);

		/*
		** retire un handler
		*/
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
	Pipeline(Pipeline const &other) = delete;
	Pipeline(Pipeline &&other) = default;
	Pipeline &operator=(Pipeline const &other) = delete;


	class Instance {
	public:
		Instance(Instance const &other) = delete;
		Instance(Instance &&other) = default;
		Instance &operator=(Instance const &other) = delete;
		
		// /* doit etre override par le module createur d'instance */
		// virtual void	write(const char *buffer, std::size_t len) = 0;
		// /* doit etre override par le module createur d'instance */
		// virtual ssize_t	read(char *buffer, std::size_t len) = 0;

		inline void setContext(InterfaceContext &ctx) { _ctx = &ctx; }

		/*
		** Permet le partage d'info entre differents hooks
		*/
		std::unordered_map<std::string, Property>	properties;

		/*
		** Request header
		*/
		HttpResponse	request;

		/*
		** Response header
		*/
		HttpResponse	response;
	protected:
		Instance() = default;

		InterfaceContext	*_ctx;

		friend Pipeline;
	};
	inline auto &createInstance() { return _instances.emplace_back(Instance()); }

	inline void	linkThreadPool(ThreadPool &pool) { _pool = &pool; }
	inline auto getThreadPool() -> auto & { return *_pool; }
	inline auto getThreadPool() const -> const auto & { return *_pool; }

	/*
	** Creer une instance de Pipeline
	*/
	template<typename T = zany::Context, typename ...Args>
	inline void	startPipeline(zany::Socket fd, Args &&...);

	/*
	** Recup un set en fonction d'un hook
	*/
	template<Hooks::Decl H>
	inline Set	&getHookSet();
	inline Set	&getHookSet(Hooks::Decl hook);

	/*
	** Execute un hook Set
	*/
	template<Hooks::Decl H>
	inline void	executeHook(Instance &instance)
		{ getHookSet<H>().execute(instance); }
private:
	ThreadPool												*_pool;
	std::list<Instance>										_instances;
	std::unordered_map<Hooks::Decl, std::unique_ptr<Set>>	_sets;
};


}

#include "Pipeline.ipp"