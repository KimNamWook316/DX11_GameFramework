#include "ShaderInfo.fx"

#define GRAVITY 9.8f

cbuffer ParticleCBuffer : register(b11)
{
	uint gParticleSpawnCount;
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
	float gEmpty;
}