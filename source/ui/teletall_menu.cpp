#include "teletall_menu.h"

//___________________________________________________________________________________________________
//|==================================================================================================|
//| * * * * * * * * * * * * * * * * * * * [O B J E C T]* * * * * * * * * * * * * * * * * * * * * * * |
//|CLASS:---------------------------------[Bar Menu]

//_____________________________
//=============>>>  Constructor.
BarMenu::BarMenu(SDL_Window* hwnd, std::vector<std::string> options, int button_w)
{
    int h; int w;
    SDL_GetWindowSize(hwnd,&w,&h);
    _menurec = {0, 0, w, h};
    for(int i = 0; i < options.size(); i++)
    {
        MenuItem.emplace_back(new MenuButton( 3 + ((button_w + 2) * i) , 3, button_w, options[i],true));
        Menu_Map[options[i]] = new ColumnMenu();
    }
    active = "null";
}
//_____________________________
//=============>>>  Destructor.
BarMenu::~BarMenu()
{
    if(!MenuItem.empty())
    {
        for (auto &e : MenuItem)
        {
            std::string name = e->GetName();
            std::map<std::string,ColumnMenu*>::iterator itr = Menu_Map.find(name);
            if(itr != Menu_Map.end())
            {
                delete itr->second;
                Menu_Map.erase(itr);
            }
            delete e;
        }     
    }
    
    
}


//_____________________________
//=============>>>  Added SubMenu.
void BarMenu::AddedSubMenu(std::string name, ColumnMenu* sub_menu)
{
    if(Menu_Map.find(name) != Menu_Map.end())
    {
        *Menu_Map[name] = std::move(*sub_menu); // so this external sub menu will be deleted inside this menu object.
    }
    for(int i =0; i< MenuItem.size(); i++)
    {
        if(MenuItem[i]->GetName() == name)
        {
            MenuItem[i]->HaveChildTrue();
        }
    }

}

//_____________________________
//=============>>>  Add Commands to buttons.
void BarMenu::AddCommand(std::vector<cmd_KEY> cmd_list)
{
    if (!MenuItem.empty() && !cmd_list.empty())
    {
        for(size_t i = 0; i<MenuItem.size(); i++)
        {
            MenuItem[i]->SetCommand(cmd_list[i]);
        }
    }
}



//_____________________________
//=============>>>  Update.
void BarMenu::Update(Telecontroller* controller)
{
    
    _menurec.h = ScreenText::GetUniversalTextHeight() + 10;
    int h;
    SDL_GetWindowSize(controller->GetHWND(), &_menurec.w, &h);

    if (controller->GetMousePoint()->InBound(0, 0, _menurec.w, _menurec.h))
    {
        controller->current_panel = PanelID::ON_MENU;
    }else
    {
        controller->current_panel = PanelID::NONE;
    }



    
    
        for (auto &b : MenuItem)
        {
            std::string name = b->GetName();
            std::map<std::string, ColumnMenu *>::iterator itr = Menu_Map.find(name);
            if (itr != Menu_Map.end())
            {

                ButtonStates state = b->GetState();

                if (active == "null")
                {
                    if (state == ButtonStates::CLICKED)
                    {
                        b->SetOpenState();
                        Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, true);
                        active = name;
                    }
                    else
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, false);
                        b->ReleaseState();
                    }
                }

                else if (active != name)
                {
                    if ((state == ButtonStates::CLICKED))
                    {
                        for (auto &b2 : MenuItem)
                        {
                            if (b2->GetName() == active)
                            {
                                b2->ShouldRest();
                                break;
                            }
                        }
                        Menu_Map[active]->Update(controller, b->GetButtonX(), _menurec.h, false);
                        Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, true);
                        active = name;
                    }
                    else
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, false);
                        b->ReleaseState();
                    }
                }

                else if (active == name)
                {
                    if (state == ButtonStates::CLICKED)
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, false);
                        active = "null";
                        b->ReleaseState();
                    }
                    if (state == ButtonStates::OPENED)
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, true);
                    }
                    else
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, false);
                        active = "null";
                        b->ReleaseState();
                    }
                }
                else
                {
                    Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, false);
                    active = "null";
                    b->ReleaseState();
                }

                //if (state == ButtonStates::CLICKED){controller->SendCommandEx(b->GetCommand(),"");}

                b->Update(controller);
            }
        }

        if (controller->ShouldCloseMenu())
        {
            for (auto &b : MenuItem)
            {
                std::string name = b->GetName();
                std::map<std::string, ColumnMenu *>::iterator itr = Menu_Map.find(name);
                if (itr != Menu_Map.end())
                {
                    Menu_Map[name]->Update(controller, b->GetButtonX(), _menurec.h, false);
                    active = "null";
                    b->ReleaseState();
                }
                b->Update(controller);
            }
        }

        controller->LinkMenuRec(&_menurec);
    }

//_____________________________
//=============>>>  Draw.
void BarMenu::Draw(SDL_Renderer* renderer)const
{
    //Drawing Rectangle
    SDL_SetRenderDrawColor(renderer, 55,55,55,255);
    SDL_RenderFillRect(renderer,&_menurec);
    //Drawing Buttons
    for(auto &b: MenuItem)
    {
        b->Draw(renderer);
        Menu_Map.find(b->GetName())->second->Draw(renderer);
    }
    
}





//___________________________________________________________________________________________________
//|==================================================================================================|
//| * * * * * * * * * * * * * * * * * * * [O B J E C T]* * * * * * * * * * * * * * * * * * * * * * * |
//|CLASS:---------------------------------[Column Menu]

//_____________________________
//=============>>>  Destructor.
ColumnMenu::~ColumnMenu()
{
    if(!MenuItem.empty())
    {
        for (auto &e : MenuItem)
        {
            std::string name = e->GetName();
            std::map<std::string,ColumnMenu*>::iterator itr = Menu_Map.find(name);
            if(itr != Menu_Map.end())
            {
                delete itr->second;
                Menu_Map.erase(itr);
            }
            delete e;
        }     
    }
    
}
//_____________________________
//=============>>>  Constructor.
ColumnMenu::ColumnMenu(std::vector<std::string> options, int button_w)
{
    int h = 16; //This is a hardcoded height, unit
    int x = 0; //so x and y will be reimplimented at UPDATE, now just a init for the rec and buttons
    int y = 0;
    _menurec = {x, y, button_w, 7 + (int)((h+4) * (int)options.size())}; //Hardcoded sizes, 
    _initPos.New(x,y);

    for(int i = 0; i < options.size(); i++)
    {
        MenuItem.emplace_back(new MenuButton( x+3 , 4 + y + i*(h+4), button_w-6, options[i],false));
        Menu_Map[options[i]] = new ColumnMenu();
    }
    _ifshown = false;
    active = "null";
}

//_____________________________
//=============>>>  Added SubMenu.
void ColumnMenu::AddedSubMenu(std::string name, ColumnMenu* sub_menu)
{
    if (Menu_Map.find(name) != Menu_Map.end())
    {
        *Menu_Map[name] = std::move(*sub_menu); // so this external sub menu will be deleted inside this menu object.
    }
    for (int i = 0; i < MenuItem.size(); i++)
    {
        if (MenuItem[i]->GetName() == name)
        {
            MenuItem[i]->HaveChildTrue();
        }
    }
}

//_____________________________
//=============>>>  Add Commands to buttons.
void ColumnMenu::AddCommand(std::vector<cmd_KEY> cmd_list)
{
    if (!MenuItem.empty() && !cmd_list.empty())
    {
        for(size_t i = 0; i<MenuItem.size(); i++)
        {
            MenuItem[i]->SetCommand(cmd_list[i]);
        }
    }
}

//_____________________________
//=============>>>  Update.
void ColumnMenu::Update(Telecontroller* controller, int x, int y, bool show)
{
    _menurec.x = _initPos.x + x;
    _menurec.y = _initPos.y + y;

    this->_ifshown = show;

    if (_ifshown)
    {
        
        for (auto &b : MenuItem)
        {
            b->PositionOffset(x, y);

            std::string name = b->GetName();
            std::map<std::string, ColumnMenu *>::iterator itr = Menu_Map.find(name);

        
            if (itr != Menu_Map.end())
            {
                ButtonStates state = b->GetState();
                
                if (active == "null")
                {
                    if (state == ButtonStates::CLICKED)
                    {
                        b->SetOpenState();
                        Menu_Map[name]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), true);
                        active = name;
                    }
                    else
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), false);
                        b->ReleaseState();
                    }
                }

                else if (active != name)
                {
                    if ((state == ButtonStates::CLICKED))
                    {
                        for (auto &b2 : MenuItem)
                        {
                            if (b2->GetName() == active)
                            {
                                b2->ShouldRest();
                                break;
                            }
                        }
                        Menu_Map[active]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), false);
                        Menu_Map[name]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), true);
                        active = name;
                    }
                    else
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), false);
                        b->ReleaseState();
                    }
                }

                else if (active == name)
                {
                    if (state == ButtonStates::CLICKED)
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), false);
                        active = "null";
                        b->ReleaseState();
                    }
                    if (state == ButtonStates::OPENED)
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), true);
                    }
                    else
                    {
                        Menu_Map[name]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), false);
                        active = "null";
                        b->ReleaseState();
                    }
                }
                else
                {
                    Menu_Map[name]->Update(controller, b->GetButtonX() + _menurec.w, b->GetButtonY(), false);
                    active = "null";
                    b->ReleaseState();
                }


                b->Update(controller);
                if(state != ButtonStates::NORMAL)
                {
                    controller->current_panel = PanelID::ON_MENU;
                }
            }
        }
    }
}



//_____________________________
//=============>>>  Draw.
void ColumnMenu::Draw(SDL_Renderer* renderer)const
{
    if (this->_ifshown)
    {
        //Drawing back rectangle
        SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
        SDL_RenderFillRect(renderer, &_menurec);
        //drawing button
        for (auto &b : MenuItem)
        {
            b->Draw(renderer);
            Menu_Map.find(b->GetName())->second->Draw(renderer);
            if(b->HaveChild())
            {
                SDL_SetRenderDrawColor(renderer, 52, 52, 52, 255);
                Point2D<int> pt(b->GetButtonX()+b->GetButtonWidth()-5,b->GetButtonY()+(b->GetButtonHeight()/2));
                SDL_RenderDrawLine(renderer,pt.x-6,b->GetButtonY()+3,pt.x,pt.y);
                SDL_RenderDrawLine(renderer,pt.x-6,b->GetButtonY()+b->GetButtonHeight()-4,pt.x,pt.y);
                SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);
                SDL_RenderDrawLine(renderer,pt.x-7,b->GetButtonY()+3,pt.x-1,pt.y);
                SDL_RenderDrawLine(renderer,pt.x-7,b->GetButtonY()+b->GetButtonHeight()-4,pt.x-1,pt.y);
                SDL_RenderDrawLine(renderer,pt.x-5,b->GetButtonY()+3,pt.x+1,pt.y);
                SDL_RenderDrawLine(renderer,pt.x-5,b->GetButtonY()+b->GetButtonHeight()-4,pt.x+1,pt.y);
            }
        }

        
        //drawing shadow lines
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 100);
        SDL_RenderDrawLine(renderer, _menurec.x - 1, _menurec.y, _menurec.x - 1, _menurec.y + _menurec.h);
        SDL_RenderDrawLine(renderer, _menurec.x - 1, _menurec.y + 1 + _menurec.h, _menurec.x - 1 + _menurec.w, _menurec.y + 1 + _menurec.h);
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 50);
        SDL_RenderDrawLine(renderer, _menurec.x - 2, _menurec.y, _menurec.x - 2, _menurec.y + _menurec.h);
        SDL_RenderDrawLine(renderer, _menurec.x - 1, _menurec.y + _menurec.h + 2, _menurec.x - 1 + _menurec.w, _menurec.y + 2 + _menurec.h);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}
