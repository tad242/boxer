/*
This program creates a group of fighters, with varied skillsets, 
and uses a fighting engine to have them compete against one another.
The user can also create their own matchups.

The play-by-play is disabled in this version.

Trevor Bonner-DeWitt
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <Windows.h>
#include <iomanip>
#include <string>

using namespace std;

int countFull = 0;

struct Fighter
{
	string name;
	int jab, hook, counter, slip, chin, speed, accuracy, recovery, health, avg, win, loss, draw;
	Fighter(void);
	int Offense(void);
	int Defense(void);
};

Fighter::Fighter()
{
	jab = (rand() % 100) + 1;
	hook = (rand() % 100) + 1;
	counter = (rand() % 100) + 1;
	slip = (rand() % 100) + 1;
	chin = (rand() % 100) + 1;
	speed = (rand() % 100) + 1;
	accuracy = (rand() % 90) + 11;
	recovery = (rand() % 90) + 11;
	health = (rand() % 400) + 601; //arbitrary
	avg = (jab + hook + counter + slip + speed + accuracy + recovery) / 7;
	win = 0;
	loss = 0;
	draw = 0;

	//Putting fighter skill in their name, to give each fighter a unique id
	name = "Fighter " + to_string(Offense()) + " " + to_string(Defense());
}

int Fighter::Offense()
{
	int offense = (jab + hook + accuracy + speed) / 4;

	return offense;
}

int Fighter::Defense()
{
	int defense = (slip + counter + chin) / 3;

	return defense;
}

class Roster : public Fighter
{
public:
	Fighter f[10];
	void Match(Fighter&, Fighter&);
	bool coinflip(int);
	void printlist(void);
};

bool Roster::coinflip(int bias)
{
	int coinflip = rand() % 100;
	bool coinland;

	coinflip += bias;

	if (coinflip >= 50)
		coinland = true;
	else
		coinland = false;

	return coinland;
}

void Roster::Match(Fighter &rd, Fighter &bl)
{
	Fighter red, blue;
	red = rd;
	blue = bl;

	bool red_d, blue_d;
	int red_off = 0, red_def = 0, blue_off = 0, blue_def = 0, red_bias, blue_bias;
	int strike_attempt, slip_attempt, counter_attempt;
	int jab_hook, damage, redAttempts = 0, blueAttempts = 0, redLanded = 0, blueLanded = 0;
	int round = 1, time = 180, prevStrikesRed = 0, prevStrikesBlue = 0, newStrikesRed = 0, newStrikesBlue = 0, roundRed = 0, roundBlue = 0;

	cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	cout << "\n\n" << red.name << " vs. " << blue.name << endl;

	while (round < 13 && red.health > 0 && blue.health > 0)
	{
		time = 180;

		red_off = red.Offense() - blue.Offense();
		blue_off = blue.Offense() - red.Offense();

		red_def = red.Defense() - blue.Defense();
		blue_def = blue.Defense() - red.Defense();

		//Whether a fighter favors offense or defense
		red_bias = red_off - red_def;
		blue_bias = blue_off - blue_def;

		prevStrikesRed = redLanded;
		prevStrikesBlue = blueLanded;

		red.health += rand() % red.recovery;
		blue.health += rand() % blue.recovery;

		if (red.health > rd.health)
			red.health = rd.health;

		if (blue.health > bl.health)
			blue.health = bl.health;

		while (time > 0 && red.health > 0 && blue.health > 0)
		{
			//Red Decision
			if (coinflip(red_bias))
				red_d = true;
			else
				red_d = false;

			//Blue Decision
			if (coinflip(blue_bias))
				blue_d = true;
			else
				blue_d = false;

			if (red_d == true && blue_d == true)
			{
				//Neither fighter defending

				//Red's Jab vs Hook Preference
				jab_hook = red.jab - red.hook;
				if (jab_hook > 40)
					jab_hook = 40;
				else if (jab_hook < -40)
					jab_hook = -40;

				redAttempts++;
				blueAttempts++;

				if (coinflip(jab_hook))
				{
					//Blue's Jab vs Hook Preference
					jab_hook = blue.jab - blue.hook;
					if (jab_hook > 40)
						jab_hook = 40;
					else if (jab_hook < -40)
						jab_hook = -40;

					if (coinflip(jab_hook))
					{
						//Red Jab vs Blue Jab
						strike_attempt = (rand() % red.jab);
						counter_attempt = (rand() % blue.jab);

						if (((rand() % 100) < red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//If both are accurate
							if (red.speed > blue.speed)
							{
								//if red is faster
								redLanded++;
								blueLanded++;

								damage = strike_attempt * (1 - (float)blue.chin / 100);
								blue.health -= damage;

								if ((counter_attempt - strike_attempt) > 0)
									damage = (counter_attempt - strike_attempt) * (1 - (float)red.chin / 100);
								else
									damage = 0;

								red.health -= damage;
							}
							else
							{
								//if blue is faster
								blueLanded++;
								redLanded++;

								damage = counter_attempt * (1 - (float)red.chin / 100);
								red.health -= damage;

								if ((strike_attempt - counter_attempt) > 0)
									damage = (strike_attempt - counter_attempt)  * (1 - (float)blue.chin / 100);
								else
									damage = 0;

								blue.health -= damage;
							}
						}
						else if (((rand() % 100) < red.accuracy) && ((rand() % 100) > blue.accuracy))
						{
							//Red makes contact, Blue misses
							redLanded++;

							damage = (strike_attempt + red.speed) * (1 - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
						}
						else if (((rand() % 100) > red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//Blue makes contact, Red misses
							blueLanded++;

							damage = (counter_attempt + blue.speed) * (1 - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
						}

					}
					else
					{
						//Red Jab vs Blue Hook
						strike_attempt = (rand() % red.jab);
						counter_attempt = (rand() % blue.hook);

						if (((rand() % 100) < red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//If both are accurate
							if (red.speed > blue.speed)
							{
								//if red is faster
								redLanded++;
								blueLanded++;

								damage = strike_attempt * (1 - (float)blue.chin / 100);
								blue.health -= damage;

								if ((counter_attempt - strike_attempt) > 0)
									damage = (counter_attempt - strike_attempt) * (1 - (float)red.chin / 100);
								else
									damage = 0;

								red.health -= damage;
							}
							else
							{
								//if blue is faster
								blueLanded++;
								redLanded++;

								damage = counter_attempt * (1 - (float)red.chin / 100);
								red.health -= damage;

								if ((strike_attempt - counter_attempt) > 0)
									damage = (strike_attempt - counter_attempt) * (1 - (float)blue.chin / 100);
								else
									damage = 0;

								blue.health -= damage;
							}
						}
						else if (((rand() % 100) < red.accuracy) && ((rand() % 100) > blue.accuracy))
						{
							//Red makes contact, Blue misses
							redLanded++;

							damage = (strike_attempt + red.speed) * (1 - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
						}
						else if (((rand() % 100) > red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//Blue makes contact, Red misses
							blueLanded++;

							damage = (counter_attempt + blue.speed) * (1 - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
						}
					}
				}
				else
				{
					//Blue's Jab vs Hook Preference
					jab_hook = blue.jab - blue.hook;
					if (jab_hook > 40)
						jab_hook = 40;
					else if (jab_hook < -40)
						jab_hook = -40;

					if (coinflip(jab_hook))
					{
						//Red Hook vs Blue Jab
						strike_attempt = (rand() % red.hook);
						counter_attempt = (rand() % blue.jab);

						if (((rand() % 100) < red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//If both are accurate
							if (red.speed > blue.speed)
							{
								//if red is faster
								redLanded++;
								blueLanded++;

								damage = strike_attempt * (1 - (float)blue.chin / 100);
								blue.health -= damage;

								if ((counter_attempt - strike_attempt) > 0)
									damage = (counter_attempt - strike_attempt) * (1 - (float)red.chin / 100);
								else
									damage = 0;

								red.health -= damage;
							}
							else
							{
								//if blue is faster
								blueLanded++;
								redLanded++;

								damage = counter_attempt * (1 - (float)red.chin / 100);
								red.health -= damage;

								if ((strike_attempt - counter_attempt) > 0)
									damage = (strike_attempt - counter_attempt)  * (1 - (float)blue.chin / 100);
								else
									damage = 0;

								blue.health -= damage;
							}
						}
						else if (((rand() % 100) < red.accuracy) && ((rand() % 100) > blue.accuracy))
						{
							//Red makes contact, Blue misses
							redLanded++;

							damage = (strike_attempt + red.speed) * (1 - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
						}
						else if (((rand() % 100) > red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//Blue makes contact, Red misses
							blueLanded++;

							damage = (counter_attempt + blue.speed) * (1 - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
						}

					}
					else
					{
						//Red Hook vs Blue Hook
						strike_attempt = (rand() % red.hook);
						counter_attempt = (rand() % blue.hook);

						if (((rand() % 100) < red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//If both are accurate
							if (red.speed > blue.speed)
							{
								//if red is faster
								redLanded++;
								blueLanded++;

								damage = strike_attempt * (1 - (float)blue.chin / 100);
								blue.health -= damage;

								if ((counter_attempt - strike_attempt) > 0)
									damage = (counter_attempt - strike_attempt) * (1 - (float)red.chin / 100);
								else
									damage = 0;

								red.health -= damage;
							}
							else
							{
								//if blue is faster
								blueLanded++;
								redLanded++;

								damage = counter_attempt * (1 - (float)red.chin / 100);
								red.health -= damage;

								if ((strike_attempt - counter_attempt) > 0)
									damage = (strike_attempt - counter_attempt) * (1 - (float)blue.chin / 100);
								else
									damage = 0;

								blue.health -= damage;
							}
						}
						else if (((rand() % 100) < red.accuracy) && ((rand() % 100) > blue.accuracy))
						{
							//Red makes contact, Blue misses
							redLanded++;

							damage = (strike_attempt + red.speed) * (1 - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
						}
						else if (((rand() % 100) > red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//Blue makes contact, Red misses
							blueLanded++;

							damage = (counter_attempt + blue.speed) * (1 - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
						}
					}
				}
			}
			else if (red_d == true && blue_d == false)
			{
				//Red swinging, blue expecting

				jab_hook = red.jab - red.hook;
				if (jab_hook > 40)
					jab_hook = 40;
				else if (jab_hook < -40)
					jab_hook = -40;

				redAttempts++;

				if (coinflip(jab_hook))
				{
					//Red Jab
					strike_attempt = (rand() % red.jab) + red.speed;
					slip_attempt = (rand() % blue.slip) + blue.speed;
					counter_attempt = (rand() % blue.counter);

					if ((strike_attempt > slip_attempt) && ((rand() % 100) < red.accuracy))
					{
						damage = (strike_attempt - slip_attempt) * (1 - (float)blue.chin / 100);
						blue.health -= damage;
						redLanded++;
					}
					else if (slip_attempt > (strike_attempt))
					{
						damage = counter_attempt * (1 - (float)red.chin / 100);
						blueAttempts++;
						if (damage > 0)
						{
							if ((rand() % 100) <= blue.accuracy)
							{
								red.health -= damage;
								blueLanded++;
							}

						}
					}
					else
					{
						if (red.jab > 2)
							red.jab -= 1;
					}
				}
				else
				{
					//Red Hook
					strike_attempt = (rand() % red.hook) + red.speed;
					slip_attempt = (rand() % blue.slip) + blue.speed;
					counter_attempt = (rand() % blue.counter);

					if ((strike_attempt > slip_attempt) && (rand() % 100) < red.accuracy)
					{
						damage = (strike_attempt - slip_attempt) * (1 - (float)blue.chin / 100);
						blue.health -= damage;
						redLanded++;
					}
					else if (slip_attempt > (strike_attempt * 2))
					{
						damage = counter_attempt * (1 - (float)red.chin / 100);
						blueAttempts++;
						if (damage > 0)
						{
							if ((rand() % 100) <= blue.accuracy)
							{
								red.health -= damage;
								blueLanded++;
							}
						}
					}
					else
					{
						if (red.hook > 2)
							red.hook -= 1;
					}
				}
			}
			else if (red_d == false && blue_d == true)
			{
				//Red expecting, blue swinging

				jab_hook = blue.jab - blue.hook;
				if (jab_hook > 40)
					jab_hook = 40;
				else if (jab_hook < -40)
					jab_hook = -40;

				blueAttempts++;

				if (coinflip(jab_hook))
				{
					//Blue Jab
					strike_attempt = (rand() % blue.jab) + blue.speed;
					slip_attempt = (rand() % red.slip) + red.speed;
					counter_attempt = (rand() % red.counter);

					if ((strike_attempt > slip_attempt) && (rand() % 100) < blue.accuracy)
					{
						damage = (strike_attempt - slip_attempt) * (1 - (float)red.chin / 100);
						red.health -= damage;
						blueLanded++;
					}
					else if (slip_attempt > (strike_attempt * 2))
					{
						damage = counter_attempt * (1 - (float)blue.chin / 100);
						redAttempts++;
						if (damage > 0)
						{
							if ((rand() % 100) <= red.accuracy)
							{
								blue.health -= damage;
								redLanded++;
							}
						}
					}
					else
					{
						if (blue.jab > 2)
							blue.jab -= 1;
					}

				}
				else
				{
					//Blue Hook
					strike_attempt = (rand() % blue.hook) + blue.speed;
					slip_attempt = (rand() % red.slip) + red.speed;
					counter_attempt = (rand() % red.counter);


					if ((strike_attempt > slip_attempt) && (rand() % 100) < blue.accuracy)
					{
						damage = (strike_attempt - slip_attempt) * (1 - (float)red.chin / 100);
						red.health -= damage;
						blueLanded++;
					}
					else if (slip_attempt > (strike_attempt * 2))
					{
						damage = counter_attempt * (1 - (float)blue.chin / 100);
						redAttempts++;
						if (damage > 0)
						{
							if ((rand() % 100) <= red.accuracy)
							{
								blue.health -= damage;
								redLanded++;
							}
						}
					}
					else
					{
						if (blue.hook > 2)
							blue.hook -= 1;
					}
				}
			}
			else
			{
				//Both defending

			}
			time -= (rand() % 12) + 2; //every event takes 2 seconds minimum
		}

		newStrikesRed = redLanded - prevStrikesRed;
		newStrikesBlue = blueLanded - prevStrikesBlue;

		if (newStrikesRed > newStrikesBlue)
			roundRed++;
		else if (newStrikesBlue > newStrikesRed)
			roundBlue++;

		round++;
	}

	if (blue.health <= 0)
	{
		cout << "\nRed Corner fighter " << red.name << "\nDEFEATS \nBlue Corner fighter " << blue.name << endl;
		rd.win++;
		bl.loss++;
	}
	else if (red.health <= 0)
	{
		cout << "\nBlue Corner fighter " << blue.name << "\nDEFEATS \nRed Corner fighter " << red.name << endl;
		bl.win++;
		rd.loss++;
	}
	else
	{
		countFull++;
		cout << "After 12 Rounds, we go to the judges. The judges score the contest "
			<< max(roundRed, roundBlue) << " to " << min(roundRed, roundBlue);

		if (roundRed > roundBlue)
		{
			cout << ", for the WINNER by UNANIMOUS DECISION...\n\n" << red.name << " of the RED CORNER!\n"
				<< "Loser: " << blue.name;
			rd.win++;
			bl.loss++;
		}
		else if (roundBlue > roundRed)
		{
			cout << ", for the WINNER by UNANIMOUS DECISION...\n\n" << blue.name << " of the BLUE CORNER!\n"
				<< "Loser: " << red.name;
			bl.win++;
			rd.loss++;
		}
		else
		{
			cout << ".\nLadies and gentleman...\nTHIS MATCH IS A DRAW!\n";
			rd.draw++;
			bl.draw++;
			countFull++;
		}
	}


}

void Roster::printlist(void)
{
	int i;

	for (i = 0; i <= 9; i++)
	{
		cout << "\n\n" << i + 1 << ": " << f[i].name;
		cout << "\n(" << f[i].win << " - " << f[i].loss << " - " << f[i].draw << ")\n";
		cout << "\nJab: " << f[i].jab << "\nHook: " << f[i].hook << "\nCounter: " << f[i].counter << "\nSlip: "
			<< f[i].slip << "\nChin: " << f[i].chin << "\nSpeed: " << f[i].speed << "\nAccuracy: " << f[i].accuracy
			<< "\nRecovery: " << f[i].recovery << "\nHealth: " << f[i].health << endl;
	}
}

int
main()
{
	srand(GetTickCount());

	int r = 10, b = 10, i = 0, j = 0;

	//Create New Roster
	Roster boxer;

	//Everybody on the roster fights everyone else
	j = 0;
	while (j < 10)
	{
		for (i = j; i <= 9; i++)
			if (i != j)
				boxer.Match(boxer.f[i], boxer.f[j]);

		j++;
	}

	while (r != 0 && b != 0)
	{
		boxer.printlist();

		cout << "\nPress 0 to quit\n";

		cout << "\nPlease select the fighter in the Red corner:\n";
		cin >> r;
		if (r == 0)
			break;

		cout << "\nPlease select a fighter in the Blue corner:\n";
		cin >> b;
		while (r == b)
			cin >> b;
		if (b == 0)
			break;

		boxer.Match(boxer.f[r - 1], boxer.f[b - 1]);
	}

	system("pause");
}
