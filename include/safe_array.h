#pragma once
#include <cstdint>
#include <iostream>
#include "Utils.h"

using Utils::REQUIRE;

template <class T>
class safe_array {
private:

	T* m_arr = nullptr;
	bool already_initialized = false;
public:
	uint64_t size;

	safe_array(uint64_t s = 0) :size(s) {
		if (s)
			m_arr = new T[size];
	}

	~safe_array() {
		delete[] m_arr;
	}

	safe_array(const safe_array& arr) = delete;
	safe_array(safe_array&& arr) = delete;

	void constructArray(uint64_t size) {
		if (!m_arr) {
			this->size = size;
			m_arr = new T[size];
		}
	}
	void constructArray(const T* arr, uint64_t size) {
		if (!m_arr) {
			this->size = size;
			m_arr = new T[size];
			memcpy(m_arr, arr, sizeof(T) * size);
			already_initialized = true;
		}
	}
	void setArray(T val) {
		if (m_arr)
			memset(m_arr, val, sizeof(T) * size);
	}

	void setArray(T* arr2, size_t size2) {
		if (size == size2 && m_arr) {
			memcpy(m_arr, arr2, sizeof(T) * size);
		}
	}

	inline T& operator[](int index) {
		REQUIRE(index >= 0 && index < size, "Out of Bounds", index);
		return m_arr[index];
	}
	inline T operator[] (int index) const {
		REQUIRE(index >= 0 && index < size, "Out of Bounds", index);
		return m_arr[index];
	}

	void clear() {
		if (m_arr)
			memset(m_arr, 0, size * sizeof(T));
	}

	void print(std::ostream& o) {
		if (m_arr) {
			for (register int i = 0; i < size; i++) {
				o << m_arr[i] << ' ';
			}
			o << '\n';
		}
	}

	T* getArray() const {
		return m_arr;
	}

	T** getPointerToArray()
	{
		return &m_arr;
	}

	bool alreadyInitialized() const
	{
		return already_initialized;
	}

	void swapArray(T*& new_pointer, size_t size) {
		T* temp = m_arr;
		m_arr = new_pointer;
		new_pointer = temp;
		this->size = size;
	}
};