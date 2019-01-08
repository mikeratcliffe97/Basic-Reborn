//
// Created by Huxy on 03/10/2018.
//

#include "MyGame.h"
#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Platform.h>
#include <Engine/Sprite.h>
#include <Engine/FileIO.h>



MyGame::~MyGame()
{
    for (auto &location : locations)
    {
        if(location)
        {
            delete location;
            location = nullptr;
        }
    }

    inputs->unregisterCallback(key_callback_id);
}

bool MyGame::init()
{
    game_height = 1080;
    game_width = 1920;
    if (!initAPI()) {
        return false;
    }
    toggleFPS();
    renderer->setWindowTitle("Spooky Spaceship");

    renderer->setClearColour(ASGE::COLOURS::WHITE);
    renderer->setWindowedMode(ASGE::Renderer::WindowMode::WINDOWED);

//File IO Fns

    createLocationData();
    createItemData();


    inputs->use_threads = true;

    key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &MyGame::keyHandler, this);

    player = new Player();
    return true;
}

void MyGame::update(const ASGE::GameTime &us)
{
    //do some stuff / what's going on here?
    if (!in_menu)
    {
        {
            switch (selection)
            {
                case MenuOption::PLAY:
                {
                    //get coding you lazy mofo

                    in_menu = false;

                    
                    if (score == MAX_SCORE)
                    {
                        max_score = true;
                    }

                    //Space Lose Condition
                    if (player->getPlayerIndex() == 32)
                    {
                        dead_space = true;
                    }


                    //Win Condition
                    if (player->getPlayerIndex() == 0 && max_score)
                    {
                        game_won = true;
                    }

                    //Vault access conditions
                    if (passcode_entered && generator_on)
                    {
                        vault_access = true;
                    }

                  //Zombie bools
                    if (zombie1_dead)
                    {
                        zombie1 = false;
                    }

                    if (zombie2_dead)
                    {
                        zombie2 = false;
                    }

                    if (zombie3_dead)
                    {
                        zombie3 = false;
                    }

                        break;

                }

//                case MenuOption::CONTROLS:
//                {
//                    //write some options and context idk
//                    break;
//                }

                case MenuOption::QUIT:
                {
                    signalExit();
                }
            }
        }
    }
}

void MyGame::render(const ASGE::GameTime &us)
{

    if (in_menu)
    {
        renderMenu();
    }
    else if (!in_menu) {

        if (game_won) {
            renderer->renderText("Congratulations!\nYou managed to get every item on the ship\n and get out alive!",
                                 game_width / 4, game_height / 2, 2.0, ASGE::COLOURS::BLACK);
        }

        else if (!game_won) {

            renderUI();

            renderer->renderText(player->getPlayerInput() + "___", 80, 800, 2.0,
                                 ASGE::COLOURS::BLACK);

            if (!valid_command && player->getPlayerIndex() != 32)
            {
                IncorrectInput();
            }

            if (player->getPlayerIndex() == 0 && !game_won)
            {
                renderer->renderText("Your ship settles on the landing pad. \n"
                                     "To the east is the airlock.\n"
                                     "To the west is only cold, empty space.", 450, 300, 2.0,
                                     ASGE::COLOURS::BLACK);
            }


            if (dead_space)
            {
                renderer->renderText("You drift off into the cold, emptiness of space. \n"
                                     "\tThank you for playing.",
                                     450, 300, 2.0, ASGE::COLOURS::BLACK);
            }


            VaultMessages();

            ZombieMessages();

            GeneratorMessages();



            //Fire Room Messages
            if (player->getPlayerIndex() == 5 && room_fire)
            {
                renderer->renderText("There is a fire at the end of the room!\n"
                                     "Find some way to extinguish it\n to acccess the Armoury", 450, 300, 2.0,
                                     ASGE::COLOURS::BLACK);
            };
        }
    }

}

void MyGame::VaultMessages() const
{
    if (!vault_access && player->getPlayerIndex() == 2)
        {
            renderer->renderText("The top secret file lies\n in the room to the East... \n"
                                 "Find the passcode and reactivate\n the ship's power to enter.", 450, 300, 2.0,
                                 ASGE::COLOURS::BLACK);
        }

    if (vault_access && player->getPlayerIndex() == 2)
        {
            renderer->renderText("The vault to the East is open!", 450, 300, 2.0, ASGE::COLOURS::BLACK);
        }
}

void MyGame::GeneratorMessages() const {//Generator Hint Texts

    if (player->getPlayerIndex() == 16 && !keycard && !generator_on)
        {
            renderer->renderText("There is a generator in this room,\n"
                                 " but you need a keycard to activate it.",
                                 450, 300, 2.0, ASGE::COLOURS::BLACK);
        }

    if (player->getPlayerIndex() == 16 && keycard && !generator_on)
        {
            renderer->renderText("You have the keycard for the generator,\n"
                                 "You should activate it.",
                                 450, 300, 2.0, ASGE::COLOURS::BLACK);
        }

    if (player->getPlayerIndex() == 16 && generator_on)
        {
            renderer->renderText("The generator is active.\n "
                                 "Power has returned to the ship!", 450, 300, 2.0,
                                 ASGE::COLOURS::BLACK);
        }
}


void MyGame::renderMenu()
{
    renderer->renderText("Spooky Spaceship", 380, 400, 3.0, ASGE::COLOURS::BLACK);
    renderer->renderText("Can you recover the files and get out alive?", 400, 480, 2.0, ASGE::COLOURS::BLACK);
    renderer->renderText(menu_option == 0 ? ">Play" : "Play", 400, 520, 2.0, ASGE::COLOURS::BLACK);
  //  renderer->renderText(menu_option == 1 ? ">Controls" : "Controls", 400, 560, 2.0, ASGE::COLOURS::BLACK);
    renderer->renderText(menu_option == 1 ? ">Quit" : "Quit", 400, 560, 2.0, ASGE::COLOURS::BLACK);
    renderer->renderText("While in-game,\ntype HELP at any point\nto toggle the help menu",
            game_width - 500, game_height - 200, ASGE::COLOURS::BLACK);


}
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wlogical-op-parentheses"


 void MyGame::renderUI()
{

    std::string location_str = "Location: " + locations[player->getPlayerIndex()]->getLocName();

    renderer->renderText(location_str, 80, 80, 2.0, ASGE::COLOURS::BLACK);
    // if (player)


    Location *current_location = locations[player->getPlayerIndex()];
    if (current_location->getItem()) {
        std::string item_str = "Items in this room: " + current_location->getItem()->getItmName();
        renderer->renderText(item_str, 80, 200, 2.0, ASGE::COLOURS::BLACK);
    }

    else if (!current_location->getItem())
    {
        std::string item_str = "Items in this room: ";
        renderer->renderText(item_str, 80, 200, 2.0, ASGE::COLOURS::BLACK);
    }

    if (help_requested)
    {
        renderer->renderText("You can: \nActivate \nUse \nRead \nGo \nTake", 80, 260, 2.0, ASGE::COLOURS::BLACK);
    }

    std::string inventory_str = "Inventory:\n";

    for (int i = 0; i < ITEM_NUM; ++i)
    {
        Item *item = player->getItem(i);
        if (item)
        {
            inventory_str += item->getItmName() + "\n";

        }
    }

    renderer->renderText(inventory_str, game_width - 400, 300, 2.0, ASGE::COLOURS::BLACK);

    // int val = score;
    int val = score;
    renderer->renderText("SCORE: " + std::to_string(val), game_width - 400, 40, 2.0, ASGE::COLOURS::BLACK);

    if (max_score)
    {
        renderer->renderText("You have the maximum possible score!\nGet back to the landing pad!",
                game_width / 2, game_height - 200, 2.0, ASGE::COLOURS::BLACK);
    }


    if(flamethrower)
    {
        renderer->renderText("Flamethrower Uses: \n" + std::to_string(flamethrower_uses),
                             game_width - 575, 120, 2.0, ASGE::COLOURS::BLACK);
    }

    if (passcode_read)
    {
        renderer->renderText("Passcode = 2603", game_width - 575, 220, 2.0, ASGE::COLOURS::BLACK);
    }

    std::string dir_string = "Possible Directions: ";
    for (int i = 0; i < 4; ++i)
    {
        switch (locations[player->getPlayerIndex()]->getDirection(i))
        {
            case Directions::N:
            {
                dir_string += "N ";
                break;
            }
            case Directions::E:
            {
                dir_string += "E ";
                break;
            }
            case Directions::S:
            {
                dir_string += "S ";
                break;
            }
            case Directions::W:
            {
                dir_string += "W ";
                break;
            }
            default:
            {
                break;
            }
        }

    }
    renderer->renderText(dir_string, 80, 120, 2.0, ASGE::COLOURS::BLACK);
    renderer->renderText("What will you do? : ", 80, 760, 2.0, ASGE::COLOURS::BLACK);
}
void MyGame::keyHandler(const ASGE::SharedEventData data)
{

    std::string temp_input;

    auto key = static_cast<const ASGE::KeyEvent *>(data.get());

    if (key->key == ASGE::KEYS::KEY_ESCAPE)
    {
        signalExit();
    }

    //Menu Options
    if (in_menu)
    {
        MenuMovement(key);
    }

    if (!in_menu)
    {
        if (key->action == ASGE::KEYS::KEY_PRESSED)
        {
            if (key->key >= ASGE::KEYS::KEY_A &&
                key->key <= ASGE::KEYS::KEY_Z ||
                key->key >= ASGE::KEYS::KEY_0 && key->key <= ASGE::KEYS::KEY_9
                || key->key == ASGE::KEYS::KEY_SPACE)
            {

                temp_input = (key->key);
                player->setInput(player->getPlayerInput() + temp_input);
            }

            if (key->key == ASGE::KEYS::KEY_BACKSPACE)
            {

                player->subtractPlayerInput(1);
                player->setInput(player->getPlayerInput() + temp_input);
            }
        }

        if (key->key == ASGE::KEYS::KEY_ENTER)
        {

            ProcessInput();
            input_entered = true;
            player->subtractPlayerInput(player->getPlayerInput().length());
        }

    }

}


void MyGame::MenuMovement(const ASGE::KeyEvent *key)
{
    if ((key->key == ASGE::KEYS::KEY_RIGHT &&
        key->action == ASGE::KEYS::KEY_RELEASED) ||
            ( key->key == ASGE::KEYS::KEY_DOWN &&
        key->action == ASGE::KEYS::KEY_RELEASED)) {
        menu_option++;
        if (menu_option > 2) {
            menu_option = 0;
        }
    }

    if ((key->key == ASGE::KEYS::KEY_LEFT &&
        key->action == ASGE::KEYS::KEY_RELEASED) ||
            (key->key == ASGE::KEYS::KEY_UP &&
        key->action == ASGE::KEYS::KEY_RELEASED)) {
        menu_option--;
        if (menu_option < 0) {
            menu_option = 2;
        }
    }

    if (key->key == ASGE::KEYS::KEY_ENTER) {
        in_menu = false;
        switch (menu_option) {
            case 0: {
                selection = MenuOption::PLAY;
                break;
            }

//            case 1: {
//                selection = MenuOption::CONTROLS;
//                break;
//            }

            case 1: {
                selection = MenuOption::QUIT;
            }
        }
    }
}

#pragma clang diagnostic pop





void MyGame::getFile(std::string filepath, std::stringstream &ss)
{
    ASGE::FILEIO::IOBuffer buffer;
    ASGE::FILEIO::File file;
    file.open(filepath, file.IOMode::READ);
    buffer = file.read();
    std::string file_str (buffer.as_char(), buffer.length);
    ss << file_str;
}


void MyGame::createLocationData()
{
    createLocationDirData();


    std::stringstream location_strm;
    std::string locations_line;
    getFile("/data/Location/locationfile.txt", location_strm);
//  getFile("locationfile.txt", lo)
    int loc_cnt = 0;

    while (getline(location_strm, locations_line))
    {
        if(locations_line.back() ==  '\n' || locations_line.back() == '\r')
        {
            locations_line.resize(locations_line.size() - 1);
        }

        Location* new_loc = new Location(locations_line, directions[loc_cnt]);
        locations[loc_cnt] = new_loc;
        ++loc_cnt;

    }
}


void MyGame::createLocationDirData() {

    std::stringstream location_dir_strm;
    std::string dir_line;

    getFile("/data/Location/locationdirfile.txt", location_dir_strm);

    int dir_cnt = 0;
    while (getline(location_dir_strm, dir_line)) {
        if (dir_line.back() == '\n' || dir_line.back() == '\r') {
            dir_line.resize(dir_line.size() - 1);
        }

        bool more_dir = true;
        for (unsigned i = 0; i < 4; ++i) {
            if (more_dir) {
                char dir = dir_line.at(i);
                switch (dir) {

                    case 'N': {
                        directions[dir_cnt][i] = Directions::N;
                        break;
                    }

                    case 'E' : {
                        directions[dir_cnt][i] = Directions::E;
                        break;
                    }

                    case 'S' : {
                        directions[dir_cnt][i] = Directions::S;

                        break;
                    }

                    case 'W' : {
                        directions[dir_cnt][i] = Directions::W;
                        break;
                    }

                    case '-' : {
                        directions[dir_cnt][i] = Directions::NullDir;
                        more_dir = false;
                        break;
                    }

                    default: {
                        break;
                    }

                }
            } else {
                directions[dir_cnt][i] = Directions::NullDir;
            }
        }
        ++dir_cnt;
    }
}


void MyGame::createItemData()
{
    std::stringstream item_strm;
    std::string item_line;

    getFile("/data/Items/itemfile.txt", item_strm);
    int item_cnt = 0;

    while (getline(item_strm, item_line))
    {
        if (item_line.back() == '\n' || item_line.back() == '\r')
        {
            item_line.resize(item_line.size() -1);
        }

        Item* new_item = new Item(item_line);
//        if (item_line == "'")
//        {
//        locations[item_cnt]->setItem()
//        }
        locations[item_cnt]->setItem(new_item);
        ++item_cnt;
    }


}


void MyGame::ProcessInput() {

    //TODO Finish data validation for verbs


    if (player->getPlayerInput() == PASS && player->getPlayerIndex() == 2)
    {
        valid_command = true;
        passcode_entered = true;
    }




    if (player->getPlayerInput() == "CHEAT")
    {
        score = 8000;
    }

    if (player->getPlayerInput() == "HELP" && !help_requested)
    {
        help_requested = true;
    }

    else if (player->getPlayerInput() == "HELP" && help_requested)
    {
        help_requested = false;
    }

    std::string first_input = "";
    std::string second_input = "";
    std::size_t space_char = player->getPlayerInput().find(' ');

    if (space_char!=std::string::npos)
    {
        std::string first_input = player->getPlayerInput().substr(0, space_char);
        std::string second_input = player->getPlayerInput().substr(space_char + 1);



        char verb_switch_char =  first_input.at(0);
        switch (verb_switch_char)
        {

            case 'R' : //Assumes user has input read
            {
                if (second_input == "PASSCODE")
            {
                if (passcode)
                {
                    passcode_read = true;
                }
            }
                break;
            }


            case 'T' : //Assumes user has input take
            {
                Location *current_location = locations[player->getPlayerIndex()];

                if (current_location->getItem()
                    && second_input == current_location->getItem()->getItmName())
                {
                    player->addItem(current_location->getItem());
                    current_location->setItem(nullptr);
                    score += 500;

                    if (second_input == "FLAMETHROWER")
                    {
                        flamethrower = true;
                    }

                    if (second_input == "FIRE EXTINGUISHER")
                    {
                        fire_extinguisher = true;
                    }

                    if (second_input == "PASSCODE")
                    {
                        passcode = true;
                    }

                    if (second_input == "KEYCARD")
                    {
                        keycard = true;
                    }
                }
                break;
            }

            case 'U' : //Assumes user has input use
            {
               if (second_input == "FLAMETHROWER")
               {
                 if (flamethrower_uses != 0)
                 {
                   --flamethrower_uses;
                   if (player->getPlayerIndex() == 11 && flamethrower)
                   {
                     //  zombie1 = false;
                       zombie1_dead = true;
                       score += 500;
                   }

                   if (player->getPlayerIndex() == 15 && flamethrower)
                   {
                       zombie2_dead = true;
                       score += 500;
                   }

                   if (player->getPlayerIndex() == 28 && flamethrower)
                   {
                       zombie3_dead = true;
                       score += 500;
                   }
               }
               }

                //Activates Generator
                if (second_input == "GENERATOR")
                {
                    if (player->getPlayerIndex() == 16 && keycard)
                    {
                        generator_on = true;
                    }
                }

               //Extinguishes fire in fire
               if (second_input == "FIRE EXTINGUISHER" && player->getPlayerIndex() == 5 && fire_extinguisher)
               {
                    room_fire = false;
                    score += 500;
               }

                break;
            }

            case 'G' : //Assumes user has input go
            {
                PlayerMovement(second_input);
                break;
            }

            case 'A' : //Assumes player has input Activate
            {
                if (second_input == "GENERATOR")
                {
                    if (player->getPlayerIndex() == 16 && keycard)
                    {
                        generator_on = true;
                    }
                }
                else
                {
                    IncorrectInput();
                }
            }
            default:
            {
                break;
            }

        }

    }

}


void MyGame::PlayerMovement(std::string second_input) {

    switch (second_input.at(0))
    {
        case 'N' : {
            valid_command = checkLocDir(Directions::N);

            if (valid_command)
            {
                if (player->getPlayerIndex() == 11 && !zombie1_dead)
                {
                    valid_command = true;
                    zombie1 = true;
                }
                else if (!zombie1)
                {
                    player->setIndex_num(player->getPlayerIndex() + 4);
                }
            } else
            {
                valid_command = false;
            }
            break;
        }

        case 'E' :
            {
            valid_command = checkLocDir(Directions::E);
            if (valid_command)
            {

                if (player->getPlayerIndex() == 28 && !zombie3_dead)
                {
                    zombie3 = true;
                }
                if (vault_access && player->getPlayerIndex() == 2)
                {
                    player->setIndex_num(3);
                }

           else if (!zombie1 && !zombie2 && !zombie3  && player->getPlayerIndex() != 2)
                {
                    player->setIndex_num(player->getPlayerIndex() + 1);
                }

            } else {
                valid_command = false;
            }
            break;
        }

        case 'S' :
            {
            valid_command = checkLocDir(Directions::S);
            if (valid_command)
            {
                if  (player->getPlayerIndex() == 15 && !zombie2_dead)
                {
                    zombie2 = true;
                }

                if (player->getPlayerIndex() == 28 && !zombie3_dead)
                {
                    zombie3 = true;
                }

                 else if (!zombie1 && !zombie2 && !zombie3)
                {
                    player->setIndex_num(player->getPlayerIndex() - 4);
                }
            }

            else
                {
                valid_command = false;
            }
            break;
        }

        case 'W' : {
            valid_command = checkLocDir(Directions::W);
            if (valid_command)
            {

                if (player->getPlayerIndex() == 11 && !zombie1_dead)
                {
                    zombie1 = true;
                }

                if (player->getPlayerIndex() == 15 && !zombie2_dead)
                {
                    zombie2 = true;
                }



                if (player->getPlayerIndex() == 5 & !room_fire)
                {
                    player->setIndex_num(player->getPlayerIndex() - 1);
                }

               else if (!zombie1 && !zombie2 && !zombie3 && player->getPlayerIndex() != 0)
                {
                    player->setIndex_num(player->getPlayerIndex() - 1);
                }

                else if (player->getPlayerIndex() == 0)
                {
                    player->setIndex_num(32);
                }

            }
            else
            {
                valid_command = false;
            }
            break;
        }
    }
}


bool MyGame::checkLocDir(Directions dir)
{
    for (int i = 0; i < 4; ++i)
    {
        if(locations[player->getPlayerIndex()]->getDirection(i) == dir)
        {
            return true;
        }
    }
    return false;
}


void MyGame::IncorrectInput()
{

    renderer->renderText("Invalid Command \n"
                         "Type another command to continue", 500, 600, 2.0, ASGE::COLOURS::BLACK);


}

void MyGame::ZombieMessages() const {//Zombie Attack Text
    if ((zombie1 && player->getPlayerIndex() == 11)
        || (zombie2 && player->getPlayerIndex() == 15)
        || (zombie3 && player->getPlayerIndex() == 28)
            )
    {
        renderer->renderText("One of the crew attacks you! You cannot leave!",
                             450, 300, 2.0, ASGE::COLOURS::BLACK);
    }

    if ((zombie1_dead && player->getPlayerIndex() == 11)
        || (zombie2_dead && player->getPlayerIndex() == 15)
        || (zombie3_dead && player->getPlayerIndex() == 28)
            )
    {
        renderer->renderText("The room stinks of burnt flesh\n"
                             "and there is a charred corpse on the floor.",
                             450, 300, 2.0, ASGE::COLOURS::BLACK);
    }


    if (!zombie1_dead && player->getPlayerIndex() == 10)
    {
        renderer->renderText("You hear an angry growling in the\nQuarantine room to the East.\n"
                             "\tYou should not go in unarmed...", 350, 300, 2.0, ASGE::COLOURS::BLACK);
    }

    if (!zombie2_dead && player->getPlayerIndex() == 14)
    {
        renderer->renderText("You hear an angry growling in the\n Medical Bay to the East.\n"
                             "\tYou should not go in unarmed...", 350, 300, 2.0, ASGE::COLOURS::BLACK);
    }
}