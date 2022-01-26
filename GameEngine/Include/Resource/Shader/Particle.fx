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

// Read / Write ������ Structured Buffer
RWStructuredBuffer<ParticleInfo> gParticleArray : register(u0);
RWStructuredBuffer<ParticleInfoShared> gParticleShared : register(u1);

[numthreads(64, 1, 1)] // ������ �׷� ������ ���� �����Ѵ�. ( x * y * z )
void ParticleUpdate(uint threadID : SV_DispatchThreadID)
{
	gParticleShared[0].SpawnEnable = gParticleSpawnEnable;
	gParticleShared[0].ScaleMin = gParticleScaleMin;
	gParticleShared[0].ScaleMax = gParticleScaleMax;
	gParticleShared[0].ColorMin = gParticleColorMin;
	gParticleShared[0].ColorMax = gParticleColorMax;
	gParticleShared[0].GravityEnable = gParticleGravity;
	
	// ���۵Ǵ� �������� ���� �����Ǵ� ��ƼŬ�� ������ ũ�ų� ���� ���
	// �߸��� �ε����� �����ϹǷ�, ó�� ���� �ʴ´�.
	if (gParticleSpawnCountMax <= threadID.x)
	{
		return;
	}

	// ����ִ� ��ƼŬ���� �Ǵ�
	if (gParticleArray[threadID.x].Alive == 0)
	{
		int spawnEnable = gParticleShared[0].SpawnEnable;
		int exchange = 0;

		if (gParticleShared[0].SpawnEnable == 1)
		{
			int inputValue = spawnCount - 1;

			// ����ȭ �Լ�
			// exchange = gParticleShared[0].SpawnCount; gParticleShared[0].SpawnCount = inputValue;
			// exchange���� ���� ��, inputValue�� ����
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

		// ����� �ϴ� ��ƼŬ�̶�� ��ƼŬ ���������� ������.
		float key = threadID.x / gParticleSpawnCountMax;
		
		// Rand�� 0 ~ 25������ ��
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

			// ������ ó���Ѵٸ� x, y, z�� ������ ������ 3�� ������, ���� ������ �߽����� ȸ���� ����� ���ؼ�
			// ȸ���� ������ �����ֵ��� �Ѵ�.
		}
	}
	// ���� ������ �Ǿ� �ִ� ��ƼŬ�� ó��
	else
	{
		gParticleArray[threadID.x].LifeTime += gDeltaTime;

		// �̵�
		if (gParticleMove == 1)
		{
			gParticleArray[threadID.x].WorldPos += gParticleArray[threadID.x].Dir * gParticleArray[threadID.x].Speed * gDeltaTime;
		}
		
		// �߷�
		if (gParticleGravity == 1)
		{

		}

		// ���� �� �Ǹ�, Dead ó��
		if (gParticleArray[threadID.x].LifeTime >= gParticleArray[threadID.x].LifeTimeMax)
		{
			gParticleArray[threadID.x].Alive = 0;
		}
	}
}

struct VertexParticle
{
	float3 Pos : POSITION;
	uint InstanceID : SV_InstanceID; // �ý��� �� ( ���� ���ۿ��� ������ִ� value �ƴ�)
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

// Geometry Shader���� ���ϱ��� ����
// parameter keyword
// in : ���� �Լ� ������ �Ѱ��� ��
// out : �Լ� �ȿ��� ����� �Ѱܹ��� ��, �Լ� �ȿ��� �� ���� �̿��� ���� �Ұ�
// inout : Reference
// https://woo-dev.tistory.com/260  : GS ����
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
