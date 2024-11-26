#pragma once
#include <memory>
#include <list>
#include <functional>
#include"Vec3.h"
#include "ObjectTag.h"
#include <DxLib.h>
#include "Collidable.h"


namespace MyEngine
{
	class Galaxy;
	class Rigidbody;
	class Collidable;
	class ColliderBase;

	class Physics final
	{
	private:
		friend Galaxy;

		enum class OnCollideInfoKind
		{
			CollideEnter,
			CollideStay,
			CollideExit,
			TriggerEnter,
			TriggerStay,
			TriggerExit
		};
		struct CollideHitInfo
		{
			bool isHit = false;
			Vec3 hitPos;
		};
		struct CollideFuncArgs
		{
			std::shared_ptr<Collidable> send;
			Collidable::Collide myCollide;
			Collidable::Collide sendCollide;
		};
		struct SendInfo
		{
			std::weak_ptr<Collidable> own;
			std::weak_ptr<Collidable> send;
			ColliderBase::ColideTag ownTag;
			ColliderBase::ColideTag sendTag; 
			Vec3 hitPos;
		};
		struct OnCollideInfoData
		{
			std::weak_ptr<Collidable> own;
			std::weak_ptr<Collidable> send;
			ColliderBase::ColideTag ownTag;
			ColliderBase::ColideTag sendTag;
			Vec3 hitPos;
			OnCollideInfoKind kind;
		};
		using SendCollideInfo = std::list<SendInfo>;
	private:

		Physics();

		Physics(const Physics&) = delete;
		void operator= (const Physics&) = delete;

	public:

		static Physics& GetInstance();

		void Entry(const std::shared_ptr<Collidable>& collidable);
		void Exit(const std::shared_ptr<Collidable>& collidable);

		void Update();
		void Draw();

		void Clear();

	private:


		void MoveNextPos() const;
		/// <summary>
	/// 判定リストの取得
	/// </summary>
	/// <returns>判定リスト</returns>
		std::vector<std::shared_ptr<Collidable>> GetCollisionList() const;
		void CheckCollide();

		CollideHitInfo IsCollide(const std::shared_ptr<Rigidbody> rigidA, const std::shared_ptr<Rigidbody> rigidB, const std::shared_ptr<Collidable::Collide> & colliderA, const std::shared_ptr<Collidable::Collide>& colliderB) const;
		void FixNextPos(const std::shared_ptr<Rigidbody> primaryRigid, std::shared_ptr<Rigidbody> secondaryRigid, const std::shared_ptr<Collidable::Collide>& primaryCollider, const std::shared_ptr<Collidable::Collide>& secondaryCollider, const CollideHitInfo info);
		/// <summary>
		/// 判定通知リストに追加する
		/// </summary>
		/// <param name="objA">オブジェクトA</param>
		/// <param name="objB">オブジェクトB</param>
		/// <param name="info">通知リスト</param>
		/// <param name="hitPos">当たった座標</param>
		void AddNewCollideInfo(const std::weak_ptr<Collidable>& objA, const std::weak_ptr<Collidable>& objB, ColliderBase::ColideTag ownTag, ColliderBase::ColideTag sendTag, SendCollideInfo& info, const Vec3& hitPos = Vec3()); void CheckSendOnCollideInfo(SendCollideInfo& preSendInfo, SendCollideInfo& newSendInfo, bool isTrigger);
		void AddOnCollideInfo(const SendInfo& info, OnCollideInfoKind kind);
		void OnCollideInfo(const std::weak_ptr<Collidable>& own, const std::weak_ptr<Collidable>& send, ColliderBase::ColideTag ownTag, ColliderBase::ColideTag sendTag, const Vec3& hitPos, OnCollideInfoKind kind);
		void FixPos() const;

	private:
		std::vector<std::shared_ptr<Collidable>> m_collidables;
		std::list<std::shared_ptr<Collidable>> m_stageCollidables;

		std::list<OnCollideInfoData> m_onCollideInfo;
		SendCollideInfo m_newCollideInfo;
		SendCollideInfo m_preCollideInfo;
		SendCollideInfo m_newTirrigerInfo;
		SendCollideInfo m_preTirrigerInfo;
	};
}

