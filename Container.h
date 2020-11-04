#pragma once

#include <unordered_map>
#include <fstream>
#include <stdlib.h>
#include <time.h>

struct result
{
	std::string discipline;
	std::string opponent;
	int result_player, result_opponent;
};

struct playerData
{
	std::string login;
	int numberOfWins, numberOfLoses;
	sf::Color color;
	std::vector<result> results;
};

class Container
{
	static std::unordered_map<std::string, playerData> map;

public:
	static void load_data(const std::string& fileName)
	{
		srand(time(NULL));
		std::ifstream file;
		file.open(fileName);
		unsigned color;
		playerData temporaryData;

		while (file >> temporaryData.login)
		{
			file >> temporaryData.numberOfWins;
			file >> temporaryData.numberOfLoses;
			file >> color;
			temporaryData.color = sf::Color(color);
			
			result temporaryResult;

			while (true)
			{
				file >> temporaryResult.discipline;
				if (temporaryResult.discipline == "END")
					break;
				file >> temporaryResult.opponent;
				file >> temporaryResult.result_player;
				file >> temporaryResult.result_opponent;
				temporaryData.results.push_back(temporaryResult);
			}

			map[temporaryData.login] = temporaryData;
			temporaryData.results.clear();
		}
		file.close();
	}

	// throws zero when object is not in map
	static playerData search(const std::string& log) throw(std::string)
	{
		try
		{
			return map.at(log);
		}
		catch (const std::out_of_range& oor)
		{
			throw "Cannot find name in database. Try again or create new profile";
		}
	}
	
	static playerData addNew(const std::string& newLogin) throw(std::string)
	{
		if (newLogin == "")
			throw("The login has to contains something");
		try
		{
			playerData probe = map.at(newLogin);
		}
		catch(const std::out_of_range& orr)
		{
			srand(time(NULL));
			map[newLogin] = playerData{ newLogin,0,0,sf::Color(rand()%255, rand()%255, rand()%255) };
			return map[newLogin];
		}
		throw "That login already exists";
	}

	static void update(playerData& playerdata)
	{
		try
		{
			playerdata = map.at(playerdata.login);
		}
		catch (const std::out_of_range& orr)
		{
			return;
		}
	}

	static void updateStatistics(const std::string& player1, const std::string& player2, const std::string& discipline, int result1, int result2, bool multiplayer)
	{
		map[player1].results.push_back(result{ discipline, player2, result1, result2 });

		if (multiplayer)
		{
			map[player2].results.push_back(result{ discipline, player1, result2, result1 });

			if (result1 > result2)
			{
				map[player1].numberOfWins++;
				map[player2].numberOfLoses++;
			}
			else
			{
				map[player1].numberOfLoses++;
				map[player2].numberOfWins++;
			}
		}

		else
		{
			if (result1 > result2)
				map[player1].numberOfWins++;
			else
				map[player1].numberOfLoses++;
		}
			
	}


	static void saveData(const std::string& fileName)
	{
		std::ofstream file;
		file.open(fileName);
		for (auto& r : map)
		{
			file << r.second.login << "\n";
			file << r.second.numberOfWins << "\n";
			file << r.second.numberOfLoses << "\n";
			file << r.second.color.toInteger() << "\n";
			
			int i;
			std::vector<result>::iterator it;

			if (r.second.results.empty())
			{
				file << "END\n";
				continue;
			}

			for (it = --r.second.results.end(), i = 0; i < 5 ; it--, i++)
			{
				file << it->discipline << "\n";
				file << it->opponent << "\n";
				file << it->result_player << "\n";
				file << it->result_opponent << "\n";
				if (it == r.second.results.begin())
					break;
			}
			
			file << "END\n";
		}
		file.close();
		map.clear();
	}

};

std::unordered_map<std::string, playerData> Container::map;