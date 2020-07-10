#include "Curve.h"

ZCurve::ZCurve(Pt start, Pt end, int degree) : _degree(degree)
{
    active = true;
    pList.push_back(start); // p1
    Pt mid1;
    mid1.x = (int)((((double)start.x) * 0.6f) + (((double)end.x) * 0.4f));
    mid1.y = start.y;
    pList.push_back(mid1); // p2
    Pt mid2;
    mid2.x = (int)((((double)start.x) * 0.4f) + (((double)end.x) * 0.6f));
    mid2.y = end.y;
    pList.push_back(mid2);
    pList.push_back(end);
};

void ZCurve::DrawCurve(SDL_Renderer *renderer)
{
    
    std::vector<Pt> tempList = pList;
    if (_degree > 1)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            std::vector<Pt> tempList2;
            for (size_t i = 0; i < tempList.size() - 1; i++)
            {
                if (i == 0)
                {
                    Pt be1 = tempList[i];
                    tempList2.emplace_back(std::move(be1));
                }
                Pt oneQ;
                oneQ.x = (int)((((double)tempList[i].x) * 0.75f) + (((double)tempList[i + 1].x) * 0.25f));
                oneQ.y = (int)((((double)tempList[i].y) * 0.75f) + (((double)tempList[i + 1].y) * 0.25f));
                //tempList.erase(tempList.begin());
                tempList2.emplace_back(std::move(oneQ));
                Pt threeQ;
                threeQ.x = (int)(((double)tempList[i].x) * 0.25f + ((double)tempList[i + 1].x) * 0.75f);
                threeQ.y = (int)(((double)tempList[i].y) * 0.25f + ((double)tempList[i + 1].y) * 0.75f);
                //tempList.erase(tempList.begin());
                tempList2.emplace_back(std::move(threeQ));

                if (i == tempList.size() - 2)
                {
                    Pt be2 = tempList[i + 1];
                    tempList2.emplace_back(std::move(be2));
                }
            }
            tempList = tempList2;
        }
    }
    for (size_t i = 0; i < tempList.size() - 1; ++i)
    {
        SDL_RenderDrawLine(renderer, tempList[i].x, tempList[i].y, tempList[i + 1].x, tempList[i + 1].y);
    }
}




Pt ZCurve::GetStart()
{
    return pList[0];
}

Pt ZCurve::GetEnd()
{
    return pList[pList.size() - 1];
}