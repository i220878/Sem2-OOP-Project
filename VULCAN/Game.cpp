// At first I was really, really annoyed that Vector2f was no longer allowed.
// But it's at times like these that I'm reminded of
// Mick Gordon's 14 thousand word report on working on DOOM Eternal for Bethesda
// Or Tim Follin who was given a guitar made of sticks and old pieces of hair and just started
// SHREDDING on it (aka the NES system). That's the prime example of "Limits influence creativity".
// And instead of being infuriated, I become motivated. Because if they were able
// to go through that and survive, then I have no right to complain about my circumstances.

// I may be depressed. I may be overweight. I may be lazy. But I'm stubborn, and I have standards.

// "Pour your blood, tears, and soul into it, and look back in pride."

// Let's do this.

#include "Game.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <time.h>
#include <iostream>
#include <math.h>

#include "Spaceship.h"
#include "Bullet.h"
#include "Alpha.h"
#include "Beta.h"
#include "Gamma.h"
#include "Bomb.h"
#include "Menu.h"
#include "HUD.h"
#include "Addon.h"
#include "Levels.h"

void rollAddon(float x, float y, Addon*& addon, Spaceship& p, Levels*& level, bool asteroidsUnlocked);
bool hitTarget(Bullet& b, Enemy& e, Spaceship& p, Addon*& addon, Levels*& level, int& totalScore, bool asteroidsUnlocked, bool encounterActive);
bool playerEnemyCollision(Enemy& e, Spaceship& p, bool encounterActive, bool beginPhase);
bool playerBombCollision(Bomb& b, Spaceship& p);
bool bulletBombCollision(Bomb& bomb, Bullet& bullet);
void enemyUpAndDown(sf::Clock& eClock, Enemy* enemy, int size, bool isBoss);
void comeForth(Enemy*&, int&, Enemy*&, int&, Enemy*&, int&, bool&, sf::Time);
void twoFromSide(sf::RenderWindow& window, bool& sideGoonsDied, Levels*& level, sf::Sprite& movingBg, float& movingBgTimer, Spaceship& player);
void moveEye(sf::Sprite&, Spaceship&);
void vanish(Enemy*&, int&, Enemy*&, int&, Enemy*&, int&, bool&, float&);
void backFromTheDead(Enemy*&, int&, Enemy*&, int&, Enemy*&, int&, bool&, float&);
bool playerBeamCollision(Enemy*& boss, Spaceship& player);
void controlShenanigans(sf::RenderWindow& window, int& controlScheme, Spaceship& player, int& slowDownM, int& slowDownR, int& steeringSensitivity, float& fireTimer, float& CSTimer, sf::Time& dT, bool asteroidsUnlocked, HUD& hud, Levels*& level);
Game::Game() 
{
    srand(time(0));
    sf::Music menuMusic;
    menuMusic.setVolume(50);
    menuMusic.setLoop(true);

    sf::Music lvlMusic;
    lvlMusic.setVolume(50);
    lvlMusic.setLoop(true);

    sf::Music lvlEndMusic;
    lvlEndMusic.setVolume(50);
    lvlEndMusic.setLoop(false);
    
    menuMusic.openFromFile("audio/01_MAINMENU.ogg");

    const int X_Resolution = 1280;
    const int Y_Resolution = 1280;
	sf::RenderWindow window(sf::VideoMode(X_Resolution, Y_Resolution), "Vulcan", sf::Style::Close | sf::Style::Titlebar); // Don't add Style::Resize or else it will default to fullscreen.
    window.setPosition(sf::Vector2i(100,10));
    window.setSize(sf::Vector2u(X_Resolution / 2, Y_Resolution / 2));
    // These commands were only used for resizing the window, as my laptop has a small screen.
    // Miss Sher Bano told me to comment them, and tell the invigilator why they were used.
    // Other than these commands for resizing, both std::vector and sf::vector have not been used
    // across any document.

    Spaceship player;

    int slowDownR = 0;
    int slowDownM = 0;
    int steering = 7;
    int steeringSensitivity = 10 - steering;
    bool asteroidsUnlocked = false;

    sf::Clock clock;

    int A_Count = 0;
    int B_Count = 0;
    int G_Count = 0;
    Enemy* enemyA = nullptr;
    Enemy* enemyB = nullptr;
    Enemy* enemyG = nullptr;
    Enemy* boss = nullptr;

    Levels* level = nullptr;

    Addon* addon = nullptr;

    HUD hud;

    int controlScheme = 2;

    bool atMain = true;
    bool atPause = false;
    bool levelClear = false;
    bool gameOver = false;
    bool highScore = false;

    sf::Clock blinkTimer;
    bool blinkActive;

    sf::Clock deltaClock;

    float fireTimer = 0;
    float CSTimer = 0;
    float empoweredTime = 0;
    float onFireTime = 0;
    
    bool debugActive = false;

    MenuBG menuBG;
    sf::Clock eClock;
    sf::Clock bossClock;

    std::string name;
    int totalScore = 0;

    sf::Texture t1;
    sf::Sprite movingBg;
    float movingBgTimer = 0;

    sf::Texture tE;
    sf::Sprite theEarth;
    float earthTimer;
    tE.loadFromFile("img/backgrounds/saveTheWorld.png");
    theEarth.setTexture(tE);
    theEarth.setOrigin(40, 40);
    theEarth.setPosition(640, 640);
    theEarth.setScale(4, 4);
    theEarth.setTextureRect(sf::IntRect(0, 0, 80, 80));
    theEarth.setColor(sf::Color(255, 255, 255, 200));

    bool sideGoonsDied = false;
    bool beginPhase = true;

    bool encounterActive = false;
    // If an encounter is active, then regular enemies won't move, shoot, or be drawn.
    // When the encounter ends, they reappear.
    float deadAliveTimer = 0;
    float encounterTimer = 0;
    float moveBoss = 0;

    bool encounteredOnce = false;
    int spawnChance = 0;
    float rollTimer = 0;
    int increasingChance = 20;
    float beamBreak = 0;
    bool survived = true;

    sfmlText score;
    sf::Sprite blackPgram;
    sf::Texture pGramTex;
    pGramTex.loadFromFile("img/parallelogram_Black.png");
    blackPgram.setTexture(pGramTex);
    blackPgram.setOrigin(0, 0);
    blackPgram.setPosition(0, 1200);
    score.text.setPosition(80, 1200);

    sf::Clock timerDelay;
    bool timeUp = false;
    timerDelay.restart();

    while (window.isOpen())
    {
        asteroidsUnlocked = true;
        score.text.setString(std::to_string(totalScore));
        if(player.getSpeed() == 4)
            asteroidsUnlocked = true; // Only has to be run once. It's not set to false again.
            // Unless lives decrements.
        if(asteroidsUnlocked == true && player.getSpeed() == 4 && controlScheme != 2)
            player.setSpeed(3);
        else if(asteroidsUnlocked == true && player.getSpeed() != 4 && controlScheme == 2)
            player.setSpeed(4);
        steeringSensitivity = 10 - steering;
        if(slowDownM > 10 || slowDownR > 10) // Put as a check because sometimes it goes out of range and
        // the functions stop working entirely.
        {
            slowDownM = 0;
            slowDownR = 0;
        }
        if(timerDelay.getElapsedTime().asSeconds() > 20)
            timeUp = true;
        if(timeUp == true)
            timerDelay.restart();

        sf::Time dT = deltaClock.restart();

        if(dT > sf::seconds(0.1))
            dT = deltaClock.restart(); // Restart again in case it's too high of a value. Happens because of the menus.

        fireTimer += dT.asSeconds();
        CSTimer += dT.asSeconds();
        movingBgTimer += dT.asSeconds();
        // std::cout << movingBgTimer << std::endl;
        earthTimer += dT.asSeconds();
        deadAliveTimer += dT.asSeconds();
        if(encounteredOnce == false)
        {
            rollTimer += dT.asSeconds();
        }
        if(level != nullptr && timeUp == true)
            if(rollTimer > 5 && encounteredOnce == false && (level->levelNumber == 2 || level->levelNumber == 3)) // Roll every 20 seconds, and for every roll there's no encounter, increase the chances of spawning.
            {
                std::cout << "Rolling..." << std::endl;
                spawnChance = rand() % 100;
                if(spawnChance < increasingChance)
                {
                    encounterActive = true;
                    encounteredOnce = true;
                }
                else 
                {
                    increasingChance += 10;
                }
                rollTimer = 0;
            }
        if(encounterActive == true)
        {
            encounterTimer += dT.asSeconds();
            moveBoss += dT.asSeconds();
            boss->setFireTime(boss->getFireTime() + dT);
            beamBreak += dT.asSeconds();
        }
        if(player.isEmpowered())
            empoweredTime += dT.asSeconds();
        if(player.isOnFire())
            onFireTime += dT.asSeconds();
        for(int i = 0; i < A_Count; i++)
            enemyA[i].setFireTime(enemyA[i].getFireTime() + dT);
        for(int i = 0; i < B_Count; i++)
            enemyB[i].setFireTime(enemyB[i].getFireTime() + dT);
        for(int i = 0; i < G_Count; i++)
            enemyG[i].setFireTime(enemyG[i].getFireTime() + dT);
        
        if(level != nullptr)
            if(movingBgTimer > 0.03333 && level->levelNumber <= 3)
            {
                // std::cout << "help" << std::endl;
                movingBg.setTextureRect(sf::IntRect(0, movingBg.getTextureRect().top - 1, 320, 320));
                movingBgTimer = 0;
                if(movingBg.getTextureRect().top <= 0)
                    movingBg.setTextureRect(sf::IntRect(0, 320, 320, 320));
            }
        if(level != nullptr)
            if(earthTimer > 0.03333 && level->levelNumber == 6)
            {
                theEarth.setTextureRect(sf::IntRect(theEarth.getTextureRect().left + 80, theEarth.getTextureRect().top, 80, 80));
                earthTimer = 0;
                if(theEarth.getTextureRect().left >= 4800)
                    theEarth.setTextureRect(sf::IntRect(0, theEarth.getTextureRect().top + 80, 80, 80));
                if(theEarth.getTextureRect().top >= 4800)
                    theEarth.setTextureRect(sf::IntRect(0, 0, 80, 80));
            }
        int rememberSelection = 0;

        while(gameOver == true)
        {
            timeUp = false;
            asteroidsUnlocked = false;
            if(lvlMusic.getStatus() == sf::Music::Playing)
                lvlMusic.stop();
            if(menuMusic.getStatus() == sf::Music::Playing)
                menuMusic.stop();
            if(boss != nullptr)
            {
                delete boss;
                boss = nullptr;
            }
            if(enemyA != nullptr)
            {
                delete[] enemyA;
                A_Count = 0;
                enemyA = nullptr;
            }
            if(enemyB != nullptr)
            {
                delete[] enemyB;
                B_Count = 0;
                enemyB = nullptr;
            }
            if(enemyG != nullptr)
            {
                delete[] enemyG;
                G_Count = 0;
                enemyG = nullptr;
            }
            if(addon != nullptr)
            {
                delete addon;
                addon = nullptr;
            }
            GameOver m(window, totalScore);
            if(m.selection == 10)
                return;
            if(m.selection == 0)
            {
                gameOver = false;
                atMain = true;
                name = "";
                level->phase = 0;
                beginPhase = true;
                sideGoonsDied = false;
            }
        }

        while(levelClear == true)
        {
            timeUp = false;
            asteroidsUnlocked = false;
            if(lvlMusic.getStatus() == sf::Music::Playing)
                lvlMusic.stop();
            if(menuMusic.getStatus() == sf::Music::Playing)
                menuMusic.stop();
            if(boss != nullptr)
            {
                delete boss;
                boss = nullptr;
            }
            if(enemyA != nullptr)
            {
                delete[] enemyA;
                A_Count = 0;
                enemyA = nullptr;
            }
            if(enemyB != nullptr)
            {
                delete[] enemyB;
                B_Count = 0;
                enemyB = nullptr;
            }
            if(enemyG != nullptr)
            {
                delete[] enemyG;
                G_Count = 0;
                enemyG = nullptr;
            }
            if(addon != nullptr)
            {
                delete addon;
                addon = nullptr;
            }
            LevelClear m(window, menuBG, name, totalScore);
            if(m.selection == 10)
                return;
            Menu_HighScores m2(window, menuBG, lvlMusic, menuMusic);
            if(m2.selection == 0)
            {
                levelClear = false;
                atMain = true;
                name = "";
                level->phase = 0;
                beginPhase = true;
                sideGoonsDied = false;
            }
            if(m2.selection == 10)
                return;
        }
        while(atPause == true)
        {
            PauseMenu m(window, rememberSelection, debugActive, menuBG);
            if(m.selection == 0) // Resume
            {
                atPause = false;
                break;
            }
            while(m.selection == 1) // Options
            {
                Menu_Options m2(window, lvlMusic, steering, menuBG);
                if(m2.selection == 10)
                    return;
                if(m2.selection == 2) // Return to Pause Menu
                    break;
            }
            while(m.selection == 2) // Help
            {
                Menu_Help m2(window, menuBG);
                if(m2.selection == 10)
                    return;
                if(m2.selection == 0) // Return to Pause Menu
                    break;
            }
            while(m.selection == 3) // High Scores
            {
                Menu_HighScores m2(window, menuBG, lvlMusic, menuMusic);
                if(m2.selection == 10)
                    return;
                if(m2.selection == 0) // Return to Pause Menu
                    break;
            }
            if(m.selection == 4) // Return to Main Menu
            {
                beginPhase = true;
                sideGoonsDied = false;
                delete level;
                level = nullptr;
                if(addon != nullptr)
                {
                    delete addon;
                    addon = nullptr;
                }

                atMain = true;
                atPause = false;
                break;
            }
            if(m.selection == 5)
            {
                return;
            }
            while(m.selection == 6) // Debug
            {
                Menu_Debug m2(window, player, asteroidsUnlocked, menuBG);
                if(m2.selection == 10)
                    return;
                if(m2.selection == 6) // Return to Main Menu
                    break;                   
            }
            if(m.selection == 10)
                return;
            rememberSelection = m.selection;
        }
        while(atMain == true)
        {
            timeUp = false;
            beginPhase = false;
            encounterActive = false;
            encounteredOnce = false;
            encounterTimer = 0;
            asteroidsUnlocked = false;
            totalScore = 0;
            if(lvlMusic.getStatus() == sf::Sound::Playing)
                lvlMusic.pause();
            if(menuMusic.getStatus() != sf::Sound::Playing)
                menuMusic.play();
                
            MainMenu m(window, rememberSelection, debugActive, menuBG);
            while(m.selection == 0) // Start
            {
                MainMenu_Start m2(window, menuBG, lvlMusic);
                if(m2.selection == 10)
                    return;
                if(m2.selection == 0) // Level 1
                {
                    level = new Level1;
                    level->generateLevel(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, player, 0, boss);
                    // for(int i = 0; i < enemy[0].enemyCount; i++){enemy[0].restartFireClock();}
                    atMain = false;
                    t1.loadFromFile("img/backgrounds/1.png");
                    movingBg.setTexture(t1);
                    movingBg.setScale(4, 4);
                    movingBg.setOrigin(160, 160);
                    movingBg.setPosition(640, 640);
                    movingBg.setTextureRect(sf::IntRect(0, 320, 320, 320));
                    beginPhase = true;
                    break;
                }
                if(m2.selection == 1) // Level 2
                {
                    MainMenu_Start_Settings m3(window, menuBG);
                    if(m3.selection == 4)
                        break;
                    level = new Level2;
                    // 1 is Easy, 2 is Medium, 3 is Hard, 4 is Return
                    // 0 is just text for the Difficulty
                    level->generateLevel(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, player, m3.selection, boss);
                    atMain = false;
                    t1.loadFromFile("img/backgrounds/2.png");
                    movingBg.setTexture(t1);
                    movingBg.setScale(4, 4);
                    movingBg.setOrigin(160, 160);
                    movingBg.setPosition(640, 640);
                    movingBg.setTextureRect(sf::IntRect(0, 320, 320, 320));
                    beginPhase = true;
                    if(m3.selection == 10)
                        return;
                    break;
                }
                if(m2.selection == 2) // Level 3
                {
                    MainMenu_Start_Settings m3(window, menuBG);
                    if(m3.selection == 4)
                        break;
                    level = new Level3;
                    level->generateLevel(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, player, m3.selection, boss);

                    atMain = false;
                    t1.loadFromFile("img/backgrounds/3.png");
                    movingBg.setTexture(t1);
                    movingBg.setOrigin(160, 160);
                    movingBg.setPosition(640, 640);
                    movingBg.setTextureRect(sf::IntRect(0, 320, 320, 320));
                    movingBg.setScale(4, 4);
                    beginPhase = true;
                    if(m3.selection == 10)
                        return;
                    break;
                }
                if(m2.selection == 3) // Boss 1
                {
                    level = new Boss1;
                    level->generateLevel(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, player, 0, boss);

                    t1.loadFromFile("img/backgrounds/4.png");
                    movingBg.setTexture(t1);
                    movingBg.setOrigin(160, 160);
                    movingBg.setPosition(640, 640);
                    movingBg.setScale(4, 4);
                    movingBg.setTextureRect(sf::IntRect(0, 0, 320, 320));
                    atMain = false;
                    beginPhase = false;
                    sideGoonsDied = true;
                    break;
                }
                if(m2.selection == 4) // Boss 2
                {
                    level = new Boss2;
                    level->generateLevel(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, player, 0, boss);

                    t1.loadFromFile("img/backgrounds/5.png");
                    movingBg.setTexture(t1);
                    movingBg.setOrigin(160, 160);
                    movingBg.setPosition(640, 640);
                    movingBg.setScale(4, 4);
                    movingBg.setTextureRect(sf::IntRect(0, 0, 320, 320));
                    atMain = false;
                    beginPhase = false;
                    sideGoonsDied = true;
                    break;
                }
                if(m2.selection == 5) // Boss 3
                {
                    level = new Boss3;
                    level->generateLevel(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, player, 0, boss);

                    t1.loadFromFile("img/backgrounds/62.png");
                    movingBg.setTexture(t1);
                    movingBg.setOrigin(160, 160);
                    movingBg.setPosition(640, 640);
                    movingBg.setScale(4, 4);
                    movingBg.setTextureRect(sf::IntRect(0, 0, 320, 320));
                    atMain = false;
                    beginPhase = false;
                    sideGoonsDied = true;
                    break;
                }
                if(m2.selection == 6) // Return to Main Menu
                {
                    break;
                }
            }
            while(m.selection == 1) // Options
            {
                Menu_Options m2(window, menuMusic, steering, menuBG);
                if(m2.selection == 10)
                    return;
                if(m2.selection == 2) // Return to Main Menu
                    break;
            }
            while(m.selection == 2) // Help
            {
                Menu_Help m2(window, menuBG);
                if(m2.selection == 10)
                    return;
                if(m2.selection == 0) // Return to Main Menu
                    break;
            }
            while(m.selection == 3) // High Scores
            {
                Menu_HighScores m2(window, menuBG, lvlMusic, menuMusic);
                if(m2.selection == 10)
                    return;
                if(m2.selection == 0) // Return to Main Menu
                    break;
            }
            while(m.selection == 4) // Exit
            {
                return;
            }
            rememberSelection = m.selection;
        }
        if(menuMusic.getStatus() != sf::Sound::Paused)
            menuMusic.stop();
        if(lvlMusic.getStatus() != sf::Sound::Playing)
            lvlMusic.play();
        if(slowDownR >= steeringSensitivity)
            slowDownR = 0;
        if(slowDownM >= 5 - player.getSpeed())
            slowDownM = 0;
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyReleased:
                if(evnt.key.code == sf::Keyboard::Key::Up && blinkTimer.getElapsedTime().asSeconds() > 3.5 && beginPhase == false)
                {
                    // std::cout << "Blink!" << std::endl;
                    // player.blink();
                    blinkActive = true;
                    blinkTimer.restart();
                    player.setMomentum(0, 0);
                }
                if(evnt.key.code == sf::Keyboard::Key::P)
                {
                    atPause = true;
                }
                if(evnt.key.code == sf::Keyboard::Key::H)
                {
                    if(encounteredOnce == false)
                    {
                        encounterActive = true;
                        encounteredOnce = true;
                    }
                }
            }
        }
        if(blinkActive == true)
        {
            if(beginPhase == false)
                player.blink(window, controlScheme, dT, blinkTimer, empoweredTime, blinkActive);
        }
        else
        {
            if(beginPhase == false)
                controlShenanigans(window, controlScheme, player, slowDownM, slowDownR, steeringSensitivity, fireTimer, CSTimer, dT, asteroidsUnlocked, hud, level);
        }
        if(player.isEmpowered() && empoweredTime > 5 && blinkActive == false)
        {
            player.setEmpowered(false);
            empoweredTime = 0;
        }
        if(player.isOnFire() && onFireTime > 5)
        {
            player.becomeOnFire(false);
            onFireTime = 0;
        }

        // if(int(clock.getElapsedTime().asMilliseconds()) % 10 == 0)
        // {
        // I am keeping this as a reminder for why doing % on time causes jittery movement.
        // It's better to just reduce the velocity instead.
        for(int i = 0; i < player.getSize(); i++)
            if(!player.getBullets()[i].isOutside())
                player.getBullets()[i].move(player.getBullets()[i].getAngle(), dT);
        for(int i = 0; i < A_Count; i++)
            for(int j = 0; j < enemyA[i].getSize(); j++)
            {
                if(!enemyA[i].getBombs()[j].isOutside())
                {
                    enemyA[i].getBombs()[j].move(dT.asSeconds());
                }
            }
        for(int i = 0; i < B_Count; i++)
            for(int j = 0; j < enemyB[i].getSize(); j++)
            {
                if(!enemyB[i].getBombs()[j].isOutside())
                {
                    enemyB[i].getBombs()[j].move(dT.asSeconds());
                }
            }
        for(int i = 0; i < G_Count; i++)
            for(int j = 0; j < enemyG[i].getSize(); j++)
            {
                if(!enemyG[i].getBombs()[j].isOutside())
                {
                    enemyG[i].getBombs()[j].move(dT.asSeconds());
                }
            }
        // }
        if(beginPhase == false && encounterActive == false)
        {
            for(int i = 0; i < A_Count; i++)
                if(enemyA[i].getFireTime().asSeconds() >= enemyA[i].getFireRate())
                {
                    enemyA[i].fire(player.getPosX(), player.getPosY());
                    enemyA[i].setFireTime(sf::Time(sf::milliseconds(0)));
                }
            for(int i = 0; i < B_Count; i++)
                if(enemyB[i].getFireTime().asSeconds() >= enemyB[i].getFireRate())
                {
                    enemyB[i].fire(player.getPosX(), player.getPosY());
                    enemyB[i].setFireTime(sf::Time(sf::milliseconds(0)));
                }
            for(int i = 0; i < G_Count; i++)
                if(enemyG[i].getFireTime().asSeconds() >= enemyG[i].getFireRate())
                {
                    enemyG[i].fire(player.getPosX(), player.getPosY());
                    enemyG[i].setFireTime(sf::Time(sf::milliseconds(0)));
                }
        }
        if(beginPhase == false && encounterActive == false)
        {
            enemyUpAndDown(eClock, enemyA, A_Count, false);
            enemyUpAndDown(eClock, enemyB, B_Count, false);
            enemyUpAndDown(eClock, enemyG, G_Count, false);
            enemyUpAndDown(bossClock, boss, 1, true);
        }

        // For every bullet, checks a hit target.
        for(int i = 0; i < A_Count; i++)
        {
            for(int j = 0; j < player.getSize(); j++)
            {
                if(beginPhase == false && encounterActive == false)
                    hitTarget(player.getBullets()[j], enemyA[i], player, addon, level, totalScore, asteroidsUnlocked, encounterActive);
            }
            for(int j = 0; j < enemyA[i].getSize(); j++) // For every bomb
            {
                // Check bomb's collision with player
                if(playerBombCollision(enemyA[i].getBombs()[j], player) == true && !player.isEmpowered())
                {
                    blinkActive = false;
                    if(level->levelNumber <= 3)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 1, 1, 0, 0);
                    else
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 4, 2, 0, 0);
                    asteroidsUnlocked = false;
                    sf::Time t = sf::milliseconds(500);
                    player.move(window, "0", 3, t);
                    if(controlScheme != 2)
                    {
                        if(controlScheme == 1)
                            hud.csBA = "12";
                        else if(controlScheme == 2)
                            hud.csBA = "32";
                        hud.csActive = true;
                        CSTimer = 0;
                        controlScheme = 2;
                    }
                    enemyA[i].yeetBomb(enemyA[i].getBombs()[j]);
                }
                else if(playerBombCollision(enemyA[i].getBombs()[j], player) == true && player.isEmpowered())
                {
                    enemyA[i].yeetBomb(enemyA[i].getBombs()[j]);
                }
                // Check bomb's collision with player's bullets
                for(int k = 0; k < player.getSize(); k++)
                {
                    if(bulletBombCollision(enemyA[i].getBombs()[j], player.getBullets()[k]) == true)
                    {
                        enemyA[i].yeetBomb(enemyA[i].getBombs()[j]);
                    }
                }
            }
            // If enemy is alive and player isn't powered up
            if(enemyA[i].getLives() != 0 && !player.isEmpowered())
            {
                // check collision
                if(playerEnemyCollision(enemyA[i], player, encounterActive, beginPhase) == true)
                {
                    blinkActive = false;
                    enemyA[i].setLives(0);
                    enemyA[i].positionSet(9000, 0);
                    // BIG DISCLAIMER WHICH TOOK A LONG TIME TO SOLVE:
                    // USING e.getP(). TO UPDATE P() IS NOT UPDATING P. YOU HAVE TO CALL A FUNCTION
                    // THEN DO IT WITHIN THERE.
                    if(level->levelNumber <= 3)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 1, 1, 0, 0);
                    else
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 4, 2, 0, 0);
                    asteroidsUnlocked = false;
                    sf::Time t = sf::milliseconds(500);
                    player.move(window, "0", 3, t);
                    if(controlScheme != 2)
                    {
                        if(controlScheme == 1)
                            hud.csBA = "12";
                        else if(controlScheme == 2)
                            hud.csBA = "32";
                        hud.csActive = true;
                        CSTimer = 0;
                        controlScheme = 2;
                    }
                }
            }
            // If enemy is alive and player is powered up
            else if(enemyA[i].getLives() != 0 && player.isEmpowered())
            {
                // check collision
                if(playerEnemyCollision(enemyA[i], player, encounterActive, beginPhase) == true)
                {
                    if(blinkActive == true)
                    {
                        // Collision happens up, down, left, or right.
                        float xD = enemyA[i].getPosX() - player.getPosX();
                        float yD = enemyA[i].getPosY() - player.getPosY();
                        // xD < yD means the x values are closer. So it's left and right.
                        // yD < xD means the y values are closer. So it's up and down.
                        float dispV = cos(player.getAngle() * (3.1416/180)) * player.getMomentumX();
                        dispV *= -1;
                        float dispH = sin(player.getAngle() * (3.1416/180)) * player.getMomentumY();
                        // std::cout << player.getMomentumX() << ", " << player.getMomentumY() << std::endl;
                        if(xD < yD) // Side Walls
                        {
                            if(player.getAngle() != 0)
                                player.setAngle(360 - player.getAngle());
                        }
                        else // Vertical Walls
                        {
                            if(player.getAngle() <= 180)
                                player.setAngle(180 - player.getAngle());
                            else
                                player.setAngle(360 - (player.getAngle() - 180));
                        }
                        // std::cout << player.getMomentumX() << ", " << player.getMomentumY() << "\n\n";
                    }
                    enemyA[i].setLives(0);
                    enemyA[i].positionSet(9000, 0);
                    totalScore += 10 * level->levelNumber;
                }
            }
        }
        for(int i = 0; i < B_Count; i++)
        {
            for(int j = 0; j < player.getSize(); j++)
            {
                if(beginPhase == false)
                    hitTarget(player.getBullets()[j], enemyB[i], player, addon, level, totalScore, asteroidsUnlocked, encounterActive);
            }
            for(int j = 0; j < enemyB[i].getSize(); j++)
            {
                if(playerBombCollision(enemyB[i].getBombs()[j], player) == true && !player.isEmpowered())
                {
                    blinkActive = false;
                    if(level->levelNumber <= 3)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 1, 1, 0, 0);
                    else
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 4, 2, 0, 0);
                    asteroidsUnlocked = false;
                    sf::Time t = sf::milliseconds(500);
                    player.move(window, "0", 3, t);
                    if(controlScheme != 2)
                    {
                        if(controlScheme == 1)
                            hud.csBA = "12";
                        else if(controlScheme == 2)
                            hud.csBA = "32";
                        hud.csActive = true;
                        CSTimer = 0;
                        controlScheme = 2;
                    }
                    enemyB[i].yeetBomb(enemyB[i].getBombs()[j]);
                }
                else if(playerBombCollision(enemyB[i].getBombs()[j], player) == true && player.isEmpowered())
                {
                    enemyB[i].yeetBomb(enemyB[i].getBombs()[j]);
                }
                for(int k = 0; k < player.getSize(); k++)
                {
                    if(bulletBombCollision(enemyB[i].getBombs()[j], player.getBullets()[k]) == true)
                    {
                        enemyB[i].yeetBomb(enemyB[i].getBombs()[j]);
                    }
                }
            }
            if(enemyB[i].getLives() != 0 && !player.isEmpowered())
            {
                if(playerEnemyCollision(enemyB[i], player, encounterActive, beginPhase) == true)
                {
                    blinkActive = false;
                    enemyB[i].setLives(0);
                    enemyB[i].positionSet(9000, 0);
                    if(level->levelNumber <= 3)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 1, 1, 0, 0);
                    else
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 4, 2, 0, 0);
                    asteroidsUnlocked = false;
                    sf::Time t = sf::milliseconds(500);
                    player.move(window, "0", 3, t);
                    if(controlScheme != 2)
                    {
                        if(controlScheme == 1)
                            hud.csBA = "12";
                        else if(controlScheme == 2)
                            hud.csBA = "32";
                        hud.csActive = true;
                        CSTimer = 0;
                        controlScheme = 2;
                    }
                }
            }
            else if(enemyB[i].getLives() != 0 && player.isEmpowered())
            {
                if(playerEnemyCollision(enemyB[i], player, encounterActive, beginPhase) == true)
                {
                    if(blinkActive == true)
                    {
                        // Collision happens up, down, left, or right.
                        float xD = enemyB[i].getPosX() - player.getPosX();
                        float yD = enemyB[i].getPosY() - player.getPosY();
                        // xD < yD means the x values are closer. So it's left and right.
                        // yD < xD means the y values are closer. So it's up and down.
                        float dispV = cos(player.getAngle() * (3.1416/180)) * player.getMomentumX();
                        dispV *= -1;
                        float dispH = sin(player.getAngle() * (3.1416/180)) * player.getMomentumY();
                        // std::cout << player.getMomentumX() << ", " << player.getMomentumY() << std::endl;
                        if(xD < yD) // Side Walls
                        {
                            if(player.getAngle() != 0)
                                player.setAngle(360 - player.getAngle());
                        }
                        else // Vertical Walls
                        {
                            if(player.getAngle() <= 180)
                                player.setAngle(180 - player.getAngle());
                            else
                                player.setAngle(360 - (player.getAngle() - 180));
                        }
                        // std::cout << player.getMomentumX() << ", " << player.getMomentumY() << "\n\n";
                    }
                    enemyB[i].setLives(0);
                    enemyB[i].positionSet(9000, 0);
                    totalScore += 20 * level->levelNumber;
                }
            }
        }
        for(int i = 0; i < G_Count; i++)
        {
            for(int j = 0; j < player.getSize(); j++)
            {
                if(beginPhase == false)
                    hitTarget(player.getBullets()[j], enemyG[i], player, addon, level, totalScore, asteroidsUnlocked, encounterActive);
            }
            for(int j = 0; j < enemyG[i].getSize(); j++)
            {
                if(playerBombCollision(enemyG[i].getBombs()[j], player) == true && !player.isEmpowered())
                {
                    blinkActive = false;
                    if(level->levelNumber <= 3)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 1, 1, 0, 0);
                    else
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 4, 2, 0, 0);
                    asteroidsUnlocked = false;
                    sf::Time t = sf::milliseconds(500);
                    player.move(window, "0", 3, t);
                    if(controlScheme != 2)
                    {
                        if(controlScheme == 1)
                            hud.csBA = "12";
                        else if(controlScheme == 2)
                            hud.csBA = "32";
                        hud.csActive = true;
                        CSTimer = 0;
                        controlScheme = 2;
                    }
                    enemyG[i].yeetBomb(enemyG[i].getBombs()[j]);
                }
                else if(playerBombCollision(enemyG[i].getBombs()[j], player) == true && player.isEmpowered())
                {
                    enemyG[i].yeetBomb(enemyG[i].getBombs()[j]);
                }
                for(int k = 0; k < player.getSize(); k++)
                {
                    if(bulletBombCollision(enemyG[i].getBombs()[j], player.getBullets()[k]) == true)
                    {
                        enemyG[i].yeetBomb(enemyG[i].getBombs()[j]);
                    }
                }
            }
            if(enemyG[i].getLives() != 0 && !player.isEmpowered())
            {
                if(playerEnemyCollision(enemyG[i], player, encounterActive, beginPhase) == true)
                {
                    blinkActive = false;
                    enemyG[i].setLives(0);
                    enemyG[i].positionSet(9000, 0);
                    // BIG DISCLAIMER WHICH TOOK A LONG TIME TO SOLVE:
                    // USING e.getP(). TO UPDATE P() IS NOT UPDATING P. YOU HAVE TO CALL A FUNCTION
                    // THEN DO IT WITHIN THERE.
                    // MAKING THE GETTER NON-CONST ALSO DIDN'T HELP.
                    if(level->levelNumber <= 3)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 1, 1, 0, 0);
                    else
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 4, 2, 0, 0);
                    asteroidsUnlocked = false;
                    sf::Time t = sf::milliseconds(500);
                    player.move(window, "0", 3, t);
                    if(controlScheme != 2)
                    {
                        if(controlScheme == 1)
                            hud.csBA = "12";
                        else if(controlScheme == 2)
                            hud.csBA = "32";
                        hud.csActive = true;
                        CSTimer = 0;
                        controlScheme = 2;
                    }
                }
            }
            else if(enemyG[i].getLives() != 0 && player.isEmpowered())
            {
                if(playerEnemyCollision(enemyG[i], player, encounterActive, beginPhase) == true)
                {
                    if(blinkActive == true)
                    {
                        // Collision happens up, down, left, or right.
                        float xD = enemyG[i].getPosX() - player.getPosX();
                        float yD = enemyG[i].getPosY() - player.getPosY();
                        // xD < yD means the x values are closer. So it's left and right.
                        // yD < xD means the y values are closer. So it's up and down.
                        float dispV = cos(player.getAngle() * (3.1416/180)) * player.getMomentumX();
                        dispV *= -1;
                        float dispH = sin(player.getAngle() * (3.1416/180)) * player.getMomentumY();
                        // std::cout << player.getMomentumX() << ", " << player.getMomentumY() << std::endl;
                        if(xD < yD) // Side Walls
                        {
                            if(player.getAngle() != 0)
                                player.setAngle(360 - player.getAngle());
                        }
                        else // Vertical Walls
                        {
                            if(player.getAngle() <= 180)
                                player.setAngle(180 - player.getAngle());
                            else
                                player.setAngle(360 - (player.getAngle() - 180));
                        }
                        // std::cout << player.getMomentumX() << ", " << player.getMomentumY() << "\n\n";
                    }
                    enemyG[i].setLives(0);
                    enemyG[i].positionSet(9000, 0);
                    totalScore += 30 * level->levelNumber;
                }
            }
        }
        if(addon != nullptr)
        {
            // std::cout << addon->getY() << std::endl;
            addon->move(dT);
            if(addon->getDanger() == true)
            {
                addon->move(dT);
            }
            window.draw(addon->getA());
            if(addon->collision(player) == true)
            {
                addon->interaction(player);
                if(addon->getDanger() == true)
                {
                    blinkActive = false;
                    if(level->levelNumber <= 3)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 1, 1, 0, 0);
                    else
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 4, 2, 0, 0);
                    asteroidsUnlocked = false;
                    sf::Time t = sf::milliseconds(500);
                    player.move(window, "0", 3, t);
                    if(controlScheme != 2)
                    {
                        if(controlScheme == 1)
                            hud.csBA = "12";
                        else if(controlScheme == 2)
                            hud.csBA = "32";
                        hud.csActive = true;
                        CSTimer = 0;
                        controlScheme = 2;
                    }
                }
                delete addon;
                addon = nullptr;
            }
            else if(addon->getY() > 1280 || addon->getY() < 0)
            {
                if(addon->getDanger() == true)
                    totalScore += 5;
                delete addon;
                addon = nullptr;
            }
        }
        if(boss != nullptr)
            if((level->levelNumber >= 4) || ((level->levelNumber == 2 || level->levelNumber == 3) && encounterActive == true))
                if(enemyA != nullptr)
                    if(true)
                    {
                        if(moveBoss > 0.003333)
                        {
                            if(boss->getVariant() == 3)
                            {
                                if(boss->getP().getPosition().y < 200)
                                    boss->positionSet(boss->getP().getPosition().x, boss->getP().getPosition().y + 1);
                            }
                            else if(boss->getVariant() == 4)
                            {
                                if(boss->getP().getPosition().y < 240)
                                    boss->positionSet(boss->getP().getPosition().x, boss->getP().getPosition().y + 1);
                            }
                            moveBoss = 0;
                        }
                        window.draw(boss->getP());
                        if(boss->getVariant() == 4)
                            for(int i = 0; i < boss->getSize(); i++)
                                if(!boss->getBombs()[i].isOutside())
                                    window.draw(boss->getBombs()[i].getB());
                    }
        

        window.draw(player.getP());

        for(int i = 0; i < player.getSize(); i++)
            window.draw(player.getBullets()[i].getB());

        int enemiesOnScreen = 0;
        int bombsOnScreen = 0;

        for(int i = 0; i < A_Count; i++)
        {   
            for(int j = 0; j < enemyA[i].getSize(); j++)
            {
                if(!enemyA[i].getBombs()[j].isOutside())
                {
                    window.draw(enemyA[i].getBombs()[j].getB());
                    bombsOnScreen++;
                }
            }
            if(enemyA[i].isOutside())
                continue;
            enemiesOnScreen++;
            window.draw(enemyA[i].getP());
        }
        for(int i = 0; i < B_Count; i++)
        {   
            for(int j = 0; j < enemyB[i].getSize(); j++)
            {
                if(!enemyB[i].getBombs()[j].isOutside())
                {
                    window.draw(enemyB[i].getBombs()[j].getB());
                    bombsOnScreen++;
                }
            }
            if(enemyB[i].isOutside())
                continue;
            enemiesOnScreen++;
            window.draw(enemyB[i].getP());
        }
        for(int i = 0; i < G_Count; i++)
        {   
            for(int j = 0; j < enemyG[i].getSize(); j++)
            {
                if(!enemyG[i].getBombs()[j].isOutside())
                {
                    window.draw(enemyG[i].getBombs()[j].getB());
                    bombsOnScreen++;
                }
            }
            if(enemyG[i].isOutside())
                continue;
            enemiesOnScreen++;
            window.draw(enemyG[i].getP());
        }

        if(enemiesOnScreen == 0 && bombsOnScreen == 0 && level->levelNumber <= 3 && encounterActive == false)
        {
            level->nextPhase(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, player);
            if(enemyA == nullptr)
            {
                level->levelEnd(levelClear, gameOver, player);
            }
            else
            {
                beginPhase = true;
                sideGoonsDied = false;
            }
        }
        if(player.getLives() <= 0)
        {
            level->levelEnd(levelClear, gameOver, player);
            gameOver = true;
        }
        if(hud.csActive == true && CSTimer < 0.5) // Meaning the CS is changing
        {
            int x = CSTimer * 1000 / 41.6667;
            int y = 0;

            if(hud.csBA == "12"){y = 0;}
            if(hud.csBA == "23"){y = 1;}
            if(hud.csBA == "31"){y = 2;}
            if(hud.csBA == "21"){y = 3;}
            if(hud.csBA == "32"){y = 4;}
            if(hud.csBA == "13"){y = 5;}
            hud.cs.setTextureRect(sf::IntRect(64*x, 64*y, 64, 64));
            // hud.csValues();
        }
        else if(hud.csActive == true && CSTimer > 0.5)
        {
            int y = 0;
            if(hud.csBA == "12"){y = 0;}
            if(hud.csBA == "23"){y = 1;}
            if(hud.csBA == "31"){y = 2;}
            if(hud.csBA == "21"){y = 3;}
            if(hud.csBA == "32"){y = 4;}
            if(hud.csBA == "13"){y = 5;}
            hud.cs.setTextureRect(sf::IntRect(64*12, 64*y, 64, 64));
            hud.csActive = false;
        }
        window.draw(hud.cs);
        window.draw(hud.blinkbarStatic);
        if(blinkActive == false)
        {
            if(blinkTimer.getElapsedTime().asSeconds() > 3.5)
                hud.blinkbarScale.setScale(1, 1);
            else
                hud.blinkbarScale.setScale(blinkTimer.getElapsedTime().asSeconds() / 3.5, 1);
        }
        else
        {
            hud.blinkbarScale.setScale(0, 1);
        }
        window.draw(hud.blinkbarScale);
        if(beginPhase == true)
        {
            twoFromSide(window, sideGoonsDied, level, movingBg, movingBgTimer, player);
            comeForth(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, beginPhase, dT);
            for(int i = 0; i < A_Count; i++)
                window.draw(enemyA[i].getSil());
            for(int i = 0; i < B_Count; i++)
                window.draw(enemyB[i].getSil());
            for(int i = 0; i < G_Count; i++)
                window.draw(enemyG[i].getSil());
        }
        else if(beginPhase == false)
            sideGoonsDied = true;
        // window.draw(staticBg);
        if(encounterActive == true)
        {
            if(boss->getFireTime().asSeconds() > boss->getFireRate() / 20.0 && boss->getPosY() >= -5)
            {
                boss->fire(player.getPosX(), player.getPosY());
                if(boss->getVariant() == 3)
                {
                    boss->beam.setTextureRect(sf::IntRect(boss->beam.getTextureRect().left + 150, 0, 150, 1280));
                    if(boss->beam.getTextureRect().left >= 300)
                        boss->beam.setTextureRect(sf::IntRect(0, 0, 150, 1280));
                }
                boss->setFireTime(sf::Time(sf::milliseconds(0)));
            }
            bool playerHitBeam = false;
            if(boss->getVariant() == 3)
            {
                if(beamBreak < 2)
                {
                    boss->move(dT);
                    boss->beam.setPosition(boss->getPosX() - 185, boss->getPosY() + 100);
                    window.draw(boss->beam);
                    boss->beam.setPosition(boss->getPosX() - 205, boss->getPosY() + 100);
                    window.draw(boss->beam);
                    boss->beam.setPosition(boss->getPosX() - 185, boss->getPosY() + 80);
                    window.draw(boss->beam);
                    boss->beam.setPosition(boss->getPosX() - 205, boss->getPosY() + 80);
                    window.draw(boss->beam);
                    playerHitBeam = playerBeamCollision(boss, player);
                    boss->beam.setPosition(boss->getPosX() + 185, boss->getPosY() + 100);
                    window.draw(boss->beam);
                    boss->beam.setPosition(boss->getPosX() + 205, boss->getPosY() + 100);
                    window.draw(boss->beam);
                    boss->beam.setPosition(boss->getPosX() + 185, boss->getPosY() + 80);
                    window.draw(boss->beam);
                    boss->beam.setPosition(boss->getPosX() + 205, boss->getPosY() + 80);
                    window.draw(boss->beam);
                    if(playerHitBeam == false)
                        playerHitBeam = playerBeamCollision(boss, player);
                }
                else if(beamBreak > 3)
                    beamBreak = 0;
            }
            for(int i = 0; i < boss->getSize(); i++)
                if(!boss->getBombs()[i].isOutside())
                    boss->getBombs()[i].move(dT.asSeconds());
                    
            for(int i = 0; i < boss->getSize(); i++)
                if(playerBombCollision(boss->getBombs()[i], player) == true)
                {
                    survived = false;
                    blinkActive = false;
                    if(level->levelNumber <= 3)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 1, 1, 0, 0);
                    else
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 640, 1024, 0, 4, 2, 0, 0);
                    asteroidsUnlocked = false;
                    sf::Time t = sf::milliseconds(500);
                    player.move(window, "0", 3, t);
                    if(controlScheme != 2)
                    {
                        if(controlScheme == 1)
                            hud.csBA = "12";
                        else if(controlScheme == 3)
                            hud.csBA = "32";
                        hud.csActive = true;
                        CSTimer = 0;
                        controlScheme = 3;
                    }
                    for(int j = 0; j < boss->getSize(); j++)
                        boss->yeetBomb(boss->getBombs()[j]);
                }

            if(playerHitBeam == true && player.isInvincible() == false)
            {
                survived = false;
                blinkActive = false;
                if(level->levelNumber <= 3)
                {
                    if(boss->getPosX() > 640)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 120, 1024, 0, 1, 1, 0, 0);
                    else if(boss->getPosX() <= 640)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 1160, 1024, 0, 1, 1, 0, 0);
                }
                else
                {
                    if(boss->getPosX() > 640)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 120, 1024, 0, 4, 2, 0, 0);
                    if(boss->getPosX() <= 640)
                        player.setState(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, window, player.getLives() - 1, 1160, 1024, 0, 4, 2, 0, 0);
                }
                asteroidsUnlocked = false;
                sf::Time t = sf::milliseconds(500);
                player.move(window, "0", 3, t);
                if(controlScheme != 2)
                {
                    if(controlScheme == 1)
                        hud.csBA = "12";
                    else if(controlScheme == 2)
                        hud.csBA = "32";
                    hud.csActive = true;
                    CSTimer = 0;
                    controlScheme = 2;
                }
            }
            for(int i = 0; i < A_Count; i++)
                for(int j = 0; j < enemyA[i].getSize(); j++)
                    if(!enemyA[i].getBombs()[j].isOutside())
                        enemyA[i].yeetBomb(enemyA[i].getBombs()[j]);
            for(int i = 0; i < B_Count; i++)
                for(int j = 0; j < enemyB[i].getSize(); j++)
                    if(!enemyB[i].getBombs()[j].isOutside())
                        enemyB[i].yeetBomb(enemyB[i].getBombs()[j]);
            for(int i = 0; i < G_Count; i++)
                for(int j = 0; j < enemyG[i].getSize(); j++)
                    if(!enemyG[i].getBombs()[j].isOutside())
                        enemyG[i].yeetBomb(enemyG[i].getBombs()[j]);
            vanish(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, encounterActive, deadAliveTimer);
        }
        if(encounterTimer > 15)
        {
            encounterTimer = 0;
            encounterActive = false;
            encounteredOnce = true;
            rollTimer = 0;
            if(survived == true)
            {
                if(boss->getVariant() == 3)
                    totalScore += 40;
                else if(boss->getVariant() == 4)
                    totalScore += 50;
            }
        }
        
        backFromTheDead(enemyA, A_Count, enemyB, B_Count, enemyG, G_Count, encounterActive, deadAliveTimer);
        if(level->levelNumber == 6)
        {
            moveEye(boss->eye, player);
            window.draw(boss->eye);
        }
        window.draw(blackPgram);
        window.draw(score.text);

        window.display();
        window.clear();
        // movingBg.setColor(sf::Color(255, 255, 255, 128));
        window.draw(movingBg);
        // std::cout << 1/dT.asSeconds() << std::endl;
        if(level->levelNumber == 6)
        {
            window.draw(theEarth);
        }
    }
}
bool playerBeamCollision(Enemy*& boss, Spaceship& player)
{
	float absX = 0;
	float absY = 0;

	absX = (boss->beam.getGlobalBounds().left + boss->beam.getGlobalBounds().width/2) - player.getPosX();
	if(absX < 0)
		absX = -absX;

	absY = (boss->beam.getGlobalBounds().top + boss->beam.getGlobalBounds().height/2) - player.getPosY();
	if(absY < 0)
		absY = -absY;
        
	if(absX < (boss->beam.getGlobalBounds().width/2 + 32) && absY < (boss->beam.getGlobalBounds().height/2 + 32))
        return true;
    return false;
}
void backFromTheDead(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, bool& encounterActive, float& deadAliveTimer)
{
    if(enemyA != nullptr)
        if(enemyA[0].getP().getColor().a >= 254)
        return;
    if(deadAliveTimer < 0.008333)
        return;
    for(int i = 0; i < A_Count; i++)
        enemyA[i].increaseAlpha();
    for(int i = 0; i < B_Count; i++)
        enemyB[i].increaseAlpha();
    for(int i = 0; i < G_Count; i++)
        enemyG[i].increaseAlpha();
    deadAliveTimer = 0;
}
void vanish(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, bool& encounterActive, float& deadAliveTimer)
{
    if(deadAliveTimer < 0.008333)
        return;
    for(int i = 0; i < A_Count; i++)
        enemyA[i].lowerAlpha();
    for(int i = 0; i < B_Count; i++)
        enemyB[i].lowerAlpha();
    for(int i = 0; i < G_Count; i++)
        enemyG[i].lowerAlpha();
    deadAliveTimer = 0;
}
void moveEye(sf::Sprite& eye, Spaceship& p)
{
    float fraction = 0;
    fraction = (p.getPosX() - 640) / 640;
    // x == 0 means fraction of -1.
    // x == 1280 means fraction of 1.
    // x == 640 means fraction of 0.
    // -1 is left, 1 is right.
    // 480, 240 to 430, 230
    float absF = fraction;
    if(absF < 0)
        absF = -absF;
    eye.setPosition(640 + 50*fraction, eye.getPosition().y - 5*absF);
}
void twoFromSide(sf::RenderWindow& window, bool& sideGoonsDied, Levels*& level, sf::Sprite& movingBg, float& movingBgTimer, Spaceship& player)
{
    if(sideGoonsDied == true)
        return;
    sf::Texture goonTex;
    sf::Sprite goon1;
    sf::Sprite goon2;
    goonTex.loadFromFile("spritesheet/Invaders.png");
    goon1.setTexture(goonTex);
    goon2.setTexture(goonTex);
    goon1.setPosition(0, 640);
    goon2.setPosition(1280, 640);
    goon1.setTextureRect(sf::IntRect((rand() % 3) * 72, 0, 72, 72));
    goon2.setTextureRect(sf::IntRect((rand() % 3) * 72, 0, 72, 72));
    goon1.setOrigin(36, 36);
    goon2.setOrigin(36, 36);
    sf::Clock collide;
    collide.restart();
    sf::Clock dT;
    dT.restart();
    float deltaT = 0;
    while(collide.getElapsedTime().asSeconds() < 1)
    {
        if(level != nullptr)
            if(movingBgTimer > 0.03333 && level->levelNumber <= 3)
            {
                movingBg.setTextureRect(sf::IntRect(0, movingBg.getTextureRect().top - 1, 320, 320));
                movingBgTimer = 0;
                if(movingBg.getTextureRect().top <= 0)
                    movingBg.setTextureRect(sf::IntRect(0, 320, 320, 320));
            }
        deltaT = dT.restart().asSeconds();
        movingBgTimer += deltaT;
        goon1.move(660 * deltaT, 0);
        goon2.move(-660 * deltaT, 0);
        window.draw(movingBg);
        window.draw(player.getP());
        window.draw(goon1);
        window.draw(goon2);
        window.display();
        window.clear();
    }
    sideGoonsDied = true;
}
void comeForth(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, bool& beginPhase, sf::Time dT)
{
    int a = 0, b = 0, g = 0;
    for(int i = 0; i < A_Count; i++)
    {
        if(enemyA[i].isOutside())
            continue;
        float angle = 90 + atan((640 - enemyA[i].getPosY()) / (640 - enemyA[i].getPosX())) * (180/3.1416);
        if(640 - enemyA[i].getPosX() < 0)
            angle += 180;
        angle += 180; // To flip it. It goes from 640, 640 towards the enemy position.

        float dispV = cos(angle * (3.1416/180));
        dispV *= -1;
        float dispH = sin(angle * (3.1416/180));

        if(!enemyA[i].silhouetteInPlace()) // If not in place, move into place.
            enemyA[i].moveFade(dispH * dT.asSeconds() * 500, dispV * dT.asSeconds() * 500);
        else // If in place, start fading.
        {
            enemyA[i].reveal();
            a++;
            if(enemyA[i].getFade().getElapsedTime().asSeconds() > 0.0333)
                enemyA[i].lowerFade();
        }
    }
    for(int i = 0; i < B_Count; i++)
    {
        if(enemyB[i].isOutside())
            continue;
        float angle = 90 + atan((640 - enemyB[i].getPosY()) / (640 - enemyB[i].getPosX())) * (180/3.1416);
        if(640 - enemyB[i].getPosX() < 0)
            angle += 180;
        angle += 180; // To flip it. It goes from 640, 640 towards the enemy position.

        float dispV = cos(angle * (3.1416/180));
        dispV *= -1;
        float dispH = sin(angle * (3.1416/180));

        if(!enemyB[i].silhouetteInPlace()) // If not in place, move into place.
            enemyB[i].moveFade(dispH * dT.asSeconds() * 500, dispV * dT.asSeconds() * 500);
        else // If in place, start fading.
        {
            enemyB[i].reveal();
            b++;
            if(enemyB[i].getFade().getElapsedTime().asSeconds() > 0.0333)
                enemyB[i].lowerFade();
        }
    }
    for(int i = 0; i < G_Count; i++)
    {
        float angle = 90 + atan((640 - enemyG[i].getPosY()) / (640 - enemyG[i].getPosX())) * (180/3.1416);
        if(640 - enemyG[i].getPosX() < 0)
            angle += 180;
        angle += 180; // To flip it. It goes from 640, 640 towards the enemy position.

        float dispV = cos(angle * (3.1416/180));
        dispV *= -1;
        float dispH = sin(angle * (3.1416/180));

        if(!enemyG[i].silhouetteInPlace()) // If not in place, move into place.
            enemyG[i].moveFade(dispH * dT.asSeconds() * 500, dispV * dT.asSeconds() * 500);
        else // If in place, start fading.
        {
            enemyG[i].reveal();
            g++;
            if(enemyG[i].getFade().getElapsedTime().asSeconds() > 0.0333)
                enemyG[i].lowerFade();
        }
    }
    int allSpawned = true;
    if(a != A_Count || b != B_Count || g != G_Count)
        allSpawned = false;
    if(a == A_Count)
    {
        for(int i = 0; i < A_Count; i++)
            if(enemyA[i].isSpawned() == false)
            {
                enemyA[i].lowerFade();
                allSpawned = false;
            }
    }
    if(b == B_Count)
    {
        for(int i = 0; i < B_Count; i++)
            if(enemyB[i].isSpawned() == false)
            {
                enemyB[i].lowerFade();
                allSpawned = false;
            }
    }
    if(g == G_Count)
    {
        for(int i = 0; i < G_Count; i++)
            if(enemyG[i].isSpawned() == false)
            {
                enemyG[i].lowerFade();
                allSpawned = false;
            }
    }
    if(allSpawned == true)
        beginPhase = false;
}
void rollAddon(float x, float y, Addon*& addon, Spaceship& p, Levels*& level, bool asteroidsUnlocked)
{
    int num = rand() % 100;
    bool spawn = false;
    if(num < 50 && level->levelNumber == 1) // 50% chance in lvl 1
        spawn = true;
    else if(num < 70 && level->levelNumber == 2) // 70% chance in lvl 2
        spawn = true;
    else if(num < 90 && level->levelNumber == 3) // 90% chance in lvl 3
        spawn = true;
    if(spawn == true) // Roll the dice. 70% chance.
    {
        bool spawnHappened = false;
        while(spawnHappened == false)
        {
            int num2 = rand() % 60; // For choosing the powerup to spawn
            if(num2 >= 0 && num2 < 10 && asteroidsUnlocked == false) // Speed
            {
                std::cout << "Speed Spawned!" << std::endl;
                addon = new Speed(x, y);
                spawnHappened = true;
            }
            else if(num2 >= 10 && num2 < 20 && !p.isOnFire()) // Fire
            {
                std::cout << "Fire Spawned!" << std::endl;
                addon = new Fire(x, y);
                spawnHappened = true;
            }
            else if(num2 >= 20 && num2 < 30 && p.getDamage() < 4 && !p.isOnFire()) // Damage
            {
                std::cout << "Damage Spawned!" << std::endl;
                addon = new Damage(x, y);
                spawnHappened = true;
            }
            else if(num2 >= 30 && num2 < 40 && p.getLives() < 5)
            {
                std::cout << "1-UP Spawned!" << std::endl;
                addon = new Life(x, y);
                spawnHappened = true;
            }
            else if(num2 >= 40 && num2 < 50 && !p.isEmpowered())
            {
                std::cout << "Power-Up Spawned!" << std::endl;
                addon = new Power_Up(x, y);
                spawnHappened = true;
            }
            else if(num2 >= 50 && num2 < 60)
            {
                std::cout << "Danger Spawned!" << std::endl;
                addon = new Danger(x, y);
                spawnHappened = true;
            }
        }
    }
}
void enemyUpAndDown(sf::Clock& eClock, Enemy* enemy, int size, bool isBoss)
{
    if(enemy == nullptr)
        return;
    if(isBoss == false)
    {
        if(eClock.getElapsedTime().asSeconds() > 0)
        {
            for(int i = 0; i < size; i++)
            {
                enemy[i].positionSet(enemy[i].getPosX(), enemy[i].getPosY() - 0.04);
            }
        }
        if(eClock.getElapsedTime().asSeconds() > 0.5)
        {
            for(int i = 0; i < size; i++)
            {
                enemy[i].positionSet(enemy[i].getPosX(), enemy[i].getPosY() + 0.085);
            }
        }
        if(eClock.getElapsedTime().asSeconds() > 1)
        {
            eClock.restart();
        }
    }
    else if(isBoss == true)
    {
        if(eClock.getElapsedTime().asSeconds() > 0)
        {
            if(enemy->getVariant() == 5)
                enemy[0].eye.setPosition(enemy[0].eye.getPosition().x, enemy[0].getPosY() - 20 - 0.02);
            enemy[0].positionSet(enemy[0].getPosX(), enemy[0].getPosY() - 0.02);
        }
        if(eClock.getElapsedTime().asSeconds() > 1)
        {
            if(enemy->getVariant() == 5)
                enemy[0].eye.setPosition(enemy[0].eye.getPosition().x, enemy[0].getPosY() - 20 + 0.04);
            enemy[0].positionSet(enemy[0].getPosX(), enemy[0].getPosY() + 0.04);
        }
        if(eClock.getElapsedTime().asSeconds() > 2)
        {
            eClock.restart();
        }    
    }
}
bool bulletBombCollision(Bomb& bomb, Bullet& bullet)
{
	float absX = 0;
	float absY = 0;

	absX = bomb.getPosX() - bullet.getPosX();
	if(absX < 0)
		absX = -absX;

	absY = bomb.getPosY() - bullet.getPosY();
	if(absY < 0)
		absY = -absY;

    if(absX < (16 + 16) && absY < (16 + 16))
    {
        return true;
    }
    return false;
}
bool playerBombCollision(Bomb& b, Spaceship& p)
{
    if(p.isInvincible())
        return false; // This is a DEBUG menu option. It's not meant to destroy the bombs.
        // isEmpowered() does that, and the code for that is somewhere else.
	float absX = 0;
	float absY = 0;

	absX = b.getPosX() - p.getPosX();
	if(absX < 0)
		absX = -absX;

	absY = b.getPosY() - p.getPosY();
	if(absY < 0)
		absY = -absY;
        
    if(absX < (32 + 16) && absY < (32 + 16))
    {
        return true;
    }
    return false;
}
bool playerEnemyCollision(Enemy& e, Spaceship& p, bool encounterActive, bool beginPhase)
{
    if(encounterActive == true || beginPhase == true)
        return false;
	float absX = 0;
	float absY = 0;

	absX = e.getPosX() - p.getPosX();
	if(absX < 0)
		absX = -absX;

	absY = e.getPosY() - p.getPosY();
	if(absY < 0)
		absY = -absY;
        
	if(absX < 64 && absY < 64) // If player hits
    // I'd be using math if it weren't for the time limit. So 64 (which is Player Sprite size and Enemy Sprite size)
    // it is.
    {
        // There was way more functionality here but it was moved into main()
        // because empowered collisions means enemy gets destroyed but player is unaffected
        return true;
    }
    return false;
}
bool hitTarget(Bullet& b, Enemy& e, Spaceship& p, Addon*& addon, Levels*& level, int& totalScore, bool asteroidsUnlocked, bool encounterActive)
{
    if(encounterActive == true)
        return false;
	float absX = 0;
	float absY = 0;

	absX = e.getPosX() - b.getPosX();
	if(absX < 0)
		absX = -absX;

	absY = e.getPosY() - b.getPosY();
	if(absY < 0)
		absY = -absY;

	if((absX < 16 + 32) && (absY < 16 + 32)) // If bullet hits
	{
        if(!p.isOnFire())
        {
            b.setPosX(10000); // REMINDER: DON'T SET THIS TO SAME COORDINATE AS THAT FOR ENEMY.
            // OR ELSE THEY COLLIDE OFF SCREEN AGAIN.
            sf::Time t = sf::milliseconds(500);
            b.move(p.getAngle(), t);
            e.setLives(e.getLives() - b.getDamage());
        }
        else
        {
            e.setLives(0);
        }
        if(e.getLives() <= 0)
        {
            if(addon == nullptr)
                rollAddon(e.getPosX(), e.getPosY(), addon, p, level, asteroidsUnlocked);
            e.positionSet(9000, 0);
            totalScore += (((e.getVariant() + 1) * 10) * level->levelNumber);
            // variant 0, Alpha, is 10. variant 1, Beta, is 20. variant 2, Gamma, is 30.
        }
		return true;
	}
    return false;
}
void controlShenanigans(sf::RenderWindow& window, int& controlScheme, Spaceship& player, int& slowDownM, int& slowDownR, int& steeringSensitivity, float& fireTimer, float& CSTimer, sf::Time& dT, bool asteroidsUnlocked, HUD& hud, Levels*& level)
{
    bool press_W = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    bool press_A = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool press_S = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    bool press_D = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && CSTimer >= 0.5)
    {
        hud.csBA = std::to_string(controlScheme);
        if(controlScheme > 0)
            controlScheme--;
        if(controlScheme == 2 && asteroidsUnlocked == false)
            controlScheme--;
        if(controlScheme == 0)
            controlScheme = 3;
        hud.csBA += std::to_string(controlScheme);
        hud.csActive = true;
        slowDownM = 0;
        CSTimer = 0;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && CSTimer >= 0.5)
    {
        hud.csBA = std::to_string(controlScheme);
        if(controlScheme < 4)
            controlScheme++;
        if(controlScheme == 2 && asteroidsUnlocked == false)
            controlScheme++;
        if(controlScheme == 4)
            controlScheme = 1;
        hud.csBA += std::to_string(controlScheme);
        hud.csActive = true;
        slowDownM = 0;
        CSTimer = 0;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        if(fireTimer * 1000.0 >= player.getFireRate()) // player.getFireRate()
        {
            if(player.isEmpowered())
                player.empoweredFire();
            else
                player.fire();
            fireTimer = 0;
        }
        else if(asteroidsUnlocked == true)
        {
            if(fireTimer * 1000.0 >= 200)
            {
                if(player.isEmpowered())
                    player.empoweredFire();
                else
                    player.fire();
                fireTimer = 0;
            }
        }
    }
    // std::cout << "Framerate: " << 1.0/dT.asSeconds() << ", fireTimer: " << fireTimer << ", fireRate: " << player.getFireRate() << std::endl;

    if(controlScheme == 1 || controlScheme == 3)
    {
        if (press_W && !press_A && !press_S && !press_D)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "u", controlScheme, dT, level->levelNumber); // Up
        }
        if (!press_W && !press_A && press_S && !press_D)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "d", controlScheme, dT, level->levelNumber); // Down
        }
        if (!press_W && press_A && !press_S && !press_D)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "l", controlScheme, dT, level->levelNumber); // Left
        }
        if (!press_W && !press_A && !press_S && press_D)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "r", controlScheme, dT, level->levelNumber); // Right
        }
        if (press_W && !press_A && !press_S && press_D)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "ur", controlScheme, dT, level->levelNumber); // Up Right
        }
        if (press_W && press_A && !press_S && !press_D)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "ul", controlScheme, dT, level->levelNumber); // Up Left
        }
        if (!press_W && press_A && press_S && !press_D)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "dl", controlScheme, dT, level->levelNumber); // Down Left
        }
        if (!press_W && !press_A && press_S && press_D)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "dr", controlScheme, dT, level->levelNumber); // Down Right
        }
        if (!press_W && !press_A && !press_S && !press_D) // No button pressed
        {
            player.setAngle(0);
            player.move(window, "0", 1, dT, level->levelNumber);
        }
    }
    
    if(controlScheme == 2)
    {
        if (press_W && !press_S)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "u", 2, dT, level->levelNumber); // Forwards
        }
        if (!press_W && press_S)
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "d", 2, dT, level->levelNumber); // Backwards
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {   
            player.setAngle(0);
            player.move(window, "0", 2, dT);
        }
        if (press_A && !press_D)
        {
            slowDownR++;
            if(slowDownR == steeringSensitivity)
                player.setAngle(player.getAngle() - (dT.asSeconds() * 1000));
            if (player.getAngle() < 0)
                player.setAngle(359);
            player.move(window, "0", 2, dT, level->levelNumber);
        }
        if (press_D && !press_A)
        {
            slowDownR++;
            if(slowDownR == steeringSensitivity)
                player.setAngle(player.getAngle() + (dT.asSeconds() * 1000));
            if (player.getAngle() >= 360)
                player.setAngle(0);
            player.move(window, "0", 2, dT, level->levelNumber);
        }
        if ((!press_W && !press_S) || (press_W && press_S))
        {
            slowDownM++;
            if(slowDownM == 5 - player.getSpeed())
                player.move(window, "m", 2, dT, level->levelNumber);
        }
    }
}
