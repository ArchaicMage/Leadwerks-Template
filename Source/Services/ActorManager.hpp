#ifndef _ACTOR_MANAGER_HPP_
	#define _ACTOR_MANAGER_HPP_
	
#pragma once
#include "../Utilities/Macros.hpp"
#include "../Common.hpp"
#include "../Utilities/HasId.hpp"
#include "../Components/Component.hpp"

#include <cassert>
#include <map>
#include <string>
#include <vector>

class Actor;

class ActorManager {	

	template <typename T1, typename T2>
	struct ActorComponentKeyComparer {
		bool operator () (const std::pair<T1, T2>& x, const std::pair<T1, T2>& y) {
			return x.first < y.first && x.second < y.second;
		}
	};	

	typedef std::pair<uint64_t, std::string> ActorComponentKey;
	typedef std::map<ActorComponentKey, const Component*, ActorComponentKeyComparer<uint64_t, std::string>> ActorComponentMap;

public:

	ActorManager(Factory<Component> pComponentFactory);
	~ActorManager(void);	

	const Actor* Fetch(uint64_t _id);

	uint64_t Create(void);

	void Delete(uint64_t _id);

	template <typename T>
	void AddComponent(uint64_t _id);

	template <typename T>
	void RemoveComponent(uint64_t _id);

	std::vector<const Component*> FetchAllComponentsFor(uint64_t _id);

protected:
	uint64_t GetNextId(void);

private:	
	Factory<Component> m_pComponentFactory;
	uint64_t m_nRunningIndex;

	std::map<uint64_t, Actor*> m_actors;	
	
	ActorComponentMap m_actorComponents;

}; // < end class.

template <typename T>
void ActorManager::AddComponent(uint64_t _id) {	

	// < Are we attempting to add a component to an non-existing
	// * actor?
	auto actor = this->Fetch(_id);
	assert(actor != nullptr);
	
	std::string type = T::ClassType();
	auto key = std::make_pair(_id, type);

	T* newComponent = (T*)(gComponentFactory.Create(type));
	assert(newComponent != nullptr);

	newComponent->setId(_id);
	
	auto actorComponent = m_actorComponents.insert(std::make_pair(key, newComponent));

}

template <typename T>
void ActorManager::RemoveComponent(uint64_t _id) {

	auto actor = this->Fetch(_id);
	if (actor == nullptr) { return; }
	
	std::string type = T::ClassType();
	auto key = std::make_pair(_id, type);

	auto iter = m_actorComponents.find(key);
	if (iter == m_actorComponents.end()) { return; }

	auto it = (iter)->second;
	if (it != nullptr) { SAFE_DELETE(it); }

	m_actorComponents.erase(iter);
	
}

#endif _ACTOR_MANAGER_HPP_