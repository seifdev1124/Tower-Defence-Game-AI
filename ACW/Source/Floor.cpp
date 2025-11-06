#include "Floor.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

Floor::Floor()
{
	gCost = 0;
	hCost = 0;
	isWalkable = true;
}

void Floor::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Floor::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

void Floor::StoreGridData(const int i, const int j, const vec3 originalColour)
{
	iIndex = i;
	jIndex = j;
	originalTileColour = originalColour;
}

int Floor::getCost() const
{
	return hCost + gCost;
}


