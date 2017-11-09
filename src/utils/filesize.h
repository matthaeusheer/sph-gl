#pragma once

template<typename T>
size_t vectorsizeof(const typename std::vector<T>& vec)
{
	return sizeof(T) * vec.size();
}