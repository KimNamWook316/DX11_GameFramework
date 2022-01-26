#include "ShaderInfo.fx"

#define GRAVITY 9.8f

cbuffer ParticleCBuffer : register(b11)
{
	uint gParticleSpawnEnable;
	float3 gParticleStartMin;
	float3 gParticleStartMax;
	uint gParticleSpawnCountMax;
	float3 gParticleScaleMin;
	float gParticleLifeTimeMin;
	float3 gParticleScaleMax;
	float gParticleLifeTimeMax;
	float4 gParticleColorMin;
	float4 gParticleColorMax;
	float gParticleSpeedMin;
	float gParticleSpeedMax;
	int gParticleMove;
	int gParticleGravity;
	float3 gParticleMoveDir;
	int gParticle2D;
	float3 gParticleMoveAngle;
	float gParticleDummy;
}

struct ParticleInfo
{
	float3 WorldPos;
	float3 Dir;
	float Speed;
	float LifeTime;
	float LifeTimeMax;
	float Alive;
	float FallTime;
	float FallStartY;
};

struct ParticleInfoShared
{
	uint SpawnEnable;

	float3 ScaleMin;
	float3 ScaleMax;

	float4 ColorMin;
	float4 ColorMax;

	int GravityEnable;
};

// Read / Write 가능한 Structured Buffer
RWStructuredBuffer<ParticleInfo> gParticleArray : register(u0);
RWStructuredBuffer<ParticleInfoShared> gParticleShared : register(u1);

[numthreads(64, 1, 1)] // 스레드 그룹 스레드 수를 지정한다. ( x * y * z )
void ParticleUpdate(uint threadID : SV_DispatchThreadID)
{
	gParticleShared[0].SpawnEnable = gParticleSpawnEnable;
	gParticleShared[0].ScaleMin = gParticleScaleMin;
	gParticleShared[0].ScaleMax = gParticleScaleMax;
	gParticleShared[0].ColorMin = gParticleColorMin;
	gParticleShared[0].ColorMax = gParticleColorMax;
	gParticleShared[0].GravityEnable = gParticleGravity;
	
	// 동작되는 스레드의 수가 생성되는 파티클의 수보다 크거나 같을 경우
	// 잘못된 인덱스로 동작하므로, 처리 하지 않는다.
	if (gParticleSpawnCountMax <= threadID.x)
	{
		return;
	}

	// 살아있는 파티클인지 판단
	if (gParticleArray[threadID.x].Alive == 0)
	{
		int spawnEnable = gParticleShared[0].SpawnEnable;
		int exchange = 0;

		if (gParticleShared[0].SpawnEnable == 1)
		{
			int inputValue = spawnCount - 1;

			// 동기화 함수
			// exchange = gParticleShared[0].SpawnCount; gParticleShared[0].SpawnCount = inputValue;
			// exchange에는 이전 값, inputValue로 갱신
			InterlockedExchange(gParticleShared[0].SpawnEnable, inputValue, exchange);

			if (exchange == spawnEnable)
			{
				gParticleArray[threadID.x].Alive = 1;
			}
		}
		
		if (gParicleArray[threadID.x].Alive == 0)
		{
			return;
		}

		// 살려야 하는 파티클이라면 파티클 생성정보를 만들어낸다.
		float key = threadID.x / gParticleSpawnCountMax;
		
		// Rand는 0 ~ 25사이의 값
		float randomPos = float3(Rand(key), Rand(key * 2.f), Rand(key * 3.f));

		randomPos /= 25.f;

		float3 startRange = gParticleStartMax - gParticleStartMin;

		gParticleArray[threadID.x].WorldPos = randomPos.x * startRange + gParticleStartMin;

		gParticleArray[threadID.x].FallTime = 0.f;
		gParticleArray[threadID.x].FallStartY = gParticleArray[threadID.x].WorldPos.y;

		gParticleArray[threadID.x].LifeTime = 0.f;
		gParticleArray[threadID.x].LifeTimeMax = randomPos.x * (gParticleLifeTimeMax - gParticleLifeTimeMin)
			+ gParticleLifeTimeMin;

		if (gParticleMove == 1)
		{
			gParticleArray[threadID.x].Speed = randomPos.x * (gParticleSpeedMax - gParticleSpeedMin)
				+ gParticleSpeedMin;
			gParticleArray[threadID.x].Dir = normalize(RandomPos.xyz * 2.f - 1.f * gParticleMoveDir);

			// 각도로 처리한다면 x, y, z의 랜덤한 각도를 3개 얻어오고, 기준 방향을 중심으로 회전된 행렬을 곱해서
			// 회전된 방향을 구해주도록 한다.
		}
	}
	// 현재 생성이 되어 있는 파티클들 처리
	else
	{
		gParticleArray[threadID.x].LifeTime += gDeltaTime;

		// 이동
		if (gParticleMove == 1)
		{
			gParticleArray[threadID.x].WorldPos += gParticleArray[threadID.x].Dir * gParticleArray[threadID.x].Speed * gDeltaTime;
		}
		
		// 중력
		if (gParticleGravity == 1)
		{

		}

		// 수명 다 되면, Dead 처리
		if (gParticleArray[threadID.x].LifeTime >= gParticleArray[threadID.x].LifeTimeMax)
		{
			gParticleArray[threadID.x].Alive = 0;
		}
	}
}

struct VertexParticle
{
	float3 Pos : POSITION;
	uint InstanceID : SV_InstanceID; // 시스템 값 ( 정점 버퍼에서 만들어주는 value 아님)
};

struct VertexParticleOutput
{
	uint InstanceID : TEXCOORD;
};

VertexParticleOutput ParticleVS(VertexParticle input)
{
	VertexParticleOutput output = (VertexParticleOutput)0;

	output.InstanceID = input.InstanceID;

	return output;
}

struct GeometryParticleOutput
{
	float4 Pos : SV_POISTION;
	float4 Color : COLOR;
	float2 UV : TEXCOOR;
	float4 ProjPos : POSITION;
};

// Geometry Shader에서 기하구조 생성
// parameter keyword
// in : 값을 함수 안으로 넘겨줄 때
// out : 함수 안에서 결과를 넘겨받을 때, 함수 안에서 이 값을 이용한 연산 불가
// inout : Reference
// https://woo-dev.tistory.com/260  : GS 설명
[maxvertexcount(6)]
void ParticleGS(point VertexParticleOutput[1],
	inout TriangleStream<GeometryParticleOutput> output)
{
}

PSOutSingle ParticlePS(GeometryParticleOutput input)
{
	PSOutSingle	output = (PSOutSingle)0;

	return output;
}
