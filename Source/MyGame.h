//
// Created by Huxy on 03/10/2018.
//

#ifndef HELLOASGE_MYGAME_H
#define HELLOASGE_MYGAME_H

#include <Engine/OGLGame.h>
#include "Location.h"
#include "Player.h"
#include "Item.h"
#include <sstream>

#include <string>
#include <Engine/FileIO.h>

class MyGame : public ASGE::OGLGame
{
public:
    virtual ~MyGame();
    bool init() override;
    void update(const ASGE::GameTime& us) override;
    void render(const ASGE::GameTime& us) override;

    //Menu Variables and Functions
    bool in_menu = true;
    void renderMenu();
    int menu_option = 0;
    enum class MenuOption : int{
        PLAY,
        QUIT};
    MenuOption  selection = MenuOption::PLAY;

    //File IO
    void getFile(std::string filepath, std::stringstream &ss);


    //Win Condition
    bool max_score = false;
    bool game_won = false;


    //Location
    void createLocationData();
    void createLocationDirData();


    //Enemies
    bool zombie1 = false;
    bool zombie1_dead = false;

    bool zombie2 = false;
    bool zombie2_dead = false;

    bool zombie3 = false;
    bool zombie3_dead = false;

    //Item

    void createItemData();

    //Generator power bools
    bool keycard = false;
    bool generator_on = false;

    //Vault Access bools
  bool passcode = false;
   bool passcode_read = false;
    bool passcode_entered = false;
    bool vault_access = false;

    //Flamethrower bools
    bool flamethrower = false;
    int flamethrower_uses = 3;

   //Flaming Corridor
   bool room_fire = true;
   bool fire_extinguisher = false;

    //Input Validation
    bool input_entered = false;
    void ProcessInput();
    bool checkLocDir(Directions dir);
    void IncorrectInput();

    //Hard code Bools

    bool valid_command = true;

    bool help_requested = true;

    bool dead_space = false;






private:


    void keyHandler(const ASGE::SharedEventData data);

    int   key_callback_id = -1;
    void renderUI();
    Location* locations[MAP_SIZE] = {nullptr};
    Directions directions[MAP_SIZE][4];
    Player* player = nullptr;



    float score = 0;

    void MenuMovement(const ASGE::KeyEvent *key);

    void Quit(const ASGE::KeyEvent *key);

    void PlayerMovement(std::string second_input);

    void ZombieMessages() const;

    void GeneratorMessages() const;

    void VaultMessages() const;
};


#endif //HELLOASGE_MYGAME_H
