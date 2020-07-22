#include "teletall_graphics.h"
#include "x_sdl2headers.h"

TTF_Font *ScreenText::gFont = NULL;
int ScreenText::counter = 0;
int ScreenText::uniHeight = 10;


void ScreenText::InitLoadMedia(int textsize)
{
    gFont = TTF_OpenFont("tltl_fonts/arial.ttf",textsize);
    if (gFont == NULL)
    {
        //SDL_ShowSimpleMessageBox(0, "TTF Error", "Font open failed", NULL);
    }
    uniHeight = textsize;
    SDL_Delay(10);
}
int ScreenText::GetUniversalTextHeight()
{
    return uniHeight;
}

ScreenText::ScreenText()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

ScreenText::~ScreenText()
{
    Free();
    TTF_CloseFont(gFont);
    gFont = NULL;
}


void ScreenText::Free()
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}


void ScreenText::loadFromRenderedText(
    std::string textureText,
    SDL_Renderer *renderer,
    SDL_Color textColor,
    SDL_Color backgroundColor,
    int mode
    )
{
    //Get rid of preexisting texture:
    Free();

    SDL_Surface *textSurface = NULL;
    if (mode == 1)
    {
        textSurface = TTF_RenderText_Shaded(gFont, textureText.c_str(), textColor, backgroundColor);
    }else if(mode == 0)
    {
        textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    }else
    {
        textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), textColor);
    }


    if (textSurface != NULL)
    {
        mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (mTexture == NULL)
        {
            SDL_ShowSimpleMessageBox(0, "TTF Error", "unable to create texture", NULL);
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    else
    {
         SDL_ShowSimpleMessageBox(0, "TTF Error", "unable to render text surface", NULL);
    }
}


void ScreenText::Draw(SDL_Renderer* renderer, int x, int y, double scale)
{
    mWidth = (int)(mWidth*scale);
    mHeight = (int)(mHeight*scale);
    SDL_Rect renderrecs = {x,y,mWidth,mHeight};
    SDL_Point pt = {x, y};
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer,mTexture, NULL, &renderrecs,0,&pt,flip);
}


void DrawDashLine(SDL_Renderer* renderer,Point2D<int> pt1, Point2D<int> pt2, int dist)
{
    Point2D<int> p1 = pt1;
    Point2D<int> p2 = pt2;

    Point2D<int> unit = p2 - p1;
    double distance = unit.Length();

    unit.MakeUnitVector();
    unit.x *= dist;
    unit.y *= dist;
    int count = (int)(distance / (dist));

    for (size_t i = 0; i < count; i++)
    {
        if(i%2 == 0){
        SDL_RenderDrawLine(renderer, p1.x + (unit.x*i), p1.y+ (unit.y*i), p1.x + (unit.x*(i+1)), p1.y+ (unit.y*(i+1)));
        }
    }
}



void DrawNurbs(SDL_Renderer* renderer, std::vector<Point2D<int>> points, int degree)
{
    if(degree == 1)
    {
        for(size_t i = 0; i<points.size()-1; i++)
        {
            SDL_RenderDrawLine(renderer,points[i].x, points[i].y, points[i+1].x, points[i+1].y);
        }
    }else if(degree == 3)
    {
        // std::vector<Point2D<int>> TempList1 = points;

        // for(size_t j = 0; j < 2; j++)
        // {
        //     std::vector<Point2D<int>> TempList;
        //     for(size_t i = 0; i< TempList1.size()-3;i++)
        //     {
        //         Point2D<int> tPt1(0, 0);
        //         tPt1.x = TempList1[i].x*0.5 + TempList1[i+1].x*0.5;
        //         tPt1.y = TempList1[i].y*0.5 + TempList1[i+1].y*0.5;
        //         TempList.push_back(tPt1);

        //         Point2D<int> tPt2(0, 0);
        //         tPt2.x = TempList1[i].x*0.125 + TempList1[i+1].x*0.75 + TempList1[i+2].x*0.125;
        //         tPt2.y = TempList1[i].y*0.125 + TempList1[i+1].y*0.75 + TempList1[i+2].y*0.125;
        //         TempList.push_back(tPt2);
        //     }
        //     TempList1 = TempList;
        // }
        // for(size_t i = 0; i<TempList1.size()-1; i++)
        // {
        //     SDL_RenderDrawLine(renderer,TempList1[i].x, TempList1[i].y, TempList1[i+1].x, TempList1[i+1].y);
        // }
        std::vector<Point2D<int>> tempList = points;

        for (size_t j = 0; j < 3; ++j)
        {
            std::vector<Point2D<int>> tempList2;
            for (size_t i = 0; i < tempList.size() - 2; i++)
            {
                if (i == 0)
                {
                    Point2D<int> be1 = tempList.front();
                    tempList2.emplace_back(std::move(be1));
                }

                Point2D<int> oneQ;
                oneQ.x = (int)((((double)tempList[i].x) * 0.5f) + (((double)tempList[i + 1].x) * 0.5f));
                oneQ.y = (int)((((double)tempList[i].y) * 0.5f) + (((double)tempList[i + 1].y) * 0.5f));
                //tempList.erase(tempList.begin());
                tempList2.emplace_back(std::move(oneQ));
                Point2D<int> threeQ;
                threeQ.x = (int)(((double)tempList[i].x) * 0.125f + ((double)tempList[i + 1].x) * 0.75f + ((double)tempList[i + 2].x) * 0.125f);
                threeQ.y = (int)(((double)tempList[i].y) * 0.125f + ((double)tempList[i + 1].y) * 0.75f + ((double)tempList[i + 2].y) * 0.125f);
                //tempList.erase(tempList.begin());
                tempList2.emplace_back(std::move(threeQ));
                Point2D<int> fQ;
                fQ.x = (int)((((double)tempList[i+1].x) * 0.5f) + (((double)tempList[i + 2].x) * 0.5f));
                fQ.y = (int)((((double)tempList[i+1].y) * 0.5f) + (((double)tempList[i + 2].y) * 0.5f));
                tempList2.emplace_back(std::move(fQ));

                if (i == tempList.size() - 3)
                {
                    Point2D<int> be2 = tempList.back();
                    tempList2.emplace_back(std::move(be2));
                }
            }
            tempList = tempList2;
        }

        for (size_t i = 0; i < tempList.size() -1; ++i)
        {
            SDL_RenderDrawLine(renderer, tempList[i].x, tempList[i].y, tempList[i + 1].x, tempList[i + 1].y);
        }
    }

   
}