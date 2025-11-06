#pragma once

#include "GameEntity.h"

class Floor :
    public GameEntity
{
    public:
        Floor* parent;
        float gCost;
        float hCost;
        bool isWalkable = true;
        int getCost() const;
        Floor();
        void Render(const IRenderHelpers&) const;
        void RenderGui(const IGuiHelpers&);
        int iIndex, jIndex;
        void StoreGridData(const int i, const int j, const vec3 originalColour);
        vec3 originalTileColour;
        Floor* GetThisFloor() { return this; }
};

