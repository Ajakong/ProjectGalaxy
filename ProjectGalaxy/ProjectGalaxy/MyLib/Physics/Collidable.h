﻿#pragma once
#include <memory>
#include <vector>
#include <list>
#include "ObjectTag.h"
#include "Rigidbody.h"
#include "ColliderBase.h"

namespace MyEngine
{
	class Physics;
//	class ColliderBase;

	/// <summary>
	/// 衝突できるもの
	/// </summary>
	class Collidable : public std::enable_shared_from_this<Collidable>
	{
		friend Physics;
	public:
		struct CollideInfo
		{
			std::shared_ptr<ColliderBase> col;
			ColideTag tag;
		};

		struct UserData
		{
			float dissolveY;	// ディゾルヴしたい高さ
			float minY;
			float maxY;
			float dummy;
			float clickedU;
			float clickedV;
			float dummy2[2];
		};

		// 優先度判別
		enum class Priority : int
		{
			Lowest,
			Low,		// 低
			Middle,		// 中
			High,		// 高
			StageGimmick,//ステージギミック
			Boss,//ボス
			Static,		// 動かない（最高）
		};

		enum State
		{
			Intro,
			Neutral,
			Walk,
			Dush,
			Jump,
			JumpBoost,
			Spin,
			Roll,
			JumpDrop,
			FullpowerJumpDrop,
			Land,
			Damage,
			Boosting,
			Operation,
			Search,
			Chase,
			ComeBack,
			Attack,
			Tackle,
			Running,
			Stan,
			Death,
			End,
		};
	public:

		float GetAngle(Vec3 a, Vec3 b)
		{
			a.Normalize();
			b.Normalize();

			float cos = Dot(a, b);
			// コサインをクリッピング
			cos = max(-1.0f, min(1.0f, cos));

			float rad = acos(cos);

#ifdef DEBUG
			DrawFormatString(0, 125, 0xffffff, "rad(%f),deg(%f)", rad, rad * 180 / DX_PI_F);
#endif

			float epsilon = 1e-6f;
			if (cos > 1.0f - epsilon) {
				return 0;  // ほぼ同じベクトルなら角度0
			}
			return rad;
		}

		// 3Dモデルの上方向ベクトルを取得する関数
		Vec3 GetUpDirection(int modelHandle)
		{
			// モデルのワールド行列を取得
			MATRIX matrix;
			matrix=MV1GetMatrix(modelHandle);

			// ワールド行列の第2列が上方向ベクトル
			Vec3 upVector = { matrix.m[1][0], matrix.m[1][1], matrix.m[1][2] };

			return upVector;
		}



		Collidable(Priority priority, ObjectTag tag);
		Collidable(std::shared_ptr<Collidable> col);
		virtual ~Collidable();

		virtual void Init()=0;
		virtual void Update() = 0;
		virtual void Draw()=0;

		// 衝突したとき
		virtual void OnCollideEnter(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag) {}
		virtual void OnCollideStay(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag) {}
		virtual void OnCollideExit(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag) {}
		virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag) {}
		virtual void OnTriggerStay(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag) {}
		virtual void OnTriggerExit(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag) {}

		/* Getter */	
		ObjectTag GetTag() const { return m_tag; }
		Priority GetPriority() const { return m_priority; }
		State GetState()const { return m_state; }
		std::string GetStateName() const { return m_stateName; }
		void SetObjectTag(ObjectTag tag) { m_tag = tag; }
		bool IsAntiGravity() { return m_isAntiGravity; }
		void SetIsActive(bool flag) { m_isActive = flag; }
		bool GetIsActive() { return m_isActive; }
		void SetUpVec(Vec3 vel) { m_nextUpVec = vel; }
		Vec3 GetUpVec() { return m_upVec; }
		Vec3 GetFrontVec() { return m_frontVec; }
		// 当たり判定を無視（スルー）するタグの追加/削除
		void AddThroughTag(ObjectTag tag);
		void RemoveThroughTag(ObjectTag tag);
		std::shared_ptr<Rigidbody> PlanetOnlyGetRigid() { return m_rigid; };
		void SetReverceGravityVec(Vec3 gravityReverce) { m_upVec = gravityReverce; }

		// 当たり判定を無視（スルー）する対象かどうか
		bool IsThroughTarget(std::shared_ptr<Collidable>) const;
		virtual bool IsDestroy() { return m_isDestroyFlag; }

		Vec3 GetKnockBackVelocity() { return (m_rigid->GetVelocity())*-1; }
		

		std::shared_ptr<Rigidbody> GetRigidbody() const { return m_rigid; }

		int gravityEffectCount = 0;
	protected:
		std::shared_ptr<CollideInfo> AddCollider(const ColliderBase::Kind& kind,const ColideTag& tag);
		void RemoveCollider(std::shared_ptr<Collidable::CollideInfo> col);

		void SetAntiGravity(bool flag = true) { m_isAntiGravity=flag; }
	protected:
		// 物理データ
		std::shared_ptr<Rigidbody> m_rigid;
		// 当たり判定データ
		std::vector<std::shared_ptr<CollideInfo>> m_colliders;
		//補間先のUpVec
		Vec3 m_nextUpVec;
		Vec3 m_upVec;
		Vec3 m_postUpVec;
		Vec3 m_frontVec;
		Vec3 m_sideVec;

		State m_state;
		std::string m_stateName;


		bool m_isDestroyFlag;

		int m_cbuffH;
		UserData* m_userData;
		
	private:
		std::list<ObjectTag>	m_throughTags;
		
		ObjectTag m_tag;
		Priority m_priority;
		bool m_isAntiGravity;
		bool m_isActive;
		
	};
}

