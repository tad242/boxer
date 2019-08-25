#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <Windows.h>
#include <iomanip>
#include <string>

using namespace std;

int countFull = 0;
int countUnan = 0;
int countSplt = 0;

struct Fighter
{
	string name;
	int jab, hook, counter, slip, chin, speed, accuracy, recovery, health, avg, win, loss, draw, ko;
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
	health = 1000;
	avg = (jab + hook + counter + slip + speed + accuracy + recovery) / 7;
	win = 0;
	loss = 0;
	draw = 0;
	ko = 0;
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
	Fighter f[20];
	void Match(Fighter&, Fighter&);
	bool coinflip(int);
	void printlist(void);
	void replace(Fighter&,Fighter&);
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

void Roster::replace(Fighter &oldguy, Fighter &newguy)
{
	//A way to add existing fighters to another roster
	oldguy = newguy;
}

void Roster::Match(Fighter &rd, Fighter &bl)
{
	Fighter red, blue, win;
	red = rd;
	blue = bl;

	bool red_d, blue_d;
	int red_off = 0, red_def = 0, blue_off = 0, blue_def = 0, red_bias, blue_bias;
	int strike_attempt, slip_attempt, counter_attempt, jab_hook, damage;
	int prevAttemptRed = 0, prevAttemptBlue = 0, newAttemptRed = 0, newAttemptBlue = 0, redAttempts = 0, blueAttempts = 0, redLanded = 0, blueLanded = 0, redDmgGiven = 0, blueDmgGiven = 0;
	int round = 1, time = 180, decRed = 0, decBlue = 0, prevStrikesRed = 0, prevStrikesBlue = 0, newStrikesRed = 0, newStrikesBlue = 0;
	int prevDamageRed = 0, prevDamageBlue = 0, newDamageRed = 0, newDamageBlue = 0, judgeRed[3], judgeBlue[3];

	judgeRed[0] = 0, judgeBlue[0] = 0;
	judgeRed[1] = 0, judgeBlue[1] = 0;
	judgeRed[2] = 0, judgeBlue[2] = 0;

	cout << "\n\n\n" << red.name << " vs. " << blue.name << endl;
	
	while (round < 13 && red.health > 0 && blue.health > 0)
	{
		cout << "\n\n~~~~~~~ ROUND " << round << " ~~~~~~~~~~~~\n";
		time = 180;

		red_off = red.Offense() - blue.Offense();
		blue_off = blue.Offense() - red.Offense();

		red_def = red.Defense() - blue.Defense();
		blue_def = blue.Defense() - red.Defense();

		red_bias = red_off - red_def;
		blue_bias = blue_off - blue_def;

		cout << "\n\nRED BIAS: " << red_bias;
		cout << "\n\nBLUE BIAS: " << blue_bias;

		prevStrikesRed = redLanded;
		prevStrikesBlue = blueLanded;

		prevAttemptRed = redAttempts;
		prevAttemptBlue = blueAttempts;

		red.health += rand() % red.recovery;
		blue.health += rand() % blue.recovery;

		if (red.health > rd.health)
			red.health = rd.health;

		if (blue.health > bl.health)
			blue.health = bl.health;

		prevDamageRed = blue.health;
		prevDamageBlue = red.health;

		while (time > 0 && red.health > 0 && blue.health > 0)
		{
			cout << "\n\nRed Health remaining: " << red.health;
			cout << "\nBlue Health remaining: " << blue.health;

			damage = 0;

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
				cout << "\nBoth fighters swing for the fences!";

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

								damage = strike_attempt * (1.1 - (float)blue.chin/100);
								blue.health -= damage;

								if ((counter_attempt - strike_attempt) > 0)
									damage = (counter_attempt - strike_attempt) * (1.1 - (float)red.chin/100);
								else
									damage = 0;

								red.health -= damage;

								cout << "\nRed lands a hard jab!\nBlue returns with a weak jab.\n";
							}
							else
							{
								//if blue is faster
								blueLanded++;
								redLanded++;

								damage = counter_attempt * (1.1 - (float)red.chin/100);
								red.health -= damage;

								if ((strike_attempt - counter_attempt) > 0)
									damage = (strike_attempt - counter_attempt)  * (1.1 - (float)blue.chin/100);
								else
									damage = 0;

								blue.health -= damage;

								cout << "\nBlue lands a hard jab!\nRed returns with a weak jab.";
							}
						}
						else if (((rand() % 100) < red.accuracy) && ((rand() % 100) > blue.accuracy))
						{
							//Red is accurate, Blue misses
							redLanded++;
							slip_attempt = rand() % blue.slip;

							if(slip_attempt < 3)
								damage = (strike_attempt + red.speed) * ((4 - slip_attempt) - (float)blue.chin/100);
							else
								damage = (strike_attempt + red.speed) * (1.1 - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
							if (blue.health <= 0 && (slip_attempt <= 3))
								rd.ko++;

							cout << "\nRed lands a jab that rocks Blue!";
						}
						else if (((rand() % 100) > red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//Blue is accurate, Red misses
							blueLanded++;
							slip_attempt = rand() % red.slip;

							if (slip_attempt < 3)
								damage = (counter_attempt + blue.speed) * ((4 - slip_attempt) - (float)red.chin/100);
							else
								damage = (counter_attempt + blue.speed) * (1.1 - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
							if (red.health <= 0 && (slip_attempt <= 3))
								bl.ko++;

							cout << "\nBlue lands a jab that rocks Red!";
						}
						else //both miss
							cout << "\nBoth fighters swing wildly but fail to land anything.";
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

								damage = strike_attempt * (1.1 - (float)blue.chin/100);
								blue.health -= damage;

								if ((counter_attempt - strike_attempt) > 0)
									damage = (counter_attempt - strike_attempt) * (1.1 - (float)red.chin/100);
								else
									damage = 0;

								red.health -= damage;

								cout << "\nRed lands a hard jab!\nBlue returns with a weak hook.\n";
							}
							else
							{
								//if blue is faster
								blueLanded++;
								redLanded++;

								damage = counter_attempt * (1.1 - (float)red.chin/100);
								red.health -= damage;

								if ((strike_attempt - counter_attempt) > 0)
									damage = (strike_attempt - counter_attempt) * (1.1 - (float)blue.chin/100);
								else
									damage = 0;

								blue.health -= damage;

								cout << "\nBlue lands a big hook!\nRed returns with a weak jab.";
							}
						}
						else if (((rand() % 100) < red.accuracy) && ((rand() % 100) > blue.accuracy))
						{
							//Red makes contact, Blue misses
							redLanded++;
							slip_attempt = rand() % blue.slip;

							if (slip_attempt < 3)
								damage = (strike_attempt + red.speed) * ((4 - slip_attempt) - (float)blue.chin / 100);
							else
								damage = (strike_attempt + red.speed) * (1.1 - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
							if (blue.health <= 0 && (slip_attempt <= 3))
								rd.ko++;

							cout << "\nRed lands a jab that rocks Blue!";
						}
						else if (((rand() % 100) > red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//Blue makes contact, Red misses
							blueLanded++;
							slip_attempt = rand() % red.slip;

							if (slip_attempt < 3)
								damage = (counter_attempt + blue.speed) * ((4 - slip_attempt) - (float)red.chin / 100);
							else
								damage = (counter_attempt + blue.speed) * (1.1 - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
							if (red.health <= 0 && (slip_attempt <= 3))
								bl.ko++;


							cout << "\nBlue lands a huge hook that rocks Red!";
						}
						else //both miss
							cout << "\nBoth fighters swing wildly but fail to land anything.";
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

								damage = strike_attempt * (1.1 - (float)blue.chin/100);
								blue.health -= damage;

								if ((counter_attempt - strike_attempt) > 0)
									damage = (counter_attempt - strike_attempt) * (1.1 - (float)red.chin/100);
								else
									damage = 0;

								red.health -= damage;

								cout << "\nRed lands a big hook!\nBlue returns with a weak jab.\n";
							}
							else
							{
								//if blue is faster
								blueLanded++;
								redLanded++;

								damage = counter_attempt * (1.1 - (float)red.chin/100);
								red.health -= damage;

								if ((strike_attempt - counter_attempt) > 0)
									damage = (strike_attempt - counter_attempt)  * (1.1 - (float)blue.chin/100);
								else
									damage = 0;

								blue.health -= damage;

								cout << "\nBlue lands a hard jab!\nRed returns with a weak hook.";
							}
						}
						else if (((rand() % 100) < red.accuracy) && ((rand() % 100) > blue.accuracy))
						{
							//Red makes contact, Blue misses
							redLanded++;
							slip_attempt = rand() % blue.slip;

							if (slip_attempt < 3)
								damage = (strike_attempt + red.speed) * ((4 - slip_attempt) - (float)blue.chin / 100);
							else
								damage = (strike_attempt + red.speed) * (1.1 - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
							if (blue.health <= 0 && (slip_attempt <= 3))
								rd.ko++;

							cout << "\nRed lands a huge hook that rocks Blue!";
						}
						else if (((rand() % 100) > red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//Blue makes contact, Red misses
							blueLanded++;
							slip_attempt = rand() % red.slip;

							if (slip_attempt < 3)
								damage = (counter_attempt + blue.speed) * ((4 - slip_attempt) - (float)red.chin / 100);
							else
								damage = (counter_attempt + blue.speed) * (1.1 - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
							if (red.health <= 0 && (slip_attempt <= 3))
								bl.ko++;

							cout << "\nBlue lands a jab that rocks Red!";
						}
						else //both miss
							cout << "\nBoth fighters swing wildly but fail to land anything.";
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

								damage = strike_attempt * (1.1 - (float)blue.chin/100);
								blue.health -= damage;

								if ((counter_attempt - strike_attempt) > 0)
									damage = (counter_attempt - strike_attempt) * (1.1 - (float)red.chin/100);
								else
									damage = 0;

								red.health -= damage;

								cout << "\nRed lands a big hook!\nBlue returns with a weak hook.\n";
							}
							else
							{
								//if blue is faster
								blueLanded++;
								redLanded++;

								damage = counter_attempt * (1.1 - (float)red.chin/100);
								red.health -= damage;

								if ((strike_attempt - counter_attempt) > 0)
									damage = (strike_attempt - counter_attempt) * (1.1 - (float)blue.chin/100);
								else
									damage = 0;

								blue.health -= damage;

								cout << "\nBlue lands a big hook!\nRed returns with a weak hook.";
							}
						}
						else if (((rand() % 100) < red.accuracy) && ((rand() % 100) > blue.accuracy))
						{
							//Red makes contact, Blue misses
							redLanded++;
							slip_attempt = rand() % blue.slip;

							if (slip_attempt < 3)
								damage = (strike_attempt + red.speed) * ((4 - slip_attempt) - (float)blue.chin / 100);
							else
								damage = (strike_attempt + red.speed) * (1.1 - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
							if (blue.health <= 0 && (slip_attempt <= 3))
								rd.ko++;

							cout << "\nRed lands a huge hook that rocks Blue!";
						}
						else if (((rand() % 100) > red.accuracy) && ((rand() % 100) < blue.accuracy))
						{
							//Blue makes contact, Red misses
							blueLanded++;
							slip_attempt = rand() % red.slip;

							if (slip_attempt < 3)
								damage = (counter_attempt + blue.speed) * ((4 - slip_attempt) - (float)red.chin / 100);
							else
								damage = (counter_attempt + blue.speed) * (1.1 - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
							if (red.health <= 0 && (slip_attempt <= 3))
								bl.ko++;

							cout << "\nBlue lands a huge hook that rocks Red!";
						}
						else //both miss
							cout << "\nBoth fighters swing wildly but fail to land anything.";
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
					strike_attempt = (rand() % red.jab);
					slip_attempt = rand() % blue.slip;
					counter_attempt = rand() % blue.counter;

					if ((strike_attempt + red.speed > slip_attempt + blue.speed) && ((rand() % 100) < red.accuracy))
					{
						if ((slip_attempt < 3) && (strike_attempt + red.speed > red.jab))
						{
							cout << "\nRed lands a big jab and Blue is hurt!" << endl;
							damage = strike_attempt  * ((4 - slip_attempt) - (float)blue.chin / 100);
							blue.speed--;
						}
						else
						{
							cout << "\nRed lands with the jab!" << endl;
							damage = (strike_attempt) * (1.1 - (float)blue.chin / 100);
						}
						blue.health -= damage;
						redLanded++;
						cout << "\nDAMAGE: " << damage << endl;
						if (blue.health <= 0 && (slip_attempt <= 3))
							rd.ko++;
					}
					else if (slip_attempt + blue.speed > (strike_attempt * 2) + red.speed)
					{
						cout << "\nBlue slips a jab";
						blueAttempts++;
						if (strike_attempt < 3)
						{
							cout << "\n and responds with a hard counter, Red is in trouble";
							damage = counter_attempt * ((4 - strike_attempt) - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
							blueLanded++;
						}
						else if ((rand() % 100) <= blue.accuracy)
						{
							cout << " and counters";
							damage = counter_attempt * (1.1 - (float)red.chin / 100);
							red.health -= damage;
							blueLanded++;
						}

						
						if (red.health <= 0 && (strike_attempt <= 3))
							bl.ko++;
						

						cout << "!" << endl;
						cout << "\nDAMAGE: " << damage << endl;
					}
					else
					{
						cout << "\nRed misses with a jab.";
						if (red.jab > 2)
							red.jab -= 1;
					}
				}
				else
				{
					//Red Hook
					strike_attempt = (rand() % red.hook);
					slip_attempt = (rand() % blue.slip);
					counter_attempt = (rand() % blue.counter);

					if ((strike_attempt + red.speed > slip_attempt + blue.speed) && ((rand() % 100) < red.accuracy))
					{
						if ((slip_attempt < 3) && (strike_attempt + red.speed > red.hook))
						{
							cout << "\nRed lands a huge hook and Blue is hurt!" << endl;
							damage = strike_attempt * ((4 - slip_attempt) - (float)blue.chin / 100);
							blue.speed--;
						}
						else
						{
							cout << "\nRed lands with the hook!" << endl;
							damage = (strike_attempt) * (1.1 - (float)blue.chin / 100);
						}
						blue.health -= damage;
						redLanded++;
						cout << "\nDAMAGE: " << damage << endl;
						if (blue.health <= 0 && (slip_attempt <= 3))
							rd.ko++;
					}
					else if (slip_attempt + blue.speed > (strike_attempt * 2) + red.speed)
					{
						cout << "\nBlue slips a hook";
						blueAttempts++;
						if (strike_attempt < 3)
						{
							cout << "\n and responds with a hard counter, Red is in trouble";
							damage = counter_attempt * ((4 - strike_attempt) - (float)red.chin / 100);
							red.health -= damage;
							red.speed--;
							blueLanded++;
						}
						else if ((rand() % 100) <= blue.accuracy)
						{
							cout << " and counters";
							damage = counter_attempt * (1.1 - (float)red.chin / 100);
							red.health -= damage;
							blueLanded++;
						}


						if (red.health <= 0 && (strike_attempt <= 3))
							bl.ko++;

						cout << "!" << endl;
						cout << "\nDAMAGE: " << damage << endl;
					}
					else
					{
						cout << "\nRed misses with a hook.";
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
					strike_attempt = (rand() % blue.jab);
					slip_attempt = (rand() % red.slip);
					counter_attempt = (rand() % red.counter);

					if ((strike_attempt + blue.speed > slip_attempt + red.speed) && ((rand() % 100) < blue.accuracy))
					{
						if ((slip_attempt < 3) && (strike_attempt + blue.speed > blue.jab))
						{
							cout << "\nBlue lands a big jab and Red is hurt!" << endl;
							damage = strike_attempt * ((4 - slip_attempt) - (float)red.chin / 100);
							red.speed--;
						}
						else
						{
							cout << "\nBlue lands with the jab!" << endl;
							damage = (strike_attempt) * (1.1 - (float)red.chin / 100);
						}
						red.health -= damage;
						blueLanded++;
						cout << "\nDAMAGE: " << damage << endl;
						if (red.health <= 0 && (slip_attempt <= 3))
							bl.ko++;
					}
					else if (slip_attempt + red.speed > (strike_attempt * 2) + blue.speed)
					{
						cout << "\nRed slips a jab";
						redAttempts++;
						if (strike_attempt < 3)
						{
							cout << "\n and responds with a hard counter, Blue is in trouble";
							damage = counter_attempt * ((4 - strike_attempt) - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
							redLanded++;
						}
						else if ((rand() % 100) <= red.accuracy)
						{
							cout << " and counters";
							damage = counter_attempt * (1.1 - (float)blue.chin / 100);
							blue.health -= damage;
							redLanded++;
						}


						if (blue.health <= 0 && (strike_attempt <= 3))
							rd.ko++;

						cout << "!" << endl;
						cout << "\nDAMAGE: " << damage << endl;
					}
					else
					{
						cout << "\nBlue misses with a jab.";
						if (blue.jab > 2)
							blue.jab -= 1;
					}

				}
				else
				{
					//Blue Hook
					strike_attempt = (rand() % blue.hook);
					slip_attempt = (rand() % red.slip);
					counter_attempt = (rand() % red.counter);


					if ((strike_attempt + blue.speed > slip_attempt + red.speed) && ((rand() % 100) < blue.accuracy))
					{
						if ((slip_attempt < 3) && (strike_attempt + blue.speed > blue.hook))
						{
							cout << "\nBlue lands a huge hook and Red is hurt!" << endl;
							damage = strike_attempt * ((4 - slip_attempt) - (float)red.chin / 100);
							red.speed--;
						}
						else
						{
							cout << "\nBlue lands with the hook!" << endl;
							damage = (strike_attempt) * (1.1 - (float)red.chin / 100);
						}
						red.health -= damage;
						blueLanded++;
						cout << "\nDAMAGE: " << damage << endl;
						if (red.health <= 0 && (slip_attempt <= 3))
							bl.ko++;
					}
					else if (slip_attempt + red.speed > (strike_attempt * 2) + blue.speed)
					{
						cout << "\nRed slips a hook";
						redAttempts++;
						if (strike_attempt < 3)
						{
							cout << "\n and responds with a hard counter, Blue is in trouble";
							damage = counter_attempt * ((4 - strike_attempt) - (float)blue.chin / 100);
							blue.health -= damage;
							blue.speed--;
							redLanded++;
						}
						else if ((rand() % 100) <= red.accuracy)
						{
							cout << " and counters";
							damage = counter_attempt * (1.1 - (float)blue.chin / 100);
							blue.health -= damage;
							redLanded++;
						}

						if (blue.health <= 0 && (strike_attempt <= 3))
							rd.ko++;

						cout << "!" << endl;
						cout << "\nDAMAGE: " << damage << endl;
					}
					else
					{
						cout << "\nBlue misses with a hook.";
						if (blue.hook > 2)
							blue.hook -= 1;
					}
				}
			}
			else
			{
				//Both defending
				cout << "\nBoth fighters pause and look for an opening.\n";

			}
		time -= (rand() % 12) + 1;
		cout << "\nTIME REMAINING: " << max(0,time) << " seconds.\n";
		}

		newAttemptRed = redAttempts - prevAttemptRed;
		newAttemptBlue = blueAttempts - prevAttemptBlue;

		newStrikesRed = redLanded - prevStrikesRed;
		newStrikesBlue = blueLanded - prevStrikesBlue;

		newDamageRed = prevDamageRed - blue.health;
		newDamageBlue = prevDamageBlue - red.health;

		if (newStrikesRed > newStrikesBlue)
			judgeRed[0]++;
		else if (newStrikesBlue > newStrikesRed)
			judgeBlue[0]++;

		if (newDamageRed > newDamageBlue)
			judgeRed[1]++;
		else if (newDamageBlue > newDamageRed)
			judgeBlue[1]++;

		if ((newDamageRed / (newStrikesRed + 1)) > (newDamageBlue / (newStrikesBlue + 1)))
			judgeRed[2]++;
		else if ((newDamageBlue / (newStrikesBlue + 1)) > (newDamageRed / (newStrikesRed + 1)))
			judgeBlue[2]++;
		else if (newAttemptRed > newAttemptBlue)
			judgeBlue[2]++;
		else
			judgeRed[2]++;

		round++;
	}

	cout << "\n\nRed\nStrikes Landed: " << redLanded << "\nStrikes Attempted: " << redAttempts;
	cout << "\n\nBlue\nStrikes Landed: " << blueLanded << "\nStrikes Attempted: " << blueAttempts << endl;


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
		cout << "After 12 Rounds, we go to the judges.\n\n";

		for (int q = 0; q < 3; q++)
		{
			if (judgeRed[q] > judgeBlue[q])
				decRed++;
			else if (judgeBlue[q] > judgeRed[q])
				decBlue++;
		}

		if (decRed + decBlue <= 1 || (decRed == decBlue))
		{
			rd.draw++;
			bl.draw++;
		}
		else if (decRed > decBlue)
		{
			rd.win++;
			bl.loss++;
			win = red;
		}
			
		else if (decBlue > decRed)
		{
			bl.win++;
			rd.loss++;
			win = blue;
		}
	

		if (decRed == 2 || decBlue == 2)
		{
			for(int w = 0;w<3;w++)
			{
				cout << "\nJudge " << w + 1 << " scores the contest "
					<< max(judgeRed[w], judgeBlue[w]) << " to " << min(judgeRed[w], judgeBlue[w]);
				if (max(judgeRed[w], judgeBlue[w]) == judgeRed[w])
					cout << " for " << red.name;
				else if (max(judgeRed[w], judgeBlue[w]) == judgeBlue[w])
					cout << " for " << blue.name;
				else
					cout << " even";
			}

			cout << "\n\nMaking your winner,";

			if (decRed + decBlue == 3)
			{
				cout << " by SPLIT decision, ";
				countSplt++;
			}
			else
				cout << " by MAJORITY decision ";

			cout << win.name << "!\n\n";
		}
		else if (decRed == 3 || decBlue == 3)
		{

			cout << ", all for the WINNER by UNANIMOUS decision...\n\n" << win.name << "!\n";
		}
		else if (decRed + decBlue == 0)
		{
			countUnan++;
			cout << ".\nLadies and gentleman...\nWE HAVE A UNANIMOUS DRAW!\n";
		}
		else if (decRed + decBlue == 1)
		{
			countUnan++;
			cout << ".\nLadies and gentleman...\WE HAVE A MAJORITY DRAW!\n";
		}
		else if (decRed == 1 && decBlue == 1)
		{
			countUnan++;
			cout << ".\nLadies and gentleman...\WE HAVE A SPLIT DRAW!\n";
		}
		else
		{
			cout << "\n\nERROR\n" << "decRed = " << decRed << "\ndecBlue = " << decBlue;
			system("pause");
		}
		}
	}
}
		



void Roster::printlist(void)
{
	int i;

	for (i = 0; i <= 19; i++)
	{
		cout << "\n" << i+1 << ": " << f[i].name;
		cout << "\n(" << f[i].win << " - " << f[i].loss << " - " << f[i].draw << ", " << f[i].ko << " KOs)\n";
		cout << "\nJab: " << f[i].jab << "\nHook: " << f[i].hook << "\nCounter: " << f[i].counter << "\nSlip: "
			<< f[i].slip << "\nChin: " << f[i].chin << "\nSpeed: " << f[i].speed << "\nAccuracy: " << f[i].accuracy << "\nRecovery: " << f[i].recovery
			<< "\nHealth: " << f[i].health << endl;
	}
}

int
main()
{
	srand(GetTickCount());

	// "Champion" Roster
	Roster champs;
	int r = 20, b = 20, i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;
	bool yep = false;

	while (l < 20)
	{
		//Create New Roster
		Roster test;
		
		//Everybody on the roster fights everyone else
		j = 0;
		while (j < 20)
		{
			for (i = j; i <= 19; i++)
				if (i != j)
					test.Match(test.f[i], test.f[j]);

			j++;
		}

		//If fighter is undefeated, add to "Champion" roster
		for (k = 0; k < 20; k++)
		{
			if (test.f[k].win == 0)
			{
				champs.replace(champs.f[l], test.f[k]);
				l++;
			}

		}
		
	}


	//play Champs against one another
	while (m < 20)
	{
		for (n = m; n <= 19; n++)
			if (n != m)
				champs.Match(champs.f[n], champs.f[m]);
		m++;
	}

		while (r != 0 && b != 0)
		{
			champs.printlist();

			cout << endl << countFull << " full-distance matches.\n";
			cout << countSplt << " split decisions.\n";
			cout << countUnan << " draws.\n";

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

			champs.Match(champs.f[r - 1], champs.f[b - 1]);
		}

	
	



	system("pause");
}