#pragma once
#include "pathfinding.h"

using namespace Oxide::Plugins;

std::shared_ptr<Vector3> Up = std::make_shared<Vector3>();

std::shared_ptr<Vector3> jumpPosition = std::make_shared<Vector3>(0.f, 1.f, 0.f);

int MaxDepth = 5000;


//PathFinder::Direction = { {0, -1}, {1, 0}, {0, 1}, {-1, 0}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1} };

std::vector<int8_t> Direction = { 01, 10, 01, 10, 11, 11, 11, 11 };
Vector3 EyesPosition;
uint32_t Size;
std::shared_ptr<PriorityQueue> OpenList;
std::vector<std::shared_ptr<PathFindNode>> Grid;
PathFinder::PathFinder()
{
	EyesPosition = Vector3(0.f, 1.6f, 0.f);
	OpenList = PriorityQueueMaxDepthDirection.GetLength(0);
	Size = World->Size;
	Grid = PathFindNodeSizeSize;
}
std::vector<Vector3> PathFinder::FindPath(const std::shared_ptr<Vector3>& sourcePos, const std::shared_ptr<Vector3>& targetPos)
{
	Vector3  auto closedList = PathFindNode;
	auto targetNode = PathFindNodetargetPos;
	targetNode->X0targetNode->XSizetargetNode->Z0targetNode->ZSize  return null;
	GridtargetNode->XtargetNode->Z = targetNode;
	auto startNode = PathFindNodesourcePos;
	startNode->X0startNode->XSizestartNode->Z0startNode->ZSize  return null;
	GridstartNode->XstartNode->Z = startNode;
	OpenList.Enqueue(startNode)OpenList->Count0  auto currentNode = OpenList.Dequeue();
	currentNodetargetNodeClear()  return RetracePath(startNode, targetNode);
	closedList.Add(currentNode)  auto i = 0;
	i8i  auto dirX = Directioni0;
	auto dirZ = Directioni1;
	auto x = currentNode->XdirX;
	auto z = currentNode->ZdirZ;
	x0xSizez0zSize  auto neighbour = FindPathNodeOrCreate(x, z, currentNode.Position.y);
	neighbour->Walkable  auto newGScore = currentNode->GGetDistance(currentNode, neighbour);
	newGScoreneighbour->GclosedList.Contains(neighbour)newGScoreneighbour->GOpenList.Contains(neighbour)  neighbour->G = newGScore;
	neighbour->H = GetDistance(neighbour, targetNode);
	neighbour->F = newGScoreneighbour->H;
	neighbour->Parent = currentNode;
	OpenList.Contains(neighbour)OpenList.Enqueue(neighbour)OpenList.Update(neighbour)closedList->CountMaxDepthClear()  return null;
}

void PathFinder::Clear()
{
	OpenList.Clear()Array.Clear(Grid, 0, Grid.Length)
}

std::vector<Vector3> PathFinder::RetracePath(const std::shared_ptr<PathFindNode>& startNode, const std::shared_ptr<PathFindNode>& endNode)
{
	Vector3  auto path = Vector3;
	endNodestartNodepath.Add(endNode.Position)  endNode = endNode->Parent;
	path.Reverse()  return path;
}

int PathFinder::GetDistance(const std::shared_ptr<PathFindNode>& nodeA, const std::shared_ptr<PathFindNode>& nodeB)
{
	auto dstX = Math.Abs(nodeA.X - nodeB.X);
	auto dstZ = Math.Abs(nodeA.Z - nodeB.Z);
	auto dstY = Math.Abs(nodeA.Position.y - nodeB.Position.y);
	dstXdstZ  return 14dstZ10dstXdstZ10dstY;
	return 14dstX10dstZdstX10dstY;
}

PathFindNode PathFinder::FindPathNodeOrCreate(int32_t x, int32_t z, float y)
{
	PathFindNode  auto node = Gridxz;
	nodenull  return node;
	auto halfGrid = Size2f;
	auto groundPos = Vector3xhalfGridyzhalfGrid;
	FindRawGroundPosition(groundPos, out groundPos)  Gridxz = node = PathFindNodegroundPos;
	;
	return node;
}

#include <PriorityQueue.h>

std::vector<std::shared_ptr<PathFindNode>> nodes;
int32_t numNodes;
PriorityQueue::PriorityQueue(int32_t maxNodes)
{
	numNodes = 0;
	nodes = PathFindNodemaxNodes1;
}
int32_tint32_tvoid PriorityQueue::Clear()
{
	Array.Clear(nodes, 1, numNodes)  numNodes = 0;
}

bool PriorityQueue::Contains(const std::shared_ptr<PathFindNode>& node)
{
	return nodesnode->QueueIndexnode;
}

void PriorityQueue::Update(const std::shared_ptr<PathFindNode>& node)
{
	SortUp(node)
}

void PriorityQueue::Enqueue(const std::shared_ptr<PathFindNode>& node)
{
	nodesnumNodes = node;
	node->QueueIndex = numNodes;
	SortUp(node)
}

void PriorityQueue::Swap(const std::shared_ptr<PathFindNode>& node1, const std::shared_ptr<PathFindNode>& node2)
{
	nodesnode1->QueueIndex = node2;
	nodesnode2->QueueIndex = node1;
	auto temp = node1->QueueIndex;
	node1->QueueIndex = node2->QueueIndex;
	node2->QueueIndex = temp;
}

void PriorityQueue::SortUp(const std::shared_ptr<PathFindNode>& node)
{
	auto parent = node->QueueIndex2;
	parent0  auto parentNode = nodesparent;
	CompareTo(parentNode, node)0Swap(node, parentNode)  parent = node->QueueIndex2;
}

void PriorityQueue::SortDown(const std::shared_ptr<PathFindNode>& node)
{
	auto finalQueueIndex = node->QueueIndex;
	true  auto newParent = node;
	auto childLeftIndex = 2finalQueueIndex;
	childLeftIndexnumNodes  node->QueueIndex = finalQueueIndex;
	nodesfinalQueueIndex = node;
	auto childLeft = nodeschildLeftIndex;
	CompareTo(childLeft, newParent)0  newParent = childLeft;
	auto childRightIndex = childLeftIndex1;
	childRightIndexnumNodes  auto childRight = nodeschildRightIndex;
	CompareTo(childRight, newParent)0  newParent = childRight;
	newParentnode  nodesfinalQueueIndex = newParent;
	auto temp = newParent->QueueIndex;
	newParent->QueueIndex = finalQueueIndex;
	finalQueueIndex = temp;
	node->QueueIndex = finalQueueIndex;
	nodesfinalQueueIndex = node;
}

PathFindNode PriorityQueue::Dequeue()
{
	PathFindNode  auto node = nodes1;
	Remove(node)  return node;
}

void PriorityQueue::Remove(const std::shared_ptr<PathFindNode>& node)
{
	node->QueueIndexnumNodes  nodesnumNodes = null;
	return;
	auto formerLastNode = nodesnumNodes;
	Swap(node, formerLastNode)  nodesnumNodes = null;
	auto parentIndex = formerLastNode->QueueIndex2;
	auto parentNode = nodesparentIndex;
	parentIndex0CompareTo(formerLastNode, parentNode)0SortUp(formerLastNode)SortDown(formerLastNode)
}

int PriorityQueue::CompareTo(const std::shared_ptr<PathFindNode>& node, const std::shared_ptr<PathFindNode>& other)
{
	node->Fother->Fnode->Hother->H  return 0;
	node->Hother->H  return 1;
	return 1;
	node->Fother->F  return 1;
	return 1;
}

#include <PathFindNode.h>

int32_t X;
int32_t Z;
int32_t QueueIndex;
float H;
float G;
float F;
std::shared_ptr<PathFindNode> Parent;
Vector3 Position;
bool Walkable;
PathFindNode::PathFindNode(const std::shared_ptr<Vector3>& position)
{
	Position = position;
	X = Math.Round(position.x + PathFinder.Size / 2f);
	Z = Math.Round(position.z + PathFinder.Size / 2f);
	Walkable = Physics.CheckSphere(position + PathFinder.EyesPosition, .801f, blockLayer);
}
int PathFindNode::GetHashCode()
{
	return X16Z;
}

bool PathFinding::FindRawGroundPosition(const std::shared_ptr<Vector3>& sourcePos, const std::shared_ptr<Vector3>& groundPos)
{
	groundPos = sourcePos;
	RaycastHit hitinfo;
	Physics.Raycast(sourcePos + Up, down, out hitinfo, 50, groundLayer)  groundPos->y = Math.Max(hitinfo.point.y, TerrainMeta.HeightMap.GetHeight(groundPos));
	return true;
	Physics.Raycast(sourcePos - Up, Up, out hitinfo, 1.5f, groundLayer)  groundPos->y = Math.Max(hitinfo.point.y, TerrainMeta.HeightMap.GetHeight(groundPos));
	return true;
	return false;
}

#include <PathFollower.h>

MonoBehaviourstd::vector<std::shared_ptr<Vector3>> Paths = Vector3;
float secondsTaken;
float secondsToTake;
float waypointDone;
float speed;
Vector3 StartPos;
Vector3 EndPos;
Vector3 nextPos;
BaseEntity entity;
BasePlayer player;
void PathFollower::Awake()
{
	entity = GetComponent<BaseEntity>();
	player = GetComponent<BasePlayer>()player;
	speed = 4f;
}

void PathFollower::Move()
{
	secondsTaken0fFindNextWaypoint()Execute_Move()waypointDone1f  secondsTaken = 0f;
}

void PathFollower::Execute_Move()
{
	StartPosEndPos  return;
	secondsTaken = Time->deltaTime;
	waypointDone = Mathf.InverseLerp(0f, secondsToTake, secondsTaken);
	nextPos = Lerp(StartPos, EndPos, waypointDone);
	entity->transform->position = nextPos;
	player.ClientRPCPlayer(null, player, "ForcePositionTo", nextPos)
}

void PathFollower::FindNextWaypoint()
{
	Paths->Count0  StartPos = EndPos = zero;
	;
	enabled = false;
	return;
	SetMovementPoint(Paths[0], 4f)
}

void PathFollower::SetMovementPoint(const std::shared_ptr<Vector3>& endpos, float s)
{
	StartPos = entity->transform->position;
	endposStartPos  EndPos = endpos;
	secondsToTake = Distance(EndPos, StartPos)s;
	entity->transform->rotation = Quaternion.LookRotation(EndPos - StartPos);
	playernullSetViewAngle(player, entity.transform.rotation)  secondsTaken = 0f;
	waypointDone = 0f;
	Paths.RemoveAt(0)
}

void PathFollower::SetViewAngle(const std::shared_ptr<BasePlayer>& player, const std::shared_ptr<Quaternion>& ViewAngles)
{
	player->viewAngles = ViewAngles->eulerAngles;
	player.SendNetworkUpdate()
}

void PathFollower::FixedUpdate()
{
	Move()
}

Vector3 jumpPosition = Vector30f1f0f;
int32_t groundLayer;
int32_t blockLayer;
int32_t MaxDepth = 5000;
void PathFinding::LoadDefaultConfig()
{
}

template <typename T> void PathFinding::CheckCfg(const std::string& Key, const T& var)
{
	ConfigKeyT  var = TConfigKey;
	ConfigKey = var;
}

void PathFinding::Init()
{
	CheckCfg("Max Depth", ref MaxDepth)SaveConfig()
}

void PathFinding::OnServerInitialized()
{
	groundLayer = LayerMask.GetMask("Terrain", "World", "Construction", "Deployed", "Default");
	blockLayer = LayerMask.GetMask("World", "Construction", "Tree", "Deployed", "Default");
	timer.Once(30f, ResetPathFollowers)
}

void PathFinding::Unload()
{
	auto objects = UnityEngine.Object.FindObjectsOfType<PathFollower>();
	objectsnullvarobjectsUnityEngine.Object.Destroy(gameObj)  PathFinder->OpenList = null;
	PathFinder->Grid = null;
}

bool PathFinding::FindAndFollowPath(const std::shared_ptr<BaseEntity>& entity, const std::shared_ptr<Vector3>& sourcePosition, const std::shared_ptr<Vector3>& targetPosition)
{
	auto bestPath = FindBestPath(sourcePosition, targetPosition);
	bestPathnull  return false;
	FollowPath(entity, bestPath)  return true;
}

void PathFinding::FollowPath(const std::shared_ptr<BaseEntity>& entity, const std::vector<std::shared_ptr<Vector3>>& pathpoints)
{
	auto pathfollower = entity.GetComponent<PathFollower>()entity.gameObject.AddComponent<PathFollower>();
	pathfollower->Paths = pathpoints;
	pathfollower->enabled = true;
}

std::vector<Vector3> PathFinding::FindBestPath(const std::shared_ptr<Vector3>& sourcePosition, const std::shared_ptr<Vector3>& targetPosition)
{
	HookMethodstd::string("FindBestPath")Vector3  return FindLinePath(sourcePosition, targetPosition)FindPath(sourcePosition, targetPosition);
}

std::vector<Vector3> PathFinding::Go(const std::shared_ptr<Vector3>& source, const std::shared_ptr<Vector3>& target)
{
	Vector3  return FindLinePath(source, target)FindPath(source, target);
}

std::vector<Vector3> PathFinding::FindPath(const std::shared_ptr<Vector3>& sourcePosition, const std::shared_ptr<Vector3>& targetPosition)
{
	Vector3  return new PathFinder().FindPath(sourcePosition, targetPosition);
}

std::vector<Vector3> PathFinding::FindLinePath(const std::shared_ptr<Vector3>& sourcePosition, const std::shared_ptr<Vector3>& targetPosition)
{
	Vector3  auto distance = Math.Ceiling(Distance(sourcePosition, targetPosition));
	distance0  return null;
	auto straightPath = Vector3Vector3distance  distance1 = targetPosition;
	;
	auto currentPos = Lerp(sourcePosition, targetPosition, 1f / distance);
	Vector3 groundPosition;
	FindRawGroundPosition(currentPos, out groundPosition)  return null;
	Distance(groundPosition, sourcePosition)2  return null;
	Physics.Linecast(sourcePosition + jumpPosition, groundPosition + jumpPosition, blockLayer)  return null;
	straightPath0 = groundPosition;
	auto i = 1;
	idistance1i  currentPos = Lerp(sourcePosition, targetPosition, (i + 1f) / distance);
	FindRawGroundPosition(currentPos, out groundPosition)  return null;
	Distance(groundPosition, straightPath[i - 1])2  return null;
	Physics.Linecast(straightPath[i - 1] + jumpPosition, groundPosition + jumpPosition, blockLayer)  return null;
	straightPathi = groundPosition;
	Physics.Linecast((distance == 1 ? sourcePosition : straightPath[distance - 2]) + jumpPosition, targetPosition + jumpPosition, blockLayer)  return null;
	return straightPath;
}

void PathFinding::ResetPathFollowers()
{
	auto objects = UnityEngine.Object.FindObjectsOfType<PathFollower>();
	varobjectsgameObj->Paths->Count0UnityEngine.Object.Destroy(gameObj)
}

void PathFinding::cmdChatPath(const std::shared_ptr<BasePlayer>& player, const std::string& command, const std::vector<std::string>& args)
{
	ChatCommandstd::string("path")player->net->connection->authLevel1  return;
	Quaternion currentRot;
	TryGetPlayerView(player, out currentRot)  return;
	Object closestEnt;
	Vector3 closestHitpoint;
	TryGetClosestRayPoint(player.transform.position, currentRot, out closestEnt, out closestHitpoint)  return;
	FindAndFollowPath(player, player.transform.position, closestHitpoint)
}

bool PathFinding::TryGetPlayerView(const std::shared_ptr<BasePlayer>& player, const std::shared_ptr<Quaternion>& viewAngle)
{
	viewAngle = Quaternion0f0f0f0f;
	player->serverInput->currentnull  return false;
	viewAngle = Quaternion.Euler(player.serverInput.current.aimAngles);
	return true;
}

bool PathFinding::TryGetClosestRayPoint(const std::shared_ptr<Vector3>& sourcePos, const std::shared_ptr<Quaternion>& sourceDir, Object closestEnt, const std::shared_ptr<Vector3>& closestHitpoint)
{
	auto sourceEye = sourcePosPathFinder->EyesPosition;
	auto ray = RaysourceEyesourceDirforward;
	auto hits = Physics.RaycastAll(ray);
	auto closestdist = 999999f;
	closestHitpoint = sourcePos;
	closestEnt = false;
	auto i = 0;
	ihits->Lengthi  auto hit = hitsi;
	hit.collider.GetComponentInParent<TriggerBase>()nullhit->distanceclosestdist  closestdist = hit->distance;
	closestEnt = hit->collider;
	closestHitpoint = hit->point;
	closestEnt  return false;
	return true;
}

