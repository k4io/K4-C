#pragma once
#include <memory>
#include <vector>
#include <Windows.h>
#include "utils/vector.hpp"
#include "rust/classes.hpp"
#include <string>

namespace Oxide {
	namespace Plugins {
		class PathFinder {
		public:
			const static std::vector<int8_t> Direction;
			PathFinder();
			static void Clear();
			static std::vector<Vector3> RetracePath(const std::shared_ptr<PathFindNode>& startNode, const std::shared_ptr<PathFindNode>& endNode);
			static int GetDistance(const std::shared_ptr<PathFindNode>& nodeA, const std::shared_ptr<PathFindNode>& nodeB);
			static PathFindNode FindPathNodeOrCreate(int32_t x, int32_t z, float y);
			const static std::shared_ptr<Vector3> EyesPosition;
			const static uint32_t Size;
			static std::shared_ptr<PriorityQueue> OpenList;
			static std::vector<std::shared_ptr<PathFindNode>> Grid;
			std::vector<Vector3> FindPath(const std::shared_ptr<Vector3>& sourcePos, const std::shared_ptr<Vector3>& targetPos);
		};
		class PriorityQueue {
		private:
			const std::vector<std::shared_ptr<PathFindNode>> nodes;
			int32_t numNodes = 0;
			void Swap(const std::shared_ptr<PathFindNode>& node1, const std::shared_ptr<PathFindNode>& node2);
			void SortUp(const std::shared_ptr<PathFindNode>& node);
			void SortDown(const std::shared_ptr<PathFindNode>& node);
			static int CompareTo(const std::shared_ptr<PathFindNode>& node, const std::shared_ptr<PathFindNode>& other);
		public:
			PriorityQueue(int32_t maxNodes);
			__declspec(property(get = GetCount))
				int32_t Count;
			int32_t GetCount() const;
			__declspec(property(get = GetMaxSize))
				int32_t MaxSize;
			int32_t GetMaxSize() const;
			void Clear();
			bool Contains(const std::shared_ptr<PathFindNode>& node);
			void Update(const std::shared_ptr<PathFindNode>& node);
			void Enqueue(const std::shared_ptr<PathFindNode>& node);
			PathFindNode Dequeue();
			void Remove(const std::shared_ptr<PathFindNode>& node);
		};
		class PathFindNode {
		public:
			const int32_t X = 0;
			const int32_t Z = 0;
			int32_t QueueIndex = 0;
			float H = 0.0f;
			float G = 0.0f;
			float F = 0.0f;
			std::shared_ptr<PathFindNode> Parent;
			std::shared_ptr<Vector3> Position;
			bool Walkable;
			PathFindNode(const std::shared_ptr<Vector3>& position);
			int GetHashCode();
		};
		class PathFollower {
		private:
			void Awake();
			void Move();
			void Execute_Move();
			void FindNextWaypoint();
			void SetViewAngle(const std::shared_ptr<BasePlayer>& player, const std::shared_ptr<Vector4>& ViewAngles);
			void FixedUpdate();
		public:
			std::vector<std::shared_ptr<Vector3>> Paths;
			float secondsTaken = 0.0f;
			float secondsToTake = 0.0f;
			float waypointDone = 0.0f;
			float speed = 0.0f;
			std::shared_ptr<Vector3> StartPos;
			std::shared_ptr<Vector3> EndPos;
			std::shared_ptr<Vector3> nextPos;
			std::shared_ptr<BaseEntity> entity;
			std::shared_ptr<BasePlayer> player;
			void SetMovementPoint(const std::shared_ptr<Vector3>& endpos, float s);
		};
		class PathFollower : public MonoBehaviour {
		private:
			void Awake();
			void Move();
			void Execute_Move();
			void FindNextWaypoint();
			void SetViewAngle(const std::shared_ptr<BasePlayer>& player, const std::shared_ptr<Vector4>& ViewAngles);
			void FixedUpdate();
			static int32_t MaxDepth;
			template <typename T> void CheckCfg(const std::string& Key, const T& var);
			void Init();
			void OnServerInitialized();
			void Unload();
			bool FindAndFollowPath(const std::shared_ptr<BaseEntity>& entity, const std::shared_ptr<Vector3>& sourcePosition, const std::shared_ptr<Vector3>& targetPosition);
			void FollowPath(const std::shared_ptr<BaseEntity>& entity, const std::vector<std::shared_ptr<Vector3>>& pathpoints);
			std::vector<Vector3> FindPath(const std::shared_ptr<Vector3>& sourcePosition, const std::shared_ptr<Vector3>& targetPosition);
			std::vector<Vector3> FindLinePath(const std::shared_ptr<Vector3>& sourcePosition, const std::shared_ptr<Vector3>& targetPosition);
			void ResetPathFollowers();
			void cmdChatPath(const std::shared_ptr<BasePlayer>& player, const std::string& command, const std::vector<std::string>& args);
			bool TryGetPlayerView(const std::shared_ptr<BasePlayer>& player, const std::shared_ptr<Vector4>& viewAngle);
			bool TryGetClosestRayPoint(const std::shared_ptr<Vector3>& sourcePos, const std::shared_ptr<Vector4>& sourceDir, Object closestEnt, const std::shared_ptr<Vector3>& closestHitpoint);
		protected:
			void LoadDefaultConfig();
		public:
			std::vector<std::shared_ptr<Vector3>> Paths;
			float secondsTaken = 0.0f;
			float secondsToTake = 0.0f;
			float waypointDone = 0.0f;
			float speed = 0.0f;
			std::shared_ptr<Vector3> StartPos;
			std::shared_ptr<Vector3> EndPos;
			std::shared_ptr<Vector3> nextPos;
			std::shared_ptr<BaseEntity> entity;
			std::shared_ptr<BasePlayer> player;
			void SetMovementPoint(const std::shared_ptr<Vector3>& endpos, float s);
			static std::shared_ptr<Vector3> jumpPosition;
			static int32_t groundLayer;
			static int32_t blockLayer;
			std::vector<Vector3> FindBestPath(const std::shared_ptr<Vector3>& sourcePosition, const std::shared_ptr<Vector3>& targetPosition);
			std::vector<Vector3> Go(const std::shared_ptr<Vector3>& source, const std::shared_ptr<Vector3>& target);
		};
	};
} /* Oxide*/ } /* Plugins*/