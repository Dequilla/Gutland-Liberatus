#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
namespace Kengine
{
template<typename Derived, typename T>
class ResourceManager
{
public:
	ResourceManager(const std::string& pathsFile)
	{
		LoadPaths(pathsFile);
	}

	virtual ~ResourceManager() { PurgeResources(); }

	T* GetResource(const std::string& id)
	{
		auto res = Find(id);
		return (res ? res->first : nullptr);
	}

	std::string GetPath(const std::string& id)
	{
		auto path = m_paths.find(id);
		return (path != m_paths.end() ? path->second : "");
	}

	bool RequireResource(const std::string& id)
	{
		auto res = Find(id);
		if (res)
		{
			++res->second;
			return true;
		}
		auto path = m_paths.find(id);
		if (path == m_paths.end()) { return false; }

		T* resource = Load(path->second);
		if (!resource) { return false; }

		m_resources.emplace(id, std::make_pair(resource, 1));
		return true;
	}

	bool ReleaseResource(const std::string& id)
	{
		if (m_resources.size() == 0) { return false; }
		auto res = Find(id);
		if (!res) { return false; }
		--res->second;

		// If the counter is a 0 we de-allocate it by using our unload function
		if (!res->second) { Unload(id); }
		return true;
	}

	void PurgeResources()
	{
		std::cout << "Purging all resources:" << std::endl;
		while (m_resources.begin() != m_resources.end())
		{
			std::cout << "Removing: " << m_resources.begin()->first << std::endl;
			delete m_resources.begin()->second.first;
			m_resources.erase(m_resources.begin());
		}
		std::cout << "Purging finished." << std::endl;
	}

protected:
	// Derived classes will implement their own versions of Load,
	// but will not rely on resolving virtual pointers to functions
	// during run-time
	T* Load(const std::string& path)
	{
		return static_cast<Derived*>(this)->Load(path);
	}

private:
	std::unordered_map<std::string, std::pair<T*, unsigned int>> m_resources;
	std::unordered_map<std::string, std::string> m_paths;

	std::pair<T*, unsigned int>* Find(const std::string& id)
	{
		auto itr = m_resources.find(id);
		return (itr != m_resources.end() ? &itr->second : nullptr);
	}

	bool Unload(const std::string& id)
	{
		auto itr = m_resources.find(id);
		if (itr == m_resources.end()) { return false; }
		delete itr->second.first;
		m_resources.erase(itr);
		return true;
	}

	void LoadPaths(const std::string& pathFile)
	{
		std::ifstream paths;
		paths.open(pathFile);
		if (paths.is_open())
		{
			std::string line;
			while (std::getline(paths, line))
			{
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;
				keystream >> pathName;
				keystream >> path;
				m_paths.emplace(pathName, path);
			}
			paths.close();
			return;
		}
		std::cerr << "! Failed loading the path file: " << pathFile << std::endl;
	}
};
}