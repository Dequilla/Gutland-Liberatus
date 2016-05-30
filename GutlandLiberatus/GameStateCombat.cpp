#include "GameStateCombat.h"
#include "StateManager.h"

GameStateCombat::GameStateCombat(StateManager* stateManager) : Kengine::BaseState(stateManager)
{}

GameStateCombat::~GameStateCombat()
{
	OnDestroy();
}

void GameStateCombat::OnCreate()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->AddCallback(StateType::Combat, "Mouse_Left",
		&GameStateCombat::MouseClick, this);
	evMgr->AddCallback(StateType::Combat, "Key_Escape",
		&GameStateCombat::MainMenu, this);
	evMgr->AddCallback(StateType::Combat, "Key_P",
		&GameStateCombat::Pause, this);
	evMgr->AddCallback(StateType::Combat, "Key_O",
		&GameStateCombat::ToggleOverlay, this);

	sf::Vector2u size = m_stateMgr->GetContext()->window->GetWindowSize();

	//Load sound
	//m_hitSoundBuffer.loadFromFile();
	m_music.openFromFile("media/Sound/Music/Bensound - Epic.ogg");
	m_music.setLoop(true);
	m_music.setVolume(10);
	m_music.play();

	m_screamBuffer.loadFromFile("media/Sound/Effects/Scream.ogg");
	m_scream.setBuffer(m_screamBuffer);
	m_scream.setLoop(false);
	m_scream.setVolume(100);

	//Selection and creation of fighting background
	//For now a static background
	m_backgroundTexture.loadFromFile("media/Textures/CombatBackground.png");
	m_background.setTexture(m_backgroundTexture);
	m_background.setPosition(sf::Vector2f(0.f, 0.f));

	//Portrait is static for now
	m_portraitTexture.loadFromFile("media/Textures/placeholderport.png");
	m_portrait.setTexture(m_portraitTexture);

	m_portrait.setPosition(sf::Vector2f(520.f, 540.f));
	m_portrait.setScale(sf::Vector2f(5.1f, 5.1f));   

	//Portraitframe is static dont move
	m_portraitFrameTexture.loadFromFile("media/Textures/portraitFrame.png");
	m_portraitFrame.setTexture(m_portraitFrameTexture);
	m_portraitFrame.setPosition(sf::Vector2f(510.f, 540.f));
	m_portraitFrame.setScale(sf::Vector2f(4.6f, 4.6f));

	//The bottom frame is static dont move
	m_bottomFrameTexture.loadFromFile("media/Textures/BottomFrame.png");
	m_bottomFrame.setTexture(m_bottomFrameTexture);
	m_bottomFrame.setPosition(sf::Vector2f(0.f, 520.f));
	m_bottomFrame.setScale(sf::Vector2f(18.3f, 3.0f));

	//Health text and font
	m_steamwreckFont.loadFromFile("media/Fonts/Steamwreck.otf");
	m_healthText.setFont(m_steamwreckFont);
	m_healthText.setPosition(sf::Vector2f(50.f, 560.f));
	m_healthText.setString(sf::String("HEALTH: 1337"));
	m_healthText.setCharacterSize(38);
	m_healthText.setColor(sf::Color(20, 20, 20, 255));

	//Hitchance text
	m_meleeHitChanceText.setFont(m_steamwreckFont);
	m_rangeHitChanceText.setFont(m_steamwreckFont);
	m_meleeHitChanceText.setPosition(sf::Vector2f(50.f, 590.f));
	m_rangeHitChanceText.setPosition(sf::Vector2f(50.f, 620.f));
	m_meleeHitChanceText.setString(sf::String("ME. HIT: 0%"));
	m_rangeHitChanceText.setString(sf::String("RA. HIT: 100%"));
	m_meleeHitChanceText.setCharacterSize(38);
	m_rangeHitChanceText.setCharacterSize(38);
	m_meleeHitChanceText.setColor(sf::Color(20, 20, 20, 255));
	m_rangeHitChanceText.setColor(sf::Color(20, 20, 20, 255));

	//Evasion text
	m_evasionText.setFont(m_steamwreckFont);
	m_evasionText.setPosition(sf::Vector2f(50.f, 650.f));
	m_evasionText.setCharacterSize(38);
	m_evasionText.setString("EVASION: 50%");
	m_evasionText.setColor(sf::Color(20, 20, 20, 255));
	
	std::string buttonString[3];
	buttonString[0] = "FIGHT";
	buttonString[1] = "ACTION";
	buttonString[2] = "ESCAPE";
	
	for (int i = 0; i < 3; i++)
	{
		m_buttons.addButton(buttonString[i], sf::Vector2f(700.f, 542.f + (58.f * i)), sf::Vector2f(200.f, 55.f), sf::Color(20, 20, 20, 255), sf::Color(50, 50, 50, 255), sf::Color(70, 70, 70, 255));
	}

	std::string extraButtonString[4];
	extraButtonString[0] = "EXBUTT1";
	extraButtonString[1] = "EXBUTT2";
	extraButtonString[2] = "EXBUTT3";
	extraButtonString[3] = "EXBUTT4";
	for (int i = 0; i < 4; i++)
	{
		m_extraButtons.addButton(extraButtonString[i], sf::Vector2f(902.f, 542.f + (44.f * i)), sf::Vector2f(200.f, 39.f), sf::Color(20, 20, 20, 255), sf::Color(50, 50, 50, 255), sf::Color(70, 70, 70, 255));
	}

	
	//Creation, addition of attacks and weapons
	m_attacks.addAttack("Punch", 0, 0, 0, 10);
	m_attacks.addAttack("Elbow", 0, 30, 0, 20);
	m_attacks.addAttack("Skull", 0, 50, 0, 70);
	m_attacks.addAttack("Kick", 0, 10, 0, 15);
	m_attacks.addAttack("Stab", 0, 60, 0, 70);
	m_attacks.addAttack("Slash", 0, 10, 0, 20);
	m_attacks.addAttack("H.Bash", 0, 10, 0, 20);

	std::string attacks[4] = { 
		"Punch", 
		"Elbow", 
		"Skull",
		"Kick" };
	m_weapons.addWeapon("Fist", 0, 0, 0, 10, 5, 10, Weapon::Type::Melee, attacks);

	attacks[0] = "Stab";
	attacks[1] = "Slash";
	attacks[2] = "H.Bash";
	attacks[3] = "Kick";
	m_weapons.addWeapon("Broken Sword", 0, 10, 10, 20, 10, 10, Weapon::Type::Melee, attacks);
	
	m_characterContext.setWeapon(m_weapons.getWeapon("Broken Sword"));
}

void GameStateCombat::OnDestroy()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_P");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
}

void GameStateCombat::Activate()
{
	if (m_music.getStatus() == sf::Music::Paused)
	{
		m_music.play();
	}
}

void GameStateCombat::Deactivate()
{
	m_music.pause();
}

void GameStateCombat::Update(const sf::Time& time)
{
	SharedContext* context = m_stateMgr->GetContext();

	//TODO Create a class to handle textobjects with addText() just like button

	m_healthText.setString("HEALTH:       " + sf::String(int2Str(m_characterContext.getCurrentHealth())));
	m_meleeHitChanceText.setString(sf::String("M.HITCHANCE:  " + int2Str((m_characterContext.getMeleeHitChance() + m_characterContext.getRangeHitChanceModifier()))));
	m_rangeHitChanceText.setString(sf::String("R.HITCHANCE:  " + int2Str((m_characterContext.getRangeHitChance() + m_characterContext.getRangeHitChanceModifier()))));
	m_evasionText.setString(sf::String("EVASION:      " + int2Str((m_characterContext.getEvasion() + m_characterContext.getEvasionModifier()))));

	sf::Vector2i mousePos = m_stateMgr->GetContext()->eventManager->GetMousePos(m_stateMgr->GetContext()->window->GetRenderWindow());
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();

	m_enemyController.update(mousePos, window);
	
	m_buttons.update(mousePos, window);
	m_extraButtons.update(mousePos, window);

	//m_frame.Update(time.asSeconds());
	m_stateMgr->GetContext()->entityManager->Update(time.asSeconds());

	if (m_playersTurn)
	{
		//	Players turn
	}
	else if (!m_playersTurn)
	{
		//	Enemies turn
		
		if (!m_enemiesGoing)
		{
			m_enemiesGoing = true;
			m_enemyTimer.restart();
			std::cout << "Enemies started attacking!" << std::endl;
		}

		if (m_enemyTimer.getElapsedTime().asSeconds() >= 1)
		{	
			bool crit = false;
			//Hardcoded 10% crit (DONT COMPLAIN)
			if ((rand() % 10 + 1) == 1) crit = true;
			int evasion = m_characterContext.getEvasion();
			int dye = (rand() % 100 + 1);
			bool evade = false;
			if (dye <= evasion && dye >= 1)
			{
				evade = true;
			}
			int hitchance = 80;
			int dyh = rand() % 100 + 1;
			bool hit = false;
			if (dyh <= hitchance && dyh >= 1)
			{
				hit = true;
			}
			unsigned int damage = rand() % 10 + 1;
			if (crit)
			{
				damage = damage * 1.5f;
			}
		
			if (hit && !evade)
			{
				int randomNumb = rand() % 3 + 1;
				m_hitSoundBuffer.loadFromFile("media/Sound/Effects/Punch" + int2Str(randomNumb) + ".ogg");
				m_hitSound.setBuffer(m_hitSoundBuffer);
				m_hitSound.setLoop(false);
				m_hitSound.setVolume(100);
				m_hitSound.play();

				m_characterContext.dealDamage(damage);
			}
			else
			{
				int randomNumb = rand() % 1 + 1;
				m_hitSoundBuffer.loadFromFile("media/Sound/Effects/Miss" + int2Str(randomNumb) + ".ogg");
				m_hitSound.setBuffer(m_hitSoundBuffer);
				m_hitSound.setLoop(false);
				m_hitSound.setVolume(100);
				m_hitSound.play();
			}
			m_enemiesHasAttacked += 1;
			m_enemyTimer.restart();
			if (m_enemiesHasAttacked == m_enemyController.getAmountOfEnemies() - m_enemyController.getCurrentAmountOfDeadEnemies())
			{
				m_playersTurn = true;
				m_enemiesGoing = false;
				m_enemiesHasAttacked = 0;
				std::cout << "Enemies finnished attacking!" << std::endl;
			}
			else if (m_enemyController.getCurrentAmountOfEnemies() == 0)
			{
				std::cout << "All enemies are dead!" << std::endl;
				m_playersTurn = true;
				m_enemiesGoing = false;
				m_enemiesHasAttacked = 0;
			}
		}
	}
}

void GameStateCombat::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();

	window->draw(m_background);

	m_enemyController.draw(window);

	window->draw(m_bottomFrame);
	
	m_buttons.draw();
	if (m_playersTurn && m_showExtraButtons)
	{
		m_extraButtons.draw();
	}

	window->draw(m_portraitFrame);
	window->draw(m_portrait);
	window->draw(m_healthText);
	window->draw(m_meleeHitChanceText);
	window->draw(m_rangeHitChanceText);
	window->draw(m_evasionText);
}

void GameStateCombat::MainMenu(Kengine::EventDetails* details)
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void GameStateCombat::Pause(Kengine::EventDetails* details)
{
	m_stateMgr->SwitchTo(StateType::Paused);
}

void GameStateCombat::ToggleOverlay(Kengine::EventDetails* details)
{
	std::cout << "ToggleOverlay" << std::endl;
	m_stateMgr->GetContext()->debugOverlay.SetDebug(!m_stateMgr->GetContext()->debugOverlay.Debug());
}

void GameStateCombat::MouseClick(Kengine::EventDetails * details)
{
	//m_showExtraButtons = false;
	//If it is the players turn buttons are available otherwise not
	if(m_playersTurn)
	{
		//Button logic
		std::string active = m_buttons.getActiveButton();
		if (active == "FIGHT")
		{
			m_showExtraButtons = true;

			if (m_buttonMode != "FIGHT")
			{
				m_buttonMode = "FIGHT";
				m_extraButtons.getButton("EXBUTT1")->setString(sf::String(m_characterContext.getWeapon().attacks[0]));
				m_extraButtons.getButton("EXBUTT1")->setVisible(true);
				m_extraButtons.getButton("EXBUTT2")->setString(sf::String(m_characterContext.getWeapon().attacks[1]));
				m_extraButtons.getButton("EXBUTT2")->setVisible(true);
				m_extraButtons.getButton("EXBUTT3")->setString(sf::String(m_characterContext.getWeapon().attacks[2]));
				m_extraButtons.getButton("EXBUTT3")->setVisible(true);
				m_extraButtons.getButton("EXBUTT4")->setString(sf::String(m_characterContext.getWeapon().attacks[3]));
				m_extraButtons.getButton("EXBUTT4")->setVisible(true);
			}

		}else if (active == "ACTION")
		{
			m_showExtraButtons = true;

			if(m_buttonMode != "ACTION")
			{
				m_buttonMode = "ACTION";
				m_extraButtons.getButton("EXBUTT1")->setString(sf::String("Change Wep"));
				m_extraButtons.getButton("EXBUTT1")->setVisible(true);
				m_extraButtons.getButton("EXBUTT2")->setString(sf::String("Action2"));
				m_extraButtons.getButton("EXBUTT2")->setVisible(false);
				m_extraButtons.getButton("EXBUTT3")->setString(sf::String("Action3"));
				m_extraButtons.getButton("EXBUTT3")->setVisible(false);
				m_extraButtons.getButton("EXBUTT4")->setString(sf::String("Action4"));
				m_extraButtons.getButton("EXBUTT4")->setVisible(false);

			
			}

			m_enemyController.generateNewRandomEnemies();
		}
		else if (active == "ESCAPE")
		{
			m_showExtraButtons = true;

			if (m_buttonMode != "ESCAPE")
			{
				m_buttonMode = "ESCAPE";
				m_extraButtons.getButton("EXBUTT1")->setString(sf::String("Run"));
				m_extraButtons.getButton("EXBUTT1")->setVisible(true);
				m_extraButtons.getButton("EXBUTT2")->setString(sf::String("Distract"));
				m_extraButtons.getButton("EXBUTT2")->setVisible(true);
				m_extraButtons.getButton("EXBUTT3")->setString(sf::String("Nothing"));
				m_extraButtons.getButton("EXBUTT3")->setVisible(false);
				m_extraButtons.getButton("EXBUTT4")->setString(sf::String("Nothing"));
				m_extraButtons.getButton("EXBUTT4")->setVisible(false);
			}

			m_enemyController.resetCombat();
		}

		//Enemy logic
		m_enemyController.changeSelected();


		if (m_buttonMode == "FIGHT")
		{

			if (m_extraButtons.getActiveButton() == "EXBUTT1")
			{
				m_attack = m_extraButtons.getButton("EXBUTT1")->getString();
				m_extraButtonsPressed = true;
			}
			else if (m_extraButtons.getActiveButton() == "EXBUTT2")
			{
				m_attack = m_extraButtons.getButton("EXBUTT2")->getString();
				m_extraButtonsPressed = true;
			}
			else if (m_extraButtons.getActiveButton() == "EXBUTT3")
			{
				m_attack = m_extraButtons.getButton("EXBUTT3")->getString();
				m_extraButtonsPressed = true;
			}
			else if (m_extraButtons.getActiveButton() == "EXBUTT4")
			{
				m_attack = m_extraButtons.getButton("EXBUTT4")->getString();
				m_extraButtonsPressed = true;
			}

			if(m_extraButtonsPressed && m_enemyController.getSelectedEnemy() != nullptr)
			{
				std::cout << "\n********************************" << std::endl;
				std::cout << " YOU ARE ATTACKING " << std::endl;
				std::cout << "Attack type: " << m_attack << std::endl;

				m_attackDetails = m_attacks.getAttack(m_attack);
				m_weaponDetails = m_characterContext.getWeapon();
				CombatEnemy* enemy = m_enemyController.getSelectedEnemy();
			
				//Damage Calculations begin
				int damage = 0;
				while(damage > m_weaponDetails.maxDamage || damage < m_weaponDetails.minimumDamage)
				{
					damage = rand() % m_weaponDetails.maxDamage;
				
				}
				std::cout << "Damage: " << damage << std::endl;
				//Damage Calculations end

				//Hit calculations begin
				int hitChance = 0;
				if(m_weaponDetails.weaponType == Weapon::Type::Melee)
				{
					hitChance = m_characterContext.getMeleeHitChance() - m_characterContext.getMeleeHitChanceModifier() - m_weaponDetails.meleeModifier - m_attackDetails.meleeHitModifier;
				}
				else if (m_weaponDetails.weaponType == Weapon::Type::Range)
				{
					hitChance = m_characterContext.getRangeHitChance() + m_characterContext.getRangeHitChanceModifier() + m_weaponDetails.rangeModifier + m_attackDetails.rangeHitModifier;
				}
				std::cout << "Hitchance: " << hitChance << std::endl;

				int dyh = (rand() % 100 + 1);
				bool didYouHit = false;
				std::cout << "Hit number: " << dyh << std::endl;

				if (dyh <= hitChance && dyh >= 1)
				{
					didYouHit = true;
					//std::cout << "You hit with a chance of: " << hitChance << ". You rolled: " << dyh << std::endl;
				}
				//Hit calculations end

				//Crit calculations begin
				int critChance = m_attackDetails.critchanceModifier + m_weaponDetails.critChance;
				int dyc = (rand() % 100 + 1);
				bool didYouCrit = false;
				if (dyc <= critChance && dyc >= 1)
				{
					didYouCrit = true;
				}
				//Crit calculations begin

				//EvasionCalculation of enemies begin
				int evasionChance = enemy->getCharacterContext()->getEvasion() + enemy->getCharacterContext()->getWeapon().evasionModifier;
				int dye = (rand() % 100 + 1);
				bool didYouEvade = false;
				if (dye <= evasionChance && dye >= 1)
				{
					didYouEvade = true;
				}
				//EvasionCalculation of enemies end

				if (didYouHit && !didYouEvade)
				{
					//Select and play sound
					int randomNumb = rand() % 3 + 1;
					m_hitSoundBuffer.loadFromFile("media/Sound/Effects/Punch" + int2Str(randomNumb) + ".ogg");
					m_hitSound.setBuffer(m_hitSoundBuffer);
					m_hitSound.setLoop(false);
					m_hitSound.setVolume(100);
					m_hitSound.play();
					
					if (didYouCrit) damage = damage *= 1.5; //Multiply damage by 150% if you crit
					enemy->getCharacterContext()->dealDamage(damage);
					enemy->updateColor();
					if (enemy->getCharacterContext()->getCurrentHealth() == 0 || enemy->getCharacterContext()->getCurrentHealth() > enemy->getCharacterContext()->getMaxHealth())
					{
						enemy->getCharacterContext()->setCurrentHealth(0);
						enemy->setDead(true);
						m_scream.play();
					}
				}
				else
				{
					int randomNumb = rand() % 1 + 1;
					m_hitSoundBuffer.loadFromFile("media/Sound/Effects/Miss" + int2Str(randomNumb) + ".ogg");
					m_hitSound.setBuffer(m_hitSoundBuffer);
					m_hitSound.setLoop(false);
					m_hitSound.setVolume(100);
					m_hitSound.play();
				}

				//m_enemyController.getSelectedEnemy()->getCharacterContext()->
			
				//Makes sure we dont access attacks whilst empty creating bugs in the code
				m_extraButtonsPressed = false;
				m_playersTurn = false;
				m_enemyController.checkCombat();
				std::cout << "********************************" << std::endl << std::endl;
			}
		}
		else if (m_buttonMode == "ACTION")
		{
			if (m_extraButtons.getActiveButton() == "EXBUTT1")
			{
				std::cout << "change wep\n";
			}
		}
		else if (m_buttonMode == "ESCAPE")
		{
			if (m_extraButtons.getActiveButton() == "EXBUTT1")
			{
				std::cout << "ESCAPE SHIT";
			}
			else if (m_extraButtons.getActiveButton() == "EXBUTT2")
			{
				std::cout << "TRICK SHIT\n";
			}
		}
	}
}
