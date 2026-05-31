#pragma once

template<typename T>
class SManager {
public:
	virtual void LoadResources(FILE* fp)
	{
		readVector(fp, entries);
	}
	const T* FindEntry(const T& val) const
	{
		for (const auto& e : entries)
		{
			if (e == val) { // usa operator==
				return &e;
			}
		}
		return nullptr;
	}
	// Variante que permite buscar con "clave simple"
	template<typename K>
	const T* FindEntry(const K& key) const
	{
		for (const auto& e : entries)
		{
			if (e == key) { // usa la sobrecarga con el tipo Key
				return &e;
			}
		}
		return nullptr;
	}
	const std::vector<T>& AllItems() const
	{
		return entries;
	}
protected:
	std::vector<T> entries;
};



