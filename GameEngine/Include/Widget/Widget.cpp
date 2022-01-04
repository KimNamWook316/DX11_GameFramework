#include "Widget.h"

CWidget::CWidget()	:
	mOwner(nullptr),
	mZOrder(0)
{
}

CWidget::CWidget(const CWidget& widget)
{
}

CWidget::~CWidget()
{
}

bool CWidget::Init()
{
	return true;
}

void CWidget::Start()
{
}

void CWidget::Update(float deltaTime)
{
}

void CWidget::PostUpdate(float deltaTime)
{
}

void CWidget::Render()
{
}
