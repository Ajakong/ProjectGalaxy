#include "Collidable.h"
#include "ColliderSphere.h"
#include "ColliderBox.h"
#include<cassert>

using namespace MyEngine;

Collidable::Collidable(Priority priority, ObjectTag tag) :
	m_priority(priority),
	m_tag(tag),
	m_isAntiGravity(false),
	m_isDestroyFlag(false),
	m_upVec(Vec3::Up())
{
	m_rigid = std::make_shared<Rigidbody>();
}

MyEngine::Collidable::Collidable(std::shared_ptr<Collidable> col) :
	m_rigid(col->m_rigid),
	m_colliders(col->m_colliders),
	m_upVec(col->m_upVec),
	m_throughTags(col->m_throughTags),
	m_tag(col->m_tag),
	m_priority(col->m_priority)
{
	m_rigid = std::make_shared<Rigidbody>();
}

Collidable::~Collidable()
{
}

std::shared_ptr<Collidable::Collide> MyEngine::Collidable::AddCollider(const ColliderBase::Kind& kind, const ColliderBase::ColideTag& tag)
{
	std::shared_ptr<Collide> collider;

	if (kind == ColliderBase::Kind::Sphere)
	{
		collider->col = std::make_shared<ColliderSphere>(tag);

	}
	else if (kind == ColliderBase::Kind::Capsule)
	{

	}
	else if (kind == ColliderBase::Kind::Box)
	{
		collider->col = std::make_shared<ColliderBox>(tag);
	}

	if (collider)
	{
		m_colliders.emplace_back(collider);
	}
	collider->tag = tag;
	return collider;
}
void MyEngine::Collidable::RemoveCollider(std::shared_ptr<Collide> col)
{
	auto it = std::find(m_colliders.begin(), m_colliders.end(), col);
	if (it == m_colliders.end()) return;
	m_colliders.erase(it);
}
/// <summary>
/// 当たり判定を無視（スルー）するタグの追加
/// </summary>
void Collidable::AddThroughTag(ObjectTag tag)
{
	bool found = (std::find(m_throughTags.begin(), m_throughTags.end(), tag) != m_throughTags.end());
	if (found)
	{
		assert(0 && L"指定タグは既に追加されています");
	}
	else
	{
		m_throughTags.emplace_back(tag);
	}
}

/// <summary>
/// 当たり判定を無視（スルー）するタグの削除
/// </summary>
void Collidable::RemoveThroughTag(ObjectTag tag)
{
	bool found = (std::find(m_throughTags.begin(), m_throughTags.end(), tag) != m_throughTags.end());
	if (!found)
	{
		assert(0 && L"指定タグは存在しません");
	}
	else
	{
		m_throughTags.remove(tag);
	}
}

// 当たり判定を無視（スルー）する対象かどうか
bool Collidable::IsThroughTarget(const std::shared_ptr<Collidable> target) const
{
	bool found = (std::find(m_throughTags.begin(), m_throughTags.end(), target->GetTag()) != m_throughTags.end());
	return found;
}