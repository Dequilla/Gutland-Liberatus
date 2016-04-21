#include "Kopter.h"

namespace Kengine
{
	Kopter::Kopter(std::string filePath) : m_seperator("="), m_filePath("options.kopt")
	{
		m_filePath = filePath;
		readFile(m_filePath);
	}

	bool Kopter::readFile(std::string filePath)
	{
		//	Returns false if it fails to read the file

		//Open the optionsfile as both read and write
		try
		{
			this->m_file.open(filePath, std::ios::in | std::ios::out);
		}
		catch (...)
		{
			std::cerr << "KOPTER ERROR: Failed to load file: " << filePath << ". \n";
			return false;
		}

		//if the file is open read through all the lines and add corresponding values to the options container
		if (this->m_file.is_open())
		{
			std::string line;
			while (std::getline(this->m_file, line))
			{

				std::vector<std::string> values(2);

				//	Seperate and store the option and its value
				values[0] = this->seperateOptions(line)[0];
				values[1] = this->seperateOptions(line)[1];

				//	Insert the values into the options-container
				this->m_optionsContainer.insert(std::pair<std::string, std::string>(values[0], values[1]));
			}
		}

		return true;
	}

	std::string Kopter::getStringOptionAt(std::string optionName)
	{
		try
		{
			this->m_optionsContainer.at(optionName);
			
		}
		catch (std::exception e)
		{
			std::cout << "KOPTER ERROR: Value at: " << optionName << " does not exist.";
			return "";
		}
		return this->m_optionsContainer.at(optionName);
	}

	bool Kopter::getBoolOptionAt(std::string optionName)
	{
		try
		{
			this->m_optionsContainer.at(optionName);
		}
		catch (...)
		{
			std::cerr << "Option: " << this->m_optionsContainer.at(optionName) << " does not exist." << std::endl;
			return false;
		}
			
		if (this->m_optionsContainer.at(optionName) == "true")
		{
			return true;
		}
		else if (this->m_optionsContainer.at(optionName) == "false")
		{
			return false;
		}

		std::cerr << "KOPTER ERROR: Value at: " << optionName << " not a booleon. \n";
		return false;

	}

	int Kopter::getIntOptionAt(std::string optionName)
	{
		
		int value = 0;
		try
		{
			value = std::stoi(this->m_optionsContainer.at(optionName));
		}
		catch(const std::exception e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "KOPTER ERROR: Value at: " << optionName << " not a int. \n";
		}

		return value;
	}

	float Kopter::getFloatOptionAt(std::string optionName)
	{
		float value = 0.0f;
		try {
			value = std::stof(this->m_optionsContainer.at(optionName));
		}
		catch(std::exception e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "KOPTER ERROR: Value at: " << optionName << " not a float. \n";
		}

		return value;
	}

	void Kopter::setStringOptionAt(std::string optionName, std::string optionValue)
	{
		//Sets the option at a keyValue to another value. You set it as a string but you can still get as bool, float, int or original string.
		std::map<std::string, std::string>::iterator it = m_optionsContainer.find(optionName);
		if (it != m_optionsContainer.end()) it->second = optionValue;
	}

	std::vector<std::string> Kopter::seperateOptions(std::string optionsLine)
	{
		//vector to temporarly store the values that will be split
		std::vector<std::string> values(2);

		values[0] = optionsLine.substr(0, optionsLine.find(m_seperator));
		values[1] = optionsLine.substr((optionsLine.find(m_seperator) + 1), (optionsLine.find("\n") - optionsLine.find(m_seperator) - 1));

		return values;
	}

	void Kopter::writeOptionsToFile()
	{
		std::ofstream m_file;
		m_file.open(m_filePath, std::ofstream::out |std::ofstream::trunc);
		
		//Create iterator to iterate through the map
		typedef std::map<std::string, std::string>::iterator it;

		//temp string to store the options
		std::string tempString = "";

		//Iterates through map, appends to tempstring
		for (it iterator = m_optionsContainer.begin(); iterator != m_optionsContainer.end(); iterator++)
		{ 
			tempString.append(iterator->first);
			tempString.append(m_seperator);
			tempString.append(iterator->second);
			tempString.append("\n");
		}

		std::cout << "Finnished writing to file: \n " << tempString << std::endl;
		
		//write it to file
		m_file << tempString;

		m_file.close();
	}

	void Kengine::Kopter::optionSetSeperator(std::string seperator)
	{
		m_seperator = seperator;
	}
}
