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
    counter ++;
}

ScreenText::~ScreenText()
{
    Free();
    counter--;
    if (counter == 0)
    {
        TTF_CloseFont(gFont);
        gFont = NULL;
    }
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
    if (mTexture != NULL)
    {
        mWidth = (int)(mWidth * scale);
        mHeight = (int)(mHeight * scale);
        SDL_Rect renderrecs = {x, y, mWidth, mHeight};
        SDL_Point pt = {x, y};
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, mTexture, NULL, &renderrecs, 0, &pt, flip);
    }
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



void DrawNurbs(SDL_Renderer* renderer, std::vector<Point2D<double>> points, int degree, const SDL_Color& rendercolor)
{
    std::vector<Point2D<double>> tempList = points;
    if (degree == 3)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            std::vector<Point2D<double>> tempList2;
            for (size_t i = 0; i < tempList.size() - 2; i++)
            {
                if (i == 0)
                {
                    Point2D<double> be1 = tempList.front();
                    tempList2.emplace_back(std::move(be1));
                }

                Point2D<double> oneQ;
                oneQ.x = ((((double)tempList[i].x) * 0.5f) + (((double)tempList[i + 1].x) * 0.5f));
                oneQ.y = ((((double)tempList[i].y) * 0.5f) + (((double)tempList[i + 1].y) * 0.5f));
                //tempList.erase(tempList.begin());
                tempList2.emplace_back(std::move(oneQ));
                Point2D<double> threeQ;
                threeQ.x = (((double)tempList[i].x) * 0.125f + ((double)tempList[i + 1].x) * 0.75f + ((double)tempList[i + 2].x) * 0.125f);
                threeQ.y = (((double)tempList[i].y) * 0.125f + ((double)tempList[i + 1].y) * 0.75f + ((double)tempList[i + 2].y) * 0.125f);
                //tempList.erase(tempList.begin());
                tempList2.emplace_back(std::move(threeQ));
                Point2D<double> fQ;
                fQ.x = ((((double)tempList[i + 1].x) * 0.5f) + (((double)tempList[i + 2].x) * 0.5f));
                fQ.y = ((((double)tempList[i + 1].y) * 0.5f) + (((double)tempList[i + 2].y) * 0.5f));
                tempList2.emplace_back(std::move(fQ));

                if (i == tempList.size() - 3)
                {
                    Point2D<double> be2 = tempList.back();
                    tempList2.emplace_back(std::move(be2));
                }
            }
            tempList = tempList2;
        }
    }

    for (size_t i = 0; i < tempList.size() -1; ++i)
        {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, rendercolor.r, rendercolor.g, rendercolor.b ,rendercolor.a);
            //Drawing the center curve
            SDL_RenderDrawLine(renderer, tempList[i].x, tempList[i].y, tempList[i + 1].x, tempList[i + 1].y);

            //The following is for anti-aliasing
            Point2D<double> vecperp(0,0);
            Point2D<double> vecperp1(0,0);
            Point2D<double> vecperp2(0,0);
            vecperp.x = tempList[i + 1].y - tempList[i].y;
            vecperp.y = - (tempList[i + 1].x - tempList[i].x);

            vecperp1 = vecperp;
            vecperp2 = vecperp;
            
            /*
            --->       -> ->
            V(line) = (v2-v1)
                                     _    _     _       _       _    _
            --->             --->   |  Vx  |   |  0  -1  |     | -Vy  |
            V(line)_|_perp = V(line)|_ Vy _| * |_ 1   0 _|  =  |_ Vx _|
                                    
            */
            vecperp.MakeUnitVector();
            vecperp *= 0.3;
            SDL_SetRenderDrawColor(renderer, rendercolor.r, rendercolor.g, rendercolor.b ,100);
            SDL_RenderDrawLineF(renderer, (double)tempList[i].x + vecperp.x, (double)tempList[i].y + vecperp.y, (double)tempList[i + 1].x + vecperp.x, (double)tempList[i + 1].y  + vecperp.y);
            vecperp *= -1;
            SDL_RenderDrawLineF(renderer, (double)tempList[i].x + vecperp.x, (double)tempList[i].y + vecperp.y, (double)tempList[i + 1].x + vecperp.x, (double)tempList[i + 1].y  + vecperp.y);

            
            vecperp1.MakeUnitVector();
            vecperp1 *= 0.6;
            SDL_SetRenderDrawColor(renderer, rendercolor.r, rendercolor.g, rendercolor.b ,60);
            SDL_RenderDrawLineF(renderer, (double)tempList[i].x + vecperp1.x, (double)tempList[i].y + vecperp1.y, (double)tempList[i + 1].x + vecperp1.x, (double)tempList[i + 1].y  + vecperp1.y);
            vecperp1 *= -1;
            SDL_RenderDrawLineF(renderer, (double)tempList[i].x + vecperp1.x, (double)tempList[i].y + vecperp1.y, (double)tempList[i + 1].x + vecperp1.x, (double)tempList[i + 1].y  + vecperp1.y);

            // vecperp2.MakeUnitVector();
            // vecperp2 *= 0.8;
            // SDL_SetRenderDrawColor(renderer, rendercolor.r, rendercolor.g, rendercolor.b ,40);
            // SDL_RenderDrawLineF(renderer, (double)tempList[i].x + vecperp2.x, (double)tempList[i].y + vecperp2.y, (double)tempList[i + 1].x + vecperp2.x, (double)tempList[i + 1].y  + vecperp2.y);
            // vecperp2 *= -1;
            // SDL_RenderDrawLineF(renderer, (double)tempList[i].x + vecperp2.x, (double)tempList[i].y + vecperp2.y, (double)tempList[i + 1].x + vecperp2.x, (double)tempList[i + 1].y  + vecperp2.y);


            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }

   
}